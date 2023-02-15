////////////////////////////////////////////////////////////////////////////
//	Created		: 19.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "edit_object_mesh.h"
#include "model_editor.h"
#include "render_model_wrapper.h"
#include "unique_name_helper.h"
#include "property_grid_panel.h"
#include "bsp_tree.h"
#include "sectors_generator.h"
#include "sectors_view.h"
#include "object_portals_panel.h"
#include "portal_generation_view.h"
#include "input_actions.h"
#include "ide.h"

#pragma managed( push, off )
#	include <xray/render/facade/editor_renderer.h>
#	include <xray/render/facade/scene_renderer.h>
#	include <xray/editor/base/collision_object_types.h>
#	include <xray/collision/space_partitioning_tree.h>
#pragma managed( pop )

using namespace xray::editor::wpf_controls;
using namespace xray::editor::wpf_controls::control_containers;

namespace xray {
namespace model_editor {

using editor_base::action_delegate;
using editor_base::execute_delegate_managed;
using editor_base::checked_delegate_managed;
using editor_base::enabled_delegate_managed;

edit_object_solid_mesh::edit_object_solid_mesh( model_editor^ me )
:super								( me ),
m_hq_collision_object				( NULL ),
m_bsp_tree							( NULL ),
m_sectors_generator					( NULL )
{
	m_type_name			= "solid_mesh" ;
	m_model				= gcnew solid_render_model_wrapper( this );
	m_physics_settings	= gcnew rigid_body_construction_info_ref( this );

	m_collision_cfg				= NEW(configs::lua_config_ptr)();

	m_unique_portal_name_helper		= gcnew unique_name_helper();
	m_collision_panel				= gcnew collision_property_grid_panel( );
	m_collision_panel->Text			= "Collision";

	m_object_portals_panel			= gcnew object_portals_panel( this );

	m_bsp_tree						= NEW ( bsp_tree )( );
	m_portal_generation_view		= gcnew portal_generation_view( m_bsp_tree );
	update_portals_generation_property_view();
	m_current_mode					= edit_mode::edit_none;
	m_mass_center_item	= gcnew mass_center_item( this );
	m_sectors_view					= gcnew sectors_view();
}

edit_object_solid_mesh::~edit_object_solid_mesh( )
{
	delete		m_sectors_view;
	DELETE		(m_sectors_generator);
	delete		m_portal_generation_view;
	DELETE		( m_bsp_tree );
	R_ASSERT	( m_collision_object == NULL, "Collision geometry isn't deleted. Call \"destroy_collision_geometry\" before" );
	delete		m_model;
	DELETE		( m_collision_cfg );
	delete		m_unique_portal_name_helper;
	delete		m_object_portals_panel;
	delete		m_collision_panel;
	delete		m_physics_settings;
	delete		m_mass_center_item;
}

property_container^ edit_object_solid_mesh::get_portals_edit_property_container( )
{
	property_container^	result = gcnew property_container;

	control_container^ container	= result->add_dock_container( false );
	container->category				= "General";
	button^ button_instance = container->add_button( "Switch to edit", gcnew Action<button^>( this, &edit_object_solid_mesh::switch_to_portals_clicked ) );
	button_instance->width	= 80;
	
	button_instance = container->add_button( "Make coplanar", gcnew Action<button^>( this, &edit_object_solid_mesh::make_portals_coplanar_clicked ) );
	button_instance->width	= 80;

	button_instance = container->add_button( "New portal", gcnew Action<button^>( this, &edit_object_solid_mesh::add_portal_clicked ) );
	button_instance->width	= 80;

	button_instance = container->add_button( "Delete portal", gcnew Action<button^>( this, &edit_object_solid_mesh::delete_selected_portals_clicked ) );
	button_instance->width	= 80;

	for each ( edit_portal^ portal in m_portals )
	{
		portal->set_selected( false );
		property_container^	sub		= gcnew property_container;
		property_descriptor^ desc	= 
			gcnew property_descriptor( "Name", gcnew property_property_value( portal, "Name" ) );
		desc->select_behavior		= select_behavior::select_parent;
		sub->properties->add		( desc );
		desc = result->properties->add_container( portal->Name, "all portals", "no description", sub );
		desc->selection_changed += gcnew System::Action<System::Boolean>(portal, &edit_portal::set_selected);
	}
	return result;
}

xray::editor::wpf_controls::property_container^	edit_object_solid_mesh::get_portals_generation_property_container( )
{
	property_container^	result = gcnew property_container;

	control_container^ container	= result->add_dock_container( false );
	container->category				= "General";
	button^ button_instance			= nullptr;

	button_instance = container->add_button( "Build sectors", gcnew Action<button^>( this, &edit_object_solid_mesh::generate_sectors_clicked ) );
	button_instance->width	= 80;

	button_instance = container->add_button( "Generate portals", gcnew Action<button^>( this, &edit_object_solid_mesh::generate_portals_clicked ) );
	button_instance->width	= 80;

	button_instance = container->add_button( "Build BSP-tree", gcnew Action<button^>( this, &edit_object_solid_mesh::generate_bsp_tree_clicked ) );
	button_instance->width	= 80;

	return result;
}

void edit_object_solid_mesh::tick( )
{
	super::tick();

	render::debug::renderer& debug_renderer =  m_model_editor->editor_renderer()->debug();

	if(m_current_mode==edit_mode::edit_portals)
	{
		for each ( edit_portal^ portal in m_portals )
			portal->render( m_model_editor->scene(), debug_renderer);

		m_bsp_tree->render( m_model_editor->scene(), debug_renderer);
		if ( m_sectors_generator )
			m_sectors_generator->render( m_model_editor->scene(), debug_renderer);
	}else
	if(m_current_mode==edit_mode::edit_collision)
	{
		//rgba
		u32 const colors_table_size = 4;
		static math::color clr_table[colors_table_size]={
			math::color(104, 253, 73, 180),
			math::color(248, 253, 73, 180),
			math::color(78, 222, 255, 180),
			math::color(235, 77, 255, 180)
		};
		
		math::color clr_sel	(255, 147, 147, 200);

		editor_base::transform_control_object^ o = get_model_editor()->m_transform_control_helper->m_object;
		u32 clr_index = 0;
		for each ( collision_primitive_item_solid_mesh^ prim in m_collision_primitives )
		{
			bool is_selected =  (o==prim);
			math::color clr = (is_selected)? clr_sel : clr_table[clr_index%colors_table_size];
			prim->render( m_model_editor->scene(), debug_renderer, clr );
			clr_index++;
		}
		m_mass_center_item->render( m_model_editor->scene(), debug_renderer );
	}

	m_model->tick( );
}

void edit_object_solid_mesh::clear_resources( )
{
	super::clear_resources	( );

	R_ASSERT( m_model_editor->editor_renderer() );
	
	for each ( edit_portal^ p in m_portals )
		delete p;

	m_portals.Clear();

	for each ( collision_primitive_item_solid_mesh^ p in m_collision_primitives )
		delete p;

	m_collision_primitives.Clear();

	if(has_preview_model())
		remove_preview_model();

	reset_modified	( );
}

void edit_object_solid_mesh::load( System::String^ model_full_name )
{
	super::load(model_full_name );

	unmanaged_string uname	( m_full_name );

	// query for collision
	fs_new::virtual_path_string			collision_name;
	collision_name.assignf	("resources/models/%s%s/collision/primitives", uname.c_str(), model_file_extension() );
	
	query_result_delegate* qr = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, 
														&edit_object_solid_mesh::on_collision_settings_ready), 
														g_allocator);

