////////////////////////////////////////////////////////////////////////////
//	Created		: 01.02.2011
//	Author		: Dmitriy Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_sound.h"
#include <xray/collision/collision_object.h>
#include <xray/editor/base/collision_object_types.h>
#include <xray/sound/sound_instance_proxy.h>
#include <xray/sound/world.h>
#include "game_world.h"
#include "game.h"
//#include "xray/math_float4x4_inline.h

namespace stalker2 {

object_sound::object_sound( game_scene& w )	:
	super			( w ),
	m_game_scene	( w ),
	m_loaded		( false ),
	m_positional	( true ),
	m_sound_type		( 0 ),
	m_emitter_type		( 0 ),
	m_world_user(w.get_game().get_sound_world().get_logic_world_user()),
	m_sound_scene		( 0 ),
	m_position			(0, 0, 0),
	m_direction			(0, 0, 1),
	m_up				(0, 1, 0)
{
	m_wav_file_name 	= "";

//	initialize_collision();
}

object_sound::~object_sound()
{
	
	//DELETE ( m_proxy );
	//DELETE ( m_sound_emitter_ptr );

	//m_collision->destroy( &debug::g_mt_allocator );
}

void object_sound::unload_contents()
{
	m_sound_emitter_ptr = 0;
	m_proxy = 0;
	//if (m_collision_geometry)
	//{
	//	m_collision_geometry->destroy(g_allocator);
	//	XRAY_DELETE_IMPL(g_allocator, m_collision_geometry);
	//}
}

void object_sound::load(configs::binary_config_value const& config_value)
{
	super::load(config_value);

	pcstr collision_geometry_name = config_value["collision_geometry"];
	m_position					  = (float3)(config_value["position"]);
	m_wav_file_name				  = config_value["sound"];

//	m_game_scene.get_game().get_game_world().query_object_by_name(
//		collision_geometry_name,
//		object_loaded_callback_type(this, &object_volumetric_sound::on_collision_object_loaded)
//	);

	load_sound();
}

void object_sound::set_transform(float4x4 const& transform)
{
	//m_transform = create_scale(scale) * create_rotation(rotation) * create_translation(position);
	m_position						= transform.c.xyz();
	m_direction						= transform.i.xyz();
	if ((m_proxy).c_ptr())
	{
		if ( m_sound_type == 0 )
			(m_proxy)->set_position				(m_position);
		else
			(m_proxy)->set_position_and_direction	(m_position, m_direction);
	}
}

void object_sound::load_contents( )
{
//	fs::path_string						wav_path;
//	wav_path.assignf("%s%s%s", "resources/sounds/single/", m_sound_name.c_str(), ".wav");
//	resources::query_resource(
//		wav_path.c_str(),
//		resources::binary_config_class,
//		boost::bind(&object_sound::on_config_loaded, this, _1),
//		g_allocator
//	);
}

void object_sound::initialize_collision( )
{
	float3 extents				(0.3f,0.3f,0.3f);

//	collision::geometry_instance* geom	= &*collision::new_box_geometry_instance( &debug::g_mt_allocator, math::create_scale(extents) );
//	m_collision->create_from_geometry( true, this, geom, editor_base::collision_object_type_dynamic | editor_base::collision_object_type_touch );
//	m_collision->insert			( m_transform );
}

void object_sound::load_sound					( )
{
	if ( m_wav_file_name == "" )
		return;

	fs::path_string wav_file_name				( m_wav_file_name );

	resources::class_id_enum class_id			= resources::unknown_data_class;
	switch ( m_emitter_type )
	{
		case 0: class_id						= resources::single_sound_class; break;
		case 1: class_id						= resources::composite_sound_class; break;
		case 2: class_id						= resources::sound_collection_class; break;
		default:								NODEFAULT();
	}
	resources::request request =
	{
		wav_file_name.c_str(),
		class_id
	};

	math::float4x4								matrix;
	matrix.identity								( );
	math::float4x4 const* matrix_pointers[] =
	{
		&matrix
	};
	
	resources::query_resource_params params		(
		&request,
		NULL,
		1, 
		boost::bind(&object_sound::on_sound_loaded, this, _1),
		g_allocator, 
		NULL,
		matrix_pointers																								
	);

	query_resources								( params );
}

void object_sound::on_sound_loaded( resources::queries_result& data )
{
	ASSERT						(data.is_successful());
	m_sound_emitter_ptr		= static_cast_resource_ptr<sound::sound_emitter_ptr>(data[0].get_unmanaged_resource());
	emit();
}

double object_sound::max_seek_val	( )
{
	return 0.0f;
}

void object_sound::emit	( )
{
	m_sound_scene = m_game_scene.get_sound_scene();

	if ( (m_sound_emitter_ptr).c_ptr() )
	{
		if ( m_sound_type == 0 )
		{
			m_proxy = ((m_sound_emitter_ptr)->emit_point_sound
			(
				m_sound_scene,
				m_world_user
			));
		}
		else
		{
			m_proxy = ((m_sound_emitter_ptr)->emit_spot_sound
			(
				m_sound_scene,
				m_world_user,
				sound::human
			));
		}
	}

	if ((m_proxy).c_ptr() == 0)
		return;

	if (m_sound_type == 0)
		(m_proxy)->set_position(m_position);
	else
		(m_proxy)->set_position_and_direction(m_position, m_direction);

	//(m_proxy)->play(sound::once);
	(m_proxy)->play(sound::looped);
}

} // namespace xray