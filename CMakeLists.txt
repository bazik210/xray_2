cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# Leave uncommented to enable Parallel Compilation in projects that don't have settings yet
add_compile_options(/MP)

# Globally disable Boost exceptions
#add_compile_definitions(BOOST_EXCEPTION_DISABLE)

# Override Boost exception with non-template, user-defined exception boost::throw_exception(std::exception const&)
# This doesn't apply if BOOST_EXCEPTION_DISABLE is defined
add_compile_definitions(BOOST_NO_EXCEPTIONS)

# Force Scaleform debug mode
# add_compile_definitions(SF_BUILD_DEBUG)

#################################
# GLOBAL COMPILER CONFIGURATION #
#################################
# C++
enable_language(CXX)
# TODO test compilation under C++17 when this madness is over
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C#
enable_language(CSharp)
include(CSharpUtilities)
if(NOT CMAKE_CSharp_COMPILER)
    message(STATUS "CSharp compiler not found!")
    return()
endif()

# /ZI
SET(CMAKE_CXX_FLAGS "/EHs")

# X-Ray 2.0
SET(XRAY_ENABLE_PRECOMPILED_HEADERS "true")

############################
# Set paths to Directories #
############################

# DirectX SDK 
set(DXSDK_DIR_PATH "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")

####################################
# Set paths to Include Directories #
####################################
# Solution directory
set(SOLUTION_DIR_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# X-Ray directory (xray includes use <xray/...> notation)
set(XRAY_DIR_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Third party directory
set(THIRDPARTY_DIR_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/")

# Boost
set(BOOST_INCLUDE_PATH "${THIRDPARTY_DIR_INCLUDE_PATH}/boost/")
# FastDelegate
set(FASTDELEGATE_INCLUDE_PATH "${THIRDPARTY_DIR_INCLUDE_PATH}/fastdelegate/")
# Stlport
set(STLPORT_INCLUDE_PATH "${THIRDPARTY_DIR_INCLUDE_PATH}/stlport/")

#################################
# GLOBAL SOLUTION CONFIGURATION #
#################################
project(X-Ray-v2.0)

# Determine platform
math(EXPR PLATFORM_BITS "${CMAKE_SIZEOF_VOID_P} * 8")
MESSAGE(STATUS "Platform bits: ${PLATFORM_BITS}")
if("${PLATFORM_BITS}" STREQUAL "64")
    set(CMAKE_GENERATOR_PLATFORM "x64")
else()
    set(CMAKE_GENERATOR_PLATFORM "x86")
endif()
MESSAGE(STATUS "Generator: ${CMAKE_GENERATOR_PLATFORM}")

# Set output directories
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PLATFORM_SYSTEM "Win")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM_SYSTEM "Lin")
else()
    set(PLATFORM_SYSTEM ${CMAKE_SYSTEM_NAME})
endif()

set(PLATFORM_DIR ${PLATFORM_SYSTEM}${PLATFORM_BITS})
#set(PLATFORM_DIR ${CMAKE_SYSTEM_NAME}${PLATFORM_BITS}-${CMAKE_CXX_COMPILER_ID}${CMAKE_CXX_COMPILER_VERSION})

# Linux .a static libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries/$<CONFIG>/lib)
# Windows .lib static libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries/$<CONFIG>/lib)
# Shared .dll libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries/$<CONFIG>)
# .exe executables
set(EXECUTABLE_OUTPUT_PATH ${SOLUTION_DIR_INCLUDE_PATH}/binaries/$<CONFIG>)

# Disable Exception Handling and Runtime Checks using any means necessary
foreach(var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_INIT CMAKE_C_FLAGS CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_INIT)
    string(REGEX REPLACE "/EH[a-z]+" "" ${var} "${${var}}")
    string(REPLACE "/EHa" "" ${var} "${${var}}")
    string(REPLACE "/EHsc" "" ${var} "${${var}}")
    string(REGEX REPLACE "/RTC[a-z0-9]+" "" ${var} "${${var}}")
    string(REPLACE "/RTCc" "" ${var} "${${var}}")
    string(REPLACE "/RTCs" "" ${var} "${${var}}")
    string(REPLACE "/RTCu" "" ${var} "${${var}}")
    string(REPLACE "/RTC1" "" ${var} "${${var}}")
    string(REPLACE "/DNDEBUG" "/DDEBUG" ${var} "${${var}}")
endforeach() 


# Main engine directory
add_subdirectory(xray)

# 3rd party libraries
add_subdirectory(third_party)

# Enable folder hierarchy view
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#############
# UTILITIES #
#############
# Clear all Target flags related to Exception Handling
function(xr_disable_target_exception_handling TARGET)
    # "Wanna see me do it again?"
    get_target_property(TARGET_COMPILE_FLAGS ${TARGET} COMPILE_OPTIONS )
    string(REGEX REPLACE "/EH[a-z]+" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/EHa" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/EHsc" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REGEX REPLACE "/RTC[a-z0-9]+" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/RTCc" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/RTCs" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/RTCu" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/RTC1" "" ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    set_target_properties(${TARGET} PROPERTIES COMPILE_OPTIONS "${TARGET_COMPILE_FLAGS}")
endfunction()

# Remove NDEBUG and _DEBUG from preprocessor definitions
# Useful only when running compiler directly - Visual Studio WILL ADD THESE on its own
# Python script takes care of removing them from .vcxproj
function(xr_remove_target_debug_definition TARGET)
    get_target_property(TARGET_COMPILE_DEFINITIONS ${TARGET} COMPILE_DEFINITIONS )
    # MSVC
    string(REPLACE "/DNDEBUG" "" ${TARGET_COMPILE_DEFINITIONS} "${${TARGET_COMPILE_DEFINITIONS}}")
    string(REPLACE "/D_DEBUG" "" ${TARGET_COMPILE_DEFINITIONS} "${${TARGET_COMPILE_DEFINITIONS}}")
    # GCC/Clang
    string(REPLACE "-NDEBUG" "" ${TARGET_COMPILE_DEFINITIONS} "${${TARGET_COMPILE_DEFINITIONS}}")
    string(REPLACE "-_DEBUG" "" ${TARGET_COMPILE_DEFINITIONS} "${${TARGET_COMPILE_DEFINITIONS}}")
    # Sanity check
    string(REPLACE "NDEBUG" "" ${TARGET_COMPILE_DEFINITIONS} "${${TARGET_COMPILE_DEFINITIONS}}")
    string(REPLACE "_DEBUG" "" ${TARGET_COMPILE_DEFINITIONS} "${${TARGET_COMPILE_DEFINITIONS}}")
    set_target_properties(${TARGET} PROPERTIES COMPILE_DEFINITIONS "${TARGET_COMPILE_DEFINITIONS}")
endfunction()

# Precompiled header blacklist - these files will be skipped when setting up pch
SET(PCH_BLACKLIST
    "boost_exception.cpp"    
    "delayhlp.cpp"
    "malloc.c"
    "ptmalloc3.c"
    "finger_print.cpp"
    "game_module_proxy.cpp"
)

# Add precompiled header
function(xr_set_target_precompiled_header TARGET PCH_NAME)
    # Dieg: This might look like a good idea at first, but remember that it's GSC we are dealing with.
    # It's not a good idea.
    # Project properties
    #target_compile_options(${TARGET} PRIVATE
    #    "/Yu${PCH_NAME}.h"
    #)

    # Per file properties
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    foreach(SOURCE_FILE ${TARGET_SOURCES})
        # Match only .c and .cpp files (looking for .c substring matches both cases)
        STRING(FIND ${SOURCE_FILE} ".c" IS_SRC)

        # Check if file is blacklisted
        MESSAGE(STATUS "${PCH_BLACKLIST}")
        foreach(BLIST_NAME ${PCH_BLACKLIST})
            STRING(FIND ${SOURCE_FILE} ${BLIST_NAME} IS_BLACKLISTED)
            if(NOT (IS_BLACKLISTED EQUAL -1))
                # File is blacklisted, break to exit this loop
                MESSAGE(STATUS "Skipping adding pch to file: ${SOURCE_FILE}")
                break()
            endif()
        endforeach()

        # Set pch properties only if file is .c or .cpp AND isn't blacklisted
        if(NOT (IS_SRC EQUAL -1) AND (IS_BLACKLISTED EQUAL -1))
            # Set /Yu (files that should use pch) and /Yc (the .cpp of pch itself) flags
            STRING(FIND ${SOURCE_FILE} "${PCH_NAME}.cpp" IS_PCH)
            if(IS_PCH EQUAL -1)
                # By default set_source_files_properties() works only for targets defined in current CMakeLists file
                # since we are in global file, we have to use scope TARGET_DIRECTORY to overcome this limitation
                set_source_files_properties(${SOURCE_FILE} TARGET_DIRECTORY ${TARGET} PROPERTIES COMPILE_FLAGS "/Yu${PCH_NAME}.h")
            else()
                set_source_files_properties(${SOURCE_FILE} TARGET_DIRECTORY ${TARGET} PROPERTIES COMPILE_FLAGS "/Yc${PCH_NAME}.h")
            endif()
        endif()
    endforeach()

#########################################################################################
    # .h file is ezpz, Visual Studio will see it without absolute path...
    #target_compile_options(${TARGET} PRIVATE
    #    "/Yu${FILE_NAME}.h"
    #)
    
    # ... the .cpp file, however, oh boy...
    #SET(REGEX_PATTERN "[A-Z]:/([A-Za-z0-9_\\.]+/)+${FILE_NAME}.cpp")
    #string(REGEX MATCH ${REGEX_PATTERN} ABS_PATH "${TARGET_SOURCES}")
    #MESSAGE(STATUS "\nPrecompiled header of ${TARGET}: \n${ABS_PATH} | Pattern: ${REGEX_PATTERN}")

    #set_source_files_properties(SOURCE "${ABS_PATH}" PROPERTIES COMPILE_FLAGS "/Yc")
#########################################################################################
endfunction()

# List all Target Compile Options
function(xr_print_target_compile_definitions TARGET)
    get_target_property(TARGET_COMPILE_DEFINITIONS ${TARGET} COMPILE_DEFINITIONS )
    message(STATUS "\nPreprocessor definitions of ${TARGET}:\n${TARGET_COMPILE_DEFINITIONS}")
endfunction()

# List all Target Compile Options
function(xr_print_target_compile_flags TARGET)
    get_target_property(TARGET_COMPILE_FLAGS ${TARGET} COMPILE_OPTIONS )
    message(STATUS "\nProperties of ${TARGET}:\n${TARGET_COMPILE_FLAGS}")
endfunction()

# List all Target Link Options
function(xr_print_target_linker_flags TARGET)
    get_target_property(TARGET_LINKER_FLAGS ${TARGET} LINK_OPTIONS )
    message(STATUS "\nLinker Properties of ${TARGET}:\n${TARGET_LINKER_FLAGS}")
endfunction()



#########################
# CONFIGURATION PRESETS #
#########################
# Called at the bottom of this CMakeLists
function(xr_setup_static_libs)
    xr_set_target_defaults_static_lib_CPP(ai)
    xr_set_target_defaults_static_lib_CPP(ai_navigation)
    xr_set_target_defaults_static_lib_CPP(animation)
    xr_set_target_defaults_static_lib_CPP(rtp)
    xr_set_target_defaults_static_lib_CPP(core)
    xr_set_target_defaults_static_lib_CPP(debugging)
    xr_set_target_defaults_static_lib_CPP(fs)
    xr_set_target_defaults_static_lib_CPP(logging)
    xr_set_target_defaults_static_lib_CPP(vfs)
    xr_set_target_defaults_static_lib_CPP(collision)
    xr_set_target_defaults_static_lib_CPP(engine)
    xr_set_target_defaults_static_lib_CPP(engine_pc)
    xr_set_target_defaults_static_lib_CPP(input)
    xr_set_target_defaults_static_lib_CPP(particle)
    xr_set_target_defaults_static_lib_CPP(physics)
    xr_set_target_defaults_static_lib_CPP(sound)
    xr_set_target_defaults_static_lib_CPP(ui)
    xr_set_target_defaults_static_lib_CPP(opcode)
    xr_set_target_defaults_static_lib_C(lua.5.1.4)
    xr_set_target_defaults_static_lib_CPP(luabind.0.8.1)
    xr_set_target_defaults_static_lib_CPP(engine_pc_dx11)
    xr_set_target_defaults_static_lib_CPP(engine_pc_opengl)
    xr_set_target_defaults_static_lib_CPP(scaleform)
endfunction()

# Called at the bottom of this CMakeLists
# TODO add missing DLLs
function(xr_setup_shared_libs)
    #xr_set_target_defaults_shared_lib(engine_pc_dx11)
    #xr_set_target_defaults_shared_lib(engine_pc_opengl)
endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_editors)
    xr_set_target_defaults_editor(editor_controls)
    xr_set_target_defaults_editor(editor_base)

    xr_set_target_defaults_editor(animation_editor)
    xr_set_target_defaults_editor(dialog_editor)
    xr_set_target_defaults_editor(model_editor)
    xr_set_target_defaults_editor(particle_editor)
    xr_set_target_defaults_editor(sound_editor)

    xr_set_target_defaults_editor(editor)

    # This is a test C# project
    xr_set_target_defaults_editor(test_form)
endfunction()

# Called at the bottom of this CMakeLists
# TODO Check what they link against not to mix x86 app with x64 lib
function(xr_setup_network)
    # network is a x64 .lib
    xr_set_target_defaults_static_lib_CPP(network)

    # some network clients are x64 (64bit), some other are x86 (32bit)
    # x86
    xr_set_target_defaults_network_32(client)
    xr_set_target_defaults_network_32(lobby_server)
    xr_set_target_defaults_network_32(login_server)

    # x64
if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    xr_set_target_defaults_network_64(game_server)
else()
    xr_set_target_defaults_network_32(game_server)
endif()
endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_plugins)
    xr_set_target_defaults_plugin(maya2011)
    xr_set_target_defaults_plugin(maya_animation)
    xr_set_target_defaults_plugin(maya2012)
    xr_set_target_defaults_plugin(maya2018)
    xr_set_target_defaults_plugin(maya2019)
    xr_set_target_defaults_plugin(maya2023)

    # Set individual configs
    set_target_properties(maya2011 PROPERTIES
		OUTPUT_NAME "xray_maya2011"
        SUFFIX ".mll"
	)
    target_include_directories(maya2011 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2011
    )

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(maya2011 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2011/lib64
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
#       ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/x64/Msvc90/Debug
    )