	resources::query_resource(
		collision_name.c_str(),
		resources::lua_config_class,
		boost::bind(&query_result_delegate::callback, qr, _1),
		g_allocator,
		0,
		0,
		assert_on_fail_false
		);
	m_portal_generation_view->set_model_name( m_full_name );
}

bool edit_object_solid_mesh::load_model_settings( )
{

	bool result = super::load_model_settings	( );
	configs::lua_config_value const& root		= (*m_settings_cfg)->get_root();

	if(result)
	{
		result = load_portals			( root );
		m_physics_settings->load_from	( *m_settings_cfg );

#if 0	
		geometry_utils::geometry_collector collector;
		string_list							selected_surfaces;

		for each ( edit_surface^ s in m_surfaces.Values )
			selected_surfaces.Add(s->name);

		collect_surfaces( m_full_name, %selected_surfaces, &collector );
		collector.write_obj_file("c:\\collected.obj", 1.0f);
#endif
	}
	return result;
}

bool edit_object_solid_mesh::load_portals( configs::lua_config_value const& cfg_root )
{
	for each ( edit_portal^ portal in m_portals )
	{	
		delete portal;
	}
	m_portals.Clear();	
	m_unique_portal_name_helper->clear();
	
	if ( cfg_root.value_exists("portals") )
	{
		configs::lua_config_value const& cfg = cfg_root["portals"];
		configs::lua_config::const_iterator portal_it		=	cfg.begin( );
		configs::lua_config::const_iterator portal_end_it	=	cfg.end( );
		for( ; portal_it != portal_end_it; ++portal_it ) 
		{
			edit_portal^ portal = gcnew edit_portal( this, m_unique_portal_name_helper, %m_portals, *portal_it );
			m_portals.Add( portal );
		}
	}
	update_portals_edit_property_view( );
	return true;
}

