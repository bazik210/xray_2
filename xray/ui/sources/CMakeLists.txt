project(ui LANGUAGES CXX)

add_library(ui STATIC
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_memory.h"
				# include
				"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../text_tree_draw_helper.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../ui.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
				# world
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_world.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_world.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_world_factory.cpp"
				# objects
				# text_edit
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_edit.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_edit.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_edit_action.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_edit_action.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_edit_defs.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_edit_initialize.cpp"
				# font
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_font.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_font.h"
				# scroll_bar
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_scroll_bar.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_scroll_bar.h"
				# scroll_view
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_scroll_pad.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_scroll_pad.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_scroll_view.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_scroll_view.h"
				# image
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_image.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_image.h"
				# text
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_text_inline.h"
				# dialog
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_dialog.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_dialog.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_dialog_input_handler.cpp"
				# !base_window
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_window.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_window.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_window_inline.h"
				# base
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_base.h"
				# progress_bar
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_progress_bar.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_progress_bar.h"
				# ./
				"${CMAKE_CURRENT_SOURCE_DIR}/ui_entry_point.cpp"
)

target_link_libraries(ui PUBLIC
	core
)

set_target_properties(ui PROPERTIES FOLDER "engine")