else()
    target_link_directories(maya2011 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2011/lib64
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
#       ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/Win32/Msvc90/Debug
    )
endif()

    set_target_properties(maya_animation PROPERTIES
	    OUTPUT_NAME "xray_maya_animation"
	)

    set_target_properties(maya2012 PROPERTIES
	    OUTPUT_NAME "xray_maya2012"
        SUFFIX ".mll"
	)
    target_include_directories(maya2012 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2012
    )

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(maya2012 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2012/lib64
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
    )
else()
    target_link_directories(maya2012 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
	    ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2012/lib32
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
    )
endif()

    set_target_properties(maya2018 PROPERTIES
	    OUTPUT_NAME "xray_maya2018"
        SUFFIX ".mll"
	)
    target_include_directories(maya2018 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2018
    )

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(maya2018 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
    )
else()
    target_link_directories(maya2018 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
    )
endif()

    set_target_properties(maya2019 PROPERTIES
	    OUTPUT_NAME "xray_maya2019"
        SUFFIX ".mll"
	)
    target_include_directories(maya2019 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2019
    )

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(maya2019 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
    )
else()
    target_link_directories(maya2019 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
    )
endif()

    set_target_properties(maya2023 PROPERTIES
	    OUTPUT_NAME "xray_maya2023"
        SUFFIX ".mll"
	)
    target_include_directories(maya2023 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2023
    )

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(maya2023 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
    )
