////////////////////////////////////////////////////////////////////////////
//	Created		: 02.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_MACRO_UNREFERENCED_PARAMETER_H_INCLUDED
#define XRAY_MACRO_UNREFERENCED_PARAMETER_H_INCLUDED

namespace xray {
namespace detail {
	inline void	unreferenced_parameter_helper ( ... ) { }
} // namespace detail
} // namespace xray

#define XRAY_UNREFERENCED_PARAMETER(parameter) \
	(void)(&parameter)

#define XRAY_UNREFERENCED_PARAMETERS(...) \
	if ( xray::identity(false) ) { xray::detail::unreferenced_parameter_helper(__VA_ARGS__); } else (void)0

#endif // #ifndef XRAY_MACRO_UNREFERENCED_PARAMETER_H_INCLUDED