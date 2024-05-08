////////////////////////////////////////////////////////////////////////////
//	Created		: 09.05.2024
//	Author		: loxotron
//	Copyright (C) dez0wave - 2024
////////////////////////////////////////////////////////////////////////////

#ifndef MANAGED_H_INCLUDED
#define MANAGED_H_INCLUDED

#define _CRT_WINDOWS

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

namespace xray {
namespace editor {

	std::string convert_cli_string(System::String^ str)
	{
		std::string managed = msclr::interop::marshal_as<std::string>(str);
		return managed;
	}

} // namespace editor	
} // namespace xray

#endif // #ifndef MANAGED_H_INCLUDED