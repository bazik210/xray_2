////////////////////////////////////////////////////////////////////////////
//	Created		: 26.05.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "scene_renderer_impl.h"
#include <xray/render/engine/world.h>
#include "command_delegate_ex.h"
#include "command_add_model.h"
#include "command_update_model.h"
#include "command_remove_model.h"
#include "command_update_model_vertex_buffer.h"
#include "command_add_light.h"
#include "command_update_light.h"
#include "command_remove_light.h"
#include "command_terrain_add_cell.h"
#include "command_terrain_remove_cell.h"
#include "command_functor.h"
#include "command_view_matrix.h"
#include "command_projection_matrix.h"
#include "command_play_particle_system.h"
#include <xray/render/base/one_way_render_channel.h>
#include <xray/render/base/debug_renderer.h>
#include <xray/render/base/scene_view_mode.h>
#include <xray/render/base/render_stage_types.h>

using xray::render::scene_renderer_impl;
using xray::render::scene_ptr;
using xray::render::scene_view_ptr;
using xray::render::render_output_window_ptr;
using xray::render::render_model_instance_base_ptr;
using xray::math::float4x4;
using xray::particle::particle_system_instance_ptr;

scene_renderer_impl::scene_renderer_impl	(
		xray::render::one_way_render_channel&	channel,
		xray::memory::base_allocator&			allocator,
		xray::render::engine::world&			render_engine_world
	) :
	m_render_engine_world	( render_engine_world ),
	m_channel				( channel ),
	m_allocator				( allocator )
{
}

void scene_renderer_impl::set_view_matrix			( scene_view_ptr const& scene_view, float4x4 const& view )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_view_matrix ) ( m_render_engine_world, scene_view, view ) );
}

void scene_renderer_impl::set_projection_matrix	( scene_view_ptr const& scene_view, float4x4 const& projection )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_projection_matrix) ( m_render_engine_world, scene_view, projection) );
}

void scene_renderer_impl::present	( render_output_window_ptr const& render_output_window, xray::render::viewport_type const& viewport )
{
	R_ASSERT	( render_output_window );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_delegate_ex< viewport_type > ) ( boost::bind( &xray::render::engine::world::present, &m_render_engine_world, render_output_window, _1), viewport ) );
}

void scene_renderer_impl::add_model	( scene_ptr const& scene, render_model_instance_base_ptr const& v, float4x4 const& transform )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( XRAY_NEW_IMPL( m_allocator, command_delegate_ex< float4x4 > ) ( boost::bind( &xray::render::engine::world::add_model, &m_render_engine_world, scene, v, _1, true), transform ) );
}

void scene_renderer_impl::remove_model( scene_ptr const& scene, render_model_instance_base_ptr const& model ) 
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &xray::render::engine::world::remove_model, &m_render_engine_world, scene, model)));
}

void scene_renderer_impl::update_model( scene_ptr const& scene, render_model_instance_base_ptr const& render_model, float4x4 const& model_to_world ) 
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( XRAY_NEW_IMPL( m_allocator, command_delegate_ex< float4x4 > ) ( boost::bind( &xray::render::engine::world::update_model, &m_render_engine_world, scene, render_model, _1), model_to_world));
}

void scene_renderer_impl::update_skeleton( render_model_instance_base_ptr const& v, float4x4 const* matrices, u32 count )
{
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_update_skeleton ) ( m_render_engine_world, v, matrices, count ) );
}

void scene_renderer_impl::play_particle_system			( scene_ptr const& scene, particle_system_instance_ptr const& in_instance, xray::float4x4 const& transform )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_play_particle_system ) ( &m_render_engine_world, scene, in_instance, transform, true, false ) );
}

void scene_renderer_impl::stop_particle_system( scene_ptr const& scene, particle_system_instance_ptr const& in_instance )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::stop_particle_system, &m_render_engine_world, scene, in_instance) ));
}

void scene_renderer_impl::remove_particle_system_instance( scene_ptr const& scene, particle_system_instance_ptr const& in_instance )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::remove_particle_system_instance, &m_render_engine_world, in_instance, scene) ));
}

void scene_renderer_impl::update_particle_system_instance( scene_ptr const& scene, particle_system_instance_ptr const& instance, xray::float4x4 const& transform, bool visible, bool paused, bool selected )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_delegate_ex< math::float4x4 > ) ( boost::bind( &engine::world::update_particle_system_instance, &m_render_engine_world, instance, scene, _1, visible, paused, selected), transform ) );
}

