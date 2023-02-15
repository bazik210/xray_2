////////////////////////////////////////////////////////////////////////////
//	Created		: 19.05.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "project.h"
#include "object_base.h"
#include "tool_base.h"
#include "level_editor.h"
#include "object_skeleton_visual.h"

#pragma managed( push, off )
#	include <xray/render/engine/model_format.h>
#include <xray/geometry_utils.h>
#pragma managed( pop )

namespace xray{
namespace editor{

scene_statistic::stat_node^ scene_statistic::find_child( stat_node::nodes^ storage, System::String^ name )
{
	for each(scene_statistic::stat_node^ n in storage)
		if(n->m_name==name)
			return n;
	return nullptr;
}

scene_statistic::stat_node^ scene_statistic::build_hierrarchy( System::String^ key )
{
	array<System::String^>^ tree		= key->Split(':');
	stat_node::nodes^ current_storage	= %m_storage;
	scene_statistic::stat_node^ result	= nullptr;

	u32 size	= tree->Length;
	for(u32 i=0; i<size; ++i)
	{
		System::String^ name = tree[i];

		result = find_child( current_storage, name );
		if(result)
		{
			current_storage = %result->m_childs;
		}else
		{
			result				= gcnew scene_statistic::stat_node;
			result->m_name		= name;
			R_ASSERT(result->m_name->Length!=0);
			result->m_value		= 0;
			current_storage->Add( result );
			current_storage		= %result->m_childs;
		}
	}

	return result;
}

void scene_statistic::add_statistic( System::String^ key, u32 value )
{
	scene_statistic::stat_node^ n = build_hierrarchy( key );
	n->m_value						+= value;
}

void scene_statistic::add_properties( stat_node::nodes^ storage, wpf_controls::property_container^ container )
{
	for each(scene_statistic::stat_node^ n in storage)
	{
		property_descriptor^ curr_desc = nullptr;

		if(n->m_childs.Count!=0)
		{
			wpf_controls::property_container^ sub	= gcnew wpf_controls::property_container;

			curr_desc = container->properties->add_container	( n->m_name, "stats", "", sub );

			add_properties							( %n->m_childs, sub );
		}


		if(n->m_value!=0)
		{ // add value item
				property_descriptor^ desc = gcnew property_descriptor( 
											n->m_name,
											"stats",
											"",
											nullptr, 
											gcnew wpf_controls::object_property_value<u32>(n->m_value)
											);

				if(curr_desc)
				{
					curr_desc->inner_properties->Add("asd", desc);
				}else
					container->properties->add	( desc );
		}

	}
}

wpf_controls::property_container^ scene_statistic::get_property_container	( )
{
	wpf_controls::property_container^ result = gcnew wpf_controls::property_container;
	add_properties	( %m_storage, result);

	//for each( System::String^ k in m_storage.Keys )
	//{
	//	wpf_controls::property_container^ to_add	= result;

	//	System::String^ prop_name					= k;
	//	array<System::String^>^ tree				= k->Split(':');

	//	u32 size  = tree->Length;
	//	for(u32 i=0; i<size-1; ++i)
	//	{
	//		System::String^ curr = tree[i];
	//	}

	//	u32 value					= m_storage[k];
	//	property_descriptor^ desc = gcnew property_descriptor( 
	//								prop_name, 
	//								gcnew wpf_controls::object_property_value<u32>(value)
	//								);

	//	result->properties->add		( desc );
	//}

	return result;
}


void on_vertices_loaded( resources::queries_result& data, 
						xray::geometry_utils::geometry_collector* collector, 
						float4x4 const* m, bool is_skeleton )
{
	if(!data.is_successful())
		return;

	resources::pinned_ptr_const<u8> vertices_ptr	( data[0].get_managed_resource() );
	resources::pinned_ptr_const<u8> indices_ptr		( data[1].get_managed_resource() );

	memory::reader vertices_reader		( vertices_ptr.c_ptr(), vertices_ptr.size() );
	memory::reader indices_reader		( indices_ptr.c_ptr(), indices_ptr.size() );

	u32 vert_struct_size				= is_skeleton ? sizeof(render::vert_boned_4w) : sizeof(render::vert_static);
	u32 position_offset					= is_skeleton ? (sizeof(u16[4])) : 0;
	
	u32 normal_offset					= position_offset + sizeof(float3);// normal lays after position

	u32 uv_offset						= is_skeleton ?  
										(
										sizeof(u16[4])
										+sizeof(float3)//p
										+sizeof(float3)//n
										+sizeof(float3)//t
										+sizeof(float3)//bn
										+sizeof(float[3])//w
										)
										: 
										(
										sizeof(float3) //p
										+sizeof(float3)//n
										+sizeof(float3)//t
										+sizeof(float3)//bn
										);

	/*u32 icount =*/ indices_reader.r_u32();
	u32 vert_start_offset = sizeof(u32);// + position_offset;

	while(!indices_reader.eof())
	{
		u16 idx0			= indices_reader.r_u16();
		u16 idx1			= indices_reader.r_u16();
		u16 idx2			= indices_reader.r_u16();

		geometry_utils::geometry_collector_vert v0,v1,v2;

		//v0
		vertices_reader.seek(vert_start_offset+vert_struct_size*idx0 + position_offset);
		v0.p				= vertices_reader.r_float3() * (*m);
		v0.p.z				*=-1.0f;

		vertices_reader.seek(vert_start_offset+vert_struct_size*idx0 + normal_offset);
		v0.n				= vertices_reader.r_float3();
		v0.n.z				*=-1.0f;

		vertices_reader.seek(vert_start_offset+vert_struct_size*idx0 + uv_offset);
		v0.uv				= vertices_reader.r_float2();
		v0.uv.y				= 1.0f - v0.uv.y;

		//v1
		vertices_reader.seek(vert_start_offset+vert_struct_size*idx1 + position_offset);
		v1.p				= vertices_reader.r_float3() * (*m);
		v1.p.z				*=-1.0f;

		vertices_reader.seek(vert_start_offset+vert_struct_size*idx1 + normal_offset);
		v1.n				= vertices_reader.r_float3();
		v1.n.z				*=-1.0f;

		vertices_reader.seek(vert_start_offset+vert_struct_size*idx1 + uv_offset);
		v1.uv				= vertices_reader.r_float2();
		v1.uv.y				= 1.0f - v1.uv.y;

		//v2
		vertices_reader.seek(vert_start_offset+vert_struct_size*idx2 + position_offset);
		v2.p				= vertices_reader.r_float3() * (*m);
		v2.p.z				*=-1.0f;

		vertices_reader.seek(vert_start_offset+vert_struct_size*idx2 + normal_offset);
		v2.n				= vertices_reader.r_float3();
		v2.n.z				*=-1.0f;

		vertices_reader.seek(vert_start_offset+vert_struct_size*idx2 + uv_offset);
		v2.uv				= vertices_reader.r_float2();
		v2.uv.y				= 1.0f - v2.uv.y;

		if( v0.p.is_similar(v1.p) || v0.p.is_similar(v2.p) || v1.p.is_similar(v2.p) )
			continue;

		collector->add_triangle( v0, v2, v1 );
	}
}

void on_model_folder_fs_iterator_ready( vfs::vfs_locked_iterator const & in_fs_it, 
									   xray::geometry_utils::geometry_collector* collector, pcstr model_path, float4x4 const* m, bool is_skeleton )
{
	if( in_fs_it.is_end() )
		return;
	
	vfs::vfs_iterator fs_it			= in_fs_it.children_begin();
	vectora<fs_new::virtual_path_string>	surface_names(g_allocator);
	while(!fs_it.is_end())
	{
		if( fs_it.is_folder() )
		{
			fs_new::virtual_path_string	sname	= model_path;
			sname.append			( "/" );
			sname.append			( fs_it.get_name() );
			sname.append			( "/" );
			sname.append			( "vertices" );
			surface_names.push_back	( sname );

			sname					= model_path;
			sname.append			( "/" );
			sname.append			( fs_it.get_name() );
			sname.append			( "/" );
			sname.append			( "indices" );
			surface_names.push_back	( sname );

		}
		++fs_it;
	}
	u32 count = surface_names.size();
	for(u32 i=0; i<count; i+=2)
	{
		resources::request r[]={
			{ surface_names[i].c_str(), resources::raw_data_class },
			{ surface_names[i+1].c_str(), resources::raw_data_class },
		};

		resources::query_resources_and_wait(
			r,
			2,
			boost::bind(&on_vertices_loaded, _1, collector, m, is_skeleton),
			g_allocator
			);
	}
}

void export_model_as_obj( project^ p, 
							xray::geometry_utils::geometry_collector& collector, 
							System::String^ name, 
							math::float4x4 const& m, 
							bool is_skeleton )
{
	XRAY_UNREFERENCED_PARAMETERS( p );
	fs_new::virtual_path_string	model_name;

	model_name.assignf("resources/models/%s.model/render", unmanaged_string(name).c_str() );
	
	resources::query_vfs_iterator_and_wait(
		model_name.c_str(), 
		boost::bind(&on_model_folder_fs_iterator_ready, _1, &collector, model_name.c_str(), &m, is_skeleton ),
		g_allocator,
		resources::recursive_true
		);
}

void project::export_as_obj( bool selection_only )
{
	System::Windows::Forms::SaveFileDialog		saveFileDialog;
	
	if( System::Windows::Forms::DialogResult::OK == saveFileDialog.ShowDialog() )
	{

		project_items_list^	selection = (selection_only) ? selection_list() : get_all_items( );

		xray::geometry_utils::geometry_collector collector(debug::g_mt_allocator);
		for each( project_item_base^ itm in selection)
		{
			object_base^ o = itm->get_object();
			if(o==nullptr)
				continue;

			if(o->owner_tool()->name()!="solid_visual")
				continue;

			bool is_skeleton = dynamic_cast<object_skeleton_visual^>(o)!=nullptr;

			export_model_as_obj			( this, collector, o->get_library_name(), o->get_transform(), is_skeleton );
		}

		System::String^ fn	= saveFileDialog.FileName->Replace("/", "\\");
		fn					+=".obj";

		collector.write_obj_file		(unmanaged_string(fn).c_str(), 100.0f, true, true );
		get_level_editor()->ShowMessageBox( System::String::Format("OBJ file export to {0} succeeded.", fn), 
											System::Windows::Forms::MessageBoxButtons::OK, 
											System::Windows::Forms::MessageBoxIcon::Information );

	}
}


}
}
