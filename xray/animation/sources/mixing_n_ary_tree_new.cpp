////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <xray/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_destroyer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include <xray/animation/base_interpolator.h>
#include "mixing_n_ary_tree_dumper.h"
#include "bone_matrices_computer.h"
#include "mixing_n_ary_tree_time_calculator.h"
#include "mixing_n_ary_tree_event_iterator.h"
#include "mixing_n_ary_tree_animation_time_calculator.h"
#include "mixing_n_ary_tree_weight_calculator.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_time_scale_start_time_modifier.h"

#include "i_editor_mixer_inline.h"

using xray::animation::mixing::n_ary_tree;
using xray::animation::skeleton;
using xray::math::float4x4;
using xray::animation::mixing::animation_state;

n_ary_tree::~n_ary_tree				( )
{
	destroy						( );
}

void n_ary_tree::destroy			( )
{
	if ( !m_root )
		return;

	if ( m_reference_counter->reference_count() > 1 ) {
		m_reference_counter		= 0;
		return;
	}

	ASSERT						( static_cast<pcvoid>(m_reference_counter.c_ptr()) <= m_interpolators );
	ASSERT						( static_cast<pcvoid>(m_interpolators) <= m_root );

	n_ary_tree_destroyer		tree_destroyer;
	for (n_ary_tree_animation_node* i = m_root; i; i=i->m_next_animation )
		i->accept				( tree_destroyer );

	for (base_interpolator const* const* i = m_interpolators, * const*	const e = i + m_interpolators_count; i != e; ++i )
		(*i)->~base_interpolator( );

	animation_state* i			= m_animation_states;
	animation_state* const e	= m_animation_states + m_animations_count;
	for ( ; i != e; ++i )
		(*i).~animation_state	( );
}

void n_ary_tree::dump_animation_states	( u32 const current_time_in_ms ) const
{
	LOG_INFO						( "-------time: %5d------------", current_time_in_ms );

	n_ary_tree_animation_node* j	= m_root;
	animation_state const* i		= m_animation_states;
	animation_state const* const e	= i + m_animations_count;
	for ( ; i != e; ++i, j = j->m_next_animation ) {
		cubic_spline_skeleton_animation_pinned	pinnded_animation ( (animation_intervals_begin( (*i).event_iterator.animation() ) + (*i).animation_interval_id)->animation() );
		LOG_TRACE					(
			"%s : weight=%f, time=%f, animation_length=%f, interval_id=%d, interval_time=%f, interval_length=%f",
			(*i).event_iterator.animation().animation().identifier(),
			(*i).weight,
			(*i).bone_matrices_computer.animation_time,
			pinnded_animation->length_in_frames()/default_fps,
			(*i).animation_interval_id,
			(*i).animation_interval_time,
			(animation_intervals_begin( *j ) + (*i).animation_interval_id)->length()
		);
	}
}

void n_ary_tree::dump_tree				( u32 const current_time_in_ms ) const
{
	LOG_TRACE						( "------------------------------" );

	mixing::n_ary_tree_dumper dumper( current_time_in_ms );
	n_ary_tree_animation_node* i = m_root;
	for ( ; i; i = i->m_next_animation )
		(*i).accept					( dumper );
}

float4x4 n_ary_tree::get_object_transform	( )
{
	return							m_animation_states ? bone_matrices_computer( 0, m_animation_states, m_animations_count ).get_object_transform( ) * m_object_transform : m_object_transform;
}