else()
    target_link_directories(maya2023 PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd_tootle-master/lib
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
    )
endif()

endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_renderers)
    xr_set_target_defaults_static_lib_CPP(renderer_engine_pc_dx11)
    xr_set_target_defaults_static_lib_CPP(renderer_core_pc_dx11)
    xr_set_target_defaults_static_lib_CPP(renderer_engine_pc_opengl)
    xr_set_target_defaults_static_lib_CPP(renderer_facade)
endfunction()

# Called at the bottom of this CMakeLists
# TODO add all SDK projects (probably done already?)
function(xr_setup_sdk)
    xr_set_target_defaults_shared_lib(texture_compressor)
    xr_set_target_defaults_shared_lib(bugtrap)
    xr_set_target_defaults_shared_lib_CLR(bugtrapN)
    xr_set_target_defaults_static_lib_C(minizip)
    xr_set_target_defaults_static_lib_C(minizipN)
    xr_set_target_defaults_static_lib_C(zlib)
    xr_set_target_defaults_static_lib_C(zlibN)
    xr_set_target_defaults_static_lib_CPP(core_cs)
    xr_set_target_defaults_static_lib_CPP(script_cs)
    xr_set_target_defaults_static_lib_CPP(FreeImage)
    xr_set_target_defaults_shared_lib(FreeImageDLL)
    xr_set_target_defaults_static_lib_C(LibJPEG)
    xr_set_target_defaults_static_lib_C(LibMNG)
    xr_set_target_defaults_static_lib_C(LibOpenJPEG)
    xr_set_target_defaults_static_lib_C(LibPNG)
    xr_set_target_defaults_static_lib_C(LibTIFF)
    xr_set_target_defaults_static_lib_CPP(OpenEXR)
    #xr_set_target_defaults_static_lib_CPP(nvcore)
    #xr_set_target_defaults_static_lib_CPP(nvimage)
    #xr_set_target_defaults_shared_lib(nvtt)
    #xr_set_target_defaults_static_lib_CPP(squish)
    xr_set_target_defaults_static_lib_CPP(LibFoundation)
    xr_set_target_defaults_static_lib_CPP(ode)
    xr_set_target_defaults_static_lib_C(ogg)
    xr_set_target_defaults_static_lib_C(theora)
    xr_set_target_defaults_static_lib_C(vorbis)
    xr_set_target_defaults_static_lib_C(vorbisfile)
    xr_set_target_defaults_static_lib_CPP(bullet)