void scene_renderer_impl::update_model_vertex_buffer( render_model_instance_base_ptr const& object, xray::vectora<xray::render::buffer_fragment> const& fragments )
{
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_update_model_vertex_buffer ) ( m_render_engine_world, object, fragments, *xray::render::editor::g_allocator ) );
}

void scene_renderer_impl::add_light( scene_ptr const& scene, u32 id, xray::render::light_props const& props)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_add_light ) ( m_render_engine_world, scene, id, props) );
}

void scene_renderer_impl::update_light( scene_ptr const& scene, u32 id, xray::render::light_props const& props)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_update_light ) ( m_render_engine_world, scene, id, props) );
}

void scene_renderer_impl::remove_light( scene_ptr const& scene, u32 id)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_remove_light ) ( m_render_engine_world, scene, id) );
}

void scene_renderer_impl::terrain_add_cell( scene_ptr const& scene, render_model_instance_base_ptr const& v )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_terrain_add_cell ) ( m_render_engine_world, scene, v) );
}

void scene_renderer_impl::terrain_remove_cell( scene_ptr const& scene, render_model_instance_base_ptr const& v )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_terrain_remove_cell ) ( m_render_engine_world, scene, v) );
}

void scene_renderer_impl::set_slomo					( scene_ptr const& scene, float time_multiplier )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::set_slomo, &m_render_engine_world, scene, time_multiplier) ));
}

void scene_renderer_impl::set_post_process			( scene_view_ptr const& scene_view, xray::resources::unmanaged_resource_ptr const& post_process_resource)
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::set_post_process, &m_render_engine_world, scene_view, post_process_resource) ));
}

void scene_renderer_impl::resize_output_window		( render_output_window_ptr const& render_output_window /*in*/ )
{
	R_ASSERT	( render_output_window );
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &xray::render::engine::world::resize_output_window, &m_render_engine_world, render_output_window) ) );
}

void scene_renderer_impl::reload_shaders			( )
{
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &xray::render::engine::world::reload_shaders, &m_render_engine_world) ) );
}

void scene_renderer_impl::reload_modified_textures	( )
{
	m_channel.owner_push_back	( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &xray::render::engine::world::reload_modified_textures, &m_render_engine_world) ) );
}

void scene_renderer_impl::draw_render_statistics( xray::ui::world& ui_world )
{
	m_render_engine_world.draw_render_statistics	( &ui_world );
}

void scene_renderer_impl::set_model_material(
		render_model_instance_base_ptr const& v,
		xray::fs::path_string const& subsurface_name,
		xray::resources::unmanaged_resource_ptr const& mtl_ptr
	)
{
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_delegate_ex<fs::path_string>) ( boost::bind( &render::engine::world::set_model_material, &m_render_engine_world, v, _1, mtl_ptr), subsurface_name ) );
}

void scene_renderer_impl::set_model_visible( render_model_instance_base_ptr const& v, xray::fs::path_string const& subsurface_name, bool value)
{
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_delegate_ex<fs::path_string>) ( boost::bind( &render::engine::world::set_model_visible, &m_render_engine_world, v, _1, value), subsurface_name ) );
}

void scene_renderer_impl::set_view_mode( scene_view_ptr const& scene_view, xray::render::scene_view_mode const& view_mode )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::set_view_mode, &m_render_engine_world, scene_view, view_mode) ));
}

void scene_renderer_impl::toggle_render_stage( xray::render::enum_render_stage_type const& stage_type, bool toggle )
{
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::toggle_render_stage, &m_render_engine_world, stage_type, toggle) ));
}

void scene_renderer_impl::set_particles_render_mode( scene_view_ptr const& scene_view, xray::particle::enum_particle_render_mode const& render_mode )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::set_particles_render_mode, &m_render_engine_world, scene_view, render_mode) ));
}

void scene_renderer_impl::enable_post_process( scene_view_ptr const& scene_view, bool enable )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::enable_post_process, &m_render_engine_world, scene_view, enable) ));
}

void scene_renderer_impl::set_model_ghost_mode( render_model_instance_base_ptr const& v, bool b )
{
	m_channel.owner_push_back  ( XRAY_NEW_IMPL( m_allocator, command_functor ) ( boost::bind( &engine::world::set_model_ghost_mode, &m_render_engine_world, v, b) ));
}

xray::particle::world& scene_renderer_impl::particle_world	( scene_ptr const& scene )
{
	R_ASSERT	( scene );
	return m_render_engine_world.particle_world( scene );
}