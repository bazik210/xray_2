////////////////////////////////////////////////////////////////////////////
//	Created		: 15.07.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_RESMAN_INFO_H_INCLUDED
#define GAME_RESMAN_INFO_H_INCLUDED

#include <boost/intrusive/set.hpp>
#include <xray/intrusive_double_linked_list.h>
#include "game_resman_satisfaction_tree.h"

namespace xray {
namespace resources {

class resource_tree_compare
{
public:
	bool	operator () (resource_base const & left, resource_base const & right) const;
};

typedef boost::intrusive::member_hook
	< 
		resource_base, 
		resource_tree_member_hook,
		& resource_base::grm_resource_tree_hook 
	> resource_tree_helper_option;

typedef	boost::intrusive::multiset< resource_base, 
							   		resource_tree_helper_option, 
							   		boost::intrusive::compare< resource_tree_compare >,
							   		boost::intrusive::constant_time_size<false> >	resource_tree_type;

typedef intrusive_list< query_result, query_result *, & query_result::m_next_in_game_resource_manager_queue, threading::mutex > out_of_memory_queue;

class memory_type_data
{
public:
	typedef	intrusive_double_linked_list<	resource_base, resource_base *, 
											& resource_base::m_prev_in_memory_type_list, & resource_base::m_next_in_memory_type_list, 
											threading::single_threading_policy,
											size_policy	>	resources_list;

	memory_type_data				() : resources_sort_actuality_tick(0) {}

	resources_list						resources;
	u64									resources_sort_actuality_tick;
	out_of_memory_queue *				queue;
};

class resource_tree_key_compare
{
public:
	bool   operator () (u32 const size, resource_base const & right) const
	{
		if ( size < right.get_size() )
			return							true;

		return								false;
	}
};

} // namespace resources
} // namespace xray

#endif // #ifndef GAME_RESMAN_INFO_H_INCLUDED