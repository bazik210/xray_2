////////////////////////////////////////////////////////////////////////////
//	Created		: 14.07.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "render_model_wrapper.h"
#include "edit_object_mesh.h"
#include "model_editor.h"

#pragma managed( push, off )
#	include <xray/render/facade/scene_renderer.h>
#	include <xray/render/facade/editor_renderer.h>
#	include <xray/render/facade/debug_renderer.h>
#include <xray/animation/mixing_animation_lexeme.h>
#include <xray/animation/mixing_math.h>
#pragma managed( pop )

namespace xray {
namespace model_editor {
render_model_wrapper_base::render_model_wrapper_base( edit_object_mesh^ owner ) 
:m_owner				( owner )
{
}

render_model_wrapper_base::~render_model_wrapper_base( )
{}

void render_model_wrapper_base::remove_from_scene( render::scene_renderer& scene_renderer, render::scene_ptr const&	scene_ptr )
{
	scene_renderer.remove_model	( scene_ptr, get_render_model_() );
	reset( );
}

void render_model_wrapper_base::add_to_scene( render::scene_renderer& scene_renderer, render::scene_ptr const& scene_ptr, math::float4x4 const& m )
{
	scene_renderer.add_model( scene_ptr, get_render_model_(), m );
}

void render_model_wrapper_base::move( render::scene_renderer& scene_renderer, render::scene_ptr const& scene_ptr, math::float4x4 const& m )
{
	scene_renderer.update_model( scene_ptr, get_render_model_(), m );
}

solid_render_model_wrapper::solid_render_model_wrapper( edit_object_mesh^ owner )
:super( owner )
{
	m_static_model		= NEW ( render::static_model_ptr)();
	m_hq_collision		= NEW ( collision::geometry_ptr )();
}

solid_render_model_wrapper::~solid_render_model_wrapper( )
{
	DELETE		( m_static_model );
	DELETE		( m_hq_collision );
}

void solid_render_model_wrapper::reset( )
{
	*m_static_model	= NULL;
	*m_hq_collision	= NULL;
}

render::render_model_instance_ptr solid_render_model_wrapper::get_render_model_( )
{ 
	return ((*m_static_model).c_ptr()!=NULL) ? (*m_static_model)->m_render_model : NULL;
}

collision::geometry_ptr solid_render_model_wrapper::get_collision_geometry( )
{
	return (*m_static_model)->m_collision_geom;
}

collision::geometry_ptr solid_render_model_wrapper::get_hq_collision_geometry( )
{
	return *m_hq_collision;
}

void solid_render_model_wrapper::set_resource( resources::unmanaged_resource_ptr model, resources::unmanaged_resource_ptr hq_collision )
{
	*m_static_model		= static_cast_resource_ptr<render::static_model_ptr>( model );
	*m_hq_collision		= static_cast_resource_ptr<collision::geometry_ptr>( hq_collision );
}

skeletal_render_model_wrapper::skeletal_render_model_wrapper( edit_object_mesh^ owner )
:super					( owner ),
m_bind_pose_matrices	( NULL ),
m_anim_bone_matrices	( NULL ),
m_current_bone_matrices	( NULL ),
m_root_bones_count		( 0 ),
m_bone_names_array		( nullptr ),
m_animation_player		( NULL )
{
	m_skeleton_model	= NEW ( render::skeleton_model_ptr)();
	m_current_animation = NEW(animation::skeleton_animation_ptr)();
	m_interpolator		= NEW(animation::instant_interpolator)();
	m_anim_timer		= NEW(timing::timer)();
}

skeletal_render_model_wrapper::~skeletal_render_model_wrapper( )
{
	reset		( );
	DELETE		( m_skeleton_model );
	DELETE		( m_animation_player );
	DELETE		( m_current_animation );
	DELETE		( m_interpolator );
	DELETE		( m_anim_timer );
}

void skeletal_render_model_wrapper::on_anim_loaded( resources::queries_result& data )
{
	m_animation_player						= NEW(animation::animation_player)( );
	m_animation_player->set_no_delete		( );
	m_animation_player->set_object_transform( math::float4x4().identity() );
	

	m_current_bone_matrices			= m_anim_bone_matrices;

	*m_current_animation			= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[0].get_managed_resource());
	m_anim_timer->start				( );
}

void skeletal_render_model_wrapper::reset( )
{
	*m_skeleton_model	= NULL;

	FREE				( m_bind_pose_matrices );
	m_bind_pose_matrices = NULL;
	
	FREE				( m_anim_bone_matrices );
	m_anim_bone_matrices = NULL;
	
	m_current_bone_matrices = NULL;
}

render::render_model_instance_ptr skeletal_render_model_wrapper::get_render_model_( )
{ 
	return ((*m_skeleton_model).c_ptr()!=NULL) ? (*m_skeleton_model)->m_render_model : NULL;
}

void skeletal_render_model_wrapper::set_resource( resources::unmanaged_resource_ptr model )
{
	*m_skeleton_model		= static_cast_resource_ptr<render::skeleton_model_ptr>( model );
	u32 bone_count			= (*m_skeleton_model)->m_skeleton->get_non_root_bones_count();
	m_root_bones_count		= (u16)(*m_skeleton_model)->m_skeleton->get_root_bones_count();

	m_bind_pose_matrices				= ALLOC(float4x4, bone_count);
	m_anim_bone_matrices				= ALLOC(float4x4, bone_count);

	(*m_skeleton_model)->m_render_model->get_bind_pose( m_bind_pose_matrices, bone_count);

	m_bone_names_array = gcnew cli::array<System::String^, 1>(bone_count);
	for(u32 i=0; i<bone_count; ++i)
	{
		pcstr bonename = (*m_skeleton_model)->m_skeleton->get_bone_name(i+m_root_bones_count);
		m_bone_names_array[i] = gcnew System::String(bonename);
	}
	goto_bind_pose			( );
}

math::float4x4  skeletal_render_model_wrapper::get_bone_matrix( System::String^ bone_name )
{
	animation::skeleton_ptr const skel	= (*m_skeleton_model)->m_skeleton;
	unmanaged_string					s(bone_name);
	animation::bone_index_type idx		= skel->get_bone_index(s.c_str());
	return m_current_bone_matrices[idx-m_root_bones_count];
}

void skeletal_render_model_wrapper::draw_bone_hierrarchy( animation::skeleton_ptr const& skel,
							animation::skeleton_bone const& root_bone, 
							float4x4* const matrices, 
							render::debug::renderer& d, 
							render::scene_ptr const& scene )
{
	System::String^ bn = gcnew System::String(root_bone.id());

	bool is_selected = (0==System::String::Compare(m_selected_bone_id, bn));

	u32 bid			= skel->get_bone_index(root_bone);
	float4x4& m		= matrices[bid-m_root_bones_count];
	if(is_selected)
		d.draw_origin( scene, m, 0.1f, false);

	math::color clr = is_selected ? math::color(255,0,0,255) : math::color(190,190,0,150) ;
	d.draw_sphere_solid	( scene, m.c.xyz(), 0.01f, clr, !is_selected );

	animation::skeleton_bone const* it				= root_bone.children_begin();;
	animation::skeleton_bone const*	children_end	= root_bone.children_end();
	for(it; it!=children_end; ++it)
	{
		animation::skeleton_bone const& b = *it;

		u32 bid_it			= skel->get_bone_index(b);
		float4x4& m_it		= matrices[bid_it-m_root_bones_count];
		d.draw_line			( scene, m.c.xyz(), m_it.c.xyz(), math::color(0,255,0,255));

		draw_bone_hierrarchy( skel, b, matrices, d, scene );
	}

}

void skeletal_render_model_wrapper::anim_play( pcstr anim_name )
{
	query_result_delegate* qr = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, 
														&skeletal_render_model_wrapper::on_anim_loaded), 
														g_allocator);

	resources::query_resource(
		anim_name,
		resources::animation_class,
		boost::bind(&query_result_delegate::callback, qr, _1),
		g_allocator);

}

