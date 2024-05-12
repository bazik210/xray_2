////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Plichko Alexander
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "project_cooker_simple.h"
#include <xray/fs/device_utils.h>
#include "object_cooker.h"
#include "object_solid_visual.h"
#include "object_light.h"
#include "object_track.h"
#include "object_environment.h"
#include "object_sky.h"
#include "object_decal.h"
#include "object_collision_geometry.h"
#include "object_proximity_trigger.h"
#include "object_scene.h"
#include "object_composite.h"
#include "object_volumetric_sound.h"
#include "object_wire.h"
//#include "game_camera.h"
#include "game_world.h" 
#include "lobby_menu.h"

namespace stalker2 {

extern pcstr editor_project_ext;
extern pcstr resources_converted_path;
extern pcstr resources_path;

project_cooker_simple::project_cooker_simple( bool editor_present)
	:super				( resources::game_project_simple_class, reuse_true, use_any_thread_id ),
	m_editor_present	( editor_present )
{}

void project_cooker_simple::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string	project_name	= parent.get_requested_path();

#ifndef MASTER_GOLD	
	// compare source and converted resource file age
	fs_new::virtual_path_string		editor_proj_path;
	editor_proj_path.assignf		("%sprojects/%s/project.xprj", resources_path, project_name.c_str() );

	fs_new::virtual_path_string		game_proj_path;
	game_proj_path.assignf			("%sprojects/%s/project_simple", resources_converted_path, project_name.c_str() );

	resources::request r[] ={
		{ editor_proj_path.c_str(),		resources::fs_iterator_class },
		{ game_proj_path.c_str(),		resources::fs_iterator_class },
	};
	resources::query_resources(
		r,
		boost::bind( &project_cooker_simple::on_fs_iterators_ready,this, _1, &parent),
		g_allocator,
		NULL,
		&parent
		);
#else // ifndef MASTER_GOLD

	// directly load game project
	fs_new::virtual_path_string			game_proj_path;
	game_proj_path.assignf			("%sprojects/%s/project_simple", resources_converted_path, project_name.c_str() );

	resources::query_resource(
		game_proj_path.c_str(),
		resources::config_class,
		boost::bind( &project_cooker_simple::on_game_project_loaded, this, _1, &parent ),
		g_allocator,
		0,
		& parent
		);
#endif
}

#ifndef MASTER_GOLD
void project_cooker_simple::on_fs_iterators_ready( resources::queries_result& data, 
	resources::query_result_for_cook* parent )
{
	R_ASSERT						( data.is_successful() );
	// compare source and converted resource file age
	vfs::vfs_iterator it_editor_project		= data[0].get_result_iterator();
	vfs::vfs_iterator it_game_project		= data[1].get_result_iterator();

	fs_new::physical_path_info const& 	editor_project_info	=	resources::get_physical_path_info(it_editor_project);
	fs_new::physical_path_info const&	game_project_info	=	resources::get_physical_path_info(it_game_project);

	if(	!m_editor_present &&
		game_project_info.is_file() && // has converted entry
		editor_project_info.last_time_of_write() < game_project_info.last_time_of_write() )
	{// load game project
		resources::query_resource(	data[1].get_requested_path(),
			resources::config_class,
			boost::bind(&project_cooker_simple::on_game_project_loaded, this, _1, parent ),
			g_allocator,
			0,
			parent
			);
	}else
	{// load xprj
		resources::query_resource(	data[0].get_requested_path(),
			resources::lua_config_class,
			boost::bind(&project_cooker_simple::on_editor_project_loaded, this, _1, parent ),
			g_allocator,
			0,
			parent
			);
	}

}
#endif // #ifndef MASTER_GOLD

void project_cooker_simple::on_game_project_loaded( resources::queries_result& data, 
	resources::query_result_for_cook* parent )
{
	R_ASSERT(data.is_successful());

	resources::query_result_for_user const& result	= data[0];

	configs::binary_config_ptr game_proj_ptr	= static_cast_resource_ptr<configs::binary_config_ptr>( result.get_unmanaged_resource() );

	create_game_objects( game_proj_ptr, parent );

	parent->finish_query			( result_success );
}

