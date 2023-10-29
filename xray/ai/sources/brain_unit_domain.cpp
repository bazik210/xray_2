////////////////////////////////////////////////////////////////////////////
//	Created		: 17.06.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "brain_unit.h"
#include "pddl_domain.h"
#include "pddl_problem.h"
#include "generalized_action.h"
#include <xray/ai/action_types.h>
#include "predicate_types.h"
#include "enemy_filter.h"
#include "weapon_filter.h"
#include "cover_filter.h"
#include "ai_world.h"
#include "action_parameter.h"
#include "goal_selector.h"
#include <xray/ai/npc.h>
#include "predicate.h"
#include "action_instance.h"

namespace xray {
namespace ai {

static void parse_enemy_filter(
		configs::binary_config_value const& filter_options,
		ai_world& world,
		planning::enemy_filter& filter
	)
{
	u32	const subfilter_id							= filter_options["subtype"];
	enemy_filter_types_enum const subfilter_type	= (enemy_filter_types_enum)subfilter_id;
	filter.set_filter_type							( subfilter_type );
	
	typedef boost::function< u32 ( pcstr ) >		find_by_name_function;
	find_by_name_function							selector;

	switch ( subfilter_type )
	{
		case enemy_filter_type_group:
			selector								= boost::bind( &ai_world::get_group_id_by_name, &world, _1 );
		break;

		case enemy_filter_type_character:
			selector								= boost::bind( &ai_world::get_character_id_by_name, &world, _1 );
		break;

		case enemy_filter_type_class:
			selector								= boost::bind( &ai_world::get_class_id_by_name, &world, _1 );
		break;

		case enemy_filter_type_outfit:
			selector								= boost::bind( &ai_world::get_outfit_id_by_name, &world, _1 );
		break;

		default:
			NODEFAULT								( );
	}
	
	configs::binary_config_value const& values		= filter_options["names"];
			
	configs::binary_config_value::const_iterator it		= values.begin();
	configs::binary_config_value::const_iterator it_end = values.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& value	= *it;
		pcstr const name							= (pcstr)value;
		u32 const id								= selector( name );
		if ( id != u32(-1) )
			filter.add_filtered_id					( id );
	}
}

static void parse_weapon_filter(
		configs::binary_config_value const& filter_options,
		ai_world& world,
		planning::weapon_filter& filter
	)
{
	u32	const subfilter_id							= filter_options["subtype"];
	weapon_filter_types_enum const subfilter_type	= (weapon_filter_types_enum)subfilter_id;
	filter.set_filter_type							( subfilter_type );
	
	typedef boost::function< u32 ( pcstr ) >		find_by_name_function;
	find_by_name_function							selector;
	
	switch ( subfilter_type )
	{
		case weapon_filter_type_melee:
			selector								= boost::bind( &ai_world::get_melee_weapon_id_by_name, &world, _1 );
		break;

		case weapon_filter_type_sniper:
			selector								= boost::bind( &ai_world::get_sniper_weapon_id_by_name, &world, _1 );
		break;

		case weapon_filter_type_heavy:
			selector								= boost::bind( &ai_world::get_heavy_weapon_id_by_name, &world, _1 );
		break;

		case weapon_filter_type_light:
			selector								= boost::bind( &ai_world::get_light_weapon_id_by_name, &world, _1 );
		break;

		case weapon_filter_type_energy:
			selector								= boost::bind( &ai_world::get_energy_weapon_id_by_name, &world, _1 );
		break;

		default:
			NODEFAULT								();
	}
	
	configs::binary_config_value const& values		= filter_options["weapons"];
			
	configs::binary_config_value::const_iterator it		= values.begin();
	configs::binary_config_value::const_iterator it_end = values.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& value	= *it;
		pcstr const name							= (pcstr)value;
		u32 const id								= selector( name );
		if ( id != u32(-1) )
			filter.add_filtered_id					( id );
	}
}