void n_ary_tree::set_object_transform		( n_ary_tree_animation_node& animation_node )
{
	animation_state& animation_state = animation_node.animation_state();
	n_ary_tree_node_animation_interval const* const animation_interval	= animation_intervals_begin( animation_node ) + animation_node.animation_state().animation_interval_id;
	current_frame_position frame_position;
	frame frame_transform;
	{
		cubic_spline_skeleton_animation_pinned pinned_animation	= cubic_spline_skeleton_animation_pinned( animation_interval->animation() );
		frame_transform						=
			pinned_animation->bone( u32(0) ).bone_frame (
				animation_state.animation_interval_time * pinned_animation->get_frames_per_second( ),
				frame_position
			);
	}

	if ( animation_state.event_iterator.are_there_any_weight_transitions() ) {
		animation_state.bone_matrices_computer.previous_object_movement.translation		= frame_transform.translation;
		animation_state.bone_matrices_computer.previous_object_movement.rotation		= math::quaternion( frame_transform.rotation );
		animation_state.bone_matrices_computer.previous_object_movement.scale			= frame_transform.scale;

		animation_state.bone_matrices_computer.accumulated_object_movement.translation	= float3( 0.f, 0.f, 0.f );
		animation_state.bone_matrices_computer.accumulated_object_movement.rotation		= math::quaternion( float3(0.f, 0.f, 0.f) );
		animation_state.bone_matrices_computer.accumulated_object_movement.scale		= float3( 1.f, 1.f, 1.f );
	}
	else {
		animation_state.bone_matrices_computer.accumulated_object_movement.translation	= frame_transform.translation;
		animation_state.bone_matrices_computer.accumulated_object_movement.rotation		= math::quaternion( frame_transform.rotation );//.conjugate();
		animation_state.bone_matrices_computer.accumulated_object_movement.scale		= frame_transform.scale;
	}
}

void n_ary_tree::set_object_transform		( float4x4 const& object_transform )
{
	for ( n_ary_tree_animation_node* current_animation = m_root; current_animation; current_animation = current_animation->m_next_animation )
		set_object_transform			( *current_animation );

	m_object_transform					= object_transform;
}

void n_ary_tree::compute_bones_matrices		( skeleton const& skeleton, float4x4* const begin, float4x4* const end ) const
{
	bone_matrices_computer( &skeleton, m_animation_states, m_animations_count ).compute_bones_matrices( begin, end );
}

void n_ary_tree::accumulate_object_movement					(
		n_ary_tree_animation_node& animation_node,
		float const animation_interval_time,
		u32 const time_in_ms
	)
{
	animation_state& animation_state = animation_node.animation_state();
	n_ary_tree_weight_calculator weight_calculator( time_in_ms );
	animation_node.accept					( weight_calculator );
	float const weight						= weight_calculator.weight( );
	animation_state.weight					= weight;
	animation_state.animation_interval_time	= animation_interval_time;
	
	n_ary_tree_node_animation_interval const* animation_interval	= animation_intervals_begin( animation_node ) + animation_state.animation_interval_id;
	cubic_spline_skeleton_animation_pinned pinned_animation	= cubic_spline_skeleton_animation_pinned( animation_interval->animation() );
	current_frame_position frame_position;
	frame const& frame_transform			=
		pinned_animation->bone( u32(0) ).bone_frame (
			(animation_interval->time_offset() + animation_interval_time) * pinned_animation->get_frames_per_second( ),
			frame_position
		);

	math::quaternion const& frame_transform_rotation	= math::quaternion(frame_transform.rotation);

	object_movement& previous_object_movement		= animation_state.bone_matrices_computer.previous_object_movement;
	object_movement& accumulated_object_movement	= animation_state.bone_matrices_computer.accumulated_object_movement;

	accumulated_object_movement.translation	+= weight*( frame_transform.translation - previous_object_movement.translation );
	accumulated_object_movement.rotation	= ( weight*( frame_transform_rotation * math::conjugate(previous_object_movement.rotation) ) ) * accumulated_object_movement.rotation;
	accumulated_object_movement.scale		*= weight*( frame_transform.scale / previous_object_movement.scale );

	previous_object_movement.translation	= frame_transform.translation;
	previous_object_movement.rotation		= frame_transform_rotation;
	previous_object_movement.scale			= frame_transform.scale;
}

