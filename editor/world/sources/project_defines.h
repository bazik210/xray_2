////////////////////////////////////////////////////////////////////////////
//	Created		: 17.03.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PROJECT_DEFINES_H_INCLUDED
#define PROJECT_DEFINES_H_INCLUDED

#pragma managed( push, off )
#	include <xray/render/engine/base_classes.h>
#pragma managed( pop )

namespace xray {
namespace editor {

public delegate void execute_delegate();

typedef controls::tree_node							tree_node;
typedef System::Windows::Forms::TreeNodeCollection	tree_node_collection;

enum	enum_selection_method{ 
		enum_selection_method_set,
		enum_selection_method_add, 
		enum_selection_method_invert, 
		enum_selection_method_subtract, 
		enum_selection_method_NONE };

ref class project;
ref class object_base;
ref class project_item_base;
ref class project_item_object;
ref class project_item_folder;
ref class tool_base;
ref class level_editor;
class editor_world;

typedef System::Collections::Generic::List<System::String^>			string_list;
typedef System::Collections::Generic::List<project_item_base^>		project_items_list;

typedef System::Collections::Generic::List<object_base^>			object_base_list;
typedef System::Collections::Generic::List<u32>						item_id_list;

using wpf_controls::property_editors::attributes::value_range_and_format_attribute;
using wpf_controls::property_editors::attributes::external_editor_attribute;

using wpf_controls::property_descriptor;
using wpf_controls::property_container;
using wpf_controls::property_editors::attributes::external_editor_event_handler;

using xray::editor_base::transform_control_base;

public ref class scene_statistic
{
public:
	void	add_statistic	( System::String^ key, u32 value );
	virtual wpf_controls::property_container^	get_property_container	( );

private:

	ref struct stat_node
	{
		typedef System::Collections::Generic::List<stat_node^>	nodes;

		System::String^		m_name;
		u32					m_value;
		nodes				m_childs;
	};

	stat_node^				build_hierrarchy	( System::String^ key );
	stat_node^				find_child			( stat_node::nodes^ storage, System::String^ name );
	void					add_properties		( stat_node::nodes^ storage, wpf_controls::property_container^ container );

	stat_node::nodes		m_storage;
};

public ref struct add_object_to_scene
{
	tool_base^			m_tool;
	System::String^		m_library_item_name;
};

public value struct id_matrix
{
	u32				id;
	math::float4x4*	matrix;
	math::float3*	pivot;
};	
typedef System::Collections::Generic::List<id_matrix>		id_matrices_list;

public enum class subst_resource_result{
	subst_one,
	subst_all_by_name,
	subst_all_any_name,
	ignore_one,
	ignore_all_by_name,
	ignore_all_any_name
};

public ref class subst_record{
public:
	subst_resource_result	result;
	System::String^			source_name;
	System::String^			dest_name;
};

} // namespace editor
} // namespace xray

#endif // #ifndef PROJECT_DEFINES_H_INCLUDED