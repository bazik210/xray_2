////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "edit_object_mesh.h"
#include "model_editor.h"
#include "property_grid_panel.h"
#include "lod_view_panel.h"
#include "ide.h"
#include "input_actions.h"

#pragma managed( push, off )
#	include <xray/render/facade/editor_renderer.h>
#	include <xray/collision/space_partitioning_tree.h>
#pragma managed( pop )

using namespace System::Windows::Forms;

namespace xray {
namespace model_editor {

using editor_base::action_delegate;
using editor_base::execute_delegate_managed;
using editor_base::checked_delegate_managed;
using editor_base::enabled_delegate_managed;

edit_object_mesh::edit_object_mesh( model_editor^ me )
:super								( me ),
m_need_refresh_surfaces_visibility	( false ),
m_need_refresh_properties			( false )
{
	m_settings_cfg				= NEW(configs::lua_config_ptr)();
	m_object_surfaces_panel		= gcnew property_grid_panel( );
	m_lods						= gcnew array<lod_descriptor^>(3);
	m_lods[0]					= gcnew lod_descriptor(this);
	m_lods[1]					= gcnew lod_descriptor(this);
	m_lods[2]					= gcnew lod_descriptor(this);
	m_lods[0]->name				= "LOD0";
	m_lods[1]->name				= "LOD1";
	m_lods[2]->name				= "LOD2";

	create_lod_view				( );
}

edit_object_mesh::~edit_object_mesh( )
{
	R_ASSERT	( m_surfaces.Count==0, "Object not empty, call Unload before" );
	delete		m_lod_view_panel;
	delete		m_object_surfaces_panel;
	DELETE		( m_settings_cfg );
}



void edit_object_mesh::tick( )
{
	super::tick();

	if ( m_need_refresh_surfaces_visibility )
	{
		refresh_surfaces_visibility_impl	( );
		m_need_refresh_surfaces_visibility	= false;
	}

	if ( m_need_refresh_properties )
	{
		m_need_refresh_properties			= false;
		m_object_surfaces_panel->set_property_container( get_surfaces_property_container() );
		m_model_editor->refresh_caption		( );
		fill_lod_view						( );
	}

	render::debug::renderer& debug_renderer =  m_model_editor->editor_renderer()->debug();

	if( m_draw_object_collision )
	{
		//collision::geometry_ptr g			= m_model->get_collision_geometry();
		//if(g.c_ptr())
		//	g->render( m_model_editor->scene(), debug_renderer, math::float4x4().identity() );

		get_model_editor()->collision_tree->render( m_model_editor->scene(), debug_renderer );
	}
}


void edit_object_mesh::clear_resources( )
{
	super::clear_resources	( );

	R_ASSERT( m_model_editor->editor_renderer() );
	
	clear_lod_view				( );

	for each( lod_descriptor^ d in m_lods)
		d->m_surfaces.Clear();

	for each ( edit_surface^ s in m_surfaces.Values )
		delete s;
	m_surfaces.Clear();

	reset_modified	( );
	delete			m_lod_view_panel;
}



void edit_object_mesh::load( System::String^ model_full_name )
{
	m_full_name				= model_full_name;
	unmanaged_string uname	( m_full_name );
	fs_new::virtual_path_string	model_name;
	model_name.assignf("resources/models/%s%s/render", uname.c_str(), model_file_extension() );
	
	fs_iterator_delegate* q = NEW(fs_iterator_delegate)(gcnew fs_iterator_delegate::Delegate(this, 
															&edit_object_mesh::on_model_folder_fs_iterator_ready), 
															g_allocator);

	resources::query_vfs_iterator(
		model_name.c_str(), 
		boost::bind(&fs_iterator_delegate::callback, q, _1),
		g_allocator,
		resources::recursive_true
		);
}



void edit_object_mesh::on_model_settings_ready( resources::queries_result& data )
{
	if( data.is_successful() )
	{
		(*m_settings_cfg)			= static_cast_resource_ptr<configs::lua_config_ptr>(data[0].get_unmanaged_resource());
	}else
		(*m_settings_cfg)			= configs::create_lua_config();

	bool load_result				= load_model_settings( );

	if(!load_result)
		set_modified	( );

	on_surfaces_loaded				( );
}

void edit_object_mesh::on_model_folder_fs_iterator_ready( vfs::vfs_locked_iterator const & in_fs_it )
{
	R_ASSERT		( m_surfaces.Count == 0 );
	
	if( in_fs_it.is_end() ) // not completed model (maybe deleted)
		return;
	
	vfs::vfs_iterator fs_it				= in_fs_it.children_begin();

	while(!fs_it.is_end())
	{
		if( fs_it.is_folder() )
		{
			System::String^ sname		= gcnew System::String(fs_it.get_name());
			edit_surface^ s				= gcnew edit_surface( this, sname );
			m_surfaces.Add				( sname, s );
			set_surface_lod				( s, m_lods[0], false );
		}
		++fs_it;
	}

	for each ( edit_surface^ s in m_surfaces.Values )
		s->query_export_properties	( );

	m_model_editor->refresh_caption();

	unmanaged_string uname	( m_full_name );
	fs_new::virtual_path_string			model_name;
	model_name.assignf		("resources/models/%s%s/settings", uname.c_str(), model_file_extension() );
	
	query_result_delegate* qr = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, 
														&edit_object_mesh::on_model_settings_ready), 
														g_allocator);