void n_ary_tree::update_synchronization_group_using_integration	(
		n_ary_tree_animation_node& animation_node,
		u32 const start_time_in_ms,
		u32 const target_time_in_ms
	)
{
	animation_state& animation_state		= animation_node.animation_state();
	n_ary_tree_base_node* time_scale_node	= *animation_node.operands( sizeof(n_ary_tree_animation_node) );

	float accumulated_animation_time		= animation_state.animation_interval_time;
	static u32 const integration_interval_length_in_ms	= 10;
	static float const integration_interval_length		= integration_interval_length_in_ms/1000.f;

	n_ary_tree_node_animation_interval const* const animation_interval	= animation_intervals_begin( animation_node ) + animation_state.animation_interval_id;
	float const animation_interval_length	= animation_interval->length();
	u32 const full_intervals_count			= (target_time_in_ms - start_time_in_ms)/integration_interval_length_in_ms;
	for ( u32 i = 0; i <= full_intervals_count; ++i ) {
		n_ary_tree_time_scale_calculator time_scale_calculator(
			i < full_intervals_count ? (start_time_in_ms + (2*i+1)*(integration_interval_length_in_ms/2)) : target_time_in_ms,
			0.f,
			i < full_intervals_count ? (start_time_in_ms + (i ? 2*i-1 : 0)*(integration_interval_length_in_ms/2)) : (start_time_in_ms + full_intervals_count*integration_interval_length_in_ms),
			n_ary_tree_time_scale_calculator::permit_transitions_destroying
		);
		time_scale_node->accept				( time_scale_calculator );
		if ( time_scale_node != time_scale_calculator.result() ) {
			R_ASSERT						( time_scale_calculator.result() );
			time_scale_node					= time_scale_calculator.result();
			*animation_node.operands( sizeof(n_ary_tree_animation_node) )	= time_scale_node;
		}

		if ( i < full_intervals_count )
			accumulated_animation_time		+= time_scale_calculator.time_scale() * integration_interval_length;
		else {
			float const tail				= float((target_time_in_ms - start_time_in_ms)%integration_interval_length_in_ms)/float(integration_interval_length_in_ms);
			accumulated_animation_time		+= time_scale_calculator.time_scale() * integration_interval_length * tail;
		}

		u32 const update_time				= i < full_intervals_count ? (start_time_in_ms + (i+1)*integration_interval_length_in_ms) : target_time_in_ms;
		if ( animation_state.event_iterator.are_there_any_weight_transitions() )
			accumulate_object_movement		(
				animation_node,
				accumulated_animation_time,
				update_time
			);

		int const synchronization_group_id	= animation_node.synchronization_group_id();
		if ( synchronization_group_id == -1 )
			continue;

		for (
				n_ary_tree_animation_node* current_driven_animation = animation_node.m_next_animation ;
				current_driven_animation && (current_driven_animation->synchronization_group_id() == synchronization_group_id) ;
				current_driven_animation = current_driven_animation->m_next_animation
			)
		{
			if ( !current_driven_animation->animation_state().event_iterator.are_there_any_weight_transitions() )
				continue;

			n_ary_tree_node_animation_interval const* animation_interval	= animation_intervals_begin( *current_driven_animation ) + current_driven_animation->animation_state().animation_interval_id;
			float const drive_animation_time_scale	= animation_interval->length() / animation_interval_length;;
			accumulate_object_movement		(
				*current_driven_animation,
				accumulated_animation_time * drive_animation_time_scale,
				update_time
			);
		}
	}
}

