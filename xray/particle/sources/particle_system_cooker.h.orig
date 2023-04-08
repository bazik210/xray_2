////////////////////////////////////////////////////////////////////////////
//	Created		: 26.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_SYSTEM_COOKER_H_INCLUDED
#define PARTICLE_SYSTEM_COOKER_H_INCLUDED

#include <xray/resources_cook_classes.h>

namespace xray {
namespace particle {

struct world;

class particle_system;
class particle_emitter;
class particle_action;

struct particle_system_cooker_data
{
	particle_system_cooker_data():
		source_info_time		(0),
		converted_info_time		(0),
		query_result			(NULL),
		source_ready			(false),
		converted_ready			(false)
	{}
	
	u32 source_info_time;
	u32 converted_info_time;
	
	fs_new::virtual_path_string			resource_path;
	resources::query_result_for_cook*	query_result;
	
	bool source_ready;
	bool converted_ready;
};


class particle_system_cooker :	
	public resources::translate_query_cook,
	public boost::noncopyable
{
	typedef resources::translate_query_cook	super;

public:
	particle_system_cooker									();
	virtual					~particle_system_cooker			() {}
	
	virtual	void			translate_query					(resources::query_result_for_cook & parent);
	virtual void			delete_resource					(resources::resource_base * resource);

	void					on_fs_iterator_ready_source		(particle_system_cooker_data* data, xray::vfs::vfs_locked_iterator const & fs_it);
	void					on_fs_iterator_ready_converted	(particle_system_cooker_data* data, xray::vfs::vfs_locked_iterator const & fs_it);
	void					on_fs_iterators_ready			(particle_system_cooker_data* data);
	void					request_convertion				(fs_new::virtual_path_string* resource_path, resources::query_result_for_cook* parent);

	void					on_config_loaded			(resources::queries_result& result);

}; // class particle_system_cooker

extern xray::uninitialized_reference<particle_system_cooker> s_particle_system_cook_object;

void register_particles_system_cooker();
void unregister_particles_system_cooker();

} // namespace particle
} // namespace xray

#endif // #ifndef PARTICLE_SYSTEM_COOKER_H_INCLUDED