void edit_object_solid_mesh::set_surface_visible( System::String^ surface_name, bool bvisible )
{
	xray::render::editor::renderer* r = m_model_editor->editor_renderer();

	r->scene().set_model_visible(m_model->get_render_model_(), unmanaged_string(surface_name).c_str(), bvisible );
}

bool edit_object_solid_mesh::save_impl( configs::lua_config_value& root )
{
	if(!are_portal_vertices_complanar() )
	{
		System::Windows::Forms::MessageBox::Show( "Not all triangles in portal(s) are coplanar", 
		"Model Assembler", 
		MessageBoxButtons::OK, MessageBoxIcon::Exclamation );
	}

	if ( m_portals.Count != 0 )
	{
		configs::lua_config_value const& portals_table = root["portals"];
		for each ( edit_portal^ portal in m_portals )
		{
			unmanaged_string portal_name( portal->Name );
			configs::lua_config_value current	=	portals_table[ portal_name.c_str() ];
			portal->save_portal						( current );	
		}
	}

	m_physics_settings->save_to	( root );

	save_collision				( );
	return						true;
}

void edit_object_solid_mesh::revert( )
{
	load_collision		( );
	super::revert		( );
}

math::aabb edit_object_solid_mesh::focused_bbox( )
{
	for each ( edit_portal^ portal in m_portals )
	{
		if ( portal->is_selected( ) )
			return portal->get_aabb( );
	}

	if ( m_bsp_tree->is_created() )
		return m_bsp_tree->get_aabb();

	return super::focused_bbox( );
}

void edit_object_solid_mesh::add_portal_clicked( button^ )
{
	System::String^ name = m_unique_portal_name_helper->get_unique_name( "new_portal_" );
	m_portals.Add( gcnew edit_portal( this, m_unique_portal_name_helper, %m_portals, name ) );
	update_portals_edit_property_view( );
	set_modified();
}

void edit_object_solid_mesh::delete_selected_portals_clicked( button^ )
{
	if ( 
		m_portals.Exists(gcnew System::Predicate<edit_portal^>( &edit_portal::is_selected ) ) &&
		System::Windows::Forms::MessageBox::Show( "Are you sure you want to delete selected portals?", "Confirm", MessageBoxButtons::OKCancel, MessageBoxIcon::Question ) == DialogResult::OK 
		) 
	{
		for ( int i = 0; i < m_portals.Count; ++i )
		{
			if ( m_portals[ i ]->is_selected() )
			{
				m_unique_portal_name_helper->remove( m_portals[ i ]->Name );
				delete m_portals[ i ];
				m_portals[ i ] = nullptr;
			}
		}
		while ( m_portals.Remove(nullptr) );
		update_portals_edit_property_view( );
		set_modified( );
		get_model_editor()->m_transform_control_helper->m_object = nullptr;
	}
}

void edit_object_solid_mesh::reset_selection( )
{
	for each ( edit_portal^ portal in m_portals )
	{
		portal->set_selected( false );
	}
}

void edit_object_solid_mesh::initialize_object_collision( )
{
	collision::geometry_ptr g		= m_model->get_hq_collision_geometry();
	if( g.c_ptr()==NULL)
		return;

	m_hq_collision_object = NEW ( collision::collision_object ) ( g_allocator, editor_base::collision_object_type_dynamic, &*collision::new_triangle_mesh_geometry_instance( g_allocator, float4x4().identity(), g.c_ptr()) );
	m_model_editor->collision_tree->insert( m_hq_collision_object , m_hq_collision_object->get_matrix( ) );
}

void edit_object_solid_mesh::destroy_object_collision( )
{
	if ( m_hq_collision_object  )
	{
		m_model_editor->collision_tree->erase( m_hq_collision_object  );
		DELETE ( m_hq_collision_object  );
		m_hq_collision_object  = NULL;
	}
}

