////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2011
//	Author		: Sergey Prishchepa
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "model_wrapper.h"

#pragma managed( push, off )
#	include <xray/animation/api.h>
#	include <xray/render/facade/editor_renderer.h>
#	include <xray/render/facade/scene_renderer.h>
#	include <xray/animation/animation_player.h>
#pragma managed( pop )

using xray::animation_editor::model_wrapper;
using namespace xray::animation;

model_wrapper::model_wrapper(
		xray::render::scene_ptr const& scene,
		xray::render::scene_renderer& scene_renderer,
		xray::render::debug::renderer& debug_renderer,
		System::String^ n,
		System::String^ mn,
		float4x4& m,
		query_result_delegate* callback
	) :
	m_scene				( NEW(render::scene_ptr) (scene) ),
	m_scene_renderer	( scene_renderer ),
	m_debug_renderer	( debug_renderer ),
	m_callback			( callback ),
	m_animation_player	( NEW(animation::animation_player)( ) )
{
	name = n;
	model_name = mn;
	camera_follow_me = false;
	m_added_to_render = false;
	m_transform = NEW(float4x4)(m);
	query_result_delegate* rq = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, &model_wrapper::on_model_loaded), g_allocator);
	unmanaged_string model_n = unmanaged_string(model_name);
	xray::resources::query_resource(
		model_n.c_str(),
		xray::resources::skeleton_model_instance_class,
		boost::bind(&query_result_delegate::callback, rq, _1),
		g_allocator,
		0,
		0,
		assert_on_fail_false
	);
}

model_wrapper::~model_wrapper()
{
	if(m_mixer) 
		destroy_editor_mixer(*g_allocator, m_mixer);

	DELETE				( m_animation_player );

	if(m_model && m_model->c_ptr())
		DELETE(m_model);

	if(m_callback)
		DELETE(m_callback);

	if(m_transform)
		DELETE(m_transform);

	DELETE(m_scene);
	m_transform = NULL;
	m_mixer = NULL;
	m_model = NULL;
	m_callback = NULL;
	m_scene = NULL;
}

xray::render::skeleton_model_ptr model_wrapper::model() 
{
	if(m_model!=NULL && m_model->c_ptr()!=NULL)
		return *m_model;

	return NULL;
}

void model_wrapper::on_model_loaded(xray::resources::queries_result& result)
{
	if(!result.is_successful())
		return;

	m_model = NEW(xray::render::skeleton_model_ptr)();
	(*m_model) = static_cast_resource_ptr<xray::render::skeleton_model_ptr>(result[0].get_unmanaged_resource());
	m_mixer = create_editor_mixer(*g_allocator, *m_animation_player);
	if(m_callback)
		m_callback->callback(result);

	m_callback = NULL;
}

void model_wrapper::render()
{
	if(m_mixer && m_added_to_render)
		m_mixer->render( *m_scene, m_scene_renderer, m_debug_renderer, *m_model);
}

void model_wrapper::set_target(mixing::expression const& expression, u32 const current_time_in_ms)
{
	if(m_mixer)
		m_mixer->set_target_and_tick(expression, current_time_in_ms);
}

void model_wrapper::tick(u32 const current_time_in_ms)
{
	if(m_mixer)
		m_mixer->tick(current_time_in_ms);
}

void model_wrapper::push_expression(mixing::expression const& expression, u32 const expression_time)
{
	if(m_mixer)
	{
		m_mixer->push_expression(expression);
		m_mixer->push_expression_time(expression_time);
	}
}

void model_wrapper::calculate_animations_events(xray::vectora<editor_animations_event>& events)
{
	if(m_mixer)
		m_mixer->calculate_animations_events(events);
}

void model_wrapper::get_current_anim_states(xray::vectora<editor_animation_state>& result, u32 current_time_in_ms)
{
	if(m_mixer)
		m_mixer->get_current_anim_states(result, current_time_in_ms);
}

camera_follower* model_wrapper::get_camera_follower()
{
	if(m_mixer)
		return &(m_mixer->get_camera_follower());

	return NULL;
}

void model_wrapper::subscribe_footsteps(mixing::animation_lexeme& l)
{
	if(m_mixer)
		m_mixer->subscribe_footsteps(l);
}

void model_wrapper::reset()
{
	if(m_mixer)
	{
		m_mixer->reset();
		m_mixer->set_model_transform(transform);
	}
}

void model_wrapper::add_to_render()
{
	if(model()!=NULL && !m_added_to_render)
	{
		m_scene_renderer.add_model( *m_scene, (*m_model)->m_render_model, transform);
		m_added_to_render = true;
	}
}

void model_wrapper::remove_from_render()
{
	if(model()!=NULL && m_added_to_render)
	{
		m_scene_renderer.remove_model( *m_scene, (*m_model)->m_render_model);
		m_added_to_render = false;
	}
}

float4x4& model_wrapper::transform::get() 
{
	return *m_transform;
}

void model_wrapper::transform::set(float4x4& val)
{
	*m_transform = val;
	if(m_mixer && m_added_to_render)
	{
		m_mixer->set_model_transform(transform);
		m_scene_renderer.update_model( *m_scene, (*m_model)->m_render_model, transform );
//		m_scene_renderer.select_model( *m_scene, (*m_model)->m_render_model, transform );
	}
}

float4x4 model_wrapper::get_object_matrix() 
{
	if(m_mixer && m_added_to_render)
		return m_mixer->get_object_matrix_for_camera();
	else
		return float4x4().identity();
}