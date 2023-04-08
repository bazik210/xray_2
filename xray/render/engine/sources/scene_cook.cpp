////////////////////////////////////////////////////////////////////////////
//	Created		: 21.03.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "scene_cook.h"
#include "scene_manager.h"
#include "scene.h"
#include <xray/render/facade/common_types.h>
#include <xray/particle/world.h>
#include <xray/render/core/res_effect.h>
#include <xray/render/core/res_texture.h>
#include <xray/render/core/res_geometry.h>
#include <xray/render/core/custom_config.h>
#include "material.h"
#include <xray/render/core/untyped_buffer.h>

namespace xray {
namespace render {

scene_cook::scene_cook():
	resources::translate_query_cook(xray::resources::scene_class, reuse_false, use_current_thread_id)
{}

void scene_cook::translate_query			( resources::query_result_for_cook&	in_out_query )
{	
	xray::render::scene_configuration renderer_configuration;
	
	if( in_out_query.user_data() )
		in_out_query.user_data()->try_get(renderer_configuration);
	
	scene* created_resource	= NEW(xray::render::scene)(renderer_configuration);
	scene_manager::ref().add_scene(created_resource);
	
	if (renderer_configuration.m_create_particle_world) 
	{
 		resources::user_data_variant data;
 		data.set( static_cast<particle::engine*>(&created_resource->m_particle_engine) );
 		resources::query_create_resource(
 			"",
			const_buffer("", 10*Mb), 
 			resources::particle_world_class, 
 			boost::bind(&scene_cook::on_particle_world_created, this, _1, created_resource, &in_out_query),
 			g_allocator,
 			&data,
			&in_out_query
 		);
 	}else
	{
		in_out_query.set_unmanaged_resource(created_resource, resources::nocache_memory, sizeof(xray::render::scene));
		in_out_query.finish_query(result_success);	
	}
}	

void scene_cook::on_particle_world_created(resources::queries_result& result, scene* created_resource, resources::query_result_for_cook* in_out_query)
{
	created_resource->m_particle_world	= static_cast_checked<xray::particle::world*>(result[0].get_unmanaged_resource().c_ptr());
	created_resource->m_particle_engine.set_particle_world( *created_resource->m_particle_world );
	in_out_query->set_unmanaged_resource(created_resource, resources::nocache_memory, sizeof(xray::render::scene));
	in_out_query->finish_query(result_success);	
}

void scene_cook::delete_resource(xray::resources::resource_base * resource)
{	
 	scene* scn = static_cast_checked<scene*>( resource );
	
	scene_manager::ref().remove_scene(scn);
	
	DELETE(scn);
}

} // namespace render
} // namespace xray