#ifndef MASTER_GOLD
void project_cooker_simple::on_editor_project_loaded( resources::queries_result& data, 
	resources::query_result_for_cook* parent )
{
	R_ASSERT							( data.is_successful() );

	resources::query_result_for_user const& result	= data[0];

	configs::lua_config_ptr editor_proj_ptr	= static_cast_resource_ptr<configs::lua_config_ptr>( result.get_unmanaged_resource() );

	make_game_project					( editor_proj_ptr, parent );
}

void process_object	(	configs::lua_config_value const& current,
						configs::lua_config_value& t_res_project_objects,
						pcstr folder_name)
{
	// not supported in game object(s)
	if(!current.value_exists("game_object_type"))
		return;

	pcstr name					= current["name"];

	fs::path_string full_name	= folder_name; 
	full_name.append			( name );

	pcstr game_object_type = current["game_object_type"];
	if(strings::equal(game_object_type, "camera"))
	{
		// do not process camera
	}else
	if(strings::equal(game_object_type, "scene"))
	{
		// do not process scene
	}else
	{
		int count							= t_res_project_objects.size();
		configs::lua_config_value current_copy = current.copy();
		t_res_project_objects[count].assign_lua_value	( current_copy );
		t_res_project_objects[count]["full_name"]		= full_name.c_str();
	}
}

void process_folder( configs::lua_config_value const& t_folders,
					configs::lua_config_value const& t_objects,
					configs::lua_config_value& t_res_project_objects,
					pcstr key,
					pcstr folder_name )
{
	configs::lua_config_value const& t_root = t_folders[key];
	configs::lua_config_iterator it			= t_root["subitems"].begin();
	configs::lua_config_iterator it_e		= t_root["subitems"].end();

	for(; it!=it_e; ++it)
	{
		//configs::lua_config_value const& t_current	= *it;
		pcstr guid						= it.key();
		pcstr type						= *it;

		if(0==strings::compare(type, "folder"))
		{
			fixed_string1024 sub_folder_name = folder_name;
			sub_folder_name					+= (pcstr)t_folders[guid]["name"];
			sub_folder_name					+= "/";

			process_folder	
			(	
				t_folders,
				t_objects,
				t_res_project_objects,
				guid,
				sub_folder_name.c_str()
			);
		}
		else
		{ // 
			R_ASSERT(strings::equal(type, "object"));

			configs::lua_config_value const& t_current = t_objects[guid];
			
			pcstr tool_name			= "";
			if (t_current.value_exists("tool_name"))
				tool_name			= t_current["tool_name"];

			
			if(0==strings::compare(tool_name, "composite"))
			{				
				process_object		(	t_current, 
					t_res_project_objects, 
					folder_name );

				configs::lua_config_iterator objects_it			= t_current["objects"].begin();
				configs::lua_config_iterator objects_it_e		= t_current["objects"].end();

				for(; objects_it!=objects_it_e; ++objects_it)
				{
					configs::lua_config_value t_current_object = *objects_it;

					fixed_string1024 sub_folder_name	= folder_name;
					sub_folder_name						+= (pcstr)t_current["name"];	
					sub_folder_name						+= "/";	

					process_object	( t_current_object, 
						t_res_project_objects, 
						sub_folder_name.c_str() );
				}
			}
			else
			{
				process_object	(	t_current, 
					t_res_project_objects, 
					folder_name );
			}
		}// object
	} //  for
}

