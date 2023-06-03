project(sound LANGUAGES CXX)

add_library(sound STATIC
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_memory.h"
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../atomic_half3.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../channels_type.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../encoded_sound_interface.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../encoded_sound_with_qualities.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../functor_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../playback_mode.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../single_sound.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_debug_stats.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_emitter.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_instance_emitter.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_instance_proxy.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_object_commands.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_producer.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_propagator_emitter.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_receiver.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_rms.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../sound_spl.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../spl_utils.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../two_way_thread_channel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../wav_utils.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../world_user.h"
					# world
					"${CMAKE_CURRENT_SOURCE_DIR}/console_commands.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/console_commands.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_world.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_world_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_world_orders.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_world_parameters.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/world_user.cpp"
					# objects
					# collection
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_cook.h"
					# single
					"${CMAKE_CURRENT_SOURCE_DIR}/single_sound.cpp"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/single_sound_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/single_sound_cook.h"
					# composite
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_cook.h"
					# emitter
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_emitter.cpp"
					# receiver
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_receiver.cpp"
					# propagator
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_propagator.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_propagator.h"
					# sound_voice
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_voice.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_voice.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_voice_callbacks.cpp"
					# proxy
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_instance_proxy.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_instance_proxy_internal.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_instance_proxy_internal.h"
					# producer
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_producer.cpp"
					# ogg
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_encode_impl.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_utils.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_utils.h"
					# sound
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_sound.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_sound_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_sound_cook.h"
					# file_contents
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_file_contents.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_file_contents.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_file_contents_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_file_contents_cook.h"
					# rms
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_rms.cpp"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_rms_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_rms_cook.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_source_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_source_cook.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_stream_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_stream_cook.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/functor_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/functor_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/functor_command_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_instance_proxy_order.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_instance_proxy_order.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_commands.cpp"
					# voice
					"${CMAKE_CURRENT_SOURCE_DIR}/voice_bridge.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/voice_bridge.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/voice_callback_handler.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/voice_factory.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/voice_factory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/voice_format.h"
					# processing_objects
					"${CMAKE_CURRENT_SOURCE_DIR}/debug_snapshot.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/debug_snapshot.h"
					# buffer
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_buffer.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_buffer.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_buffer_factory.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_buffer_factory.h"
					# encoded_sound_interface
					"${CMAKE_CURRENT_SOURCE_DIR}/encoded_sound_interface.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/encoded_sound_with_qualities.cpp"
					# ogg_encoded_sound_interface
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_encoded_sound_interface.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_encoded_sound_interface.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_encoded_sound_interface_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ogg_encoded_sound_interface_cook.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/encoded_sound_with_qualities_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/encoded_sound_with_qualities_cook.h"
					# wav_encoded_sound_interface
					"${CMAKE_CURRENT_SOURCE_DIR}/wav_encoded_sound_interface.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/wav_encoded_sound_interface.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/wav_encoded_sound_interface_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/wav_encoded_sound_interface_cook.h"
					# wav
					"${CMAKE_CURRENT_SOURCE_DIR}/wav_utils.cpp"
					# spl
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_spl.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/spl_utils.cpp"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_spl_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_spl_cook.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/spl_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/spl_cook.h"
					# calculation
					"${CMAKE_CURRENT_SOURCE_DIR}/panning_lut.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/panning_lut.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/panning_lut_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/panning_lut_cook.h"
					# scene
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_cook.h"
					# debug
					"${CMAKE_CURRENT_SOURCE_DIR}/debug_statistic.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/debug_statistic.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_debug_stats.cpp"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_entry_point.cpp"
)

target_link_libraries(sound PUBLIC
	core
	ogg
)

set_target_properties(sound PROPERTIES FOLDER "engine")
