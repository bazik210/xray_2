////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "client.h"
#include <boost/thread.hpp>
#include <xray/login_server/login_structures.h>
#include <conio.h>
#include <locale.h>
#include <boost/tuple/tuple.hpp>

#define XRAY_MANY_CONNECTIONS_TEST	1

#if !XRAY_MANY_CONNECTIONS_TEST
static void sign_in				( boost::asio::io_service& io_service, xray::client& client )
{
	for (;;) {
		printf					( "login:      " );
		char account_name[ xray::max_account_name_length ];
		scanf_s					( "%s", account_name, sizeof(account_name) - 1 );

		printf					( "password:   " );
		char password[ xray::max_account_name_length + 1 ];
		scanf_s					( "%s", password, sizeof(password) - 1 );

		client.sign_in			( account_name, password );
		while ( !client.is_signed_in() && !client.has_last_command_failed() )
			io_service.poll		( );

		if ( client.is_signed_in() ) {
			printf				( "successfully signed in\r\n" );
			return;
		}

		ASSERT					( client.has_last_command_failed() );
		printf					( "there was an error during sign in: %s\r\n", client.last_command_error_description() );

		for (;;) {
			printf				( "Would you like to try again? (Y/N) " );
			char const choice	= (char)toupper(_getch());
			_putch				( choice );
			printf				( "\r\n" );
			switch ( choice ) {
				case 'Y' :		break;
				case 'N' :		return;
				default  :		continue;
			}
			printf				( "-----------------------------------------\r\n" );
			break;
		}
	}
}

static void sign_up				( boost::asio::io_service& io_service, xray::client& client )
{
	for (;;) {
		xray::sign_up_info sign_up_info;
		printf					( "login:    " );
		scanf_s					( "%s", sign_up_info.account_name, sizeof(sign_up_info.account_name) - 1 );

		printf					( "password: " );
		scanf_s					( "%s", sign_up_info.password, sizeof(sign_up_info.password) - 1 );

		printf					( "email:    " );
		scanf_s					( "%s", sign_up_info.email, sizeof(sign_up_info.email) - 1 );

		bool finished;
		client.sign_up			( sign_up_info, finished );
		while ( !finished && !client.has_last_command_failed() )
			io_service.poll		( );

		if ( !client.has_last_command_failed() ) {
			printf				( "sign up was successful, sign in using your newly created login/password\r\n" );
			break;
		}

		printf					( "there was an error during sign up: %s\r\n", client.last_command_error_description() );

		for (;;) {
			printf				( "Would you like to try again? (Y/N) " );
			char const choice	= (char)toupper(_getch());
			_putch				( choice );
			printf				( "\r\n" );
			switch ( choice ) {
				case 'Y' :		break;
				case 'N' :		return;
				default  :		continue;
			}
			printf				( "-----------------------------------------\r\n" );
			break;
		}
	}
}

static bool login_screen		( boost::asio::io_service& io_service, xray::client& client )
{
	printf						( "press 1 to sign in\r\n" );
	printf						( "press 2 to sign up\r\n" );
	printf						( "press Q to Quit\r\n" );

	while ( !_kbhit() )
		io_service.poll			( );

	char const choice			= (char)toupper(_getch());
	switch ( choice ) {
		case 'Q' :				return false;
		case '1' : {
			sign_in				( io_service, client );
			break;
		}
		case '2' : {
			sign_up				( io_service, client );
			break;
		}
		default : {
			printf				( "invalid character (%c), cannot make a choice\r\n", choice );
			break;
		}
	}

	printf						( "-----------------------------------------\r\n" );
	return						true;
}

static bool lobby_screen		( boost::asio::io_service& io_service, xray::client& client )
{
	printf						( "press 1 to sign out\r\n" );
	printf						( "press 2 to play game\r\n" );
	printf						( "press Q to Quit\r\n" );

	for (;;) {
		while ( !_kbhit() )
			io_service.poll		( );

		char const choice		= (char)toupper(_getch());
		switch ( choice ) {
			case 'Q' :			return false;
			case '1' : {
				client.sign_out	( );

				while ( client.is_signed_in() && !client.has_last_command_failed() )
					io_service.poll	( );

				if ( !client.is_signed_in() ) {
					printf		( "successfully signed out\r\n" );
					printf		( "-----------------------------------------\r\n" );
					return		true;
				}
				
				break;
			}
			case '2' : {
				client.set_status ( xray::client::ready_for_battle );
				printf			( "-----------------------------------------\r\n" );
				return			true;
			}
			default : {
				printf			( "invalid character (%c), cannot make a choice\r\n", choice );
				break;
			}
		}
	}
}

#else // #if !XRAY_MANY_CONNECTIONS_TEST

class random32
{
public:
	inline			random32	( u32 const start_seed = 0 ) : m_seed( start_seed ) { }
	inline	u32		seed		( )													{ return m_seed; }
	inline	void	seed		( u32 const seed )									{ m_seed = seed; }
	inline	u32		random		( u32 const range )	
	{
		m_seed		= 0x08088405*m_seed + 1;
		return		u32(u64(m_seed)*u64(range) >> 32);
	}