void project_cooker_simple::make_game_project( configs::lua_config_ptr& editor_config, 
									   resources::query_result_for_cook* parent_query )
{
	fs_new::virtual_path_string project_name		= parent_query->get_requested_path();
	fs_new::virtual_path_string cell_prefix			= project_name;

	configs::lua_config_ptr	game_config_lua	= configs::create_lua_config( project_name.c_str() );

	configs::lua_config_value game_project_root			= ( *game_config_lua );
	configs::lua_config_value editor_project_root		= ( *editor_config );

	// startup camera
	game_project_root["camera"].assign_lua_value	( editor_project_root["camera"]/*.copy()*/ );

	configs::lua_config_value const& t_folders		= editor_project_root["folders"];
	configs::lua_config_value const& t_objects		= editor_project_root["objects"];
	configs::lua_config_value game_config_objects	= game_project_root["objects"];
	configs::lua_config_value& t_res_project_objects= game_config_objects;

	timing::timer t;
	t.start();
	process_folder						
	(	
		t_folders, 
		t_objects,
		t_res_project_objects,
		"00000000-0000-0000-0000-000000000000",
		""	
	);

	LOG_INFO("make game project take %f sec", t.get_elapsed_sec());


	// save game project to lua file
	fs_new::virtual_path_string		path_to_save;
	path_to_save.appendf			( "%sprojects/%s/project_simple", resources_converted_path, project_name.c_str() );
	fs_new::native_path_string		path_to_save_disk_path;
	bool need_save_to_file			= true;//!m_editor_present;


	if(need_save_to_file)
	{
		bool convert_result = resources::convert_virtual_to_physical_path( &path_to_save_disk_path, path_to_save, resources::sources_mount );
		R_ASSERT						( convert_result );
		fs_new::synchronous_device_interface const & device	=	get_synchronous_device();
		fs_new::create_folder_r			( device, path_to_save_disk_path.c_str(), false );
		game_config_lua->save_as_binary	( path_to_save_disk_path.c_str() );

#if 1
		fs_new::native_path_string	lua_fn	= path_to_save_disk_path;
		lua_fn							+= ".lua";
		game_config_lua->save_as		( lua_fn.c_str(), configs::target_sources );
#endif
	}

	xray::configs::binary_config_ptr resource_config_ptr	= game_config_lua->get_binary_config();
	create_game_objects( resource_config_ptr, parent_query );
}
#endif

void project_cooker_simple::create_game_objects( xray::configs::binary_config_ptr project_cfg, resources::query_result_for_cook* parent_query )
{

	simple_game_project*	proj		= NEW(simple_game_project)();
	proj->m_config = project_cfg;

	lobby_menu* lobby;

	if( parent_query->user_data() )
		parent_query->user_data()->try_get(lobby);

	configs::binary_config::const_iterator it	= (*project_cfg)["objects"].begin();
	configs::binary_config::const_iterator it_e = (*project_cfg)["objects"].end();

	for ( ;it!=it_e;++it ){
		pcstr type								= (*it)["game_object_type"];

		int load = 1;

		game_object_*	resource				= NULL;
		
		if( strings::equal("solid_visual", type))
		{
			resource = NEW(object_solid_visual)( *lobby );
		}else 
		if( strings::equal("dynamic_visual", type))
		{
			resource = NEW(object_dynamic_visual)( *lobby );
		}else 
		if( strings::equal("skeleton_visual", type))
		{
			resource = NEW(object_skeleton_visual)( *lobby );
		}else 
		if( strings::equal("environment", type))
		{
			resource = NEW(object_environment)( *lobby );
		}else 
		if( strings::equal("sky", type))
		{
			resource = NEW(object_sky)( *lobby );
		}else 
		if( strings::equal("decal", type))
		{
			resource = NEW(object_decal)( *lobby );
		}else 
		if( strings::equal("light", type))
		{
			resource = NEW(object_light)( *lobby );
		}else
		if( strings::equal("particle", type))
		{
			resource = NEW(object_particle_visual)( *lobby );
		}else
		if( strings::equal("speedtree", type))
		{
			resource = NEW(object_speedtree_visual)( *lobby );
		}else
		if( strings::equal("collision_geometry", type))
		{
			resource = NEW(object_collision_geometry)( *lobby );
		}else
		if( strings::equal("composite", type))
		{
			resource = NEW(object_composite)( *lobby );
		}else
		if( strings::equal("volumetric_sound", type))
		{
			resource = NEW(object_volumetric_sound)( *lobby );
		}else	
		if( strings::equal("wire_set", type))
		{
			resource = NEW(object_wire)( *lobby );
		}else	
		{
			load = 0;
			//NOT_IMPLEMENTED				( );
		}

		if (load) {
			resource->load(*it);

			proj->m_objects.push_back(resource);
		}
		load = 1;

	}

	parent_query->set_unmanaged_resource	( proj, resources::nocache_memory, sizeof(simple_game_project) );

	parent_query->finish_query	( result_success );	
}

void project_cooker_simple::delete_resource( resources::resource_base * resource )
{
	DELETE( resource );
}


} // namespace stalker2