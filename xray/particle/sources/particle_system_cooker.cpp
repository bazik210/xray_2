////////////////////////////////////////////////////////////////////////////
//	Created		: 26.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "particle_system_cooker.h"

#include "help_functions.h"
#include "particle_entity_types.h"
#include "particle_actions.h"
#include "particle_system.h"

#include "help_functions.h"
#include "particle_emitter.h"
#include "particle_entity_types.h"


namespace xray {
namespace particle {

#pragma message(XRAY_TODO("Lain 2 IronNick : do not read files manually, use managed/unmanaged cooks that do it for you!"))

particle_system_cooker::particle_system_cooker() 
	: super(resources::particle_system_class, reuse_true, use_resource_manager_thread_id)
{
}

void particle_system_cooker::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string		request_path		(parent.get_requested_path());
	
	// Remove last word, "_static"
	buffer_string::size_type pos = request_path.find("_static");
	if (pos!=buffer_string::npos && pos==request_path.length()-strings::length("_static"))
		request_path.set_length( pos );
	
	fs_new::virtual_path_string		root_path			=  "resources/particles";
	fs_new::virtual_path_string		root_path_conv		=  "resources.converted/particles";
	
	particle_system_cooker_data * data		=  UNMANAGED_NEW(data, particle_system_cooker_data);
	
	data->resource_path.append				(request_path.c_str());
	data->query_result						= &parent;
	
	resources::query_vfs_iterator(
		root_path.c_str(), 
		boost::bind(&particle_system_cooker::on_fs_iterator_ready_source, this, data, _1),
		xray::resources::unmanaged_allocator(),
		resources::recursive_true,
		parent.get_parent_query()
	);
	
