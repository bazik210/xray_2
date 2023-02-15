////////////////////////////////////////////////////////////////////////////
//	Created		: 06.10.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ogg_stereo_source_cook.h"
#include <xray/fs_utils.h>
#include "ogg_utils.h"

namespace xray {
namespace sound {

pcstr _stereo					= "_stereo";

ogg_stereo_source_cook::ogg_stereo_source_cook	( ) :
	super ( resources::ogg_stereo_raw_file, reuse_true, use_current_thread_id )
{}

void ogg_stereo_source_cook::translate_query			( resources::query_result_for_cook&	parent )
{
	fs::path_string	ogg_path		= parent.get_requested_path();

#ifdef MASTER_GOLD
	resources::query_resource(
		ogg_path.c_str(),
		resources::raw_data_class,
		boost::bind(&ogg_stereo_source_cook::on_ogg_file_loaded, this, _1, &parent),
		&memory::g_mt_allocator,
		NULL,
		& parent
	);
#else // #ifdef MASTER_GOLD

	fs_new::virtual_path_string	wav_path		= ogg_path;

	wav_path.replace				( _converted_local_path, _sound_sources_path );
	wav_path.replace				( _ogg_ext, "" );
	wav_path.append					( _wav_ext );

	// compare source and converted resource file age

	fs_new::physical_path_info const & source_file_info	=	resources::get_physical_path_info(wav_path, resources::sources_mount);

	R_ASSERT						( source_file_info.is_file() );

	fs_new::physical_path_info const & ogg_file_info	=	resources::get_physical_path_info(ogg_path, resources::converted_mount);

	if ( ogg_file_info.is_file() && source_file_info.last_time_of_write() < ogg_file_info.last_time_of_write() )
	{ 
		//use converted entry
		resources::query_resource(
			ogg_path.c_str(),
			resources::raw_data_class,
			boost::bind(&ogg_stereo_source_cook::on_ogg_file_loaded, this, _1, &parent ),
			&memory::g_mt_allocator,
			NULL,
			& parent
		);
	}
	else
	{ 
		fs_new::native_path_string input_file_name_disk_path;
		if ( !resources::convert_virtual_to_physical_path(&input_file_name_disk_path, wav_path, resources::sources_mount))
		{
			parent.finish_query	(result_error);
			return;
		}

		fs_new::virtual_path_string ogg_path		= parent.get_requested_path( );
//		change_substring				( ogg_path, _wav_ext, _ogg_ext );
		fs_new::native_path_string	output_file_name_disk_path;
		if ( !resources::convert_virtual_to_physical_path(&output_file_name_disk_path, ogg_path, resources::sources_mount))
		{
			parent.finish_query			( result_error );
			return;
		}

		fs_new::synchronous_device_interface const & device	=	get_synchronous_device();
		ogg_utils::encode_sound_file(
			input_file_name_disk_path.c_str(),
			output_file_name_disk_path.c_str(),
			device,
			16,
			2,
			44100,
			320
		);

		parent.finish_query			( result_requery );
	}
#endif // #ifdef MASTER_GOLD
}

void ogg_stereo_source_cook::delete_resource			( resources::resource_base* resource )
{
	XRAY_UNREFERENCED_PARAMETERS( resource );
	NOT_IMPLEMENTED				( );

}

void ogg_stereo_source_cook::on_ogg_file_loaded		( resources::queries_result& data,  resources::query_result_for_cook* parent_query )
{
	R_ASSERT							( data.is_successful() );
	parent_query->set_managed_resource	( data[0].get_managed_resource() );
	parent_query->finish_query			( result_success );
}

} // namespace sound
} // namespace xray