	resources::query_resource(
		model_name.c_str(),
		resources::lua_config_class,
		boost::bind(&query_result_delegate::callback, qr, _1),
		g_allocator
		);
}



bool edit_object_mesh::load_model_settings( )
{
	bool result = true;
	configs::lua_config_value & root = (*m_settings_cfg)->get_root();

	if(root.size())
	{
		auto mat_settings = root["material_settings"];
		configs::lua_config_value & msettings = mat_settings;
		for each ( edit_surface^ s in m_surfaces.Values )
		{
			unmanaged_string sg_name( s->name );
			if(msettings.value_exists(sg_name.c_str()))
			{
				configs::lua_config_value const& current	= msettings[sg_name.c_str()];
				s->load_material_settings					( current );
			}else
			{
				s->setup_default_material_settings			( );
				result										= false;
			}
		}

		auto lod_hierrarchy = root["lod_hierrarchy"];
		configs::lua_config_value & lods_table = lod_hierrarchy;
		for each ( lod_descriptor^ l in m_lods )
		{
			unmanaged_string lod_name( l->name );

			if(lods_table.value_exists(lod_name.c_str()))
			{
				configs::lua_config_value const& current	= lods_table[lod_name.c_str()];
				l->load_settings							( current );
			}
		}	

		if((int)msettings.size() != m_surfaces.Count)
			result								= false;
	}
	else
	{
		for each ( edit_surface^ s in m_surfaces.Values )
			s->setup_default_material_settings( );

		result = true;
	}

	return result;
}



void edit_object_mesh::on_surfaces_loaded( )
{
	query_preview_model				( );
	m_object_surfaces_panel->set_property_container( get_surfaces_property_container() );
	m_model_editor->refresh_caption	( );
	fill_lod_view					( );
}

void edit_object_mesh::refresh_surfaces_visibility_impl( )
{
	if(!has_preview_model())
		return;

	for each ( edit_surface^ s in m_surfaces.Values )
	{
		bool vis	= s->Visible;
		if(get_view_isolate_selected())
			vis		&= s->Selected;
		
		set_surface_visible( s->name, vis );
	}
}


bool edit_object_mesh::save( )
{
	// save impl..
	configs::lua_config_ptr config_ptr	= configs::create_lua_config( );
	configs::lua_config_value& root		= config_ptr->get_root();

	for each ( edit_surface^ s in m_surfaces.Values )
	{
		unmanaged_string sg_name( s->name );
		auto mat_settings = root["material_settings"];
		configs::lua_config_value current	= mat_settings[sg_name.c_str()];
		s->save_material_settings			( current );
	}	

	auto lod_hierrarchy = root["lod_hierrarchy"];
	configs::lua_config_value & lods_table = lod_hierrarchy;
	for each ( lod_descriptor^ d in m_lods )
	{
		unmanaged_string lod_name( d->name );

		configs::lua_config_value current	= lods_table[lod_name.c_str()];
		d->save_settings					( current );
	}	
	
	save_impl( root );

	(*m_settings_cfg)->get_root().assign_lua_value	( config_ptr->get_root() );

	fs_new::virtual_path_string					config_name;

	config_name.assignf				( "resources/models/%s%s/settings", 
										unmanaged_string(m_full_name).c_str(),
										model_file_extension()
										);

	(*m_settings_cfg)->save_as		( config_name.c_str(), configs::target_sources );

	return super::save				( );
}



