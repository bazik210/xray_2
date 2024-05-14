////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_SHARED_STRING_H_INCLUDED
#define XRAY_SHARED_STRING_H_INCLUDED

#include <xray/strings_shared_profile.h>

template class XRAY_CORE_API xray::intrusive_ptr<
	xray::strings::shared::profile,
	xray::strings::shared::detail::intrusive_base,
	xray::threading::simple_lock
>;

namespace xray {

class XRAY_CORE_API shared_string {
public:
	inline				shared_string						( );
			IMPLICIT	shared_string						( pcstr value );
	inline	IMPLICIT	shared_string						( shared_string const& object );

public:
	inline	pcstr		c_str								( ) const;
	inline	bool		operator!							( ) const;
	inline	bool		operator<							( shared_string const& string ) const;
	inline	bool		operator==							( shared_string const& string ) const;
	inline	bool		operator!=							( shared_string const& string ) const;
	inline	u32			length								( ) const;
	inline	u32			size								( ) const;

public:
	typedef pcstr ( shared_string::*unspecified_bool_type )	( ) const;

	// Deprecated: https://en.cppreference.com/w/cpp/io/basic_ios/operator_bool
	//inline				operator unspecified_bool_type		( ) const;
#if (_MSC_VER > 1500) && !defined(_USING_V100_SDK71_)
	inline  explicit	operator bool						( ) const;
#else
	inline				operator unspecified_bool_type		() const;
#endif

private:
	typedef strings::shared::profile::profile_ptr			profile_ptr;

private:
	profile_ptr			m_pointer;
}; // class shared_string

} // namespace xray

#include <xray/shared_string_inline.h>

#endif // #ifndef XRAY_SHARED_STRING_H_INCLUDED