static void parse_cover_filter(
		configs::binary_config_value const& filter_options,
		ai_world& world,
		planning::cover_filter& filter
	)
{
	u32	const subfilter_id							= filter_options["subtype"];
	cover_filter_types_enum const subfilter_type	= (cover_filter_types_enum)subfilter_id;
	filter.set_filter_type							( subfilter_type );
	
	typedef boost::function< u32 ( pcstr ) >		find_by_name_function;
	find_by_name_function							selector;

	switch ( subfilter_type )
	{
		case cover_filter_type_node:
			selector								= boost::bind( &ai_world::get_node_id_by_name, &world, _1 );
		break;

		default:
			NODEFAULT								( );
	}
	
	configs::binary_config_value const& values		= filter_options["nodes"];
			
	configs::binary_config_value::const_iterator it		= values.begin();
	configs::binary_config_value::const_iterator it_end = values.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& value	= *it;
		pcstr const name							= (pcstr)value;
		u32 const id								= selector( name );
		if ( id != u32(-1) )
			filter.add_filtered_id					( id );
	}
}

static planning::base_filter* create_filter			(
		configs::binary_config_value const& filter_options,
		ai_world& world
	)
{
	u32	const filter_id								= filter_options["type"];
	filter_types_enum filter_type					= (filter_types_enum)filter_id;

	planning::base_filter* result					= 0;

	if ( filter_type == filter_type_enemy )
	{
		planning::enemy_filter* filter				= NEW( planning::enemy_filter );
		parse_enemy_filter							( filter_options, world, *filter );
		result										= filter;
	}
	else if ( filter_type == filter_type_weapon )
	{
		planning::weapon_filter* filter				= NEW( planning::weapon_filter );
		parse_weapon_filter							( filter_options, world, *filter );
		result										= filter;
	}
	else if ( filter_type == filter_type_cover )
	{
		planning::cover_filter* filter				= NEW( planning::cover_filter );
		parse_cover_filter							( filter_options, world, *filter );
		result										= filter;
	}

	R_ASSERT										( result );

	if ( filter_options.value_exists( "filters" ) )
	{
		configs::binary_config_value const& values	= filter_options["filters"];
			
		configs::binary_config_value::const_iterator it		= values.begin();
		configs::binary_config_value::const_iterator it_end = values.end();

		for ( ; it != it_end; ++it )
		{
			configs::binary_config_value const& value		= *it;
			planning::base_filter* subfilter				= create_filter( value, world );
			result->add_subfilter							( subfilter );
		}
	}
	
	return											result;
}

static void fill_action_parameter	(
		configs::binary_config_value const& options,
		planning::action_parameter& parameter,
		brain_unit& brain
	)
{
	using namespace					selectors;
	
	bool const iterate_first_only	= options.value_exists( "iterate_first_only" ) ? options["iterate_first_only"] : true;
	parameter.iterate_first			( iterate_first_only );

	pcstr selector_name				= options.value_exists( "selector" ) ? (pcstr)options["selector"] : "";
	target_selector_base* selector	= brain.get_selector_by_name( selector_name );
	parameter.set_selector			( selector );
}

