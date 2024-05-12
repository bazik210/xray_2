////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Plichko Alexander
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PROJECT_COOKER_SIMPLE_H_INCLUDED
#define PROJECT_COOKER_SIMPLE_H_INCLUDED

#include <xray/resources_cook_classes.h>
#include "game_project.h"

namespace stalker2 {

	class game_project ;

	class project_cooker_simple	:	public resources::translate_query_cook,
									public boost::noncopyable					
	{
		typedef resources::translate_query_cook		super;
	public:
									project_cooker_simple		( bool editor_present );

		virtual	void				translate_query			( resources::query_result_for_cook&	parent );
		virtual void				delete_resource			( resources::resource_base* resource );

	private:
				void				on_game_project_loaded( resources::queries_result& data, 
													resources::query_result_for_cook* parent );
#ifndef MASTER_GOLD	
		void				on_fs_iterators_ready	( resources::queries_result& data, 
			resources::query_result_for_cook* parent );

		void				on_editor_project_loaded( resources::queries_result& data, 
			resources::query_result_for_cook* parent );

		void				make_game_project		( configs::lua_config_ptr& editor_project, 
			resources::query_result_for_cook* parent );

		void				on_cells_loaded			( resources::queries_result& data,
			resources::query_result_for_cook* parent,
			game_project* project );
#endif // MASTER_GOLD
		void create_game_objects( xray::configs::binary_config_ptr project_cfg, resources::query_result_for_cook* parent_query );

		bool				m_editor_present;

	}; // class project_cooker_simple

} // namespace stalker2

#endif // #ifndef PROJECT_COOKER_SIMPLE_H_INCLUDED