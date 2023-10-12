////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "grass_world.h"
#include "render_model_grass.h"
#include "grass_template.h"
#include <xray/collision/space_partitioning_tree.h>
#include <xray/render/core/resource_manager.h>
#include <xray/render/facade/render_stage_types.h>
#include <xray/render/core/res_geometry.h>
#include <xray/render/core/backend.h>
#include "renderer_context.h"
#include "geometry_type.h"
#include <xray/render/core/res_effect.h>
#include <xray/render/core/custom_config.h>
#include <xray/math_randoms_generator.h>
#include "system_renderer.h"
#include "statistics.h"
#include "grass_patch.h"
#include <xray/console_command.h>

namespace xray {
namespace render {


static bool s_draw_grass_debug_value				=	false;
static xray::console_commands::cc_bool s_draw_grass_debug("draw_grass_debug", s_draw_grass_debug_value, false, xray::console_commands::command_type_engine_internal);

grass_world::grass_world()
{
	m_patches_tree									=	&*collision::new_space_partitioning_tree(g_allocator, 1.f, 1024);
}

grass_world::~grass_world()
{
	grass_patches_type::const_iterator it_p			=	m_patches.begin();
	grass_patches_type::const_iterator end_p		=	m_patches.end();
	
	for (; it_p != end_p; ++it_p)
	{
		grass_patch* patch							=	*it_p;
		DELETE										(patch);
	}
	
	grass_templates_type::iterator	it_t			=	m_templates.begin();
	grass_templates_type::iterator	end_t			=	m_templates.end();
	
	for (; it_t != end_t; ++it_t)
	{
		grass_template* templ						=	(*it_t);
		
		DELETE										(templ);
	}
	
	collision::delete_space_partitioning_tree		(m_patches_tree);
}

void grass_world::add_template(u32 const in_id, grass_render_model_ptr const& in_render_model)
{
	m_templates.push_back							(NEW(grass_template)(in_id, in_render_model));
}

void grass_world::remove_template(u32 const in_id)
{
	grass_templates_type::iterator	it_t			=	m_templates.begin();
	grass_templates_type::iterator	end_t			=	m_templates.end();
	
	for (; it_t != end_t; ++it_t)
	{
		grass_template* templ						=	*it_t;
		if (templ->m_index == in_id)
		{
			DELETE									(templ);
			m_templates.erase						(it_t);
			
#ifndef MASTER_GOLD
//			find_patches							(templ, m_patches_to_remove);
#endif // #ifndef MASTER_GOLD
			
			break;
		}
	}
}

void grass_world::add_instance(u32 const				in_id, 
							   u32 const				in_template_id, 
							   math::color const&		in_color, 
							   math::float4x4 const&	in_transform)
{
	grass_template* templ							=	id_to_template(in_template_id);
	ASSERT_CMP										(templ, !=, NULL, "Adding instance before template!");
	
	if (templ)
	{
#ifndef MASTER_GOLD
//		grass_patch* found_path						=	find_patch(in_transform.lines[3].xyz(), templ);
//		
//		if (found_path)
//			m_changed_patches.push_back				(found_path);
//		else
//		{
//			
//		}
#endif // #ifndef MASTER_GOLD		
		
		//if (std::find(m_changed_templates.begin(), m_changed_templates.end(), templ) == m_changed_templates.end())
		//	m_changed_templates.push_back			(templ);
		
		templ->m_instances.push_back				(
			NEW(grass_instance)(
				in_id, 
				templ, 
				in_color, 
				in_transform
			)
		);
	}
}

void grass_world::remove_instance(u32 const	in_id)
{
	grass_templates_type::const_iterator	it		=	m_templates.begin();
	grass_templates_type::const_iterator	end		=	m_templates.end();
	
	for (; it != end; ++it)
	{
		grass_template* templ						=	*it;
		
		grass_instances_type::iterator it_instance	=	templ->m_instances.begin();
		grass_instances_type::iterator end_instance	=	templ->m_instances.end();
		
		for (; it_instance != end_instance; ++it_instance)
		{
			grass_instance* instance				=	*it_instance;
			
			if (instance->m_index == in_id)
			{
#ifndef MASTER_GOLD
//				grass_patch* found_path				=	find_patch(instance->m_transform.lines[3].xyz(), templ);
//				
//				if (found_path)
//					m_changed_patches.push_back		(found_path);
#endif // #ifndef MASTER_GOLD
				DELETE								(instance);
				templ->m_instances.erase			(it_instance);
				return;
			}
		}
	}
}

grass_template* grass_world::id_to_template(u32 const id) const
{
	grass_template* result							=	NULL;
	grass_templates_type::const_iterator	it		=	m_templates.begin();
	grass_templates_type::const_iterator	end		=	m_templates.end();
	
	for (; it != end; ++it)
	{
		result										=	(*it);
		
		if (result->m_index == id)
			break;
	}
	
	return result;
}

u32	grass_world::find_patches(grass_template* templ, grass_world::grass_patches_type& out_patches)
{
	grass_patches_type::const_iterator it			=	m_patches.begin();
	grass_patches_type::const_iterator end			=	m_patches.end();
	
	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		if (templ == patch->m_template)
		{
			out_patches.push_back					(patch);
		}
	}
	return out_patches.size();
}

grass_patch* grass_world::find_patch(math::float3 const& point, grass_template* templ)
{
	grass_patches_type::const_iterator it			=	m_patches.begin();
	grass_patches_type::const_iterator end			=	m_patches.end();
	
	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		if (patch->m_aabb.min <= point && 
			patch->m_aabb.max >= point && 
			templ == patch->m_template)
		{
			return patch;
		}
	}
	return NULL;
}