static planning::generalized_action* create_action_prototype(
		planning::action_types_enum action_type,
		configs::binary_config_value const& action_options,
		planning::pddl_domain const& domain
	)
{
	using namespace					planning;
	
	float const action_cost			= action_options["cost"];
	pcstr const caption				= actions_captions[action_type];
	generalized_action* result		= NEW( generalized_action )( domain, action_type, caption, action_cost );
	
	if ( action_type == action_type_attack )
	{
		result->add_parameter_type	( parameter_type_npc );
		result->add_parameter_type	( parameter_type_weapon );

		result->set_preconditions	( predicate( predicate_type_weapon_loaded, _1 ) );
		result->set_effects			( predicate( predicate_type_target_dead, _0 ) );
	}
	else if ( action_type == action_type_attack_melee )
	{
		result->add_parameter_type	( parameter_type_npc );
		result->add_parameter_type	( parameter_type_weapon );

		result->set_preconditions	( predicate( predicate_type_target_in_melee_range, _0 ) );
		result->set_effects			( predicate( predicate_type_target_dead, _0 ) );
	}
	else if ( action_type == action_type_survey_area )
	{
		result->set_preconditions	( !predicate( predicate_type_patrolling ) );
		result->set_effects			( predicate( predicate_type_patrolling ) );
	}
	else if ( action_type == action_type_reload_weapon )
	{
		result->add_parameter_type	( parameter_type_weapon );

		result->set_preconditions	( !predicate( predicate_type_weapon_loaded, _0 ) );
		result->set_effects			( predicate( predicate_type_weapon_loaded, _0 ) );
	}
	else if ( action_type == action_type_uncover )
	{
		result->set_preconditions	( predicate( predicate_type_at_cover ) );
		result->set_effects			( !predicate( predicate_type_at_cover ) );
	}
	else if ( action_type == action_type_cloak )
	{
		result->set_preconditions	( !predicate( predicate_type_invisible ) );
		result->set_effects			( predicate( predicate_type_invisible ) );
	}
	else if ( action_type == action_type_take_cover )
	{
		result->add_parameter_type	( parameter_type_node );

		result->set_preconditions	( !predicate( predicate_type_at_cover ) );
		result->set_effects			( predicate( predicate_type_at_cover ) );
	}
	else if ( action_type == action_type_attack_from_cover )
	{
		result->add_parameter_type	( parameter_type_npc );
		result->add_parameter_type	( parameter_type_weapon );

		result->set_preconditions	( predicate( predicate_type_at_cover ) &&
									  predicate( predicate_type_weapon_loaded, _1 )
									);
		result->set_effects			( predicate( predicate_type_target_dead, _0 ) );
	}
	
	return							result; 
}

static planning::parameter_filters_type* create_parameter_filters	(
		configs::binary_config_value const& options,
		ai_world& world,
		pcstr parameter_number
	)
{
	using namespace											planning;
	parameter_filters_type* result							= 0;
	
	if ( options.value_exists( parameter_number ) )
	{
		result												= NEW( parameter_filters_type );
		configs::binary_config_value const& filters			= options[parameter_number];
		
		configs::binary_config_value::const_iterator it		= filters.begin();
		configs::binary_config_value::const_iterator it_end = filters.end();

		for ( ; it != it_end; ++it )
		{
			configs::binary_config_value const& value		= *it;
			intrusive_filters_list* new_list				= NEW( intrusive_filters_list )( create_filter( value, world ) );
			result->push_back								( new_list );
		}
	}
	return													result;
}

static planning::parameters_filters_type* create_parameters_filters(
		configs::binary_config_value const& parameters_options,
		ai_world& world
	)
{
	using namespace							planning;
	
	parameters_filters_type* filters		= NEW( parameters_filters_type );
	
	if ( parameter_filters_type* parameter_filters = create_parameter_filters( parameters_options, world, "parameter0_filter" ) )
	{
		parameter_filters_item_type* item	= NEW( parameter_filters_item_type )( parameter_filters );	
		filters->push_back					( item );
	}
	if ( parameter_filters_type* parameter_filters = create_parameter_filters( parameters_options, world, "parameter1_filter" ) )
	{
		parameter_filters_item_type* item	= NEW( parameter_filters_item_type )( parameter_filters );	
		filters->push_back					( item );
	}
	if ( parameter_filters_type* parameter_filters = create_parameter_filters( parameters_options, world, "parameter2_filter" ) )
	{
		parameter_filters_item_type* item	= NEW( parameter_filters_item_type )( parameter_filters );	
		filters->push_back					( item );
	}
	if ( parameter_filters_type* parameter_filters = create_parameter_filters( parameters_options, world, "parameter3_filter" ) )
	{
		parameter_filters_item_type* item	= NEW( parameter_filters_item_type )( parameter_filters );	
		filters->push_back					( item );
	}

	return									filters;
}

