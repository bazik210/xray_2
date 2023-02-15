////////////////////////////////////////////////////////////////////////////
//	Created		: 22.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "render_model_grass.h"
#include <xray/render/facade/vertex_input_type.h>
#include <xray/render/core/decl_utils.h>
#include <xray/render/engine/model_format.h>
#include <xray/render/core/res_declaration.h>
#include <xray/render/core/resource_manager.h>
#include "model_manager.h"
#include <xray/render/core/custom_config.h>
#include <xray/render/core/untyped_buffer.h>
#include <xray/render/core/res_effect.h>
#include <xray/render/core/res_geometry.h>

#pragma warning( push )
#pragma warning( disable : 4995 )
#include <d3dx9mesh.h>
#pragma warning( pop )

namespace xray {
namespace render {

grass_render_surface::grass_render_surface( )
{
	m_vertex_input_type = grassmesh_vertex_input_type;
}

/*
D3DVERTEXELEMENT9	static_decl[] =
{
	{0, 0,  D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_POSITION,	0 },
	{0, 12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_NORMAL,	0 },
	{0, 16, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TANGENT,	0 },
	{0, 20, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_BINORMAL,	0 },
	{0, 24, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TEXCOORD,	0 },
	D3DDECL_END()
};
*/

void grass_render_surface::load( configs::binary_config_value const& properties, memory::chunk_reader& chunk)
{
	super::load					( properties, chunk);
	
	D3DVERTEXELEMENT9 const*  vFormat = NULL;
	
	memory::reader reader		= chunk.open_reader( model_chunk_vertices);
	vFormat						= ( D3DVERTEXELEMENT9*) reader.pointer();
	u32 const dcl_len			= D3DXGetDeclLength( vFormat) + 1;
	
	vector<D3D_INPUT_ELEMENT_DESC> decl_code;
	decl_utils::convert_vertex_declaration( vector<D3DVERTEXELEMENT9>( vFormat,vFormat+dcl_len), decl_code);
	
	ref_declaration decl		= resource_manager::ref().create_declaration( &*decl_code.begin(), decl_code.size());
	reader.advance				( dcl_len*sizeof( D3DVERTEXELEMENT9));
	
	// The visuals are create with dynamic usage temporary.
	bool dynamic		= true;
	
	m_render_geometry.vertex_count	= reader.r_u32				();
	u32 vStride						= D3DXGetDeclVertexSize		( vFormat,0);
	untyped_buffer_ptr vb			= resource_manager::ref().create_buffer( m_render_geometry.vertex_count*vStride, reader.pointer(), enum_buffer_type_vertex, dynamic);
	
//--- read indices here
	
	reader								= chunk.open_reader( model_chunk_indices);
	m_render_geometry.index_count		= reader.r_u32();
	m_render_geometry.primitive_count	= m_render_geometry.index_count/3;
	
	untyped_buffer_ptr ib				= resource_manager::ref().create_buffer( m_render_geometry.index_count*2, reader.pointer(), enum_buffer_type_index, false);
	
	m_render_geometry.geom				= resource_manager::ref().create_geometry( NULL, vStride, *vb, *ib);
}

grass_render_model::~grass_render_model()
{
	children::iterator			it = m_children.begin();
	children::const_iterator	en = m_children.end();
	for( ; it != en; ++it)
		DELETE( *it );
	
	if(m_locators)
		FREE(m_locators);
}

void grass_render_model::append_surface( render_surface* child )
{
	m_children.push_back(static_cast<grass_render_surface*>(child));
}

} // namespace render 
} // namespace xray 