endfunction()

# Called at the bottom of this CMakeLists
# TODO add all utilities (VERY low priority)
function(xr_setup_utilities)

endfunction()

# Called at the bottom of this CMakeLists
# TODO add all games after(!) all other configs are done
function(xr_setup_games)

    xr_set_target_defaults_static_lib_CPP(game)

    xr_set_target_defaults_stalker2(stalker2_pc_dx11)
    #target_compile_definitions(stalker2_pc_dx11 PRIVATE
    #	XRAY_STATIC_LIBRARIES=0
    #)

    target_include_directories(stalker2_pc_dx11 PRIVATE
    #	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Include
    #	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Src
        ${DXSDK_DIR_PATH}/Include
    )
    #target_link_libraries(stalker2_pc_dx11 PRIVATE
    #	game
    #	engine_pc_dx11
    #	x3daudio.lib
    #	legacy_stdio_definitions.lib
    #	xaudio2.lib
    #)
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        target_link_directories(stalker2_pc_dx11 PRIVATE
                ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
        #       ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/x64/Msvc90/Debug
            ${DXSDK_DIR_PATH}/Lib/x64
        )
    else()
        target_link_directories(stalker2_pc_dx11 PRIVATE
                ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
        #       ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/Win32/Msvc90/Debug
            ${DXSDK_DIR_PATH}/Lib/x86
        )
    endif()

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        xr_set_target_defaults_static_lib_CPP(stalker2_pc_opengl)
        target_compile_definitions(stalker2_pc_opengl PRIVATE
            PLATFORM_WIN64
            WIN64
        )
    else()
        xr_set_target_defaults_static_lib_CPP(stalker2_pc_opengl)
        target_compile_definitions(stalker2_pc_opengl PRIVATE
            PLATFORM_WIN32
            WIN32
        )
    endif()

    target_include_directories(stalker2_pc_opengl PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    #	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Include
    #	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Src
        ${DXSDK_DIR_PATH}/Include
    )
    #target_link_libraries(stalker2_pc_opengl PRIVATE
    #	engine_pc_opengl
    #	legacy_stdio_definitions.lib
    #)

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        target_link_directories(stalker2_pc_opengl PRIVATE
            ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
        #   ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/x64/Msvc90/Debug
            ${DXSDK_DIR_PATH}/Lib/x64
    )
    else()
        target_link_directories(stalker2_pc_opengl PRIVATE
            ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
        #   ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/Win32/Msvc90/Debug
            ${DXSDK_DIR_PATH}/Lib/x86
        )
    endif()

