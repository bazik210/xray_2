////////////////////////////////////////////////////////////////////////////
//	Created		: 07.03.2012
//	Author		: Plichko Alexander
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_FACTORY_H_INCLUDED
#define FLASH_FACTORY_H_INCLUDED

namespace Scaleform
{
	namespace Render
	{
		class TreeRoot;
		namespace ContextImpl{
			template < class C > class DisplayHandle;
		}
		using ContextImpl::DisplayHandle;
		
	}
	namespace GFx{
		class System;
		class Loader;
		class Movie;
		class MovieDef;				
		typedef Render::DisplayHandle<Render::TreeRoot> MovieDisplayHandle;
	}
}

namespace stalker2 {

class game;

struct flash_movie
{

public:
	Scaleform::GFx::MovieDef*					m_movie_def;	
	Scaleform::GFx::Movie*						m_movie;	
	const Scaleform::GFx::MovieDisplayHandle*	m_handle;
// 	void activate	( ){ };
// 	void deactivate	( ){ };
};


class flash_factory: private boost::noncopyable {
public:
	flash_factory( 	game& game );
	~flash_factory( );

	flash_movie*	create_movie ( char* file_name );
	void			destroy_movie( char* file_name );
	void			tick();
	

private:
	Scaleform::GFx::Loader* m_gfx_loader;
	game&					m_game;
}; // class flash_factory
}; // class stalker2
#endif // #ifndef FLASH_FACTORY_H_INCLUDED