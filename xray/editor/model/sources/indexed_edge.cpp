////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "indexed_edge.h"
#include "bsp_tree_utilities.h"
namespace xray {
namespace model_editor {


struct edge_unused_and_contains : public std::unary_function<u32, bool>
{
	edge_unused_and_contains( u32 index ):m_index( index ){}
	bool operator() ( indexed_edge const& edge ) const
	{
		return !edge.is_used() && ( m_index == edge.i1 || m_index ==edge.i2 );
	}
private:
	u32 m_index;
};//struct edge_unused_and_contains

void extract_circuits_from_edges( edges_buffer_type& edges, circuits_type& circuits )
{
	edges_buffer_type::iterator it;
	u32 index;
	while ( ( it = std::find_if( edges.begin(), edges.end(), std::not1( std::mem_fun_ref( &indexed_edge::is_used ) ) ) ) != edges.end() )
	{
		index_vector circuit;
		circuit.push_back( it->i1 );
		do 
		{
			index	= it->i2 != circuit.back() ? it->i2 : it->i1;
			circuit.push_back( index );
			it->used = true;
		} 
		while ( ( it = std::find_if( edges.begin(), edges.end(), edge_unused_and_contains( index ) ) ) != edges.end() );
		R_ASSERT( circuit.end() == std::unique( circuit.begin(), circuit.end() ) );
		circuits.push_back( circuit );
	}
}

void remove_unnecessary_vertices_from_circuit( coord_vector const vertices, index_vector& circuit )
{
	if ( circuit.empty() )
		return;
	if ( circuit.front() == circuit.back() )
		circuit.pop_back();
	index_vector result_circuit;
	result_circuit.reserve( circuit.size() );
	for ( u32 i = 0; i < circuit.size(); ++i )
	{
		u32 const current	= circuit[ i ];
		u32 const previous	= circuit[ ( circuit.size() + i - 1 ) % circuit.size() ];
		u32 const next		= circuit[ ( i + 1 ) % circuit.size() ];
		if ( !is_between( vertices[ current ], vertices[ previous ], vertices[ next ] ) )
			result_circuit.push_back( current );
	}
	circuit = result_circuit;
}
}//namespace xray 
}//namespace model_editor