	resources::query_vfs_iterator(
		root_path_conv.c_str(), 
		boost::bind(&particle_system_cooker::on_fs_iterator_ready_converted, this, data, _1),
		xray::resources::unmanaged_allocator(),
		resources::recursive_true,
		parent.get_parent_query()
	);
}

void particle_system_cooker::on_fs_iterator_ready_source( particle_system_cooker_data* data, xray::vfs::vfs_locked_iterator const & fs_it )
{
	fs_new::virtual_path_string source_path;
	source_path.append						(data->resource_path.begin(), data->resource_path.end());
	source_path.append						(".particle");
	vfs::vfs_iterator fs_it_source		=	fs_it.find_child(source_path.c_str());
	
	if ( fs_it_source.is_end() )
		data->source_info_time = 0;
	else
	{
		fs_new::native_path_string const & physical_path	=	fs_it_source.get_physical_path();
		fs_new::physical_path_info const & info	=	resources::get_physical_path_info(physical_path);
		data->source_info_time			=	info.last_time_of_write();
	}
	
	data->source_ready					=	true;
	if( data->converted_ready)
		on_fs_iterators_ready				(data);
}

void particle_system_cooker::on_fs_iterator_ready_converted( particle_system_cooker_data* data, xray::vfs::vfs_locked_iterator const & fs_it )
{
	fs_new::virtual_path_string converted_path;
	converted_path.append			(data->resource_path.c_str());
	converted_path.append			(".binary_particle");
	
	xray::vfs::vfs_iterator fs_it_converted = fs_it.is_end() ? fs_it : fs_it.find_child(converted_path.c_str());

	if ( fs_it_converted.is_end() )
	{
		request_convertion				(&data->resource_path, data->query_result);
		// TODO: deleting
		//UNMANAGED_DELETE(data);
		return;
	}

	fs_new::physical_path_info	converted_info	=	resources::get_physical_path_info(fs_it_converted.get_physical_path());
	
	data->converted_info_time = converted_info.last_time_of_write();
	data->converted_ready = true;
	
	if( data->source_ready)
		on_fs_iterators_ready( data);
}

void particle_system_cooker::on_fs_iterators_ready( particle_system_cooker_data* data )
{
	if(data->source_info_time > data->converted_info_time)
	{
		request_convertion	(&data->resource_path, data->query_result);
		UNMANAGED_DELETE	(data);
		return;
	}
	
	fs_new::virtual_path_string converted_res_path;
	converted_res_path.assignf				("resources.converted/particles/%s.binary_particle", data->resource_path.c_str());
	
	fs_new::native_path_string				destination_path;
	
	if ( !resources::convert_virtual_to_physical_path(&destination_path, converted_res_path, resources::sources_mount) )
	{
		data->query_result->finish_query	(result_error);
		return;
	}
	
	fs_new::synchronous_device_interface const & device	=	get_synchronous_device();
	
	fs_new::file_type * file;
	if ( !device->open(&file, destination_path, fs_new::file_mode::open_existing, fs_new::file_access::read) )
		return;
	
	device->seek(file,0,fs_new::seek_file_end);
	u32 read_data_size = (u32)device->tell(file);
	device->seek(file,0,fs_new::seek_file_begin);
	
	u32 resource_size = read_data_size + sizeof(particle_system);
	
	u8* resource_in_buffer	= UNMANAGED_ALLOC(u8, resource_size);
	u8* read_data_in_buffer	= resource_in_buffer + sizeof(particle_system);
	
	particle_system* ps	 = new(resource_in_buffer) particle_system;
	
	device->read( file, read_data_in_buffer, read_data_size );
	device->close( file );
	
	mutable_buffer load_buffer(read_data_in_buffer, read_data_size);
	ps->load_binary(load_buffer);
	
	resources::query_result_for_cook* parent	= data->query_result;
	parent->set_unmanaged_resource				( ps, resources::memory_type_non_cacheable_resource, sizeof(particle_system) );
	parent->finish_query						( result_success );

	UNMANAGED_DELETE(data);
}

void particle_system_cooker::request_convertion(fs_new::virtual_path_string* resource_path, resources::query_result_for_cook* parent)
{
	fs_new::virtual_path_string new_name;
	new_name.assignf			("resources/particles/%s.particle", resource_path->c_str());
	
	query_resource( new_name.c_str(), 
		resources::binary_config_class, 
		boost::bind( &particle_system_cooker::on_config_loaded, this, _1), 
		xray::resources::unmanaged_allocator(),
		NULL, 
		parent 
		);
}

void particle_system_cooker::on_config_loaded(resources::queries_result& result)
{
	using namespace fs_new;

	resources::query_result_for_cook * const	parent	=	result.get_parent_query();
	
	if (!result.is_successful())
	{
		parent->finish_query(result_error);
		return;
	}
	
	configs::binary_config_ptr config_ptr			= static_cast_checked<configs::binary_config *>( result[0].get_unmanaged_resource().c_ptr() );
	configs::binary_config_value const& config	= config_ptr->get_root();
	
	particle_system temp_resource;
	temp_resource.set_defaults();
	temp_resource.load_from_config(config, false);
	
	mutable_buffer temp_buffer;
	
	u32 write_data_size = temp_resource.save_binary(temp_buffer, true);
	u32 resource_size   = sizeof(particle_system) + write_data_size;
	
	u8* out_buffer	= UNMANAGED_ALLOC(u8, resource_size);
	xray::memory::zero(out_buffer, resource_size);
	
	u8* resource_out_buffer = out_buffer;
	u8* write_out_buffer = out_buffer + sizeof(particle_system);
	
	mutable_buffer save_buffer(write_out_buffer, write_data_size);
	temp_resource.save_binary(save_buffer, false);
	
	fs_new::virtual_path_string converted_res_path;
	converted_res_path.append("resources.converted/particles/");
	
	// Remove last word, "_static"
	fs_new::virtual_path_string request_path = parent->get_requested_path();
	buffer_string::size_type pos = request_path.find("_static");
	if (pos!=buffer_string::npos && pos==request_path.length()-strings::length("_static"))
		request_path.set_length( pos );
	
	converted_res_path.append(request_path.c_str());
	converted_res_path.append(".binary_particle");
	
	fs_new::native_path_string	destination_path;
	if ( !resources::convert_virtual_to_physical_path(& destination_path, converted_res_path, resources::sources_mount) )
	{
		parent->finish_query (result_error);
		return;
	}

	synchronous_device_interface const & device	=	get_synchronous_device();
	file_type * file;
	if ( !device->open(&file, destination_path, file_mode::create_always, file_access::write) )
	{
		parent->finish_query (result_error);
		return;
	}

	device->write( file, out_buffer + sizeof(particle_system), write_data_size);
	device->close( file );
	
	particle_system* new_resource	= new (resource_out_buffer) particle_system;
	
	mutable_buffer load_buffer(out_buffer + sizeof(particle_system), write_data_size);
	new_resource->load_binary(load_buffer);
	
	parent->set_unmanaged_resource( new_resource, resources::memory_type_non_cacheable_resource, sizeof(particle_system) );
	parent->finish_query( result_success );
}

void particle_system_cooker::delete_resource(resources::resource_base * resource)
{
	resource->~resource_base();
	UNMANAGED_FREE(resource);
}

xray::uninitialized_reference<particle_system_cooker>	s_particle_system_cook_object;

void register_particles_system_cooker()
{
	static bool particles_system_cooker_registered	= false;
	
	if (particles_system_cooker_registered) return;
	
	XRAY_CONSTRUCT_REFERENCE	( s_particle_system_cook_object, particle_system_cooker ) ( );
	resources::register_cook	( s_particle_system_cook_object.c_ptr() );
	
	particles_system_cooker_registered = true;
}

void unregister_particles_system_cooker()
{
	static bool particles_system_cooker_unregistered = false;
	
	if (particles_system_cooker_unregistered) return;
	
	resources::unregister_cook	( resources::particle_system_class );
	XRAY_DESTROY_REFERENCE		( s_particle_system_cook_object );
	
	particles_system_cooker_unregistered = true;
}

} // namespace particle
} // namespace xray