endfunction()

# Set target name for game .exe files
function(xr_set_game_name TARGET)
	if (MSVC)
		if (CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(PLATFORM_NAME "WIN64")
		else()
			set(PLATFORM_NAME "WIN32")
		endif()
	endif()

	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${TARGET}-dx11-${PLATFORM_NAME}"
	)
endfunction()

# Set target name for 32bit networking .exe files
function(xr_set_network_name_32 TARGET)
	set(PLATFORM_NAME "win32")

	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${TARGET}-${PLATFORM_NAME}"
	)
endfunction()

# Set target name for 64bit networking .exe files
function(xr_set_network_name_64 TARGET)
	set(PLATFORM_NAME "win64")

	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${TARGET}-${PLATFORM_NAME}"
	)
endfunction()

# Set target name for libs
function(xr_set_lib_name TARGET)
	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "xray_${TARGET}"
	)
endfunction()

###################
# TARGET DEFAULTS #
###################

# Setup definitions, compiler options and additional include directories
# TODO check if CLR has to be enabled in C++/CLI Properties tab in Visual Studio as well
#      see example in function xr_set_target_defaults_editor
# TODO check if .NET version has to be specified as well (related to TODO above)
function(xr_set_target_defaults_static_lib_CLR TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    endif()

    # set properties for CLR
    target_compile_definitions(${TARGET} PRIVATE _MANAGED)
    set_target_properties(${TARGET} PROPERTIES COMMON_LANGUAGE_RUNTIME "")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /Ox
        /Ob2
        /Oi
        /Ot
        /Oy
        /GT
        /GF
        /MD
        /GS-
        /fp:fast
        /Y-
        /MP
    )

    # Enable CLR again thanks to C# projects having separate property for that in VS2019 and higher :)
    set_target_properties(${TARGET} PROPERTIES VS_GLOBAL_CLRSupport "true")

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
	    ${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_static_lib_C TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    endif()

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /Ox
        /Ob2
        /Oi
        /Ot
        /Oy
        /GT
        /GF
        /MD
        /GS-
        /fp:fast
        /Y-
        /MP
    )

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_stalker2 TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
    #         XRAY_STATIC_LIBRARIES
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
    #         XRAY_STATIC_LIBRARIES
        )
    endif()

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    set_target_properties(${TARGET} PROPERTIES LINK_LIBRARIES "" LINK_FLAGS "/LARGEADDRESSAWARE /BASE:0x00010000 /DYNAMICBASE:NO /FIXED:NO /NODEFAULTLIB:libcmt")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        $<$<CONFIG:Release>:/GF /Oi /Ox /Ot /Oy /Ob2 /GT /GS- /fp:fast>
        $<$<CONFIG:RelWithDebInfo>:/GF /Oi /Ox /Ot /Oy /Ob2 /GT /GS- /fp:fast>
        $<$<CONFIG:Debug>:/GF- /Gy /RTC1 /fp:precise /fp:except->
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /MD
        /Y-
        /MP
    )

    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
    )
	
    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_static_lib_CPP TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
    #         XRAY_STATIC_LIBRARIES
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
    #         XRAY_STATIC_LIBRARIES
        )
    endif()
    # set compiler flags																				
    target_compile_options(${TARGET} PRIVATE
        $<$<CONFIG:Release>:/GF /Oi /Ox /Ob2 /Ot /Oy /GT /GS- /fp:fast>
        $<$<CONFIG:RelWithDebInfo>:/GF /Oi /Ox /Ot /Oy /Ob2 /GT /GS- /fp:fast>
        $<$<CONFIG:Debug>:/GF- /Gy /RTC1 /fp:precise /fp:except->
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /MD
        /Y-
        /MP
    )

    set_target_properties(${TARGET} PROPERTIES LINK_FLAGS "/LARGEADDRESSAWARE")

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_shared_lib TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    else()
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    endif()

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        $<$<CONFIG:Release>:/Ox /Ob2 /Oi /Ot /Oy /GS- /GF /fp:fast>
        $<$<CONFIG:RelWithDebInfo>:/GF /Oi /Ox /Ot /Oy /Ob2 /GT /GS- /fp:fast>
        $<$<CONFIG:Debug>:/GF- /GS /fp:precise /fp:except->
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /GF
        /MD
        /fp:fast
        /Y-
        /MP
    )

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
    # set Additional Library Directories
      target_link_directories(${TARGET} PRIVATE
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      )

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /DEBUG
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /FIXED:NO
        /MACHINE:X64
    )