void grass_world::repopulate()
{
 	grass_patches_type::const_iterator it_p			=	m_patches_to_remove.begin();
 	grass_patches_type::const_iterator end_p		=	m_patches_to_remove.end();
 	
 	for (; it_p != end_p; ++it_p)
 	{
 		grass_patch* patch							=	*it_p;
		DELETE										(patch);
 	}
	m_patches_to_remove.clear						();
}

void grass_world::populate(float const patch_size_ground, float const patch_size_height)
{
// 	grass_patches_type::const_iterator it_p			=	m_patches.begin();
// 	grass_patches_type::const_iterator end_p		=	m_patches.end();
// 	
// 	for (; it_p != end_p; ++it_p)
// 	{
// 		grass_patch* patch							=	*it_p;
// 		DELETE										(patch);
// 	}
	
// 	struct patch_info
// 	{
// 		grass_template*			templ;
// 		grass_instances_type	instances;
// 		math::float3			origin_aligned;
// 	};
// 	
// 	typedef render::vector<patch_info> patch_infos_type;
// 	
// 	patch_infos_type patch_infos;
// 	
	grass_templates_type::iterator it				=	m_templates.begin();
	grass_templates_type::iterator end				=	m_templates.end();
	
	for (; it != end; ++it)
	{
		grass_template* templ						=	*it;
		
		grass_instances_type::iterator it_instance	=	templ->m_instances.begin();
		grass_instances_type::iterator end_instance	=	templ->m_instances.end();
		
		//u32 const num_instances						=	templ->m_instances.size();
		
		for (; it_instance != end_instance; ++it_instance)
		{
			grass_instance* instance				=	*it_instance;
			math::float3 const origin				=	instance->m_transform.lines[3].xyz();
			
			math::float3 const origin_aligned		=	float3(
															(float)math::floor(origin.x / patch_size_ground),
															(float)math::floor(origin.y / patch_size_height),
															(float)math::floor(origin.z / patch_size_ground)
														) * float3(patch_size_ground, patch_size_height, patch_size_ground);
			
			
			grass_patch* found_patch				=	find_patch(origin, templ);
			
			if (!found_patch)
			{
				grass_patch* new_patch				=	NEW(grass_patch)(
															m_patches_tree, 
															templ,
															origin_aligned, 
															float3(patch_size_ground, patch_size_height, patch_size_ground)
														);
				
				m_patches.push_back					(new_patch);
				
				new_patch->m_instances.push_back	(instance);
			}
			else
			{
				found_patch->m_instances.push_back	(instance);
			}
		}
	}
	
	create_patch_render_buffers						();
}

