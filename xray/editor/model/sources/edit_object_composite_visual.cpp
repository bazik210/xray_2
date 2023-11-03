//////////////////////////////////////////////////////////////////////////////
////	Created		: 14.07.2011
////	Author		: Andrew Kolomiets
////	Copyright (C) GSC Game World - 2011
//////////////////////////////////////////////////////////////////////////////
//
#include "pch.h"
#include "edit_object_composite_visual.h"
#include "property_grid_panel.h"
#include "model_editor.h"
#include "render_model_wrapper.h"
#include "unique_name_helper.h"
#include "input_actions.h"
#include "ide.h"

#pragma managed( push, off )
#	include <xray/render/facade/editor_renderer.h>
#	include <xray/render/facade/scene_renderer.h>
#	include <xray/editor/base/collision_object_types.h>
#	include <xray/collision/space_partitioning_tree.h>
#pragma managed( pop )

namespace xray{
namespace model_editor{

edit_object_composite_visual::edit_object_composite_visual( model_editor^ parent )
:super( parent )
{
	m_type_name				= "composite";
	m_model_config			= NEW(configs::lua_config_ptr)();
	m_object_contents_panel = gcnew property_grid_panel;

	//m_collision_cfg				= NEW(configs::lua_config_ptr)();
	//m_collision_panel			= gcnew collision_property_grid_panel( );
	//m_collision_panel->Text		= "Collision";

	//m_current_mode				= edit_mode::edit_none;

	//m_mass_center_item	= gcnew mass_center_item( this );
}

edit_object_composite_visual::~edit_object_composite_visual( )
{
	delete		m_object_contents_panel;
	DELETE		( m_model_config );

	//DELETE		( m_collision_cfg );
	//delete		m_collision_panel;
	//delete		m_mass_center_item;
}

void edit_object_composite_visual::tick( )
{
	super::tick();

//	render::debug::renderer& debug_renderer =  m_model_editor->editor_renderer()->debug();

//if(m_current_mode==edit_mode::edit_collision)
//	{
//		//rgba
//		u32 const colors_table_size = 4;
//		static math::color clr_table[colors_table_size]={
//			math::color(104, 253, 73, 180),
//			math::color(248, 253, 73, 180),
//			math::color(78, 222, 255, 180),
//			math::color(235, 77, 255, 180)
//		};
//		
//		math::color clr_sel	(255, 147, 147, 200);
//
//		editor_base::transform_control_object^ o = get_model_editor()->m_transform_control_helper->m_object;
//		u32 clr_index = 0;
//		for each ( collision_primitive_item_solid_mesh^ prim in m_collision_primitives )
//		{
//			bool is_selected =  (o==prim);
//			math::color clr = (is_selected)? clr_sel : clr_table[clr_index%colors_table_size];
//			prim->render( m_model_editor->scene(), debug_renderer, clr );
//			clr_index++;
//		}
//		//m_mass_center_item->render( m_model_editor->scene(), debug_renderer );
//	}

}

IDockContent^ edit_object_composite_visual::find_dock_content( System::String^ persist_string )
{
	if(persist_string=="xray.model_editor.property_grid_panel")
		return m_object_contents_panel;
	//else if(persist_string=="xray.model_editor.collision_property_grid_panel")
	//	return m_collision_panel;
	else
		return nullptr;
}
void edit_object_composite_visual::refresh_surfaces_panel( )
{
	m_object_contents_panel->set_property_container( get_contents_property_container() );
}

void edit_object_composite_visual::clear_resources( )
{
	super::clear_resources	( );
	for each( composite_visual_item^ item in m_contents )
		delete item;

	m_contents.Clear();
}

void edit_object_composite_visual::set_default_layout( )
{
	m_object_contents_panel->Show	( m_model_editor->get_ide()->main_dock_panel, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft );
}

bool edit_object_composite_visual::complete_loaded( )
{
	return true;
}

void edit_object_composite_visual::reset_selection( )
{
}

//bool edit_object_composite_visual::collision_panel_visible( )
//{
//	return !m_collision_panel->IsHidden;
//}

//void edit_object_composite_visual::set_dock_panel_visible(WeifenLuo::WinFormsUI::Docking::DockContent^ panel)
//{
//	if( !panel->IsHidden )
//		panel->Hide();
//	else
//	{
//		if( panel->DockPanel ) 
//			panel->Show	( m_model_editor->get_ide()->main_dock_panel );
//		else
//			panel->Show	( m_model_editor->get_ide()->main_dock_panel, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft );
//	}
//}

math::aabb edit_object_composite_visual::focused_bbox( )
{
	math::aabb result = math::create_identity_aabb();

	for each( composite_visual_item^ item in m_contents )
	{
		if(item->get_selected())
			return item->get_aabb();
		else
			result.modify(item->get_aabb());
	}
	return result;
}
}
}
