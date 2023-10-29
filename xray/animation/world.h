////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_ANIMATION_WORLD_H_INCLUDED
#define XRAY_ANIMATION_WORLD_H_INCLUDED

namespace xray {

namespace input{
	struct world;
} // namespace input

namespace render {
namespace debug {

	class renderer;

} // namespace debug 

namespace game {

	class renderer;

} // namespace game

} // namespace render

namespace animation {

struct handler;
class bi_spline_skeleton_animation;
class  i_animation_controller_set;
class  i_animation_controller;
class base_controller;

struct XRAY_NOVTABLE world {

	virtual	void						clear_resources					( ) = 0;
	virtual	void						tick							( ) = 0;
	
//	virtual void						set_test_scene					( render::scene_ptr const& scene ) = 0;

	virtual bi_spline_skeleton_animation* create_skeleton_animation_data( u32 bones_count ) = 0;
	virtual	void						destroy							( bi_spline_skeleton_animation	* &p ) =0;

	virtual	i_animation_controller*		create_controller					(i_animation_controller_set &set) {

		//configs::binary_config_ptr m_load_temp_settings = static_cast_resource_ptr< configs::binary_config_ptr >(data[0].get_unmanaged_resource());
		//configs::binary_config_value cfg = m_load_temp_settings->get_root();
		//base_controller* ctrl = create_controller(set, m_load_temp_settings, *m_animation_world);

		i_animation_controller* anim_controller = 0; 
		return anim_controller;
	};

protected:
	XRAY_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world )
}; // class world

} // namespace animation
} // namespace xray

#endif // #ifndef XRAY_ANIMATION_WORLD_H_INCLUDED