void grass_world::create_patch_render_buffers()
{
	grass_patches_type::const_iterator it			=	m_patches.begin();
	grass_patches_type::const_iterator end			=	m_patches.end();
	
	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		
		patch->create_render_buffer					();
	}
}

void grass_world::render_debug(renderer_context* context)
{
	if (!s_draw_grass_debug_value)
		return;
	
	math::float3 const& view_position				=	context->get_view_pos();
	
	grass_patches_type::const_iterator it			=	m_visible_patches.begin();
	grass_patches_type::const_iterator end			=	m_visible_patches.end();
	
	for (; it != end; ++it)
	{
		grass_patch* patch							=	*it;
		
		system_renderer::ref().draw_aabb			(patch->m_aabb, math::color(1.0f, 0.3f, 0.3f, 1.0f));
		
		grass_instances_type::iterator it_instance	=	patch->m_instances.begin();
		grass_instances_type::iterator end_instance	=	patch->m_instances.end();
		
		for (; it_instance != end_instance; ++it_instance)
		{
			grass_instance* instance				=	*it_instance;
			math::float3 origin						=	instance->m_transform.lines[3].xyz();
			
			if (math::squared_length(view_position - patch->m_origin) < 30.0f * 30.0f)
			{
				system_renderer::ref().draw_aabb	(
					math::create_aabb_min_max(
						origin - 0.5f * float3(0.25f, 0.25f, 0.25f), 
						origin + float3(0.25f, 0.25f, 0.25f)
					), 
					math::color(0.3f, 1.0f, 0.3f, 1.0f)
				);
			}
		}
	}
}

void grass_world::process_culling(renderer_context*	context, 
								  float const		first_lod_distance)
{
	m_visible_patches.clear							();
	m_visible_patches.reserve						(256);
	
	statistics::ref().grass_stat_group.num_total_patches.value	=	m_patches.size();
	
	collision::objects_type objects					=	g_allocator;
	objects.reserve									(m_patches.size());
	
	math::frustum view_frustum						(context->get_vp());
	
	m_patches_tree->cuboid_query					(u32(-1), view_frustum, objects );
	
	float const first_lod_distance_squared			=	first_lod_distance * first_lod_distance;
	
	for (collision::objects_type::const_iterator it = objects.begin(), end = objects.end(); it != end; ++it)
	{
		grass_patch* patch							=	static_cast<grass_patch*>((*it)->user_data());
		
		float const to_aabb_center_squared			=	math::squared_length(context->get_view_pos() - patch->m_aabb.center());
		
		if (first_lod_distance_squared < to_aabb_center_squared)
			continue;
		
		m_visible_patches.push_back					(patch);
	}
}