void n_ary_tree::update_animation_state	(
		n_ary_tree_animation_node& animation_node,
		u32 const start_time_in_ms,
		u32 const target_time_in_ms
	)
{
	animation_state& animation_state	= animation_node.animation_state();
	n_ary_tree_weight_calculator weight_calculator( target_time_in_ms );
	weight_calculator.visit		( animation_node );
	animation_state.weight		= weight_calculator.weight( );

	if ( !animation_node.animation_to_synchronize_with() ) {
		animation_state.animation_interval_time	=
			n_ary_tree_animation_time_calculator(
				animation_node,
				start_time_in_ms,
				animation_state.animation_interval_time,
				target_time_in_ms,
				false
			).animation_time( );
		return;
	}

	mixing::animation_state const* const driving_animation_state	= &animation_node.animation_to_synchronize_with()->animation_state();
	R_ASSERT_CMP				( driving_animation_state, <, &animation_state );

	n_ary_tree_node_animation_interval const* driving_animation_interval	= animation_intervals_begin( *animation_node.animation_to_synchronize_with() ) + driving_animation_state->animation_interval_id;
	n_ary_tree_node_animation_interval const* animation_interval			= animation_intervals_begin( animation_node ) + animation_state.animation_interval_id;
	animation_state.animation_interval_time	= animation_interval->length() / driving_animation_interval->length() * (*driving_animation_state).animation_interval_time;
}

void n_ary_tree::update_synchronization_group	(
		n_ary_tree_animation_node& animation_node,
		u32 const start_time_in_ms,
		u32 const target_time_in_ms
	)
{
	animation_state& animation_state					= animation_node.animation_state();
	R_ASSERT											( !animation_node.animation_to_synchronize_with() );
	int const synchronization_group_id					= animation_node.synchronization_group_id( );
	bool are_there_any_weight_transitions				= animation_state.event_iterator.are_there_any_weight_transitions();
	if ( !are_there_any_weight_transitions ) {
		if ( synchronization_group_id != -1 ) {
			for (
					n_ary_tree_animation_node* current_driven_animation = animation_node.m_next_animation ;
					current_driven_animation && (current_driven_animation->synchronization_group_id() == synchronization_group_id) ;
					current_driven_animation = current_driven_animation->m_next_animation
				)
			{
				if ( current_driven_animation->animation_state().event_iterator.are_there_any_weight_transitions() ) {
					are_there_any_weight_transitions		= true;
					break;
				}
			}
		}
	}

	if ( are_there_any_weight_transitions )
		update_synchronization_group_using_integration	( animation_node, start_time_in_ms, target_time_in_ms );
	else
		update_animation_state							( animation_node, start_time_in_ms, target_time_in_ms );

	if ( synchronization_group_id == -1 )
		return;

	for (
			n_ary_tree_animation_node* current_driven_animation = animation_node.m_next_animation ;
			current_driven_animation && (current_driven_animation->synchronization_group_id() == synchronization_group_id) ;
			current_driven_animation = current_driven_animation->m_next_animation
		)
	{
		if ( current_driven_animation->animation_state().event_iterator.are_there_any_weight_transitions() )
			continue;
		
		update_animation_state	( *current_driven_animation, start_time_in_ms, target_time_in_ms );
	}
}

bool n_ary_tree::update_animation_states( u32 const start_time_in_ms, u32 const target_time_in_ms )
{
	bool result					= false;
	for ( n_ary_tree_animation_node* current_animation = m_root; current_animation; current_animation = current_animation->m_next_animation ) {
		if ( !current_animation->animation_to_synchronize_with() )
			update_synchronization_group	( *current_animation, start_time_in_ms, target_time_in_ms );

		if ( current_animation->animation_state().event_iterator->event_time_in_ms == target_time_in_ms )
			result				= result || (( current_animation->animation_state().event_iterator->event_type & time_event_need_new_object_transform ) != 0);
	}
	
	return						result;
}

void n_ary_tree::dispatch_callbacks		( u32 const target_time_in_ms ) const
{
	for ( animation_state const* i = m_animation_states, * const e = m_animation_states + m_animations_count; i != e; ++i ) {
		if ( i->event_iterator->event_time_in_ms != target_time_in_ms )
			continue;

		if ( (i->event_iterator->event_type & time_event_channel_callback_should_be_fired) == 0 )
			continue;

		i->event_iterator.dispatch_callbacks	( );
	}
}

void n_ary_tree::remove_animation	(
		n_ary_tree_animation_node*& i,
		n_ary_tree_animation_node* j
	)
{
	if ( j ) {
		j->m_next_animation		= i->m_next_animation;
	}
	else {
		m_root					= i->m_next_animation;
		R_ASSERT				( m_root );
	}

	n_ary_tree_destroyer destroyer;
	i->accept					( destroyer );

	if ( j )
		i						= j->m_next_animation;
	else
		i						= m_root;

	--m_animations_count;
}

