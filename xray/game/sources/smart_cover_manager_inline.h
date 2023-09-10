////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.11.2007
//	Author	: Dmitriy Iassenev
//	Copyright (C) GSC Game World
////////////////////////////////////////////////////////////////////////////

#ifndef SMART_COVER_MANAGER_INLINE_H_INCLUDED
#define SMART_COVER_MANAGER_INLINE_H_INCLUDED

namespace smart_cover {

	inline bool manager::member() const
	{
		return	(m_member);
	}

	inline void manager::member(bool const value)
	{
		m_member = value;
	}

} // namespace smart_cover

#endif // #ifndef SMART_COVER_MANAGER_INLINE_H_INCLUDED