template < typename FiltersOwnerType >
static void fill_filter_sets		(
		configs::binary_config_value const& options,
		ai_world& world,
		FiltersOwnerType& owner
	)
{
	using namespace					planning;

	if ( options.value_exists( "filter_sets" ) )
	{
		configs::binary_config_value const& filters			= options["filter_sets"];
			
		configs::binary_config_value::const_iterator it		= filters.begin();
		configs::binary_config_value::const_iterator it_end = filters.end();

		for ( ; it != it_end; ++it )
		{
			configs::binary_config_value const& value		= *it;
			parameters_filters_type* parameters_filters		= create_parameters_filters( value, world );
			owner.add_filters_list							( parameters_filters );
		}
	}
}

template < typename T >
inline boost::function< void ( T ) > action			( boost::function< void ( T ) > const& result )
{
	return result;
}

template < typename T1, typename T2 >
inline boost::function< void ( T1, T2 ) > action	( boost::function< void ( T1, T2 ) > const& result )
{
	return result;
}

template < typename T1, typename T2, typename T3 >
inline boost::function< void ( T1, T2, T3 ) > action ( boost::function< void ( T1, T2, T3 ) > const& result )
{
	return result;
}

template < typename T1, typename T2, typename T3, typename T4 >
inline boost::function< void ( T1, T2, T3, T4 ) > action ( boost::function< void ( T1, T2, T3, T4 ) > const& result )
{
	return result;
}