void skeletal_render_model_wrapper::goto_bind_pose( )
{
	m_current_bone_matrices			= m_bind_pose_matrices;
	DELETE							( m_animation_player );
	m_animation_player				= NULL;
	*m_current_animation			= 0;
}

void skeletal_render_model_wrapper::tick( )
{
//	if(mode==1)
	if( (*m_skeleton_model).c_ptr() )
	{ // draw skeleton
		render::scene_renderer& r		= m_owner->get_model_editor()->editor_renderer()->scene();
		render::scene_ptr const& scene	= m_owner->get_model_editor()->scene();

		animation::skeleton_ptr const skel			= (*m_skeleton_model)->m_skeleton;
		render::render_model_instance_ptr model		= get_render_model_();

		u32 const non_root_bones_count	= skel->get_non_root_bones_count( );
		if(m_animation_player)
		{
			u32 current_time				= m_anim_timer->get_elapsed_msec();

			mutable_buffer buffer( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );
			
			animation::mixing::animation_lexeme	current_lexeme(
				animation::mixing::animation_lexeme_parameters(
					buffer, 
					"idle",
					*m_current_animation
				).time_scale( 0.3f )
			);

			m_animation_player->set_target_and_tick	( current_lexeme, current_time );

			m_animation_player->compute_bones_matrices( *skel, m_anim_bone_matrices, m_anim_bone_matrices+non_root_bones_count );
		}

		r.update_skeleton	(	model,
								m_current_bone_matrices,
								non_root_bones_count );
		
		animation::skeleton_bone const& root_bone = skel->get_bone(m_root_bones_count);

		xray::render::debug::renderer& d		= m_owner->get_model_editor()->editor_renderer()->debug();
		draw_bone_hierrarchy					( skel, root_bone, m_current_bone_matrices, d, scene );

	}
}


} // namespace model_editor
} // namespace xray

