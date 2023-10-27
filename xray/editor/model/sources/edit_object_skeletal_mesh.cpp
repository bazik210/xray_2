////////////////////////////////////////////////////////////////////////////
//	Created		: 19.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "edit_object_mesh.h"
#include "render_model_wrapper.h"
#include "property_grid_panel.h"
#include "model_editor.h"
#include "ide.h"
#include "input_actions.h"
#include "edit_object_mesh.h"
//#include "properties_holder.h"

#pragma managed( push, off )
#	include <xray/render/facade/editor_renderer.h>
#	include <xray/render/facade/scene_renderer.h>
//#	include <xray/editor/base/collision_object_types.h>
//#	include <xray/collision/space_partitioning_tree.h>
#pragma managed( pop )

using namespace xray::editor::wpf_controls;
using namespace xray::editor::wpf_controls::control_containers;
using namespace xray::editor_base;

namespace xray {
namespace model_editor {

using editor_base::action_delegate;
using editor_base::execute_delegate_managed;
using editor_base::checked_delegate_managed;
using editor_base::enabled_delegate_managed;

ref class anim_instance_ui_type_editor : public xray::editor::wpf_controls::property_editors::i_external_property_editor
{
	typedef xray::editor::wpf_controls::property_descriptor property_descriptor;
public:
	virtual	void run_editor(xray::editor::wpf_controls::property_editors::property^ prop);

}; // ref class anim_instance_ui_type_editor

void anim_instance_ui_type_editor::run_editor(xray::editor::wpf_controls::property_editors::property^ prop)
{
	System::String^ current = nullptr;
	System::String^ filter = nullptr;
	System::String^ selected = nullptr;

	if (prop->value)
		current = safe_cast<System::String^>(prop->value);

	property_descriptor^ pd = safe_cast<property_descriptor^>(prop->descriptor);

	if (xray::editor_base::resource_chooser::choose("animations_list", current, filter, selected, false))
	{
		prop->value = "resources/animations/" + selected;
	}
};

edit_object_skeletal_mesh::edit_object_skeletal_mesh( model_editor^ me )
:super( me )
{
	m_type_name						= "skeletal_mesh" ;
	m_model							= gcnew skeletal_render_model_wrapper( this );
	m_collision_cfg					= NEW(configs::lua_config_ptr)();
	m_collision_panel				= gcnew collision_property_grid_panel( );
	m_collision_panel->Text			= "Collision";
	m_motion_name					= "resources/animations/single/human/common_anim_slot_1/free/run_move_fwd_aim_1";
}

edit_object_skeletal_mesh::~edit_object_skeletal_mesh( )
{
	delete		m_model;
	DELETE		( m_collision_cfg );
}

bool edit_object_skeletal_mesh::has_preview_model( )
{
	return m_model->get_render_model_().c_ptr()!=NULL;
}

void edit_object_skeletal_mesh::tick( )
{
	super::tick( );

	m_model->tick( );
	if(m_current_mode==edit_mode::edit_collision)
	{
		render::debug::renderer& debug_renderer =  m_model_editor->editor_renderer()->debug();

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
		for each ( collision_primitive_item_skeletal_mesh^ prim in m_collision_primitives )
		{
			bool is_selected =  (o==prim);
			math::color clr = (is_selected)? clr_sel : clr_table[clr_index%colors_table_size];
			prim->render( m_model_editor->scene(), debug_renderer, clr );
			clr_index++;
		}
	}
}

bool edit_object_skeletal_mesh::save_impl(  configs::lua_config_value& root )
{
	save_collision		( );
	return				true;
}

void edit_object_skeletal_mesh::revert( )
{
	load_collision		( );
	super::revert		( );
}

void edit_object_skeletal_mesh::load( System::String^ model_full_name )
{
	super::load( model_full_name );
	unmanaged_string uname	( m_full_name );

	// query for collision
	fs_new::virtual_path_string			collision_name;
	collision_name.assignf	("resources/models/%s%s/hit_targets", uname.c_str(), model_file_extension() );
	
	query_result_delegate* qr = NEW(query_result_delegate)(gcnew query_result_delegate::Delegate(this, 
														&edit_object_skeletal_mesh::on_collision_settings_ready), 
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
}

bool edit_object_skeletal_mesh::load_model_settings( )
{
	return super::load_model_settings( );
}

void edit_object_skeletal_mesh::save_collision( )
{
	if(m_collision_primitives.Count)
	{
		// save impl..
		configs::lua_config_ptr config_ptr	= configs::create_lua_config( );
		configs::lua_config_value root		= config_ptr->get_root()["hit_targets"];

		int i=0;
		for each ( collision_primitive_item_skeletal_mesh^ prim in m_collision_primitives )
		{
			configs::lua_config_value current	= root[i];
			prim->save							( current );
			++i;
		}	

		(*m_collision_cfg)->get_root().assign_lua_value	( config_ptr->get_root() );
		fs_new::virtual_path_string			config_name;
		config_name.assignf		( "resources/models/%s%s/hit_targets", unmanaged_string(m_full_name).c_str(), model_file_extension() );
		(*m_collision_cfg)->save_as( config_name.c_str(), configs::target_sources );
	}else
	{
		System::String^ config_name		= System::String::Format("{0}/sources/models/{1}{2}/hit_targets", m_model_editor->get_resources_path(), m_full_name, gcnew System::String(model_file_extension()) );
		editor_base::fs_helper::remove_to_recycle_bin( gcnew System::String(config_name) );
	}
}

bool edit_object_skeletal_mesh::load_collision( )
{
	for each ( collision_primitive_item_skeletal_mesh^ p in m_collision_primitives ) 
		delete p;

	m_collision_primitives.Clear();

	bool result = true;

	configs::lua_config_value const& root = (*m_collision_cfg)->get_root();

	configs::lua_config_value const& composite	= root["hit_targets"];
	configs::lua_config_iterator it				= composite.begin();
	configs::lua_config_iterator it_e			= composite.end();
	
	for( ;it!=it_e; ++it)
	{
		configs::lua_config_value const& current	= (*it);
		collision_primitive_item_skeletal_mesh^ itm	= gcnew collision_primitive_item_skeletal_mesh(m_model_editor, this);
		itm->load									( current );
		m_collision_primitives.Add					( itm );
	}

	if(complete_loaded())
		m_collision_panel->set_property_container( get_collision_property_container() );
	
	return							result;
}

void edit_object_skeletal_mesh::on_collision_settings_ready( resources::queries_result& data )
{
	if( data.is_successful() )
	{
		(*m_collision_cfg)			= static_cast_resource_ptr<configs::lua_config_ptr>(data[0].get_unmanaged_resource());
	}else
		(*m_collision_cfg)			= configs::create_lua_config();

	bool load_result				= load_collision( );

	if(!load_result)
		set_modified	( );
}

void edit_object_skeletal_mesh::reset_selection( )
{
}


void edit_object_skeletal_mesh::clear_resources( )
{
	super::clear_resources( );

	for each ( collision_primitive_item_skeletal_mesh^ p in m_collision_primitives )
		delete p;

	m_collision_primitives.Clear();

	if(has_preview_model())
		remove_preview_model();

	reset_modified	( );
}


void edit_object_skeletal_mesh::register_actions( bool bregister )
{
	super::register_actions						( bregister );

	System::String^								action_name;
	editor_base::input_engine^ input_engine		= m_model_editor->get_input_engine();
	editor_base::gui_binder^ gui_binder			= m_model_editor->get_gui_binder();
	System::Windows::Forms::MenuStrip^ ide_menu_strip = m_model_editor->get_ide()->top_menu;
	editor_base::action_delegate^ a				= nullptr;

	action_name										= "Collision View";
	if(bregister)
	{
		a												= gcnew	action_delegate( action_name, gcnew execute_delegate_managed(this, &edit_object_skeletal_mesh::action_collision_panel_view) );
		a->set_checked( gcnew checked_delegate_managed	( this, &edit_object_skeletal_mesh::collision_panel_visible));
		input_engine->register_action					( a, "" );
		gui_binder->add_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem", 0);
	}else
	{
		input_engine->unregister_action					( action_name );
		gui_binder->remove_action_menu_item				( ide_menu_strip, action_name, "ViewMenuItem" );
	}
}

math::float4x4  edit_object_skeletal_mesh::get_bone_matrix( System::String^ bone_name )
{
	return m_model->get_bone_matrix( bone_name );
}

void edit_object_skeletal_mesh::apply_surface_material(  System::String^ surface_name, resources::unmanaged_resource_ptr mtl )
{
	edit_surface^ surface = get_surface( surface_name);
	if(!has_preview_model())
		return;

	xray::render::editor::renderer* r = m_model_editor->editor_renderer();

	r->scene().set_model_material	(	m_model->get_render_model_(),
								unmanaged_string(surface->name).c_str(), 
								mtl );
}

void edit_object_skeletal_mesh::set_surface_visible(  System::String^ surface_name, bool bvisible )
{
	xray::render::editor::renderer* r = m_model_editor->editor_renderer();
	r->scene().set_model_visible(m_model->get_render_model_(), unmanaged_string(surface_name).c_str(), bvisible );
}

bool edit_object_skeletal_mesh::collision_panel_visible( )
{
	return !m_collision_panel->IsHidden;
}

void edit_object_skeletal_mesh::action_collision_panel_view( )
{
	set_dock_panel_visible(  m_collision_panel );
}

void edit_object_skeletal_mesh::set_default_layout( )
{
	super::set_default_layout( );
}

IDockContent^ edit_object_skeletal_mesh::find_dock_content( System::String^ persist_string )
{
	if(persist_string=="xray.model_editor.collision_property_grid_panel")
		return m_collision_panel;
	else
		return super::find_dock_content( persist_string );
}

math::aabb edit_object_skeletal_mesh::focused_bbox( )
{
	return super::focused_bbox( );
}



void edit_object_skeletal_mesh::switch_to_collision_clicked( button^ )
{
	set_edit_mode( edit_mode::edit_collision );
}

void edit_object_skeletal_mesh::set_edit_mode( edit_mode mode )
{
	m_current_mode = mode;

	{// collision
		bool activate = (mode == edit_mode::edit_collision);
		for each ( collision_primitive_item_skeletal_mesh^ prim in m_collision_primitives )
			prim->activate( activate );
		
	}
}

void edit_object_skeletal_mesh::remove_collision_primitive_clicked( button^ button )
{
	collision_primitive_item_skeletal_mesh^ prim	= safe_cast<collision_primitive_item_skeletal_mesh^>(button->tag);
	m_collision_primitives.Remove					( prim );

	prim->set_selected								( false );

	delete											prim;
	
	set_modified									( );
	m_collision_panel->set_property_container		( get_collision_property_container() );
}

void edit_object_skeletal_mesh::duplicate_collision_primitive_clicked( button^ button )
{
	collision_primitive_item_skeletal_mesh^ prim	= safe_cast<collision_primitive_item_skeletal_mesh^>(button->tag);
	

	collision_primitive_item_skeletal_mesh^ prim_copy	= gcnew collision_primitive_item_skeletal_mesh(m_model_editor, this);
	prim_copy->type						= prim->type;
	prim_copy->position					= prim->position;
	prim_copy->rotation					= prim->rotation;
	prim_copy->scale					= prim->scale;
	prim_copy->bone_name				= prim->bone_name;
	prim_copy->armor					= prim->armor;
	prim_copy->health					= prim->health;
	m_collision_primitives.Add			( prim_copy );

	prim_copy->activate					( true );

	m_collision_panel->set_property_container( get_collision_property_container() );

	prim_copy->set_selected				( true );
	set_modified						( );
}

void edit_object_skeletal_mesh::add_sphere_collision_primitive_clicked( button^ )
{
	collision_primitive_item_skeletal_mesh^ prim		= gcnew collision_primitive_item_skeletal_mesh(m_model_editor, this);
	prim->type						= (int)collision::primitive_sphere;
	prim->position					= Float3(float3(0,0,0));
	prim->rotation					= Float3(float3(0,0,0));
	prim->scale						= Float3(float3(1,1,1));
	prim->bone_name					= m_model->m_bone_names_array[0];
	m_collision_primitives.Add		( prim );
	set_modified					( );
	m_collision_panel->set_property_container( get_collision_property_container() );
	prim->activate					( true );
}

void edit_object_skeletal_mesh::add_box_collision_primitive_clicked ( button^ )
{
	collision_primitive_item_skeletal_mesh^ prim		= gcnew collision_primitive_item_skeletal_mesh(m_model_editor, this);
	prim->type						= (int)collision::primitive_box;
	prim->position					= Float3(float3(0,0,0));
	prim->rotation					= Float3(float3(0,0,0));
	prim->scale						= Float3(float3(1,1,1));
	prim->bone_name					= m_model->m_bone_names_array[0];
	m_collision_primitives.Add		( prim );
	set_modified					( );
	m_collision_panel->set_property_container( get_collision_property_container() );
	prim->activate					( true );
}

void edit_object_skeletal_mesh::select_bone( System::String^ bone_name )
{
	m_model->select_bone( bone_name );
}

void edit_object_skeletal_mesh::goto_bind_pose( button^ )
{
	m_model->goto_bind_pose();
}

void edit_object_skeletal_mesh::anim_play( button ^ )
{
	m_model->anim_play( m_motion_name );
}


editor::wpf_controls::property_container^ edit_object_skeletal_mesh::get_collision_property_container( )
{
	cli::array<System::String^, 1> ^ animation_bones = 	m_model->m_bone_names_array;

	property_container^	result		= gcnew property_container;

	control_container^ container	= result->add_dock_container( false );
	container->category				= "General";
	button^ but = container->add_button( "Switch to edit", gcnew Action<button^>( this, &edit_object_skeletal_mesh::switch_to_collision_clicked ) );
	but->width	= 100;

	but = container->add_button		( "BindPose", gcnew Action<button^>( this, &edit_object_skeletal_mesh::goto_bind_pose ) );
	but->width	= 100;

	but = container->add_button		( "AnimPlay", gcnew Action<button^>( this, &edit_object_skeletal_mesh::anim_play ) );
	but->width	= 100;

//	System::String^ empty = " ";
//	property_descriptor^ desc0 = gcnew property_descriptor(" ", container->category, "stub", empty, gcnew object_property_value<System::String^>(empty));
//	desc0->is_read_only = true;
//	desc0->select_behavior = select_behavior::select;
//	result->properties->add(desc0);

	motion_name = gcnew String(m_motion_name);
	property_descriptor^ desc1 = gcnew property_descriptor("Animation path", container->category, "animation_name", motion_name, gcnew object_property_value<System::String^>(motion_name));
	desc1->is_read_only = false;
	desc1->select_behavior = select_behavior::select;
	//System::String^ selected = nullptr;
	//bool result = editor_base::resource_chooser::choose("solid_visual", motion_name, nullptr, selected, true);
	desc1->set_external_editor_style(anim_instance_ui_type_editor::typeid, true);
	result->properties->add(desc1);

	//m_motion_name = prop->value;

	//property_container^ sub = gcnew property_container;
	//prim->m_current_desc = result->properties->add_container(name, "all", "no description", sub);
	//sub->inner_value = gcnew property_descriptor("Anim Path", m_model, "animation_name");
	//sub->inner_value->tag = path;
	//sub->inner_value->set_external_editor_style(test_instance_ui_type_editor::typeid, true);

	
//	property_descriptor^ prop_descriptor = gcnew property_descriptor(this, "selected_value");
//	prop_descriptor->dynamic_attributes->add(gcnew editor::wpf_controls::property_editors::attributes::combo_box_items_attribute( gcnew cli::array<System::String^, 1>(3) { "huinia", "govno", "musor" }));


//	result->properties["AnimationFile"]->dynamic_attributes->add(
//		gcnew editor::wpf_controls::property_editors::attributes::combo_box_items_attribute(gcnew cli::array<System::String^, 1>(3) { "huinia", "govno", "musor" })
//	);

	//container = result->add_dock_container(false);
	//container->category = "Animation";
	//System::String^ selected = nullptr;
	//bool result = editor_base::resource_chooser::choose("solid_visual", path, nullptr, selected, true);
	//test_instance_ui_type_editor create_path;
	//property_container^ sub = gcnew property_container;
	//editor_base::object_properties::initialize_property_container(path, sub);

//	String^ name = "Animation path";
//	String^ default_path = "resources/animations/single/human/common_anim_slot_1/free/run_move_fwd_aim_1";
//	configs::lua_config_ptr config_ptr = configs::create_lua_config();
//	configs::lua_config_value anim_path = config_ptr->get_root()["anim_path"];
//	anim_path["anim_path"] = "resources/animations/single/human/common_anim_slot_1/free/run_move_fwd_aim_1";

//	property_value_string^ prop_value = gcnew property_value_string("path", anim_path, default_path);
//	property_descriptor^ desc = gcnew property_descriptor(name, container->category, "animation_name", default_path, prop_value);
//	desc->is_read_only = true;
//	desc->select_behavior = select_behavior::select;
//	result->properties->add(desc);

	//prim->m_current_desc = result->properties->add_container(name, "all", "no description", sub);
	//sub->inner_value = gcnew property_descriptor("Anim Path", m_model, "animation_name");
	//sub->inner_value->tag = path;
	//sub->inner_value->set_external_editor_style(test_instance_ui_type_editor::typeid, true);

	container						= result->add_dock_container( false );
	container->category				= "Add new item";

	but		= container->add_button( "sphere", gcnew Action<button^>( this, &edit_object_skeletal_mesh::add_sphere_collision_primitive_clicked ) );
	but->width	= 50;
	but		= container->add_button	( "box", gcnew Action<button^>( this, &edit_object_skeletal_mesh::add_box_collision_primitive_clicked ) );
	but->width	= 40;
	//but		= container->add_button	( "cyliner", gcnew Action<button^>( this, &edit_object_skeletal_mesh::add_cylinder_collision_primitive_clicked ) );
	//but->width	= 50;

	int i=0;
	for each ( collision_primitive_item_skeletal_mesh^ prim in m_collision_primitives )
	{
		property_container^	sub		= gcnew property_container;
		editor_base::object_properties::initialize_property_container( prim, sub );
		
		System::String^ name				= System::String::Format("{0}         ", i++);
		prim->m_current_desc				= result->properties->add_container	( name, "all", "no description", sub );
		
		sub->inner_value					= gcnew property_descriptor( name, prim, "type" );

		sub->inner_value->set_compo_box_style(
			gcnew cli::array<System::String^, 1>(3){
				"sphere", 
				"box", 
				"cylinder", 
			}
		);

		prim->m_current_desc->add_button( " x ", "", gcnew Action<button^>( this, &edit_object_skeletal_mesh::remove_collision_primitive_clicked ) )->tag = prim;
		prim->m_current_desc->add_button( "duplicate", "", gcnew Action<button^>( this, &edit_object_skeletal_mesh::duplicate_collision_primitive_clicked ) )->tag = prim;
		
		prim->m_current_desc->selection_changed		+= gcnew System::Action<System::Boolean>(prim, &collision_primitive_item_skeletal_mesh::set_selected);

		property_descriptor^ desc		= gcnew property_descriptor( "bone_name", prim, "bone_name" );
		desc->select_behavior		= select_behavior::select_parent;
		sub->properties->add		( desc );
		if(animation_bones)
			desc->set_compo_box_style( animation_bones );

	}

	return result;
}

} // namespace model_editor
} // namespace xray