static void fill_action_instances(
		configs::binary_config_value const& action_options,
		brain_unit& brain,
		ai_world& world,
		planning::pddl_domain const& domain,
		planning::pddl_problem& problem
	)
{
	using namespace											planning;
	using namespace											selectors;

	configs::binary_config_value const& actions_value		= action_options["actions"];

	configs::binary_config_value::const_iterator it			= actions_value.begin();
	configs::binary_config_value::const_iterator it_end		= actions_value.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& instance_value	= *it;
		u32	const action_type								= instance_value["id"];
		planning::action_types_enum const type				= (planning::action_types_enum)action_type;
		planning::generalized_action const* const prototype	= domain.get_action_by_type( type );
		R_ASSERT											( prototype, "generalized action wasn't found in domain" );

		action_instance* result								= NEW( action_instance )( prototype );

		if ( action_type == action_type_attack )
		{
			configs::binary_config_value const& param0		= instance_value["parameter0"];
			configs::binary_config_value const& param1		= instance_value["parameter1"];
			
			action_parameter* parameter0					= NEW( action_parameter )( parameter_type_npc );
			action_parameter* parameter1					= NEW( action_parameter )( parameter_type_weapon );

			fill_action_parameter							( param0, *parameter0, brain );
			fill_action_parameter							( param1, *parameter1, brain );

			result->add_parameter							( parameter0 );
			result->add_parameter							( parameter1 );

			problem.add_action_instance						(
				*result,
				action< npc const*, weapon const* >( boost::bind( &npc::prepare_to_attack, &brain.get_npc(), ::_1, ::_2 ) ),
				action< npc const*, weapon const* >( boost::bind( &npc::attack, &brain.get_npc(), ::_1, ::_2 ) ),
				action< npc const*, weapon const* >( boost::bind( &npc::stop_attack, &brain.get_npc(), ::_1, ::_2 ) )
			);
		}
		else if ( action_type == action_type_attack_melee )
		{
			configs::binary_config_value const& param0		= instance_value["parameter0"];
			configs::binary_config_value const& param1		= instance_value["parameter1"];
			
			action_parameter* parameter0					= NEW( action_parameter )( parameter_type_npc );
			action_parameter* parameter1					= NEW( action_parameter )( parameter_type_weapon );

			fill_action_parameter							( param0, *parameter0, brain );
			fill_action_parameter							( param1, *parameter1, brain );

			result->add_parameter							( parameter0 );
			result->add_parameter							( parameter1 );

			problem.add_action_instance						(
				*result,
				action< npc const*, weapon const* >( boost::bind( &npc::prepare_to_attack, &brain.get_npc(), ::_1, ::_2 ) ),
				action< npc const*, weapon const* >( boost::bind( &npc::attack_melee, &brain.get_npc(), ::_1, ::_2 ) ),
				action< npc const*, weapon const* >( boost::bind( &npc::stop_attack, &brain.get_npc(), ::_1, ::_2 ) )
			);
		}
		else if ( action_type == action_type_survey_area )
		{
			boost::function< void () > empty_function;
			problem.add_action_instance						(
				*result,
				empty_function,
				boost::bind( &npc::survey_area, &brain.get_npc() ),
				boost::bind( &npc::stop_patrolling, &brain.get_npc() )
			);
		}
		else if ( action_type == action_type_reload_weapon )
		{
			configs::binary_config_value const& param0		= instance_value["parameter0"];
			
			action_parameter* parameter0					= NEW( action_parameter )( parameter_type_weapon );
			fill_action_parameter							( param0, *parameter0, brain );
			
			result->add_parameter							( parameter0 );

			boost::function< void ( weapon const* ) >		empty_function;
			problem.add_action_instance						(
				*result,
				empty_function,
				action< weapon const* >( boost::bind( &npc::reload, &brain.get_npc(), ::_1 ) ),
				empty_function
			);
		}
		else if ( action_type == action_type_uncover )
		{
		}
		else if ( action_type == action_type_cloak )
		{
		}
		else if ( action_type == action_type_take_cover )
		{
			configs::binary_config_value const& param0		= instance_value["parameter0"];
			
			action_parameter* parameter0					= NEW( action_parameter )( parameter_type_node );
			fill_action_parameter							( param0, *parameter0, brain );
			
			result->add_parameter							( parameter0 );

			boost::function< void ( game_object const* ) >	empty_function;
			problem.add_action_instance						(
				*result,
				empty_function,
				empty_function,
				empty_function
			);
		}
		else if ( action_type == action_type_attack_from_cover )
		{
			configs::binary_config_value const& param0		= instance_value["parameter0"];
			configs::binary_config_value const& param1		= instance_value["parameter1"];
			
			action_parameter* parameter0					= NEW( action_parameter )( parameter_type_npc );
			action_parameter* parameter1					= NEW( action_parameter )( parameter_type_weapon );

			fill_action_parameter							( param0, *parameter0, brain );
			fill_action_parameter							( param1, *parameter1, brain );

			result->add_parameter							( parameter0 );
			result->add_parameter							( parameter1 );
			
			problem.add_action_instance						(
				*result,
				action< npc const*, weapon const* >( boost::bind( &npc::prepare_to_attack, &brain.get_npc(), ::_1, ::_2 ) ),
				action< npc const*, weapon const* >( boost::bind( &npc::attack_from_cover, &brain.get_npc(), ::_1, ::_2 ) ),
				action< npc const*, weapon const* >( boost::bind( &npc::stop_attack, &brain.get_npc(), ::_1, ::_2 ) )
			);
		}
		else
			LOG_ERROR										( "Unknown action type was declared - %d", action_type );

		fill_filter_sets									( instance_value, world, *result );
	}
}

