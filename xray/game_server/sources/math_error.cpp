////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifndef MASTER_GOLD
int _matherr	( struct _exception *exception )
{
	return		( xray::debug::on_math_error( exception ) );
}
#endif // #ifndef MASTER_GOLD