project(maya_animation LANGUAGES CXX)

add_library(maya_animation STATIC
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_library_linkage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_memory.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../data_header.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../discrete_data.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../raw_bi_spline.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
							# world
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_world.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_world.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/raw_bi_spline.cpp"
							# sisl
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/construct.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/destruct.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/makecvkreg.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1219.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1220.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1221.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1227.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1350.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1351.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1353.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1700.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1701.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1706.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1710.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1712.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1940.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1941.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1942.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1943.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1944.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1945.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1946.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1947.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1948.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1949.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1950.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1951.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s1961.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6chpar.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6dist.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6err.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6existbox.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6knotmult.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6length.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6lufacp.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6lusolp.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6newbox.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/s6ratder.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh1922.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh1927.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh1929.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh6getlist.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh6getothr.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh6getprev.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sh6ismain.cpp"
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sisl.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sisl/sislP.h"
							# track
							"${CMAKE_CURRENT_SOURCE_DIR}/test_track.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/test_track.h"
							# test_animation
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_discrete.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_discrete.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_hermite.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_hermite.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_data.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_data.h"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_discrete_channels.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_discrete_channels.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_discrete_channels_inline.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/animation_data.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/animation_data.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/animation_data_inline.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/discrete_data.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/maya_animation_entry_point.cpp"
)

target_include_directories(maya_animation PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/maya_animation/sources
)

# Precompiled headers have to stay enabled for this one
if(NOT ("${XRAY_ENABLE_PRECOMPILED_HEADERS}" STREQUAL "true"))
	target_compile_options(maya_animation PRIVATE
		"/Yupch.h"
	)
	set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp PROPERTY VS_SETTINGS "PrecompiledHeader=Create")
endif()

set_target_properties(maya_animation PROPERTIES FOLDER "engine/plugins")