bool edit_object_solid_mesh::are_portal_vertices_complanar( )
{
	using namespace System::Windows::Forms;
	for each ( edit_portal^ portal in m_portals )
	{
		if ( portal->are_vertices_coplanar() )
			continue;
		String^ message = String::Format( 
			"Vertices of portal \"{0}\" are not coplanar. Would you like it will be fixed automatically? Otherwise you should fix it manually",
			portal->Name );
		DialogResult result = System::Windows::Forms::MessageBox::Show( message, "Some vertices are not coplanar", MessageBoxButtons::YesNo, MessageBoxIcon::Question );
		if ( result == DialogResult::No )
			return false;
		portal->make_vertices_coplanar();
	}
	return true;
}

IDockContent^ edit_object_solid_mesh::find_dock_content(System::String^ persist_string)
{
	if(persist_string=="xray.model_editor.collision_property_grid_panel")
		return m_collision_panel;
	else if(persist_string=="xray.model_editor.object_portals_panel")
		return m_object_portals_panel;
	else if(persist_string=="xray.model_editor.portal_generation_view")
		return m_portal_generation_view;
	else if(persist_string=="xray.model_editor.sectors_view")
		return m_sectors_view;
	else
		return super::find_dock_content( persist_string );
}

void edit_object_solid_mesh::register_actions( bool bregister )
{
	super::register_actions						( bregister );

	System::String^ action_name;
	editor_base::input_engine^ input_engine		= m_model_editor->get_input_engine();
	editor_base::gui_binder^ gui_binder			= m_model_editor->get_gui_binder();
	System::Windows::Forms::MenuStrip^ ide_menu_strip = m_model_editor->get_ide()->top_menu;
	editor_base::action_delegate^ a				= nullptr;

	action_name										= "Collision View";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_solid_mesh::action_collision_panel_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_solid_mesh::collision_panel_visible));
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}

	action_name										= "Portals View";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_solid_mesh::action_portals_panel_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_solid_mesh::portals_panel_visible) );
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}

	action_name										= "BSP-tree view";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_solid_mesh::action_portal_generation_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_solid_mesh::portal_generation_view_visible) );
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}

	action_name										= "Sectors view";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_solid_mesh::action_sectors_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_solid_mesh::sectors_view_visible) );
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}


	action_name = "Add vertex to portal";
	if (bregister)
	{
		a = gcnew action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_solid_mesh::action_add_vertex_to_portal) );
		a->set_enabled( gcnew editor_base::enabled_delegate_managed(this, &edit_object_solid_mesh::is_portals_edit_mode));
		input_engine->register_action( a, "Ctrl+LButton(View)" );
	}
	else
	{
		input_engine->unregister_action	( action_name );
	}

	action_name		= "Delete selected vertex in portal";
	if (bregister)
	{
		a = gcnew action_delegate(action_name, gcnew execute_delegate_managed(this, &edit_object_solid_mesh::action_delete_vertex_in_portal));
		a->set_enabled( gcnew editor_base::enabled_delegate_managed(this, &edit_object_solid_mesh::is_portals_edit_mode));
		input_engine->register_action( a, "Delete(View)" );
	}
	else
	{
		input_engine->unregister_action	( action_name );
	}
}

bool edit_object_solid_mesh::portals_panel_visible( )
{
	return !m_object_portals_panel->IsHidden;
}

bool edit_object_solid_mesh::is_portals_edit_mode( )
{
	return ( m_current_mode==edit_mode::edit_portals);
}

bool edit_object_solid_mesh::is_collision_edit_mode( )
{
	return ( m_current_mode==edit_mode::edit_collision);
}

void edit_object_solid_mesh::make_portals_coplanar_clicked( button^ )
{
	for each ( edit_portal^ portal in m_portals )
		portal->make_vertices_coplanar();
}

void edit_object_solid_mesh::action_add_vertex_to_portal( )
{
	for each ( edit_portal^ portal in m_portals )
		if ( portal->is_selected() )
		{
			portal->add_vertex();
			return;
		}
}

void edit_object_solid_mesh::action_delete_vertex_in_portal( )
{
	for each ( edit_portal^ portal in m_portals )
		if ( portal->is_selected() && portal->delete_selected_vertices() )
			return;
	delete_selected_portals_clicked( nullptr );
}


bool edit_object_solid_mesh::collision_panel_visible( )
{
	return !m_collision_panel->IsHidden;
}

