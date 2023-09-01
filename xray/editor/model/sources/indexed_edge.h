////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef INDEXED_EDGE_H_INCLUDED
#define INDEXED_EDGE_H_INCLUDED
#include "bsp_typedefs.h"

namespace xray {
namespace model_editor {

struct indexed_edge
{
	u32 i1;
	u32 i2;
	bool used;
	indexed_edge( u32 one, u32 two ):
	i1( std::min( one, two ) ), i2( std::max( one, two ) ), used( false ){}
	
	bool operator < ( indexed_edge const& other ) const
	{
		return i1 < other.i1 || ( i1 == other.i1 && i2 < other.i2 );
	}

	bool operator == ( indexed_edge const& other ) const
	{
		return i1 == other.i1 && i2 == other.i2;
	}

	bool is_used() const
	{
		return used;
	}
};//struct indexed_edge
typedef buffer_vector<indexed_edge> edges_buffer_type;
typedef vector< index_vector > circuits_type;
void extract_circuits_from_edges( edges_buffer_type& edges, circuits_type& circuits );
void remove_unnecessary_vertices_from_circuit( coord_vector const vertices, index_vector& circuit );
} // namespace model_editor
} // namespace xray

#endif // #ifndef INDEXED_EDGE_H_INCLUDED