else()
    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /DEBUG
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /FIXED:NO
        /MACHINE:X86
    )
endif()

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_shared_lib_CLR TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    endif()

    # set properties for CLR
    target_compile_definitions(${TARGET} PRIVATE _MANAGED)
    set_target_properties(${TARGET} PROPERTIES COMMON_LANGUAGE_RUNTIME "")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /Ox
        /Ob2
        /Oi
        /Ot
        /Oy
        /GT
        /GF
        /MD
        /GS-
        /fp:fast
        /Y-
        /MP
    )

    # Enable CLR again thanks to C# projects having separate property for that in VS2019 and higher :)
    set_target_properties(${TARGET} PROPERTIES VS_GLOBAL_CLRSupport "true")

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
	# set Additional Library Directories
	target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	)

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        target_link_options(${TARGET} PRIVATE
            /INCREMENTAL:NO
            /DEBUG
            /OPT:REF
            /OPT:ICF
            /DYNAMICBASE:NO
            /FIXED:NO
            /MACHINE:X64
        )
    else()
        target_link_options(${TARGET} PRIVATE
            /INCREMENTAL:NO
            /DEBUG
            /OPT:REF
            /OPT:ICF
            /DYNAMICBASE:NO
            /FIXED:NO
            /MACHINE:X86
        )
    endif()

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

