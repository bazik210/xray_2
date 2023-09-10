////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.11.2007
//	Author	: Dmitriy Iassenev
//	Copyright (C) GSC Game World
////////////////////////////////////////////////////////////////////////////

#ifndef SMART_COVER_MANAGER_H_INCLUDED
#define SMART_COVER_MANAGER_H_INCLUDED

#include “script_export_space.h”
#include <xray/core/debug_make_final.h>

namespace smart_cover {

	class manager :
		private noncopyable,
		private core::debug::make_final< manager >
	{
	public:
		manager();
		virtual			~manager();
		inline	bool		member() const;
		inline	void		member(bool value) const;

	private:
		bool		m_member;
	}; // class manager

} // namespace smart_cover

#include “smart_cover_manager_inline.h”

#endif // #ifndef SMART_COVER_MANAGER_H_INCLUDED