n_ary_tree::process_event_result_enum n_ary_tree::process_event	(
		n_ary_tree_animation_node*& current_animation_node,
		n_ary_tree_animation_node* const previous_animation_node,
		u32 const event_types
	)
{
	animation_state& animation_state	= current_animation_node->animation_state();
	u32 const event_type				= animation_state.event_iterator->event_type & event_types;
	if ( event_type & time_event_animation_ended ) {
#ifndef MASTER_GOLD
		n_ary_tree_weight_calculator	weight_calculator( animation_state.event_iterator->event_time_in_ms );
		weight_calculator.visit			( *current_animation_node );
		float const weight				= weight_calculator.weight( );
		R_ASSERT						( weight_calculator.null_weight_found() );
		R_ASSERT_CMP					( weight, ==, 0.f );
#endif // #ifndef MASTER_GOLD

		i_editor_mixer::call_user_callback (
			editor_animations_event::finish_animation,
			weight_calculator.weight_transition_ended_time_in_ms(),
			current_animation_node,
			0
		);

		mixing::animation_state** const new_end	= std::remove( m_animation_events, m_animation_events + m_animations_count, &animation_state );
		R_ASSERT_U						( new_end == (m_animation_events + m_animations_count - 1 ) );

		remove_animation				( current_animation_node, previous_animation_node );
		return							process_event_result_animation_removed;
	}

	if ( event_type & time_event_new_animation_started ) {
		set_object_transform			( *current_animation_node );
	}

	if ( event_type & time_event_animation_interval_ended ) {
		animation_state.animation_interval_id	= animation_state.event_iterator->animation_interval_id;
		animation_state.animation_interval_time	= animation_state.event_iterator->animation_interval_time;
		if ( !current_animation_node->animation_to_synchronize_with() )
			n_ary_tree_time_scale_start_time_modifier(
				*current_animation_node,
				animation_state.event_iterator->event_time_in_ms,
				animation_state.animation_interval_time
			);
	}

	if ( event_type & time_event_channel_callback_should_be_fired ) { }
	
	if ( event_type & time_event_time_direction_changed ) {
		if ( !current_animation_node->animation_to_synchronize_with() )
			n_ary_tree_time_scale_start_time_modifier(
				*current_animation_node,
				animation_state.event_iterator->event_time_in_ms,
				animation_state.animation_interval_time
			);
	}

	if ( event_type & time_event_weight_transitions_finished ) { }

	return								process_event_result_nothing_to_do;
}

void n_ary_tree::process_events			( u32 const target_time_in_ms, u32 const event_types )
{
	animation_state* current_animation_state		= m_animation_states;
	animation_state* current_alive_animation_state	= current_animation_state;
	n_ary_tree_animation_node* previous_animation	= 0;
	for ( n_ary_tree_animation_node* current_animation = m_root; current_animation; ) {
		R_ASSERT_CMP					( current_animation_state->event_iterator->event_time_in_ms, >=, target_time_in_ms );
		if ( current_animation_state->event_iterator->event_time_in_ms == target_time_in_ms ) {
			if ( current_animation_state->event_iterator->event_type & event_types ) {
				if ( process_event( current_animation, previous_animation, event_types ) == process_event_result_animation_removed ) {
					++current_animation_state;
					continue;
				}
			}
		}

		*current_alive_animation_state++ = *current_animation_state++;
		previous_animation				= current_animation;
		current_animation				= current_animation->m_next_animation;
	}

	for ( ; current_alive_animation_state != current_animation_state; ++current_alive_animation_state )
		current_alive_animation_state->~animation_state	( );
}