# The only difference between Static and Shared editor libs are Linkage settings
# C++ projects use #pragma comment(lib, "libname.lib") directives within source code to add library linkage
function(xr_set_target_defaults_editor TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            _WIN32_WINNT=0x0501
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
            _WIN32_WINNT=0x0501
            PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    endif()

	# set properties for CLR
	target_compile_definitions(${TARGET} PRIVATE _MANAGED)
	set_target_properties(${TARGET} PROPERTIES COMMON_LANGUAGE_RUNTIME "")

   	set_target_properties(${TARGET} PROPERTIES LINK_LIBRARIES "")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        $<$<CONFIG:Release>:/Ox /Ob2 /Oi /Ot /Oy /GT /GS- /GF /Gy- /fp:fast>
        $<$<CONFIG:RelWithDebInfo>:/GF /Oi /Ox /Ot /Oy /Ob2 /GT /GS- /fp:fast>
        $<$<CONFIG:Debug>:/GF- /Gy /GS /fp:precise>
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /MD
        /Y-
        /MP
    )

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    # disable macro Warnings
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /MP
    )

    # Enable CLR again thanks to C# projects having separate property for that in VS2019 and higher :)
    set_target_properties(${TARGET} PROPERTIES VS_GLOBAL_CLRSupport "true")

    #xr_print_target_compile_flags(${TARGET})
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        target_link_options(${TARGET} PRIVATE
            /DEBUG
            /SUBSYSTEM:WINDOWS
            /DYNAMICBASE:NO
            /DELAY:UNLOAD
            /MACHINE:X64
            /CLRTHREADATTRIBUTE:STA
        )
    else()
        target_link_options(${TARGET} PRIVATE
            /DEBUG
            /SUBSYSTEM:WINDOWS
            /DYNAMICBASE:NO
            /DELAY:UNLOAD
            /MACHINE:X86
            /CLRTHREADATTRIBUTE:STA
        )
    endif()

    # Visual Studio 17 (2022) and above require a flag to indicate .DLLs for CLR
    if(NOT CMAKE_GENERATOR MATCHES "Visual Studio 1[0-6]")
    set_property(TARGET ${TARGET} PROPERTY VS_GLOBAL_ManagedAssembly "true")
    endif()

    target_compile_definitions(${TARGET} PUBLIC MBCS _MBCS)

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_network_32 TARGET)
    # set output file name
    xr_set_network_name_32(${TARGET})

    # set platform as x86
    target_compile_definitions(${TARGET} PRIVATE
        WIN32
        _CONSOLE
        _HAS_EXCEPTIONS=0
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /O2
        /Ob2
        /Oi
        /Oy
        /GT
        /GF
        /MT
        /GS-
        /arch:SSE2
        /fp:fast
        /MP
    )

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /DEBUG
        /SUBSYSTEM:CONSOLE
        /OPT:REF
        /OPT:ICF
        /MACHINE:X86
        /DELAYLOAD:libmysql.dll
        /DELAYLOAD:ws2_32.dll
        /DELAYLOAD:user32.dll
        /DELAYLOAD:gdi32.dll
        /DELAYLOAD:mswsock.dll
        /DELAYLOAD:advapi32.dll
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_network_64 TARGET)
    # set output file name
    xr_set_network_name_64(${TARGET})

    # set platform as x64
    target_compile_definitions(${TARGET} PRIVATE
      PLATFORM_WIN64
      _HAS_EXCEPTIONS=0
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Zi
        /Ob2
        /Oi
        /Ot
        /Oy
        /GT
        /GF
        /MD
        /GS-
        /fp:fast
        /MP
    )

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
        ${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /DEBUG
        /SUBSYSTEM:CONSOLE
        /LARGEADDRESSAWARE
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /FIXED:NO
        /DELAY:UNLOAD
		/MACHINE:X64
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()

function(xr_set_target_defaults_plugin TARGET)

    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        # set platform as x64
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN64
            WIN64
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    else()
        # set platform as x86
        target_compile_definitions(${TARGET} PRIVATE
            PLATFORM_WIN32
            WIN32
            WINDOWS
            _HAS_EXCEPTIONS=0
        )
    endif()

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
        /wd4005 /wd4131 /wd4275 /wd4458 /wd4459 /wd4945
        /W4
        /Ox
        /Ob2
        /Oi
        /Ot
        /Oy
        /GT
        /GL
        /GF
        /MD
        /GS-
        /fp:fast
        /MP
    )

    xr_disable_target_exception_handling(${TARGET})

    # set Additional Include Directories
    target_include_directories(${TARGET} PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )
	
    # set Additional Library Directories
    target_link_directories(${TARGET} PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )

    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /DELAY:UNLOAD
        /delayload:openmayaanim.dll 
        /delayload:xray_texture_compressor.dll
    )

    # List all target properties
    xr_print_target_compile_definitions(${TARGET})
    xr_print_target_compile_flags(${TARGET})
    xr_print_target_linker_flags(${TARGET})

