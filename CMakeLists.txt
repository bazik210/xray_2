cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# Leave uncommented to enable Parallel Compilation in projects that don't have settings yet
add_compile_options(/MP)

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

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries)

# Disable Exception Handling and Runtime Checks using any means necessary
foreach(var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_INIT CMAKE_C_FLAGS CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_INIT)
        string(REGEX REPLACE "/EH[a-z]+" "" ${var} "${${var}}")
        string(REPLACE "/EHa" "" ${var} "${${var}}")
        string(REPLACE "/EHsc" "" ${var} "${${var}}")
		string(REGEX REPLACE "/RTC[a-z0-9]+" "" ${var} "${${var}}")
		string(REPLACE "/RTCc" "" ${var} "${${var}}")
		string(REPLACE "/RTCs" "" ${var} "${${var}}")
		string(REPLACE "/RTCu" "" ${var} "${${var}}")
		string(REPLACE "/RTC1" "" ${var} "${${var}}")
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
	xr_set_target_defaults_static_lib_CPP(engine_pc_dx11)
	xr_set_target_defaults_static_lib_CPP(engine_pc_opengl)
    xr_set_target_defaults_static_lib_CPP(input)
    xr_set_target_defaults_static_lib_CPP(particle)
    xr_set_target_defaults_static_lib_CPP(physics)
    xr_set_target_defaults_static_lib_CPP(sound)
    xr_set_target_defaults_static_lib_CPP(ui)
	xr_set_target_defaults_static_lib_CPP(opcode)
endfunction()

# Called at the bottom of this CMakeLists
# TODO add DLLs
function(xr_setup_shared_libs)
    
endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_editors)
    xr_set_target_defaults_editor(animation_editor)
    xr_set_target_defaults_editor(dialog_editor)
    xr_set_target_defaults_editor(editor)
    xr_set_target_defaults_editor(editor_base)
    xr_set_target_defaults_editor(editor_controls)
    xr_set_target_defaults_editor(model_editor)
    xr_set_target_defaults_editor(particle_editor)
    xr_set_target_defaults_editor(sound_editor)

    # This is a test C# project
    xr_set_target_defaults_editor(test_form)

    # Set dependencies of wpf_controls
    target_link_libraries(wpf_controls PUBLIC WPFToolkit.Extended)
    target_link_libraries(wpf_controls PUBLIC WeifenLuo.WinFormsUI.Docking)

    # Set directories for #using directive
    target_compile_options(wpf_controls PUBLIC
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
	
    set_target_properties(editor_controls PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(model_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(particle_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(sound_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(WeifenLuo.WinFormsUI.Docking PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v2.0")
    set_target_properties(WPFToolkit.Extended PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(wpf_controls PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")

    # Set Default Namespace
    set_property(TARGET WeifenLuo.WinFormsUI.Docking PROPERTY VS_GLOBAL_RootNamespace "WeifenLuo.WinFormsUI")
    set_property(TARGET WPFToolkit.Extended PROPERTY VS_GLOBAL_RootNamespace "Microsoft.Windows.Controls")
    set_property(TARGET wpf_controls PROPERTY VS_GLOBAL_RootNamespace "xray.editor.wpf_controls")

    # Specify DOTNET assembly references

    set_target_properties(editor_controls PROPERTIES VS_DOTNET_REFERENCES
        "PresentationCore;PresentationFramework;System;System.Data;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended"
    )

    set_target_properties(model_editor PROPERTIES VS_DOTNET_REFERENCES
        "Microsoft.Office.Interop.Excel;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11"
    )

    set_target_properties(particle_editor PROPERTIES VS_DOTNET_REFERENCES
        "Microsoft.Office.Interop.Excel;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11"
    )

    set_target_properties(sound_editor PROPERTIES VS_DOTNET_REFERENCES
        "Microsoft.Office.Interop.Excel;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11"
    )

    set_target_properties(test_form PROPERTIES VS_DOTNET_REFERENCES
        "System;System.Data;System.Drawing;System.Windows.Forms;System.Xml"
    )

    set_target_properties(WeifenLuo.WinFormsUI.Docking PROPERTIES VS_DOTNET_REFERENCES
        "System;System.Data;System.Design;System.Drawing;System.Windows.Forms;System.Xml"
    )

    set_target_properties(WPFToolkit.Extended PROPERTIES VS_DOTNET_REFERENCES
        "System;System.Data;System.Xml;System.Core;System.Xml.Linq;System.Data.DataSetExtensions;UIAutomationProvider;WindowsBase;PresentationCore;PresentationFramework;"
    )

    set_target_properties(wpf_controls PROPERTIES VS_DOTNET_REFERENCES
        "PresentationFramework.Aero;System;System.Core;System.Drawing;System.Windows.Forms;System.Xml.Linq;System.Data.DataSetExtensions;System.Data;System.Xml;UIAutomationProvider;UIAutomationTypes;WindowsBase;PresentationCore;PresentationFramework;WindowsFormsIntegration"
    )

endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_network)
    # network is a x64 .lib
    xr_set_target_defaults_static_lib_CPP(network)

    # some network clients are x64 (64bit), some other are x86 (32bit)
    # x86
    xr_set_target_defaults_network_32(client)
    xr_set_target_defaults_network_32(lobby_server)
    xr_set_target_defaults_network_32(login_server)

    # x64
    xr_set_target_defaults_network_64(game_server)
endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_plugins)
    xr_set_target_defaults_plugin(maya2011)
    xr_set_target_defaults_plugin(maya_animation)
    xr_set_target_defaults_plugin(maya2012)

    # Set individual configs
    set_target_properties(maya2011 PROPERTIES
		OUTPUT_NAME "xray.maya2011"
        SUFFIX ".mll"
	)
    target_include_directories(maya2011 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2011
    )
    target_link_directories(maya2011 PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2011/lib64
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd/tootle/libraries
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
        ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/x64/Msvc90/Release
    )

    set_target_properties(maya_animation PROPERTIES
		OUTPUT_NAME "xray_maya_animation"
	)

    set_target_properties(maya2012 PROPERTIES
		OUTPUT_NAME "xray.maya2012"
        SUFFIX ".mll"
	)
    target_include_directories(maya2012 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2012
    )
    target_link_directories(maya2012 PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
        ${THIRDPARTY_DIR_INCLUDE_PATH}/maya2012/lib64
        ${THIRDPARTY_DIR_INCLUDE_PATH}/amd/tootle/libraries
        ${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
    )
endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_renderers)
    xr_set_target_defaults_static_lib_CPP(renderer_engine_pc_dx11)
    xr_set_target_defaults_static_lib_CPP(renderer_core_pc_dx11)
    xr_set_target_defaults_static_lib_CPP(renderer_engine_pc_opengl)
    xr_set_target_defaults_static_lib_CPP(renderer_facade)

    target_include_directories(renderer_engine_pc_dx11 PRIVATE
        ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Include
        ${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Src
    )

endfunction()

# Called at the bottom of this CMakeLists
# TODO add all SDK projects (non-Editor)
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
    xr_set_target_defaults_shared_lib(FreeImage)
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
endfunction()

# Called at the bottom of this CMakeLists
# TODO add all utilities
function(xr_setup_utilities)

endfunction()

# Called at the bottom of this CMakeLists
# TODO add all games after(!) all other configs are done
function(xr_setup_games)

endfunction()

# Set target name for game .exe files
function(xr_set_game_name TARGET)
	if (MSVC)
		if (CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(PLATFORM_NAME "win64")
		else()
			set(PLATFORM_NAME "win64")
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

    # set platform as x64
	target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
    )

	# set properties for CLR
	target_compile_definitions(${TARGET} PRIVATE _MANAGED)
	set_target_properties(${TARGET} PROPERTIES COMMON_LANGUAGE_RUNTIME "")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # list all Target properties
    xr_print_target_compile_flags(${TARGET})
endfunction()

function(xr_set_target_defaults_static_lib_C TARGET)
	# set output file name
    xr_set_lib_name(${TARGET})

    # set platform as x64
	target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # list all Target properties
    xr_print_target_compile_flags(${TARGET})
endfunction()

function(xr_set_target_defaults_static_lib_CPP TARGET)
	# set output file name
    xr_set_lib_name(${TARGET})

    # set platform as x64
	target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
        XRAY_STATIC_LIBRARIES
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # list all Target properties
    xr_print_target_compile_flags(${TARGET})
endfunction()

function(xr_set_target_defaults_shared_lib TARGET)
	# set output file name
    xr_set_lib_name(${TARGET})

    # set platform as x64
	target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # list all Target properties
    xr_print_target_compile_flags(${TARGET})

    
    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /DEBUG
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /FIXED:NO
        /MACHINE:X64
    )

    xr_print_target_linker_flags(${TARGET})
endfunction()

function(xr_set_target_defaults_shared_lib_CLR TARGET)
	# set output file name
    xr_set_lib_name(${TARGET})

    # set platform as x64
	target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
    )

	# set properties for CLR
	target_compile_definitions(${TARGET} PRIVATE _MANAGED)
	set_target_properties(${TARGET} PROPERTIES COMMON_LANGUAGE_RUNTIME "")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # list all Target properties
    xr_print_target_compile_flags(${TARGET})

    
    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /DEBUG
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /FIXED:NO
        /MACHINE:X64
    )

    xr_print_target_linker_flags(${TARGET})
endfunction()

# The only difference between Static and Shared editor libs are Linkage settings
# C++ projects use #pragma comment(lib, "libname.lib") directives within source code to add library linkage
function(xr_set_target_defaults_editor TARGET)
    # set output file name
    xr_set_lib_name(${TARGET})

    # set platform as x64
	target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
    )

	# set properties for CLR
	target_compile_definitions(${TARGET} PRIVATE _MANAGED)
	set_target_properties(${TARGET} PROPERTIES COMMON_LANGUAGE_RUNTIME "")

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    # disable Warning 4945
    target_compile_options(${TARGET} PRIVATE
        /wd4945
    )

    # Enable CLR again thanks to C# projects having separate property for that in VS2019 and higher :)
    set_target_properties(${TARGET} PROPERTIES VS_GLOBAL_CLRSupport "true")

    xr_print_target_compile_flags(${TARGET})

    target_link_options(${TARGET} PRIVATE
        /DEBUG
        /SUBSYSTEM:WINDOWS
        /DYNAMICBASE:NO
        /DELAY:UNLOAD
        /MACHINE:X64
        /CLRTHREADATTRIBUTE:STA
    )
	
	# Visual Studio 17 (2022) and above require a flag to indicate .DLLs for CLR
	if(NOT CMAKE_GENERATOR MATCHES "Visual Studio 1[0-6]")
        set_property(TARGET ${TARGET} PROPERTY VS_GLOBAL_ManagedAssembly "true")
	endif()

    # Set dependencies
    target_link_libraries(${TARGET} PUBLIC 
		wpf_controls
	)

    xr_print_target_linker_flags(${TARGET})
endfunction()

function(xr_set_target_defaults_network_32 TARGET)
    # set output file name
    xr_set_network_name_32(${TARGET})

    # set platform as x86
    target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN32
        WIN32
        _CONSOLE
        _HAS_EXCEPTIONS=0
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    xr_print_target_compile_flags(${TARGET})

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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    xr_print_target_compile_flags(${TARGET})

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

    xr_print_target_linker_flags(${TARGET})
endfunction()

function(xr_set_target_defaults_plugin TARGET)
    # set platform as x64
    target_compile_definitions(${TARGET} PRIVATE
        PLATFORM_WIN64
        WIN64
        WINDOWS
        _HAS_EXCEPTIONS=0
    )

    # set compiler flags
    target_compile_options(${TARGET} PRIVATE
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
        ${XRAY_DIR_INCLUDE_PATH}
		${THIRDPARTY_DIR_INCLUDE_PATH}
    )

    xr_print_target_compile_flags(${TARGET})

    target_link_options(${TARGET} PRIVATE
        /INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
        /DYNAMICBASE:NO
        /DELAY:UNLOAD
    )

    xr_print_target_linker_flags(${TARGET})
endfunction()



#############################################
# CALL SETUP FUNCTIONS AND FINALIZE CONFIGS #
#############################################

# Project setup using presets
xr_setup_static_libs()
xr_setup_shared_libs()
xr_setup_editors()
xr_setup_network()
xr_setup_plugins()
xr_setup_renderers()
xr_setup_sdk()
xr_setup_utilities()
xr_setup_games()

# Set startup project to Stk2 DX11
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT stalker2_pc_dx11)