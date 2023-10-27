////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef EDIT_OBJECT_BASE_H_INCLUDED
#define EDIT_OBJECT_BASE_H_INCLUDED


#include "model_editor_object.h"
#include "edit_surface.h"
#include "edit_portal.h"
#include "collision_primitive_item.h"

namespace xray {

namespace model_editor {

using namespace System;
using namespace System::Windows;
using namespace xray::editor::wpf_controls::property_editors;

using xray::editor::wpf_controls::control_containers::button;

ref class model_editor;
ref class property_grid_panel;
ref class collision_property_grid_panel;
ref class lod_view_panel;
ref class object_portals_panel;
ref class unique_name_helper;
class bsp_tree;
ref class solid_render_model_wrapper;
ref class skeletal_render_model_wrapper;
ref class portal_generation_view;
class sectors_generator;
ref class sectors_view;

/*
public ref class anim_instance_ui_type_editor : public xray::editor::wpf_controls::property_editors::i_external_property_editor
{
	typedef xray::editor::wpf_controls::property_descriptor property_descriptor;
public:
	virtual	void			run_editor(xray::editor::wpf_controls::property_editors::property^ prop);
}; // ref class anim_instance_ui_type_editor
*/

ref class lod_descriptor
{
	typedef editor::wpf_controls::hypergraph::properties_node	hypergraph_node;
public:
							lod_descriptor	( edit_object_mesh^ eo ):m_edit_object(eo){}
	void					load_settings	( configs::lua_config_value const& v );
	void					save_settings	( configs::lua_config_value& v );

	property System::String^ name;

	surfaces_list			m_surfaces;
	edit_object_mesh^		m_edit_object;
	hypergraph_node^		m_hypergraph_node;
};

ref class edit_object_mesh abstract : public model_editor_object
{
	typedef model_editor_object super;
protected:
	typedef editor::wpf_controls::hypergraph::link_event_args link_event_args;
	typedef editor::wpf_controls::hypergraph::node_event_args node_event_args;
	typedef editor::wpf_controls::hypergraph::selection_event_args selection_event_args;
	typedef editor::wpf_controls::hypergraph::menu_event_args menu_event_args;
	typedef System::Collections::Generic::List<xray::editor::wpf_controls::hypergraph::link_id^> links_list;

private:
	surfaces_dict				m_surfaces;
	property_grid_panel^		m_object_surfaces_panel;
	lod_view_panel^				m_lod_view_panel;
	void						on_model_folder_fs_iterator_ready	( vfs::vfs_locked_iterator const & fs_it );
	void						on_model_settings_ready				( resources::queries_result& data );
	void						refresh_surfaces_visibility_impl ( );

protected:
	virtual bool				load_model_settings					( );

public:
							edit_object_mesh		( model_editor^ me );
	virtual					~edit_object_mesh		( );

	virtual void			load						( System::String^ model_full_name ) override;
	virtual bool			save						( ) override;
	virtual void			revert						( ) override;
	virtual void			tick						( ) override;

	virtual void				register_actions			( bool bregister ) override;

	virtual pcstr				model_file_extension		( ) =0;
	virtual bool				save_impl					(  configs::lua_config_value& root ) =0;
	virtual void				clear_resources				( ) override;
	virtual bool				has_preview_model			( ) =0;
	void						on_surfaces_loaded			( );
	virtual math::aabb			focused_bbox				( ) override;
	math::float3				focused_point				( );
	virtual IDockContent^		find_dock_content			( System::String^ persist_string ) override;
	virtual	bool				complete_loaded				( ) override;
	virtual void				action_refresh_preview_model( ) override;

	bool						lod_panel_visible			( );
	bool						surfaces_panel_visible		( );
	virtual void				apply_surface_material		(  System::String^ surface_name, resources::unmanaged_resource_ptr mtl ) =0;
	virtual void				set_surface_visible			(  System::String^ surface_name, bool bvisible ) =0;
	bool						has_surface					( System::String^ sname ){ return m_surfaces.ContainsKey(sname); }
	edit_surface^				get_surface					( System::String^ sname ){ return m_surfaces[sname]; }
	void						refresh_properties_in_grid	( );
	void						set_surface_lod				( edit_surface^ s, lod_descriptor^ d, bool bmodify );

	lod_view_panel^				get_lod_view				( ) { return m_lod_view_panel; }
	void						refresh_surfaces_visibility	( ) {m_need_refresh_surfaces_visibility=true;}
	editor::wpf_controls::property_container^			get_surfaces_property_container	( );


	bool						lod_link_create_query		( System::String^ input_node, System::String^ output_node );
	void						lod_view_context_menu_opening( );
	void						lod_view_on_link_creating	( System::Object^, link_event_args^ args );
	void						lod_view_on_node_selecting	( System::Object^, selection_event_args^ args );
	void						lod_view_menu_item_click	( System::Object^, menu_event_args^ args );

	virtual void				action_draw_object_collision( ) override;
	void						action_surfaces_panel_view	( );
	virtual	void				action_view_isolate_selected( ) override;
	void						action_lod_panel_view		( );
	void						set_dock_panel_visible		( WeifenLuo::WinFormsUI::Docking::DockContent^ panel );
	virtual void				set_default_layout			( ) override;


private:
	System::Windows::Controls::MenuItem^	m_delete_link_menu_item;
	System::Windows::Controls::MenuItem^	m_move_to_lod_menu_item;
	System::Windows::Controls::MenuItem^	m_move_to_lod0_menu_item;
	System::Windows::Controls::MenuItem^	m_move_to_lod1_menu_item;
	System::Windows::Controls::MenuItem^	m_move_to_lod2_menu_item;
	bool									m_need_refresh_surfaces_visibility;
	bool									m_need_refresh_properties;
	array<lod_descriptor^>^					m_lods;

	void						fill_lod_view				( );
	void						clear_lod_view				( );
	void						create_lod_view				( );
	void						selection_change_lod		( lod_descriptor^ new_descriptor );

protected:
	configs::lua_config_ptr*		m_settings_cfg;
	virtual void				query_preview_model			( ) =0;
	void						update_surfaces_property_view		( );

};

ref class edit_object_solid_mesh : public edit_object_mesh
{
	typedef edit_object_mesh super;
	
public:
	enum class edit_mode{edit_none, edit_portals, edit_collision};

							edit_object_solid_mesh		( model_editor^ me );
	virtual					~edit_object_solid_mesh		( );

	virtual void			tick						( ) override;
	virtual bool			save_impl					(  configs::lua_config_value& root ) override;
	virtual void			revert						( ) override;
	virtual void			load						( System::String^ model_full_name ) override;
	virtual void			clear_resources				( ) override;
	virtual void			set_default_layout			( ) override;
	virtual IDockContent^	find_dock_content			( System::String^ persist_string ) override;
	virtual math::aabb		focused_bbox				( ) override;
	virtual void			register_actions			( bool bregister ) override;
	virtual pcstr			model_file_extension		( ) override { return ".model"; }
	virtual bool			has_preview_model			( ) override;
	virtual void			apply_surface_material		(  System::String^ surface_name, resources::unmanaged_resource_ptr mtl ) override;
	virtual void			set_surface_visible			(  System::String^ surface_name, bool bvisible ) override;
	virtual void			reset_selection				( ) override;
	virtual bool			load_model_settings			( ) override;
	virtual void			query_preview_model			( ) override;

	void					update_portals_edit_property_view	( );
	void					update_portals_generation_property_view	( );
	void					action_collision_panel_view	( );

	collision_property_grid_panel^ get_collision_panel				( )	{ return m_collision_panel; }

	xray::editor::wpf_controls::property_container^	get_portals_edit_property_container			( );
	xray::editor::wpf_controls::property_container^	get_portals_generation_property_container	( );
	editor::wpf_controls::property_container^		get_collision_property_container	( );

	bool						are_portal_vertices_complanar		( );
	void						action_add_vertex_to_portal			( );
	void						action_delete_vertex_in_portal		( );
	void						set_edit_mode						( edit_mode mode );

protected:
	bool						is_collision_edit_mode						( );
	bool						is_portals_edit_mode						( );
	void						switch_to_portals_clicked					( button^ );
	void						make_portals_coplanar_clicked				( button^ );

	void						remove_collision_primitive_clicked			( button^ );
	void						duplicate_collision_primitive_clicked		( button^ );
	void						add_sphere_collision_primitive_clicked		( button^ );
	void						add_box_collision_primitive_clicked			( button^ );
	void						add_cylinder_collision_primitive_clicked	( button^ );
	void						switch_to_collision_clicked					( button^ );
	void						add_portal_clicked							( button^ );
	void						delete_selected_portals_clicked				( button^ );

	edit_mode					m_current_mode;
private:

	// preview model
	void						on_preview_model_ready		( resources::queries_result& data );
	void						remove_preview_model		( );
	void						set_preview_model			( );

// contents loading
	void						on_collision_settings_ready	( resources::queries_result& data );
	bool						load_portals				( configs::lua_config_value const& cfg_root );

	bool						load_collision				( );
	void						save_collision				( );

	void						initialize_object_collision	( );
	void						destroy_object_collision	( );
	void						action_portals_panel_view	( );
	void						action_portal_generation_view( );
	void						action_sectors_view			( );
	bool						portals_panel_visible		( );
	bool						collision_panel_visible		( );
	bool						portal_generation_view_visible( );
	bool						sectors_view_visible		( );
	void						generate_bsp_tree_clicked	( button^ );
	void						generate_portals_clicked	( button^ );
	void						generate_sectors_clicked	( button^ );

	typedef System::Collections::Generic::List<edit_portal^> portal_list;
	portal_list						m_portals;

	collision_property_grid_panel^	m_collision_panel;
	object_portals_panel^			m_object_portals_panel;
	portal_generation_view^			m_portal_generation_view;
	solid_render_model_wrapper^		m_model;
	unique_name_helper^				m_unique_portal_name_helper;
	collision::collision_object*	m_collision_object;
	bsp_tree*						m_bsp_tree;
	sectors_generator*				m_sectors_generator;
	sectors_view^					m_sectors_view;

	collision::collision_object*	m_hq_collision_object;

	configs::lua_config_ptr*					m_collision_cfg;
public:
	solid_collision_list						m_collision_primitives;
	mass_center_item^							m_mass_center_item;
	rigid_body_construction_info_ref^			m_physics_settings;
}; // class edit_object_solid_mesh


ref class edit_object_skeletal_mesh : public edit_object_mesh
{
	typedef edit_object_mesh super;
	
public:
	enum class edit_mode{edit_none, edit_portals, edit_collision};

							edit_object_skeletal_mesh	( model_editor^ me );
							~edit_object_skeletal_mesh	( );

	virtual void			tick						( ) override;
	virtual bool			save_impl					(  configs::lua_config_value& root ) override;
	virtual void			revert						( ) override;
	virtual void			load						( System::String^ model_full_name ) override;
	virtual void			clear_resources				( ) override;
	virtual void			set_default_layout			( ) override;
	virtual IDockContent^	find_dock_content			( System::String^ persist_string ) override;
	virtual math::aabb		focused_bbox				( ) override;
	virtual void			register_actions			( bool bregister ) override;
	virtual pcstr			model_file_extension		( ) override { return ".skinned_model"; }
	virtual bool			has_preview_model			( ) override;
	virtual void			apply_surface_material		(  System::String^ surface_name, resources::unmanaged_resource_ptr mtl ) override;
	virtual void			set_surface_visible			(  System::String^ surface_name, bool bvisible ) override;
	virtual void			reset_selection				( ) override;
	virtual bool			load_model_settings			( ) override;
	virtual void			query_preview_model			( ) override;
	collision_property_grid_panel^ get_collision_panel	( )	{ return m_collision_panel; }
	editor::wpf_controls::property_container^	get_collision_property_container	( );
	math::float4x4								get_bone_matrix						( System::String^ bone_name );
	void						select_bone							( System::String^ bone_name );
	void						set_edit_mode						( edit_mode mode );

private:
	bool						collision_panel_visible		( );
	void						action_collision_panel_view	( );
	void						save_collision				( );
	bool						load_collision				( );
	void						on_collision_settings_ready	( resources::queries_result& data );
	void						switch_to_collision_clicked	( button^ );
	void						goto_bind_pose				( button^ );
	void						anim_play					( button ^ );
	void						select_animation_name		(xray::editor::wpf_controls::property_editors::property^, Object^);
	void						remove_collision_primitive_clicked( button^ );
	void						duplicate_collision_primitive_clicked( button^ );
	void						add_sphere_collision_primitive_clicked		( button^ );
	void						add_box_collision_primitive_clicked			( button^ );

	void						on_preview_model_ready		( resources::queries_result& data );
	void						remove_preview_model		( );
	void						set_preview_model			( );


	edit_mode							m_current_mode;
	skeletal_collision_list				m_collision_primitives;
	configs::lua_config_ptr*			m_collision_cfg;
	collision_property_grid_panel^		m_collision_panel;
	skeletal_render_model_wrapper^		m_model;
	pcstr								m_motion_name;
	System::String^						m_motion_str;
	IntPtr								ptrStr;

	[DisplayNameAttribute("motion"), CategoryAttribute("General")]
	[DefaultValueAttribute(""), ReadOnlyAttribute(false)]
	property System::String^ motion_name
	{
		System::String^ get()
		{
			return					m_motion_str;
		}

		void set(System::String^ motion)
		{
			m_motion_str = motion;
		}
	}

}; // class edit_object_skeletal_mesh


} // namespace model_editor
} // namespace xray

#endif // #ifndef EDIT_OBJECT_BASE_H_INCLUDED
