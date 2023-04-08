////////////////////////////////////////////////////////////////////////////
//	Created		: 14.07.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODEL_WRAPPER_H_INCLUDED
#define RENDER_MODEL_WRAPPER_H_INCLUDED

#pragma managed( push, off )
#include <xray/render/facade/model.h>
#include <xray/animation/animation_player.h>
#include <xray/animation/instant_interpolator.h>
#pragma managed( pop )

namespace xray {
namespace model_editor {

ref class edit_object_mesh;

ref class render_model_wrapper_base abstract
{
public:
											render_model_wrapper_base	( edit_object_mesh^ owner );
											~render_model_wrapper_base	( );

	virtual render::render_model_instance_ptr get_render_model_		( ) =0;
	void									remove_from_scene		( render::scene_renderer& scene_renderer, render::scene_ptr const& scene_ptr );
	void									add_to_scene			( render::scene_renderer& scene_renderer, render::scene_ptr const& scene_ptr, math::float4x4 const& m );
	void									move					( render::scene_renderer& scene_renderer, render::scene_ptr const& scene_ptr, math::float4x4 const& m );
	virtual void							tick					( ){};
	virtual void							reset					( ) =0;

protected:
	edit_object_mesh^						m_owner;
};

ref class solid_render_model_wrapper : public render_model_wrapper_base
{
	typedef render_model_wrapper_base super;
public:
											solid_render_model_wrapper( edit_object_mesh^ owner );
											~solid_render_model_wrapper( );
	virtual render::render_model_instance_ptr get_render_model_		( ) override;
	collision::geometry_ptr					get_collision_geometry	( );
	collision::geometry_ptr					get_hq_collision_geometry( );
	virtual void							tick					( ) override{};
	virtual void							reset					( ) override;
	void									set_resource			( resources::unmanaged_resource_ptr model, resources::unmanaged_resource_ptr collision );

private:
	render::static_model_ptr*				m_static_model;
	collision::geometry_ptr*				m_hq_collision;
};

ref class skeletal_render_model_wrapper : public render_model_wrapper_base
{
	typedef render_model_wrapper_base super;
public:
											skeletal_render_model_wrapper( edit_object_mesh^ owner );
											~skeletal_render_model_wrapper( );
	virtual render::render_model_instance_ptr get_render_model_		( ) override;
	virtual void							tick					( ) override;
	virtual void							reset					( ) override;
	void									set_resource			( resources::unmanaged_resource_ptr model );

	void									select_bone				( System::String^ bone_name ) {m_selected_bone_id = bone_name; }
	math::float4x4							get_bone_matrix			( System::String^ bone_name );
	void									goto_bind_pose			( );
	void									anim_play				( pcstr anim_name );

private:
	void									on_anim_loaded			( resources::queries_result& data );

	void									draw_bone_hierrarchy	( animation::skeleton_ptr const& skel,
																		animation::skeleton_bone const& root_bone, 
																		float4x4* const matrices, 
																		render::debug::renderer& d, 
																		render::scene_ptr const& scene );

	render::skeleton_model_ptr*				m_skeleton_model;
	float4x4*								m_bind_pose_matrices;
	float4x4*								m_anim_bone_matrices;
	float4x4*								m_current_bone_matrices;
	u32										m_root_bones_count;
	System::String^							m_selected_bone_id;
	animation::animation_player*			m_animation_player;
	animation::skeleton_animation_ptr*		m_current_animation;
	animation::instant_interpolator*		m_interpolator;
	timing::timer*							m_anim_timer;

public:
	cli::array<System::String^, 1>^			m_bone_names_array;
};

} // namespace model_editor
} // namespace xray

#endif // #ifndef RENDER_MODEL_WRAPPER_H_INCLUDED