static planning::goal* create_goal_by_type	(
		planning::goal_types_enum goal_type,
		configs::binary_config_value const& goal_options,
		planning::pddl_domain const& domain,
		brain_unit& brain,
		ai_world& world
	)
{
	using namespace				planning;
	using namespace				selectors;
	
	u32 const priority			= goal_options["priority"];
	pcstr const goal_caption	= goals_captions[goal_type];
	goal* result				= NEW( goal )( goal_type, priority, goal_caption );

	if ( goal_type == goal_type_kill_enemy )
	{
		configs::binary_config_value const& param0_value = goal_options["parameter0"];
		configs::binary_config_value const& param1_value = goal_options["parameter1"];

		action_parameter* param0 = NEW( action_parameter )( parameter_type_npc );
		action_parameter* param1 = NEW( action_parameter )( parameter_type_weapon );

		fill_action_parameter	( param0_value, *param0, brain );
		fill_action_parameter	( param1_value, *param1, brain );

		result->add_parameter	( param0 );
		result->add_parameter	( param1 );
	}
	else if ( goal_type == goal_type_blind_fire )
	{
		configs::binary_config_value const& param0_value = goal_options["parameter0"];
		
		action_parameter* param0 = NEW( action_parameter )( parameter_type_weapon );

		fill_action_parameter	( param0_value, *param0, brain );

		result->add_parameter	( param0 );
	}

	fill_filter_sets			( goal_options, world, *result );
	
	R_ASSERT ( goal_options.value_exists( "target_world_state_properties" ), "target_world_state_properties section is missing" );

	configs::binary_config_value const& properties		= goal_options["target_world_state_properties"];
			
	configs::binary_config_value::const_iterator it		= properties.begin();
	configs::binary_config_value::const_iterator it_end = properties.end();
	
	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& property	= *it;
	
		u32	const property_id							= property["property_id"];
		predicate_types_enum const type					= (predicate_types_enum)property_id;
		bool const property_value						= property["value"];

		pddl_world_state_property_impl::indices_type	parameters_indices;

		if ( property.value_exists( "parameters_indices" ) )
		{
			configs::binary_config_value const& indices	= property["parameters_indices"];

			configs::binary_config_value::const_iterator it_indices		= indices.begin();
			configs::binary_config_value::const_iterator it_end_indices	= indices.end();
	
			for ( ; it_indices != it_end_indices; ++it_indices )
				parameters_indices.push_back			( *it_indices );
		}
		
		result->add_target_property						( domain[type], property_value, parameters_indices );
	}

// 	if ( goal_options.value_exists( "filter_sets" ) )
// 	{
// 		configs::binary_config_value const& filters			= goal_options["filter_sets"];
// 			
// 		configs::binary_config_value::const_iterator it		= filters.begin();
// 		configs::binary_config_value::const_iterator it_end = filters.end();
// 
// 		for ( ; it != it_end; ++it )
// 		{
// 			configs::binary_config_value const& value		= *it;
// 			filters_list* filter_list						= create_filters_list( value, world );
// 			result->add_filters_list						( filter_list );
// 		}
// 	}
	
	return													result;
}

template < typename T >
inline boost::function< bool ( T ) > predicate			( boost::function< bool ( T ) > const& result )
{
	return result;
}

template < typename T1, typename T2 >
inline boost::function< bool ( T1, T2 ) > predicate		( boost::function< bool ( T1, T2 ) > const& result )
{
	return result;
}

template < typename T1, typename T2, typename T3 >
inline boost::function< bool ( T1, T2, T3 ) > predicate ( boost::function< bool ( T1, T2, T3 ) > const& result )
{
	return result;
}

template < typename T1, typename T2, typename T3, typename T4 >
inline boost::function< bool ( T1, T2, T3, T4 ) > predicate ( boost::function< bool ( T1, T2, T3, T4 ) > const& result )
{
	return result;
}

