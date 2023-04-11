cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

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



####################################
# Set paths to Include Directories #
####################################
# Solution directory
set(SOLUTION_DIR_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Boost
set(BOOST_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/boost/")
# FastDelegate
set(FASTDELEGATE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/fastdelegate/")
# Stlport
set(STLPORT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/stlport/")



#################################
# GLOBAL SOLUTION CONFIGURATION #
#################################
project(X-Ray-v2.0)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOLUTION_DIR_INCLUDE_PATH}/binaries)

# Disable Exception Handling and Runtime Checks using any means necessary
foreach(var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_INIT)
        string(REGEX REPLACE "/EH[a-z]+" " " ${var} "${${var}}")
        string(REPLACE "/EHa" " " ${var} "${${var}}")
        string(REPLACE "/EHsc" " " ${var} "${${var}}")
		string(REGEX REPLACE "/RTC[a-z0-9]+" " " ${var} "${${var}}")
		string(REPLACE "/RTCc" " " ${var} "${${var}}")
		string(REPLACE "/RTCs" " " ${var} "${${var}}")
		string(REPLACE "/RTCu" " " ${var} "${${var}}")
		string(REPLACE "/RTC1" " " ${var} "${${var}}")
endforeach()

# Main engine directory
add_subdirectory(xray)

# 3rd party libraries
add_subdirectory(amd)
add_subdirectory(BugTrap)
add_subdirectory(minizip)
add_subdirectory(zlib)
add_subdirectory(cs)
add_subdirectory(freeimage)
add_subdirectory(nvidia)
add_subdirectory(vorbis)
add_subdirectory(bullet)
add_subdirectory(WildMagic)
add_subdirectory(lua)
add_subdirectory(luabind)
add_subdirectory(ode)
add_subdirectory(ogg)
add_subdirectory(opcode)
add_subdirectory(theora)

# C# project directories
add_subdirectory(WeifenLuo.WinFormsUI.Docking)
add_subdirectory(WPFToolkit.Extended)

# Enable folder hierarchy view
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



#############
# UTILITIES #
#############
# Clear all Target flags related to Exception Handling
function(xr_disable_target_exception_handling TARGET)
    # "Wanna see me do it again?"
    get_target_property(TARGET_COMPILE_FLAGS ${TARGET} COMPILE_OPTIONS )
    string(REGEX REPLACE "/EH[a-z]+" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/EHa" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
    string(REPLACE "/EHsc" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
	string(REGEX REPLACE "/RTC[a-z0-9]+" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
	string(REPLACE "/RTCc" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
	string(REPLACE "/RTCs" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
	string(REPLACE "/RTCu" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
	string(REPLACE "/RTC1" " " ${TARGET_COMPILE_FLAGS} "${${TARGET_COMPILE_FLAGS}}")
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
    xr_set_target_defaults_static_lib(ai)
    xr_set_target_defaults_static_lib(ai_navigation)
    xr_set_target_defaults_static_lib(animation)
    xr_set_target_defaults_static_lib(rtp)
    xr_set_target_defaults_static_lib(core)
    xr_set_target_defaults_static_lib(debug)
    xr_set_target_defaults_static_lib(fs)
    xr_set_target_defaults_static_lib(logging)
    xr_set_target_defaults_static_lib(vfs)
    xr_set_target_defaults_static_lib(collision)
    xr_set_target_defaults_static_lib(engine)
    xr_set_target_defaults_static_lib(input)
    xr_set_target_defaults_static_lib(particle)
    xr_set_target_defaults_static_lib(physics)
    xr_set_target_defaults_static_lib(sound)
    xr_set_target_defaults_static_lib(ui)
endfunction()

# Called at the bottom of this CMakeLists
# TODO add DLLs
function(xr_setup_shared_libs)
    
endfunction()

# Called at the bottom of this CMakeLists
# TODO set target rootNamespace for proper assembly set_property(TARGET yourTargetName PROPERTY VS_GLOBAL_RootNamespace yourRootNamespace)
# TODO set WPF settings for wpf_controls: https://stackoverflow.com/questions/2074144/generate-c-sharp-project-using-cmake/46247927#46247927
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
    target_link_libraries(test_form WPFToolkit.Extended)
    target_link_libraries(test_form WeifenLuo.WinFormsUI.Docking)

    # Set dependencies of wpf_controls
    target_link_libraries(wpf_controls WPFToolkit.Extended)
    target_link_libraries(wpf_controls WeifenLuo.WinFormsUI.Docking)

    # Set directories for #using directive
    # TODO check if this works at all
    target_compile_options(animation_editor PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(dialog_editor PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(editor PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(editor_base PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(editor_controls PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(model_editor PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(particle_editor PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(sound_editor PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )
    target_compile_options(wpf_controls PRIVATE
        /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
    )

    # Set DOTNET version for each project
    set_target_properties(animation_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(dialog_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(editor_base PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(editor_controls PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(model_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(particle_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(sound_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(WeifenLuo.WinFormsUI.Docking PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v2.0")
    set_target_properties(WPFToolkit.Extended PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
    set_target_properties(wpf_controls PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")

    # Specify DOTNET assembly references
    # TODO check if this works for Local assemblies i.e. WeifenLuo that we compile on our own
    #      assemblies appear in Object Browser so maybe this is the rootNamespace problem?
    set_target_properties(animation_editor PROPERTIES VS_DOTNET_REFERENCES
        "Microsoft.Office.Interop.Excel;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Design;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11"
    )

    set_target_properties(dialog_editor PROPERTIES VS_DOTNET_REFERENCES
        "Microsoft.Office.Interop.Excel;PresentationCore;PresentationFramework;System;System.Data;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11"
    )

    set_target_properties(editor PROPERTIES VS_DOTNET_REFERENCES
        "bugtrapN-x64;Microsoft.Office.Interop.Excel;Microsoft.VisualBasic;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Design;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_animation_editor;xray_dialog_editor;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11;xray_model_editor;xray_particle_editor;xray_sound_editor"
    )

    set_target_properties(editor_base PROPERTIES VS_DOTNET_REFERENCES
        "PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Design;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;wpf_controls;WPFToolkit.Extended;xray_editor_controls;xray_engine_pc_dx11"
    )

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
    xr_set_target_defaults_static_lib(network)

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
        ${SOLUTION_DIR_INCLUDE_PATH}/maya2011
    )
    target_link_directories(maya2011 PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
        ${SOLUTION_DIR_INCLUDE_PATH}/stlport/libraries/x64
        ${SOLUTION_DIR_INCLUDE_PATH}/maya2011/lib64
        ${SOLUTION_DIR_INCLUDE_PATH}/amd/tootle/libraries
        ${SOLUTION_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
        ${SOLUTION_DIR_INCLUDE_PATH}/scaleform/Lib/x64/Msvc90/Release
    )

    set_target_properties(maya_animation PROPERTIES
		OUTPUT_NAME "xray_maya_animation"
	)

    set_target_properties(maya2012 PROPERTIES
		OUTPUT_NAME "xray.maya2012"
        SUFFIX ".mll"
	)
    target_include_directories(maya2012 PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/maya2012
    )
    target_link_directories(maya2012 PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
        ${SOLUTION_DIR_INCLUDE_PATH}/maya2012/lib64
        ${SOLUTION_DIR_INCLUDE_PATH}/amd/tootle/libraries
        ${SOLUTION_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
    )
endfunction()

# Called at the bottom of this CMakeLists
function(xr_setup_renderers)
    xr_set_target_defaults_static_lib(renderer_engine_pc_dx11)
    xr_set_target_defaults_static_lib(renderer_core_pc_dx11)
    xr_set_target_defaults_static_lib(renderer_engine_pc_opengl)
    xr_set_target_defaults_static_lib(renderer_facade)

    target_include_directories(renderer_engine_pc_dx11 PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/scaleform/Include
        ${SOLUTION_DIR_INCLUDE_PATH}/scaleform/Src
    )

endfunction()

# Called at the bottom of this CMakeLists
# TODO add all SDK projects (non-Editor)
function(xr_setup_sdk)
    xr_set_target_defaults_shared_lib(texture_compressor)
    xr_set_target_defaults_shared_lib(bugtrap)
    xr_set_target_defaults_shared_lib(bugtrapN)
    xr_set_target_defaults_static_lib(minizip)
    xr_set_target_defaults_static_lib(minizipN)
    xr_set_target_defaults_static_lib(zlib)
    xr_set_target_defaults_static_lib(zlibN)
    xr_set_target_defaults_static_lib(core_cs)
    xr_set_target_defaults_static_lib(script_cs)
    xr_set_target_defaults_shared_lib(FreeImage)
    xr_set_target_defaults_static_lib(LibJPEG)
    xr_set_target_defaults_static_lib(LibMNG)
    xr_set_target_defaults_static_lib(LibOpenJPEG)
    xr_set_target_defaults_static_lib(LibPNG)
    xr_set_target_defaults_static_lib(LibTIFF)
    xr_set_target_defaults_static_lib(OpenEXR)

    # Set individual configs IF needed
    # TODO finish individual project setup
    target_link_directories(texture_compressor PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
        ${SOLUTION_DIR_INCLUDE_PATH}/stlport/libraries/x64
    )

    target_compile_definitions(bugtrapN PRIVATE
        _WINDOWS    
        _USRDLL
        BUGTRAP_EXPORT
    )
    target_link_directories(bugtrapN PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
    )

    target_compile_definitions(minizip PRIVATE
        WIN32
        WIN64=0
        _LIB
        _CRT_SECURE_NO_WARNINGS
    )

    target_compile_definitions(minizipN PRIVATE
        _LIB
        _CRT_SECURE_NO_WARNINGS
    )

    target_compile_definitions(zlib PRIVATE
        WIN32
        WIN64=0
        _LIB
        _CRT_SECURE_NO_WARNINGS
    )

    target_compile_definitions(zlibN PRIVATE
        _LIB
        _CRT_SECURE_NO_WARNINGS
    )

    target_compile_definitions(core_cs PRIVATE
        CS_STATIC_LIBRARIES
        _SECURE_SCL=0
        _HAS_ITERATOR_DEBUGGING=0
    )

    target_compile_definitions(script_cs PRIVATE
        CS_STATIC_LIBRARIES
        _SECURE_SCL=0
        _HAS_ITERATOR_DEBUGGING=0
    )

    target_include_directories(FreeImage PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/OpenEXR/IlmImf
        ${SOLUTION_DIR_INCLUDE_PATH}/OpenEXR/Imath
        ${SOLUTION_DIR_INCLUDE_PATH}/OpenEXR/Iex
        ${SOLUTION_DIR_INCLUDE_PATH}/OpenEXR/Half
    )
    target_compile_definitions(FreeImage PRIVATE
        OPJ_STATIC
        FREEIMAGE_EXPORTS
        _CRT_SECURE_NO_DEPRECATE
    )
    target_link_directories(FreeImage PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/binaries
    )

    target_include_directories(LibJPEG PRIVATE
        ${SOLUTION_DIR_INCLUDE_PATH}/zlib
    )
    target_compile_definitions(LibJPEG PRIVATE
        _LIB
        _CRT_SECURE_NO_DEPRECATE
    )
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
function(xr_set_target_defaults_static_lib TARGET)
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
        ${STLPORT_INCLUDE_PATH}
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
        ${STLPORT_INCLUDE_PATH}
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
        ${STLPORT_INCLUDE_PATH}
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
        set_target_properties(${TARGET} PROPERTY VS_GLOBAL_ManagedAssembly "true")
	endif()

    # Set dependencies
    target_link_libraries(${TARGET} WPFToolkit.Extended)
    target_link_libraries(${TARGET} WeifenLuo.WinFormsUI.Docking)
    target_link_libraries(${TARGET} wpf_controls)

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
        ${STLPORT_INCLUDE_PATH}
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
        ${STLPORT_INCLUDE_PATH}
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
        ${STLPORT_INCLUDE_PATH}
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
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT stalker2_pc_dx_11)