void edit_object_mesh::revert( )
{
	load_model_settings	( );
	super::revert		( );
}

math::aabb edit_object_mesh::focused_bbox( )
{
	math::aabb bbox		= math::create_invalid_aabb();
	bool btest			= false;
	bool selected_only	= get_view_isolate_selected();

	for each ( edit_surface^ s in m_surfaces.Values )
	{
		if(!s->Visible)
			continue;

		if(!s->Selected && selected_only)
			continue;

		if(btest)
			bbox.modify( *s->m_bbox );
		else
		{
			bbox	= *s->m_bbox;
			btest	= true;
		}
	}

	return (btest) ? bbox : math::create_identity_aabb();
}

math::float3 edit_object_mesh::focused_point( )
{
	return focused_bbox().center();
}

void edit_object_mesh::refresh_properties_in_grid( )
{
	m_need_refresh_properties = true;
}

void edit_object_mesh::action_draw_object_collision( )
{
	super::action_draw_object_collision();
	refresh_surfaces_visibility	( );
}



void edit_object_mesh::set_default_layout( )
{
	m_lod_view_panel->Show			( m_model_editor->get_ide()->main_dock_panel, WeifenLuo::WinFormsUI::Docking::DockState::DockRight );
	m_object_surfaces_panel->Show	( m_model_editor->get_ide()->main_dock_panel, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft );
	//m_object_portals_panel->Show	( m_model_editor->get_ide()->main_dock_panel, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft );
}


IDockContent^ edit_object_mesh::find_dock_content(System::String^ persist_string)
{
	if(persist_string=="xray.model_editor.lod_view_panel")
		return m_lod_view_panel;
	else if(persist_string=="xray.model_editor.property_grid_panel")
		return m_object_surfaces_panel;
	else
		return (nullptr);
}


bool edit_object_mesh::complete_loaded( )
{
	return has_preview_model();
}


void edit_object_mesh::update_surfaces_property_view( )
{
	m_object_surfaces_panel->refresh_properties( );
}

void edit_object_mesh::action_view_isolate_selected( )
{
	super::action_view_isolate_selected	( );
	refresh_surfaces_visibility			( );
}

void edit_object_mesh::register_actions( bool bregister )
{
	super::register_actions						( bregister );

	System::String^ action_name;
	editor_base::input_engine^ input_engine		= m_model_editor->get_input_engine();
	editor_base::gui_binder^ gui_binder			= m_model_editor->get_gui_binder();
	System::Windows::Forms::MenuStrip^ ide_menu_strip = m_model_editor->get_ide()->top_menu;
	editor_base::action_delegate^ a				= nullptr;

	action_name										= "LOD View";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_mesh::action_lod_panel_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_mesh::lod_panel_visible) );
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}

	action_name										= "Surfaces View";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_mesh::action_surfaces_panel_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_mesh::surfaces_panel_visible));
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}
}

void edit_object_mesh::action_surfaces_panel_view( )
{
	set_dock_panel_visible( m_object_surfaces_panel );
}

void edit_object_mesh::action_lod_panel_view( )
{
	set_dock_panel_visible(  m_lod_view_panel );
}

bool edit_object_mesh::surfaces_panel_visible( )
{
	return !m_object_surfaces_panel->IsHidden;
}

bool edit_object_mesh::lod_panel_visible( )
{
	return !m_lod_view_panel->IsHidden;
}

void edit_object_mesh::set_dock_panel_visible( WeifenLuo::WinFormsUI::Docking::DockContent^ panel )
{
	if( !panel->IsHidden )
		panel->Hide();
	else
	{
		if( panel->DockPanel ) 
			panel->Show	( m_model_editor->get_ide()->main_dock_panel );
		else
			panel->Show	( m_model_editor->get_ide()->main_dock_panel, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft );
	}
}


} // namespace model_editor
} // namespace xray

