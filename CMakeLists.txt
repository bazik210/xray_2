# 3.16 is first release with precompiled headers support
cmake_minimum_required(VERSION 3.16)

# Setup compiler flags (MSVC 17 flags doesn't change :( )
if(MSVC)
	# Disable exceptions
	string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

	# Disable RTTI
	string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
else()
	# Disable exceptions
	string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

	# Disable RTTI
	string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

#project(xray LANGUAGES C CXX CSharp)
project(xray LANGUAGES C CXX)

# Add C# support for CMake
#include(CSharpUtilities)

# Platform string (shitty check)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(XR_PLATFORM_NAME "Win64")
else()
	set(XR_PLATFORM_NAME "Win32")
endif()

# Some global stuff
set(XR_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../../binaries/${XR_PLATFORM_NAME}")
set(XR_THIRDPARTY_PATH "${CMAKE_SOURCE_DIR}/../")
set(XR_STATIC_BUILD FALSE)
set(XR_GOLD_MASTER FALSE)

# some global function
function(xr_set_project_params TARGET)
	
endfunction()

add_subdirectory(debug)