	inline	float	random_f	( float const range )	
	{
		return		range * ( random( 1024 * 1024 ) / ( 1024.f * 1024.f ) );
	}

	inline	size_t	operator ( )( u32 const range)	
	{
		return		random( range );
	}

private:
	u32				m_seed;
}; // class random

std::string generate_string		( random32& random, u32 const min, u32 const max )
{
	u32 const string_length		= min + random.random(max - min);
	char* const buffer			= static_cast<char*>( _alloca( (string_length + 1)*sizeof(char) ) );
	for (u32 i=0; i<string_length; ++i)
		buffer[i]				= 'a' + rand() % ('z' - 'a' + 1);
	buffer[ string_length ]		= 0;
	return						buffer;
}

std::string generate_account_name	( random32& random, std::string const& thread_prefix, u32 const min, u32 const max )
{
	ASSERT						( max > thread_prefix.length() );
	return						thread_prefix + generate_string( random, min >= thread_prefix.length() ? min - thread_prefix.length() : 0, max - thread_prefix.length() );
}

typedef boost::tuples::tuple< std::string, boost::posix_time::ptime, bool > client_info;
typedef std::map< std::string, client_info > accounts_type;
typedef std::vector< accounts_type::value_type* > offline_clients_type;

static void sign_up				(
		random32& random,
		boost::asio::io_service& io_service,
		offline_clients_type& offline_clients,
		xray::client& client,
		accounts_type& accounts,
		std::string const& thread_prefix
	)
{
	bool finished;
	xray::sign_up_info sign_up_info;
	std::string account_name;
	std::string password;
	std::string email;
	for (;;) {
		account_name	= generate_account_name( random, thread_prefix, 4, xray::max_account_name_length );
		password		= generate_string( random, 4, xray::max_password_length );
		email			= generate_string( random, 4, xray::max_email_length );
		strcpy			( sign_up_info.account_name, account_name.c_str() );
		strcpy			( sign_up_info.password, password.c_str() );
		strcpy			( sign_up_info.email, email.c_str() );

		if ( accounts.find( account_name ) != accounts.end() )
			continue;

		offline_clients.push_back	(
			&*accounts.insert(
				std::make_pair(
					account_name,
					boost::make_tuple(
						password,
						boost::posix_time::microsec_clock::local_time() - boost::posix_time::seconds( 2*xray::login_seconds_to_next_sign_in_attempt ),
						false
					)
				)
			).first
		);
		break;
	}

	client.sign_up		( sign_up_info, finished );
	while ( !finished && !client.has_last_command_failed() )
		io_service.poll	( );

	if ( client.has_last_command_failed() ) {
		accounts.erase			( accounts.find( offline_clients.back()->first ) );
		offline_clients.pop_back( );
	}
}

u32 const total_sign_up_count	= 256;//8*1024;
u32 const thread_pool_size		= 128;
u32 const sign_up_count			= total_sign_up_count/thread_pool_size;
u32 const online_deviation		= total_sign_up_count/(4*thread_pool_size);

typedef std::vector< std::pair<xray::client*, accounts_type::value_type*> >	online_clients_type;

static bool is_destroying			= false;

static void sign_out				(
		boost::asio::io_service& io_service,
		offline_clients_type& offline_clients,
		online_clients_type& online_clients
	)
{
	ASSERT							( !online_clients.empty() );
	if ( online_clients.empty() )
		return;

	u32 const client_index			= rand() % online_clients.size();
	xray::client& client			= *online_clients[ client_index ].first;
	client.sign_out					( );

	while ( client.is_signed_in() && !client.has_last_command_failed() ) {
		io_service.poll				( );
		boost::this_thread::sleep	( boost::posix_time::milliseconds(0) );
	}

	ASSERT							( online_clients[ client_index ].second->second.get<2>() == true );
	online_clients[ client_index ].second->second.get<2>()	= false;
	offline_clients.push_back		( online_clients[ client_index ].second );
	online_clients.erase			( online_clients.begin() + client_index );
	delete							&client;
}

