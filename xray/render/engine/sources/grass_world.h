////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_GRASS_WORLD_H_INCLUDED
#define XRAY_RENDER_GRASS_WORLD_H_INCLUDED

#include <xray/resources_intrusive_base.h>
#include <xray/resources_cook_classes.h>
#include "grass_template.h"
#include "grass_instance.h"

namespace xray {

namespace collision {
	struct space_partitioning_tree;
} // namespace collision

namespace render {

enum enum_render_stage_type;
class renderer_context;	
struct grass_instance;
struct grass_patch;

struct grass_world
{
	typedef render::vector<grass_template*>				grass_templates_type;
	typedef render::vector<grass_patch*>				grass_patches_type;
	
							grass_world					();
							~grass_world				();
	void					initialize_test_grass		(resources::query_result_for_cook& parent);
	void					on_sub_resources_loaded		(xray::resources::queries_result& data);
	void					add_template				(u32 const in_id, grass_render_model_ptr const& in_render_model);
	void					remove_template				(u32 const in_id);
	void					add_instance				(u32 const				in_id, 
														 u32 const				in_template_id, 
														 math::color const&		in_color, 
														 math::float4x4 const&	in_transform);
	void					remove_instance				(u32 const				in_id);
	
	void					populate					(float const patch_size_ground, float const patch_size_height);
	void					repopulate					();
	void					render						(renderer_context* context, 
														 math::float3 const& viewer_position,
														 enum_render_stage_type stage_type, 
														 u32 const				tech_index,
														 float const			draw_distance = 1000000.0f,
														 bool					stencil_mask = true);
	void					process_culling				(renderer_context* context,
														 float const first_lod_distance);
	
	void					render_debug				(renderer_context* context);
	
	grass_patch*			find_patch					(math::float3 const& point, grass_template* templ);
	u32						find_patches				(grass_template* templ, grass_patches_type& out_patches);
	//grass_patches_type
	
private:
	void					create_patch_render_buffers	();
	grass_template*			id_to_template				(u32 const id) const;
	
	grass_templates_type								m_templates;
	grass_patches_type									m_patches;
	grass_patches_type									m_visible_patches;
	grass_patches_type									m_changed_patches;
	grass_patches_type									m_patches_to_remove;
	grass_templates_type								m_changed_templates;
	collision::space_partitioning_tree*					m_patches_tree;
}; // struct grass_world

} // namespace render
} // namespace xray

#endif // #ifndef XRAY_RENDER_GRASS_WORLD_H_INCLUDED