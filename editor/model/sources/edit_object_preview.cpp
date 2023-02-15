////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "edit_object_mesh.h"
#include "model_editor.h"
#include "property_grid_panel.h"
#include "render_model_wrapper.h"

#pragma managed( push, off )
#include <xray/render/facade/editor_renderer.h>
#include <xray/render/facade/scene_renderer.h>
#include <xray/render/facade/model.h>
#pragma managed( pop )

namespace xray {
namespace model_editor {


void edit_object_solid_mesh::query_preview_model( )
{
	query_result_delegate* q = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, 
														&edit_object_solid_mesh::on_preview_model_ready), 
														g_allocator);

	resources::class_id_enum cls = /*resources::skeleton_model_instance_class*/ resources::static_model_instance_class;

	unmanaged_string		model_name(m_full_name);
	fs_new::virtual_path_string			hq_collision_path;

	hq_collision_path.assignf( "resources/models/%s%s/hq_collision", model_name.c_str(), model_file_extension() );
	resources::request r[]={
		{ model_name.c_str(), cls},
		{ hq_collision_path.c_str(), resources::collision_geometry_class }
	};

	resources::query_resources	(
		r,
		boost::bind(&query_result_delegate::callback, q, _1),
		g_allocator
		);
}

void edit_object_solid_mesh::on_preview_model_ready( resources::queries_result& data )
{
	if ( data[0].is_successful() )
	{
		if(has_preview_model())
			remove_preview_model();

		resources::unmanaged_resource_ptr c = data[1].get_unmanaged_resource();
		m_model->set_resource		( data[0].get_unmanaged_resource(), c );
		set_preview_model			( );
	}
}

void edit_object_solid_mesh::remove_preview_model( )
{
	m_model->remove_from_scene	( m_model_editor->editor_renderer()->scene(), m_model_editor->scene() );
	destroy_object_collision	( );
}

void edit_object_solid_mesh::set_preview_model( )
{
	m_model->add_to_scene(m_model_editor->editor_renderer()->scene(), m_model_editor->scene(), math::float4x4().identity() );
	m_model_editor->refresh_caption					( );
	action_refresh_preview_model					( );
	m_model_editor->action_focus_to_selection		( );
	initialize_object_collision						( );
}

void edit_object_mesh::action_refresh_preview_model( )
{
	super::action_refresh_preview_model();

	update_surfaces_property_view();

	for each ( edit_surface^ s in m_surfaces.Values )
		apply_surface_material( s->name, s->preview_material() );
}


void edit_object_skeletal_mesh::query_preview_model( )
{
	query_result_delegate* q = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, 
														&edit_object_skeletal_mesh::on_preview_model_ready), 
														g_allocator);

	unmanaged_string		model_name(m_full_name);
	fs_new::virtual_path_string			hq_collision_path;

	hq_collision_path.assignf( "resources/models/%s%s/hq_collision", model_name.c_str(), model_file_extension() );

	resources::query_resource	(
		model_name.c_str(), resources::skeleton_model_instance_class,
		boost::bind(&query_result_delegate::callback, q, _1),
		g_allocator
		);
}

void edit_object_skeletal_mesh::on_preview_model_ready( resources::queries_result& data )
{
	if ( data[0].is_successful() )
	{
		if(has_preview_model())
			remove_preview_model();

		m_model->set_resource		( data[0].get_unmanaged_resource() );
		set_preview_model			( );
		m_collision_panel->set_property_container( get_collision_property_container() );
	}
}

void edit_object_skeletal_mesh::remove_preview_model( )
{
	m_model->remove_from_scene	( m_model_editor->editor_renderer()->scene(), m_model_editor->scene() );
}

void edit_object_skeletal_mesh::set_preview_model( )
{
	m_model->add_to_scene(m_model_editor->editor_renderer()->scene(), m_model_editor->scene(), math::float4x4().identity() );
	m_model_editor->refresh_caption					( );
	action_refresh_preview_model					( );
	m_model_editor->action_focus_to_selection		( );
}

} // namespace model_editor
} // namespace xray


