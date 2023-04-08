////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_patrol_graph_add_key_action.h"
#include "object_patrol_graph.h"
#include <xray/editor/base/collision_object_types.h>
#include "collision_object_dynamic.h"

namespace xray{
namespace editor{

object_patrol_graph_add_key_action::object_patrol_graph_add_key_action( String^ name, editor_base::scene_view_panel^ view )
:super			( name ),
m_window_view	( view )
{}

bool			object_patrol_graph_add_key_action::do_it						( )
{
	if( !m_window_view->is_mouse_in_view( ) )
		return false;

	if( m_object_patrol_graph == nullptr )
		return false;

	collision::object const* object		= NULL;

	math::float3 pos;

	if( m_window_view->ray_query( editor_base::collision_object_type_dynamic | editor_base::collision_object_type_terrain, &object, &pos ) )
	{
		ASSERT( object->get_type( ) & ( editor_base::collision_object_type_dynamic | editor_base::collision_object_type_terrain ) );

		m_object_patrol_graph->add_key( pos );

		return true;
	}
	else
		return false;
}

void			object_patrol_graph_add_key_action::set_patrol_graph_object		( object_patrol_graph^ obj )
{
	m_object_patrol_graph = obj;
}

} // namespace editor
} // namespace xray