static void sign_in					(
		boost::asio::io_service& io_service,
		offline_clients_type& offline_clients,
		online_clients_type& online_clients,
		pcstr const login_host
	)
{
	ASSERT							( !offline_clients.empty() );
	if ( offline_clients.empty() )
		return;

	for (;;) {
		u32 const client_index			= rand() % offline_clients.size();
		accounts_type::value_type& found = *offline_clients[ client_index ];

		// check if client hasn't been signed in
		ASSERT							( found.second.get<2>() == false );

		// check if client hasn't been signed in previous time just a millisecond ago
		boost::posix_time::ptime const& now = boost::posix_time::microsec_clock::local_time();
		if ( now - found.second.get<1>() <= boost::posix_time::seconds(2*xray::login_seconds_to_next_sign_in_attempt) )
			continue;

		found.second.get<1>()			= now;
		found.second.get<2>()			= true;

		xray::client* const client		= new xray::client( io_service, login_host );
		online_clients.push_back		( std::make_pair(client, &found) );
		offline_clients.erase			( offline_clients.begin() + client_index );

		client->sign_in					( found.first.c_str(), found.second.get<0>().c_str() );
		while ( !client->is_signed_in() && !client->has_last_command_failed() )
			io_service.poll				( );

		if ( client->has_last_command_failed() ) {
			found.second.get<2>()		= false;
			offline_clients.push_back	( &found );
			delete						online_clients.back().first;
			online_clients.pop_back		( );
			continue;
		}

		break;
	}
}

LONG not_signed_up_enoguh_thread_count	= thread_pool_size;

static void test_server				( pcstr const login_host, std::string* thread_prefix )
{
	try {
		random32 random;
		accounts_type accounts;
		offline_clients_type offline_clients;
		online_clients_type online_clients;
		boost::asio::io_service io_service;
		{
			xray::client sign_up_client( io_service, login_host );
			bool signed_up_enough		= false;
			do {
				if ( accounts.size() <= sign_up_count ) {
					sign_up				( random, io_service, offline_clients, sign_up_client, accounts, *thread_prefix );
					continue;
				}

				if ( !signed_up_enough ) {
					signed_up_enough	= true;
					if ( !InterlockedDecrement(&not_signed_up_enoguh_thread_count) )
						printf			( "all the threads have been signed up enough!\r\n" );

					while ( not_signed_up_enoguh_thread_count )
						boost::this_thread::sleep	( boost::posix_time::milliseconds(100) );
				}

				u32 const online_count	= sign_up_count - online_deviation;
				if ( online_clients.size() < online_count - online_deviation ) {
					sign_in				( io_service, offline_clients, online_clients, login_host );
					continue;
				}

				if ( online_clients.size() >= online_count + online_deviation ) {
					sign_out			( io_service, offline_clients, online_clients );
					continue;
				}

				int const number		= rand( );
				if ( number % 10 == 0 ) {
					sign_up				( random, io_service, offline_clients, sign_up_client, accounts, *thread_prefix );
					continue;
				}

				if ( number % 2 == 0 ) {
					sign_out			( io_service, offline_clients, online_clients );
					continue;
				}

				sign_in					( io_service, offline_clients, online_clients, login_host );
				continue;

			} while ( !is_destroying );

			while ( !online_clients.empty() ) {
				sign_out				( io_service, offline_clients, online_clients );
				continue;
			}
		}
	}
	catch ( std::exception const& exception ) {
		printf							( "exception has been raised: %s\r\nexiting...\r\n", exception.what() );
	}

	delete thread_prefix;
}
#endif // #if !XRAY_MANY_CONNECTIONS_TEST

int main							( int argc, char* argv[] )
{
	setlocale						( LC_ALL, "" );

	pcstr const login_host			= argc < 2 ? "localhost" : argv[1];
	if ( argc < 2 )
		printf						( "please specify login host as a first argument, assuming localhost\r\n" );

#if !XRAY_MANY_CONNECTIONS_TEST
	boost::asio::io_service io_service;
	{
		xray::client client( io_service, login_host );
		for ( ; ; ) {
			if ( !client.is_signed_in() ) {
				if ( !login_screen( io_service, client ) )
					break;
			}
			else {
				if ( !lobby_screen( io_service, client ) )
					break;
			}
		}
	}
#else // #if !XRAY_MANY_CONNECTIONS_TEST
	DWORD process_affinity_mask, system_affinity_mask;
	GetProcessAffinityMask			( GetCurrentProcess(), &process_affinity_mask, &system_affinity_mask );
	SetProcessAffinityMask			( GetCurrentProcess(), process_affinity_mask ^ 1 );


	char computer_name[32];
	DWORD computer_name_length;
	computer_name_length			= sizeof(computer_name);
	GetComputerName					( computer_name, &computer_name_length );
	computer_name[ computer_name_length++ ]	= '_';
	computer_name[ computer_name_length ]	= 0;

	std::string computer_name_string	= computer_name;

	boost::thread_group threads;
	for (u32 i = 0; i < thread_pool_size; ++i) {
		char temp[8];
		_itoa						( i, temp, 10 );
		threads.create_thread		(
			boost::bind(
				&test_server,
				login_host,
				new std::string(computer_name_string + std::string( temp ) + "_")
			)
		);
	}

	printf							( "press 'Q' to quit...\r\n" );

	for (;;) {
		while ( !_kbhit() )
			boost::this_thread::sleep	( boost::posix_time::milliseconds(100) );

		if ( (char)toupper(_getch()) == 'Q' )
			break;
	}

	is_destroying					= true;

	threads.join_all				( );
#endif // #if !XRAY_MANY_CONNECTIONS_TEST

	return							0;
}