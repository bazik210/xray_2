////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "grass_patch.h"
#include "grass_instance.h"
#include <xray/collision/api.h>
#include <xray/collision/space_partitioning_tree.h>
#include <xray/render/core/res_geometry.h>
#include <xray/render/core/custom_config.h>
#include <xray/render/core/resource_manager.h>
#include <xray/render/facade/render_stage_types.h>
#include <xray/render/core/res_geometry.h>
#include <xray/render/core/backend.h>
#include "renderer_context.h"
#include "geometry_type.h"
#include <xray/render/core/res_effect.h>

namespace xray {
namespace render {


const D3D_INPUT_ELEMENT_DESC grass_instanced_layout[] = 
{
	{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",			0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 12,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",			0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"BINORMAL",		0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 20, 	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, 	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"WorldTransform",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 0,	D3D_INPUT_PER_INSTANCE_DATA, 1},
	{"WorldTransform",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16,	D3D_INPUT_PER_INSTANCE_DATA, 1},
	{"WorldTransform",	2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 32,	D3D_INPUT_PER_INSTANCE_DATA, 1},
	{"GrassColor",		0, DXGI_FORMAT_R8G8B8A8_UNORM,		1, 48,	D3D_INPUT_PER_INSTANCE_DATA, 1},
};

struct grass_instance_data
{
	math::float4		world_transform0;
	math::float4		world_transform1;
	math::float4		world_transform2;
	math::color			grass_color;
}; // struct grass_instance_data


grass_patch::grass_patch(collision::space_partitioning_tree* const in_collision_tree,
						 grass_template* templ,
						 math::float3 const& in_origin,
						 math::float3 const& in_sizes):
	m_collision_tree	(in_collision_tree),
	m_template			(templ),
	m_aabb				(math::create_identity_aabb()),
	m_collision_object	(NULL),
	m_collision_geometry(NULL),
	m_visible			(true),
	m_origin			(in_origin),
	m_sizes				(in_sizes)
{
	math::float4x4 const transform				=	math::create_scale(m_sizes * 0.5f) * math::create_translation(m_origin + m_sizes * 0.5f);
	m_collision_geometry						=	&*collision::new_box_geometry_instance(g_allocator, transform);
	m_collision_object							=	&*collision::new_collision_object(g_allocator, 1, m_collision_geometry, this);
	
	m_collision_tree->insert					(m_collision_object, transform);
	m_aabb.modify								(transform);
	
	m_instance_decl								=	resource_manager::ref().create_declaration(grass_instanced_layout);
}

grass_patch::~grass_patch()
{
	if (m_collision_tree && m_collision_object)
		m_collision_tree->erase					(m_collision_object);
	
	collision::delete_object					(g_allocator, m_collision_object);
	collision::delete_geometry_instance			(g_allocator, m_collision_geometry);	
}

u32	grass_patch::get_instance_data_stride() const
{
	return sizeof(grass_instance_data);
}

void grass_patch::create_render_buffer()
{
	u32 const num_instances						=	m_instances.size();
	
	if (!num_instances)
		return;
	
	grass_instance_data* instance_data			=	NEW_ARRAY(grass_instance_data, num_instances);
	
	grass_instances_type::const_iterator it		=	m_instances.begin();
	grass_instances_type::const_iterator end	=	m_instances.end();
	
	u32 index									=	0;
	for (; it != end; ++it)
	{
		grass_instance& current_instance		=	**it;
		grass_instance_data& current_data		=	instance_data[index];
		
		current_data.world_transform0			=	float4(current_instance.m_transform.lines[0].xyz(), current_instance.m_transform.lines[3].x);
		current_data.world_transform1			=	float4(current_instance.m_transform.lines[1].xyz(), current_instance.m_transform.lines[3].y);
		current_data.world_transform2			=	float4(current_instance.m_transform.lines[2].xyz(), current_instance.m_transform.lines[3].z);
		current_data.grass_color				=	current_instance.m_color;
		
		index++;
	}
	
	m_instance_vb								=	resource_manager::ref().create_buffer(
														num_instances * sizeof(grass_instance_data), 
														instance_data, 
														enum_buffer_type_vertex, 
														false
													);
	
	DELETE_ARRAY								(instance_data);
}

} // namespace render
} // namespace xray