static pcstr get_event_id	( u32 const event )
{
	using namespace xray::animation::mixing;

	switch ( event ) {
		case time_event_new_animation_started				: return "new animation started";
		case time_event_animation_ended						: return "animation ended";
		case time_event_animation_interval_ended			: return "animation interval ended";
		case time_event_channel_callback_should_be_fired	: return "channel callback";
		case time_event_time_direction_changed				: return "time direction changed";
		case time_event_weight_transitions_finished			: return "weight transitions finished";
		default												: NODEFAULT( return "<unknown event id>" );
	}
}

static void log_event		( xray::animation::mixing::animation_event const& event, n_ary_tree_animation_node& animation )
{
	LOG_TRACE	( "\tevent_types:" );
	for (u32 event_type = event.event_type; event_type; event_type &= event_type - 1 )
		LOG_TRACE	( "\t\t%s", get_event_id(event_type ^ (event_type & (event_type - 1)) ) );
	LOG_TRACE	( "\tanimation               : %s", animation.animation().identifier() );
	LOG_TRACE	( "\ttime                    : %dms", event.event_time_in_ms );
	LOG_TRACE	( "\tanimation interval id   : %d", event.animation_interval_id );
	LOG_TRACE	( "\tanimation interval time : %fs", event.animation_interval_time );
}

void n_ary_tree::update_event_iterators	( u32 const target_time_in_ms )
{
	// for each event iterator, with event which has just happenned, increment
	// it and insert into a proper position to keep event iterators in a sorted order
	while ( (*m_animation_events)->event_iterator->event_time_in_ms == target_time_in_ms ) {
		LOG_TRACE						( "processed animation event:" );
		log_event						( *(*m_animation_events)->event_iterator, (*m_animation_events)->event_iterator.animation() );
		++((*m_animation_events)->event_iterator);
		animation_state** const found	= std::lower_bound( m_animation_events + 1, m_animation_events + m_animations_count, *m_animation_events, event_iterator_predicate() );
		animation_state* const backup	= *m_animation_events;
		std::copy						( m_animation_events + 1, found, m_animation_events );
		*(found - 1)					= backup;
	}

	LOG_TRACE							( "NEXT animation events:" );
	for ( animation_state const* const* i = m_animation_events, * const* const e = m_animation_events + m_animations_count; i != e; ++i ) {
		LOG_TRACE						( "animation event:" );
		log_event						( *(*i)->event_iterator, (*i)->event_iterator.animation() );
	}
}

void n_ary_tree::tick					( u32 const target_time_in_ms )
{
	if ( !m_root )
		return;

	R_ASSERT_CMP						( m_tree_actual_time_in_ms, <=, target_time_in_ms );
	u32 last_event_time_in_ms			= m_tree_actual_time_in_ms;
	m_tree_actual_time_in_ms			= target_time_in_ms;
	for ( ; (*m_animation_events)->event_iterator->event_time_in_ms <= target_time_in_ms;  ) {
		u32 const event_time_in_ms		= (*m_animation_events)->event_iterator->event_time_in_ms;
		R_ASSERT_CMP					( last_event_time_in_ms, <=, event_time_in_ms );

		bool const need_new_transform	= update_animation_states( last_event_time_in_ms, event_time_in_ms );
		if ( need_new_transform ) {
			process_events				( event_time_in_ms, time_event_new_animation_started );
			float4x4 const& new_object_transform	= get_object_transform( );
			process_events				( event_time_in_ms, time_event_all_events ^ time_event_new_animation_started );
			update_event_iterators		( event_time_in_ms );
			set_object_transform		( new_object_transform );
		}
		else {
			process_events				( event_time_in_ms, time_event_all_events );
			update_event_iterators		( event_time_in_ms );
		}

		dispatch_callbacks				( event_time_in_ms );
		last_event_time_in_ms			= event_time_in_ms;
	}

	if ( last_event_time_in_ms != target_time_in_ms ) {
		bool const need_new_transform	= update_animation_states( last_event_time_in_ms, target_time_in_ms );
		R_ASSERT_U						( !need_new_transform );
		dump_animation_states			( target_time_in_ms );
	}
}