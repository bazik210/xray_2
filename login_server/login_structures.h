////////////////////////////////////////////////////////////////////////////
//	Created		: 02.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_LOGIN_STRUCTURES_H_INCLUDED
#define XRAY_LOGIN_STRUCTURES_H_INCLUDED

#include "constants.h"

namespace xray {

struct sign_up_info {
	char	account_name[ max_account_name_length ];
	char	password[ max_password_length ];
	char	email[ max_email_length ];
}; // struct sign_up_info

} // namespace xray

#endif // #ifndef XRAY_LOGIN_STRUCTURES_H_INCLUDED