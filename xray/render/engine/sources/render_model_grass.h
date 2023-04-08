////////////////////////////////////////////////////////////////////////////
//	Created		: 22.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_ENGINE_RENDER_MODEL_GRASS_H_INCLUDED
#define XRAY_ENGINE_RENDER_MODEL_GRASS_H_INCLUDED

#include "render_model.h"

namespace xray {
namespace render {

class grass_render_surface :public render_surface
{
	typedef render_surface	super;
public:
	grass_render_surface 	( );
	virtual			~grass_render_surface 	( ){};
	virtual void	load					( configs::binary_config_value const& properties, memory::chunk_reader& r );
};


//shared
class grass_render_model: public render_model
{
	typedef render_model			super;
public:
	typedef render::vector<grass_render_surface*>	children;
	typedef children::iterator						children_it;
	
public:
						grass_render_model		( ){}
	virtual				~grass_render_model	( );
	virtual	void		append_surface			( render_surface* );
	
	children					m_children;
};

typedef	resources::resource_ptr<
			grass_render_model,
			resources::unmanaged_intrusive_base
		> grass_render_model_ptr;


} // namespace render 
} // namespace xray 

#endif // #ifndef XRAY_ENGINE_RENDER_MODEL_GRASS_H_INCLUDED