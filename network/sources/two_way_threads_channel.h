////////////////////////////////////////////////////////////////////////////
//	Created		: 24.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_NETWORK_TWO_WAY_THREADS_CHANNEL_H_INCLUDED
#define ENGINE_NETWORK_TWO_WAY_THREADS_CHANNEL_H_INCLUDED

#include <xray/intrusive_spsc_queue.h>
#include <xray/one_way_threads_channel.h>
#include "order.h"
#include "response.h"

namespace xray {
namespace network {

typedef intrusive_spsc_queue< order, order, &order::next_for_orders >				orders_queue_type;
typedef intrusive_spsc_queue< response, response, &response::next_for_responses >	responses_queue_type;

typedef one_way_threads_channel<
	responses_queue_type,
	responses_queue_type
>	responses_channel_type;

typedef one_way_threads_channel<
	orders_queue_type,
	orders_queue_type
>	orders_channel_type;

struct two_way_threads_channel : private boost::noncopyable {
	inline two_way_threads_channel	(
			memory::doug_lea_allocator& responses_allocator,
			memory::base_allocator& orders_allocator
		) :
		responses	( responses_allocator ),
		orders		( orders_allocator )
	{
	}

	responses_channel_type	responses;
	orders_channel_type		orders;
}; // struct two_way_threads_channel

} // namespace network
} // namespace xray

#endif // #ifndef ENGINE_NETWORK_TWO_WAY_THREADS_CHANNEL_H_INCLUDED