endfunction()



#############################################
# CALL SETUP FUNCTIONS AND FINALIZE CONFIGS #
#############################################

# Project setup using presets
xr_setup_static_libs()
xr_setup_shared_libs()
xr_setup_network()
xr_setup_plugins()
xr_setup_renderers()
xr_setup_sdk()
xr_setup_utilities()
xr_setup_editors()
xr_setup_games()

# Precompiled headers (pch filename letter cases matter! StdAfx != stdafx)
if("${XRAY_ENABLE_PRECOMPILED_HEADERS}" STREQUAL "true")
    # X-Ray 2.0 projects
    xr_set_target_precompiled_header(ai pch)
    xr_set_target_precompiled_header(ai_navigation pch)
    xr_set_target_precompiled_header(animation pch)
    xr_set_target_precompiled_header(rtp pch)
    xr_set_target_precompiled_header(core pch)
    xr_set_target_precompiled_header(debugging pch)
    xr_set_target_precompiled_header(fs pch)
    xr_set_target_precompiled_header(vfs pch)
    xr_set_target_precompiled_header(logging pch)

    xr_set_target_precompiled_header(editor_controls pch)
    xr_set_target_precompiled_header(editor_base pch)
    xr_set_target_precompiled_header(animation_editor pch)
    xr_set_target_precompiled_header(dialog_editor pch)
    xr_set_target_precompiled_header(model_editor pch)
    xr_set_target_precompiled_header(particle_editor pch)
    xr_set_target_precompiled_header(sound_editor pch)
    xr_set_target_precompiled_header(editor pch)

    xr_set_target_precompiled_header(client pch)
    xr_set_target_precompiled_header(game_server pch)
    xr_set_target_precompiled_header(lobby_server pch)
    xr_set_target_precompiled_header(login_server pch)
    xr_set_target_precompiled_header(network pch)

    xr_set_target_precompiled_header(maya_animation pch)
    xr_set_target_precompiled_header(maya2011 pch)
    xr_set_target_precompiled_header(maya2012 pch)
    xr_set_target_precompiled_header(maya2018 pch)
    xr_set_target_precompiled_header(maya2019 pch)
    xr_set_target_precompiled_header(maya2023 pch)

    xr_set_target_precompiled_header(renderer_facade pch)
    xr_set_target_precompiled_header(renderer_core_pc_dx11 pch)
    xr_set_target_precompiled_header(renderer_engine_pc_dx11 pch)
    xr_set_target_precompiled_header(renderer_engine_pc_opengl pch)

    # Dieg: Dima's shit has 2 different pch files and I can't be bothered to sort this out
    #xr_set_target_precompiled_header(core_cs pch)
    #xr_set_target_precompiled_header(script_cs pch)

    xr_set_target_precompiled_header(fs_pack pch)
    xr_set_target_precompiled_header(graphics_benchmark pch)
    xr_set_target_precompiled_header(resources_performance_test pch)
    xr_set_target_precompiled_header(rms_generator pch)
    xr_set_target_precompiled_header(rtp_learn pch)
    xr_set_target_precompiled_header(shader_compiler pch)
    xr_set_target_precompiled_header(shader_generator pch)
    xr_set_target_precompiled_header(sound_system_usage_example pch)
    xr_set_target_precompiled_header(test pch)
    xr_set_target_precompiled_header(test_animations pch)
    xr_set_target_precompiled_header(test_form stdafx)
    xr_set_target_precompiled_header(texture_compressor_test pch)
    xr_set_target_precompiled_header(vfs_pack pch)

    xr_set_target_precompiled_header(collision pch)
    xr_set_target_precompiled_header(engine pch)
    xr_set_target_precompiled_header(input pch)
    xr_set_target_precompiled_header(particle pch)
    xr_set_target_precompiled_header(physics pch)
    xr_set_target_precompiled_header(sound pch)
    xr_set_target_precompiled_header(ui pch)

    xr_set_target_precompiled_header(game pch)
    xr_set_target_precompiled_header(stalker2_pc_dx11 pch)
    xr_set_target_precompiled_header(stalker2_pc_opengl pch)

    # Other projects (3rd party etc.)
    xr_set_target_precompiled_header(texture_compressor pch)
    xr_set_target_precompiled_header(bugtrap stdafx)
    xr_set_target_precompiled_header(bugtrapN stdafx)
    xr_set_target_precompiled_header(opcode StdAfx)
endif()

# Set startup project to Stk2 DX11
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT stalker2_pc_dx11)