void brain_unit::create_domain	( configs::binary_config_value const& options )
{
	m_domain					= NEW( planning::pddl_domain );

	m_domain->register_parameter_type< npc const* >( planning::parameter_type_npc );
	m_domain->register_parameter_type< weapon const* >( planning::parameter_type_weapon );
	m_domain->register_parameter_type< game_object const* >( planning::parameter_type_node );

	m_domain->add_predicate		(
		predicate_type_patrolling,
		predicates_captions[predicate_type_patrolling],
		boost::bind( &npc::is_patrolling, &get_npc() )
	);
	m_domain->add_predicate		(
		predicate_type_at_cover,
		predicates_captions[predicate_type_at_cover],
		boost::bind( &npc::is_at_cover, &get_npc() )
	);
	m_domain->add_predicate		(
		predicate_type_no_danger,
		predicates_captions[predicate_type_no_danger],
		boost::bind( &npc::is_safe, &get_npc() )
	);
	m_domain->add_predicate		(
		predicate_type_invisible,
		predicates_captions[predicate_type_invisible],
		boost::bind( &npc::is_invisible, &get_npc() )
	);
	m_domain->add_predicate		(
		predicate_type_target_dead,
		predicates_captions[predicate_type_target_dead],
		predicate< npc const* >( boost::bind( &ai_world::is_target_dead, &m_world, _1 ) )
	);
 	m_domain->add_predicate		(
 		predicate_type_target_in_melee_range,
 		predicates_captions[predicate_type_target_in_melee_range],
 		predicate< npc const* >( boost::bind( &npc::is_target_in_melee_range, &get_npc(), _1 ) )
 	);
	m_domain->add_predicate		(
		predicate_type_weapon_loaded,
		predicates_captions[predicate_type_weapon_loaded],
		predicate< weapon const* >( boost::bind( &ai_world::is_weapon_loaded, &m_world, _1 ) )
	);
	m_domain->add_predicate		(
		predicate_type_at_node,
		predicates_captions[predicate_type_at_node],
		predicate< game_object const* >( boost::bind( &npc::is_at_node, &get_npc(), _1 ) )
	);

	configs::binary_config_value const& actions_value			= options["action_types"];

	configs::binary_config_value::const_iterator it				= actions_value.begin();
	configs::binary_config_value::const_iterator it_end			= actions_value.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& action_type_value	= *it;
		u32	const action_type									= action_type_value["id"];
		planning::action_types_enum const type					= (planning::action_types_enum)action_type;
		if ( planning::generalized_action* action				=
				create_action_prototype							(
					type,
					action_type_value,
					*m_domain
				)
			)
		{
			m_domain->add_action								( action );
		}
		else
			LOG_ERROR( "Unknown action type was declared - %d", action_type );
	}
}

void brain_unit::create_problem	( configs::binary_config_value const& options )
{
	m_problem					= NEW( planning::pddl_problem )( *m_domain );
	fill_action_instances		( options, *this, m_world, *m_domain, *m_problem );	
}

void brain_unit::create_goal_selector( configs::binary_config_value const& options )
{
	using namespace												planning;

	R_ASSERT													( m_domain );
	m_goal_selector												= NEW( goal_selector )( *m_domain, *m_problem, m_blackboard );
	
	configs::binary_config_value const& goals_value				= options["goals"];

	configs::binary_config_value::const_iterator it				= goals_value.begin();
	configs::binary_config_value::const_iterator it_end			= goals_value.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& goal_type_value		= *it;
		u32	const goal_type										= goal_type_value["type"];
		goal_types_enum const type								= (goal_types_enum)goal_type;
		if ( planning::goal* new_goal							=
				create_goal_by_type								(
					type,
					goal_type_value,
					*m_domain,
					*this,
					m_world
				)
			)
		{
			m_goal_selector->add_goal							( new_goal );
		}
		else
			LOG_ERROR( "Unknown goal type was declared - %d", goal_type );
	}

	m_goal_selector->sort_goals									( );
}

void brain_unit::delete_filter	( planning::base_filter* filter_to_be_deleted )
{
	while ( planning::base_filter* subfilter_to_be_deleted = filter_to_be_deleted->pop_subfilter() )
		delete_filter			( subfilter_to_be_deleted );

	DELETE						( filter_to_be_deleted );
}

} // namespace ai
} // namespace xray