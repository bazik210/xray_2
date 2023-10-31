project(test_form LANGUAGES CXX CSharp)

add_executable(test_form
					# Source Files
					"${CMAKE_CURRENT_SOURCE_DIR}/AssemblyInfo.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_form.cpp"
					# Header Files
					"${CMAKE_CURRENT_SOURCE_DIR}/form1.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/Form1.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/Form1.resX"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h"
					# Resource Files
					"${CMAKE_CURRENT_SOURCE_DIR}/app.ico"
					"${CMAKE_CURRENT_SOURCE_DIR}/app.rc"
					# ./
					# MISSING
					#"${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt"
)

set_target_properties(test_form PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")

set_target_properties(test_form PROPERTIES VS_DOTNET_REFERENCES
    "System;System.Data;System.Drawing;System.Windows.Forms;System.Xml"
)

set_target_properties(test_form PROPERTIES FOLDER "engine/utilities")
