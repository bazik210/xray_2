project(stalker2 LANGUAGES CXX)

add_executable(stalker2_pc_dx11 WIN32
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/../../finger_print.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/math_error_win.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_library_linkage.cpp"
						# application
						"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_application.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_application.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_application_win.cpp"
						# resources
						"${CMAKE_CURRENT_SOURCE_DIR}/icon.ico"
						"${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc.rc"
						# game_module_proxy
						"${CMAKE_CURRENT_SOURCE_DIR}/../../game_module_proxy.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../game_module_proxy.h"
						# My Amplifier Results
						# MISSING
						#"${CMAKE_CURRENT_SOURCE_DIR}/My Amplifier Results/r027hs/r027hs.ampl"
						#"${CMAKE_CURRENT_SOURCE_DIR}/My Amplifier Results/r028hs/r028hs.ampl"
						#"${CMAKE_CURRENT_SOURCE_DIR}/My Amplifier Results/r029hs/r029hs.ampl"
						#"${CMAKE_CURRENT_SOURCE_DIR}/My Amplifier Results/r030hs/r030hs.ampl"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_entry_point.cpp"
)

set_target_properties(stalker2_pc_dx11 PROPERTIES FOLDER "stalker2")

#add_dependencies(stalker2_pc_dx11 game collision engine engine_pc engine_pc_dx11 input particle physics sound ui)

add_dependencies(stalker2_pc_dx11
	game
	editor
	engine_pc_dx11
)

############################

add_executable(stalker2_pc_opengl WIN32
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/../../finger_print.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/math_error_win.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_library_linkage.cpp"
					# application
					"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_application.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_application.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_application_win.cpp"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/icon.ico"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc.rc"
					# game_module_proxy
					"${CMAKE_CURRENT_SOURCE_DIR}/../../game_module_proxy.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../game_module_proxy.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/stalker2_pc_entry_point.cpp"
)

set_target_properties(stalker2_pc_opengl PROPERTIES LINK_LIBRARIES "" FOLDER "stalker2")

#add_dependencies(stalker2_pc_opengl game collision engine engine_pc engine_pc_opengl input particle physics sound ui)

add_dependencies(stalker2_pc_opengl
	game
	engine_pc_dx11
	engine_pc_opengl
)