void grass_world::render(renderer_context*			context, 
						 math::float3 const&		viewer_position,
						 enum_render_stage_type		stage_type, 
						 u32 const					tech_index,
						 float const				draw_distance,
						 bool						stencil_mask)
{
	typedef grass_render_model::children::iterator render_models_iterator_type;
	
	grass_patches_type::const_iterator it_patch		=	m_visible_patches.begin();
	grass_patches_type::const_iterator end_patch	=	m_visible_patches.end();
	
	float const draw_distance_squared				=	draw_distance * draw_distance;
	
	for (; it_patch != end_patch; ++it_patch)
	{
		grass_patch* patch							=	*it_patch;

		grass_template& templ						=	*patch->m_template;
		
		render_models_iterator_type it_s			=	templ.m_render_model->m_children.begin();
		render_models_iterator_type it_s_end		=	templ.m_render_model->m_children.end();
		
		for (; it_s != it_s_end; ++it_s)
		{
			grass_render_surface* surface			= *it_s;
			
			if (!surface->get_material_effects().m_effects[stage_type].c_ptr())
				continue;
			
			float const to_aabb_center_squared		=	math::squared_length(viewer_position - patch->m_aabb.center());
			
			if (draw_distance_squared < to_aabb_center_squared)
				continue;
			
			if (tech_index == 0)
				statistics::ref().grass_stat_group.num_visible_patches.value++;
			
			statistics::ref().grass_stat_group.num_rendered_patches.value++;
			
			surface->get_material_effects().m_effects[stage_type]->apply(tech_index);
			
			surface->m_render_geometry.geom->apply	();
			
			backend::ref().set_declaration			(&*patch->m_instance_decl);
			backend::ref().set_vb_instance_data		(&*patch->m_instance_vb, patch->get_instance_data_stride(), 0);
			
			context->set_w							(float4x4().identity());
			
			if (tech_index == 0 && stencil_mask)
				backend::ref().set_stencil_ref		(all_geometry_type + static_geometry_type);
			
			backend::ref().render_indexed_instanced	(
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
				surface->m_render_geometry.primitive_count * 3, 
				0, 
				0,
				patch->m_instances.size(),
				0
			);
		}
	}
}


void grass_world::initialize_test_grass(resources::query_result_for_cook& parent)
{
	resources::request requests[]					=	{
														 {"resources/models/nature/bush/grass_small_01.model/render", resources::grass_render_model_class},
														 {"resources/models/nature/bush/grass_small_02.model/render", resources::grass_render_model_class},
														 {"resources/models/nature/bush/grass_small_03.model/render", resources::grass_render_model_class},
														 {"resources/models/nature/bush/grass_small_04.model/render", resources::grass_render_model_class},
														 {"resources/models/nature/bush/grass_small_05.model/render", resources::grass_render_model_class},
														 {"resources/models/nature/bush/grass_small_06.model/render", resources::grass_render_model_class},
														};

	query_resources									(
		requests,
		boost::bind(&grass_world::on_sub_resources_loaded, this, _1), 
		render::g_allocator, 
		NULL
	);
}


void grass_world::on_sub_resources_loaded(xray::resources::queries_result& data)
{
	u32 const num_requests							=	data.size();

	for (u32 request_index = 0; request_index < num_requests; request_index++)
	{
		if (data[request_index].is_successful())
		{
			add_template							(
				request_index, 
				static_cast_resource_ptr<grass_render_model_ptr>(data[request_index].get_unmanaged_resource())
			);
		}
	}

	math::random32									r(1000);
	
	u32 idx											=	0;

	for (int i = 0; i < 6; i++)
	for (int y = 0; y < 128; y++)
	{
		for (int x = 0; x < 128; x++)
		{
			float4x4 translation_matrix				=	math::create_translation(float3(64.0f * i + 0.5f * x + r.random_f(0.5f) - 0.25f, 0.0f, 0.5f * y + r.random_f(0.5f) - 0.25f));
			float4x4 rotation_matrix				=	math::create_rotation(float3(0.0f, 1.0f, 0.0f), r.random_f(math::pi_x2));
			float4x4 scale_matrix					=	math::create_scale(float3(r.random_f(0.5f) + 0.75f, r.random_f(1.0f) + 0.75f, r.random_f(0.5f) + 0.75f));
			
			add_instance							(
				idx++, 
				i, 
				math::color(
					r.random_f(0.1f) + 0.25f, 
					r.random_f(0.35f) + 0.5f, 
					r.random_f(0.1f) + 0.25f, 
					1.0f
				), 
				scale_matrix * rotation_matrix * translation_matrix
			);
		}
	}
	
	populate										(16.0f, 2.0f);

}

} // namespace render
} // namespace xray
