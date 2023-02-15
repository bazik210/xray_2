////////////////////////////////////////////////////////////////////////////
//	Created		: 22.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_NETWORK_CLIENT_ERROR_CODES_H_INCLUDED
#define XRAY_NETWORK_CLIENT_ERROR_CODES_H_INCLUDED

namespace xray {
namespace network {

enum client_error_codes_enum {
	host_cannot_be_resolved,
	server_cannot_be_connected,
	unable_to_write_to_socket,
	unable_to_read_from_socket,
}; // enum client_error_codes_enum

} // namespace network
} // namespace xray

#endif // #ifndef XRAY_NETWORK_CLIENT_ERROR_CODES_H_INCLUDED