void edit_object_solid_mesh::action_portal_generation_view( )
{
	this->set_dock_panel_visible( m_portal_generation_view );
}

bool edit_object_solid_mesh::portal_generation_view_visible( )
{	
	return !m_portal_generation_view->IsHidden;
}

bool edit_object_solid_mesh::sectors_view_visible( )
{	
	return !m_sectors_view->IsHidden;
}

void edit_object_solid_mesh::action_sectors_view()
{
	set_dock_panel_visible( m_sectors_view );
}

void edit_object_solid_mesh::switch_to_collision_clicked( button^ )
{
	set_edit_mode( edit_mode::edit_collision );
}

void edit_object_solid_mesh::generate_bsp_tree_clicked( button^ /*button*/  )
{
//?	button->IsEnabled = false;
	collision::geometry_ptr g = m_model->get_collision_geometry();
	R_ASSERT( g.c_ptr() != NULL );
	m_bsp_tree->generate_bsp_tree( g->vertices(), g->vertex_count(), g->indices(), g->index_count() );
	m_bsp_tree->fill_tree_view( m_portal_generation_view->tree_view );
}
void edit_object_solid_mesh::switch_to_portals_clicked( button^ )
{
	set_edit_mode( m_current_mode != edit_mode::edit_portals ? edit_mode::edit_portals : edit_mode::edit_none );
}

void edit_object_solid_mesh::generate_portals_clicked( button^ /*button*/ )
{
//?	button->IsEnabled = false;
	m_bsp_tree->generate_portals();
}

void edit_object_solid_mesh::generate_sectors_clicked	( button^ /*button*/ )
{
	if ( !m_sectors_generator )
	{
//?		button->IsEnabled = false;
		collision::geometry_ptr g = m_model->get_collision_geometry();
		R_ASSERT( g.c_ptr() != NULL );
		m_sectors_generator = NEW( sectors_generator )( g->vertices(), g->vertex_count(), g->indices(), g->index_count() );
		generated_portals_type const& tree_portals = m_bsp_tree->get_portals();
		generated_portals_type::const_iterator const tree_portals_end = tree_portals.end();
		for ( generated_portals_type::const_iterator it = tree_portals.begin(); it != tree_portals_end; ++it )
		{
			m_sectors_generator->add_portal( it->to_sector_generator_portal() );
		}

		generated_portals_type edited_portals;
		for each ( edit_portal^ portal in m_portals )
		{
			m_sectors_generator->add_portal( portal->to_sector_generator_portal() );
		}
		m_sectors_generator->generate_sectors();
		xray::editor::wpf_controls::property_container^ sectors_properties = gcnew xray::editor::wpf_controls::property_container();
		m_sectors_generator->fill_property_container( sectors_properties );
		m_sectors_view->set_property_container( sectors_properties );
	}
}



void edit_object_solid_mesh::set_edit_mode( edit_mode mode )
{
	m_current_mode = mode;

	{// collision
		bool activate = (mode == edit_mode::edit_collision);
		for each ( collision_primitive_item_solid_mesh^ prim in m_collision_primitives )
			prim->activate( activate );
		
		// mass center
		m_mass_center_item->activate( activate );
	}
}
void edit_object_solid_mesh::apply_surface_material( System::String^ surface_name, resources::unmanaged_resource_ptr mtl )
{
	edit_surface^ surface = get_surface( surface_name);
	if(!has_preview_model())
		return;

	xray::render::editor::renderer* r = m_model_editor->editor_renderer();

	r->scene().set_model_material	(	m_model->get_render_model_(),
								unmanaged_string(surface->name).c_str(), 
								mtl );
}

void edit_object_solid_mesh::set_default_layout( )
{
	super::set_default_layout( );
}

void edit_object_solid_mesh::update_portals_edit_property_view( )
{
	m_object_portals_panel->set_property_container( get_portals_edit_property_container( ) );
}

void edit_object_solid_mesh::update_portals_generation_property_view( )
{
	m_portal_generation_view->set_property_container( get_portals_generation_property_container( ) );
}

void edit_object_solid_mesh::action_collision_panel_view( )
{
	set_dock_panel_visible(  m_collision_panel );
}

void edit_object_solid_mesh::action_portals_panel_view( )
{
	set_dock_panel_visible( m_object_portals_panel );
}


bool edit_object_solid_mesh::has_preview_model( )
{
	return m_model->get_render_model_().c_ptr()!= NULL;
}

} // namespace model_editor
} // namespace xray
