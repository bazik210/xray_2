////////////////////////////////////////////////////////////////////////////
//	Created		: 07.02.2012
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "query_processor.h"
#include <boost/uuid/sha1.hpp>
#include "boost/date_time/gregorian/gregorian_types.hpp"

namespace xray {

static void log_last_stmt_error					( pcstr const procedure_caption, MYSQL_STMT* const statement )
{
	char const* const msg		= mysql_stmt_error( statement );
	if ( *msg )
		LOG_ERROR				( "%s: %s\r\n", procedure_caption, msg );
}

inline MYSQL_STMT* query_processor::new_statement ( )
{
	MYSQL_STMT* const result	= mysql_stmt_init( &m_connection );
	if ( !result )
		LOG_ERROR				( "prepared statement initialization error: out of memory" );
	return						result;
}

inline void query_processor::prepare_statement	( statements_types_enum const statement_type, pcstr const statement_string, u32 const statement_string_length )
{
	if ( mysql_stmt_prepare( m_statements[statement_type], statement_string, statement_string_length*sizeof(char) ) )
		log_last_stmt_error		( "prepare statement error", m_statements[statement_type] );
}

template < int Count >
inline void query_processor::prepare_statement	( statements_types_enum const statement_type, char const (&statement_string)[Count] )
{
	prepare_statement			( statement_type, statement_string, Count );
}

query_processor::query_processor				( MYSQL& connection, boost::program_options::variables_map const& options ) :
	m_connection				( connection ),
	m_account_id				( u32(-1) ),
	m_invalid_attempts_count	( u32(-1) ),
	m_squad_leader_id			( u32(-1) ),
	m_session_id				( u32(-1) ),
	m_account_name_length		( u32(-1) ),
	m_email_length				( u32(-1) ),
	m_ip_address_length			( u32(-1) ),
	m_results_count				( u32(-1) )
{
	if ( !mysql_thread_safe() )
		LOG_ERROR				( "MySQL library is not thread safe!\r\n" );

	mysql_init					( &m_connection );

	bool const sql_server_connection_result	=
		mysql_real_connect		(
			&m_connection,
			options["database_server"].as< std::string >().c_str(),
			options["database_user"].as< std::string >().c_str(),
			options["database_password"].as< std::string >().c_str(),
			options["database_name"].as< std::string >().c_str(),
			0,
			NULL,
			0
		) != NULL;
	if ( !sql_server_connection_result )
	{
		char const* const error_message = mysql_error( &m_connection );
		LOG_ERROR				( "can't connect to stalker database:%s\r\nexiting...\r\n", error_message );
		return;
	}

	for ( u32 i = 0; i < statements_types_enum_count; ++i )
		m_statements[ i ]		= new_statement( );

	prepare_statement			( add_new_user_statement_type,						"INSERT INTO stalker.accounts(account_name,password_hash,email,next_sign_in_attempt_time) VALUES(?,?,?,?)" );
	prepare_statement			( update_sign_in_stats_statement_type,				"UPDATE stalker.accounts SET invalid_sign_in_attempts_count=?,next_sign_in_attempt_time=? WHERE account_id=?" );
	prepare_statement			( add_new_online_user_statement_type,				"INSERT INTO stalker.online_accounts(account_id,ip_address,squad_leader_id,last_activity_time) VALUES(?,?,?,?)" );
	prepare_statement			( set_last_activity_time_statement_type,			"UPDATE stalker.online_accounts SET last_activity_time=? WHERE session_id=?" );
	prepare_statement			( disconnect_inactive_users_statement_type,			"DELETE FROM stalker.online_accounts WHERE last_activity_time < ?" );
	prepare_statement			( select_hash_by_account_id_statement_type,			"SELECT password_hash FROM stalker.accounts WHERE account_id=?" );
	prepare_statement			( delete_user_by_account_id_statement_type,			"DELETE FROM stalker.online_accounts WHERE account_id=?" );
	prepare_statement			( select_account_id_by_session_id_statement_type,	"SELECT account_id FROM stalker.online_accounts WHERE session_id=?" );
	prepare_statement			( select_account_data_by_name_statement_type,		"SELECT account_id,password_hash,invalid_sign_in_attempts_count,next_sign_in_attempt_time FROM stalker.accounts WHERE account_name=?" );
	prepare_statement			( select_account_sign_info_by_name_statement_type,	"SELECT COUNT(*) FROM stalker.accounts WHERE account_name=?" );

	bind_parameters				( );
}

void query_processor::fill_string_digest		( pcstr const string, digest_type& digest )
{
	boost::uuids::detail::sha1	sha1;
	sha1.process_block			( string, string + strlen( string ) );
	sha1.get_digest				( digest );
}

inline void query_processor::bind_statement_parameters	( statements_types_enum const statement_type, binds_offsets_enum const bind_offset )
{
	if ( mysql_stmt_bind_param( m_statements[statement_type], &m_binds[ bind_offset ] ) )
		log_last_stmt_error		( "bind statement error", m_statements[statement_type] );
}

inline void query_processor::bind_statement_results	( statements_types_enum const statement_type, binds_offsets_enum const bind_offset )
{
	if ( mysql_stmt_bind_result( m_statements[statement_type], &m_binds[ bind_offset ] ) )
		log_last_stmt_error		( "bind statement result error", m_statements[statement_type] );
}

void query_processor::bind_parameters			( )
{
	memset						( m_binds, 0, sizeof( m_binds ) );

	m_binds[ select_hash_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_LONG;
	m_binds[ select_hash_bind_offset + 0 ].buffer			= &m_account_id;
	m_binds[ select_hash_bind_offset + 0 ].buffer_length	= sizeof( m_account_id );

	bind_statement_parameters	( select_hash_by_account_id_statement_type, select_hash_bind_offset );

	m_binds[ add_new_user_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_VARCHAR;
	m_binds[ add_new_user_bind_offset + 0 ].buffer			= m_sign_up_info.account_name;
	m_binds[ add_new_user_bind_offset + 0 ].buffer_length	= max_account_name_length;
	m_binds[ add_new_user_bind_offset + 0 ].length			= &m_account_name_length;

	m_binds[ add_new_user_bind_offset + 1 ].buffer_type	= MYSQL_TYPE_BLOB;
	m_binds[ add_new_user_bind_offset + 1 ].buffer			= m_password_digest;
	m_binds[ add_new_user_bind_offset + 1 ].buffer_length	= sizeof( m_password_digest );

	m_binds[ add_new_user_bind_offset + 2 ].buffer_type	= MYSQL_TYPE_VARCHAR;
	m_binds[ add_new_user_bind_offset + 2 ].buffer			= m_sign_up_info.email;
	m_binds[ add_new_user_bind_offset + 2 ].buffer_length	= max_email_length;
	m_binds[ add_new_user_bind_offset + 2 ].length			= &m_email_length;

	m_binds[ add_new_user_bind_offset + 3 ].buffer_type	= MYSQL_TYPE_DATETIME;
	m_binds[ add_new_user_bind_offset + 3 ].buffer			= &m_aux_time;
	m_binds[ add_new_user_bind_offset + 3 ].buffer_length	= sizeof( m_aux_time );

	bind_statement_parameters	( add_new_user_statement_type, add_new_user_bind_offset );

	m_binds[ update_sign_in_stats_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_LONG;
	m_binds[ update_sign_in_stats_bind_offset + 0 ].buffer			= &m_invalid_attempts_count;
	m_binds[ update_sign_in_stats_bind_offset + 0 ].buffer_length	= sizeof( m_invalid_attempts_count );

	m_binds[ update_sign_in_stats_bind_offset + 1 ].buffer_type	= MYSQL_TYPE_DATETIME;
	m_binds[ update_sign_in_stats_bind_offset + 1 ].buffer			= &m_aux_time;
	m_binds[ update_sign_in_stats_bind_offset + 1 ].buffer_length	= sizeof( m_aux_time );

	m_binds[ update_sign_in_stats_bind_offset + 2 ].buffer_type	= MYSQL_TYPE_LONG;
	m_binds[ update_sign_in_stats_bind_offset + 2 ].buffer			= &m_account_id;
	m_binds[ update_sign_in_stats_bind_offset + 2 ].buffer_length	= sizeof( m_account_id );

	bind_statement_parameters	( update_sign_in_stats_statement_type, update_sign_in_stats_bind_offset );

	m_binds[ add_new_online_user_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_LONG;
	m_binds[ add_new_online_user_bind_offset + 0 ].buffer			= &m_account_id;
	m_binds[ add_new_online_user_bind_offset + 0 ].buffer_length	= sizeof( m_account_id );

	m_binds[ add_new_online_user_bind_offset + 1 ].buffer_type		= MYSQL_TYPE_VARCHAR;
	m_binds[ add_new_online_user_bind_offset + 1 ].buffer			= m_ip_address;
	m_binds[ add_new_online_user_bind_offset + 1 ].buffer_length	= max_host_name_length;
	m_binds[ add_new_online_user_bind_offset + 1 ].length			= &m_ip_address_length;

	m_binds[ add_new_online_user_bind_offset + 2 ].buffer_type		= MYSQL_TYPE_LONG;
	m_binds[ add_new_online_user_bind_offset + 2 ].buffer			= &m_squad_leader_id;
	m_binds[ add_new_online_user_bind_offset + 2 ].buffer_length	= sizeof( m_squad_leader_id );

	m_binds[ add_new_online_user_bind_offset + 3 ].buffer_type		= MYSQL_TYPE_DATETIME;
	m_binds[ add_new_online_user_bind_offset + 3 ].buffer			= &m_aux_time;
	m_binds[ add_new_online_user_bind_offset + 3 ].buffer_length	= sizeof( m_aux_time );

	bind_statement_parameters	( add_new_online_user_statement_type, add_new_online_user_bind_offset );

	m_binds[ set_last_activity_time_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_DATETIME;
	m_binds[ set_last_activity_time_bind_offset + 0 ].buffer			= &m_aux_time;
	m_binds[ set_last_activity_time_bind_offset + 0 ].buffer_length	= sizeof( m_aux_time );

	m_binds[ set_last_activity_time_bind_offset + 1 ].buffer_type		= MYSQL_TYPE_LONG;
	m_binds[ set_last_activity_time_bind_offset + 1 ].buffer			= &m_session_id;
	m_binds[ set_last_activity_time_bind_offset + 1 ].buffer_length	= sizeof( m_session_id );

	bind_statement_parameters	( set_last_activity_time_statement_type, set_last_activity_time_bind_offset );

	m_binds[ disconnect_inactive_users_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_DATETIME;
	m_binds[ disconnect_inactive_users_bind_offset + 0 ].buffer		= &m_aux_time;
	m_binds[ disconnect_inactive_users_bind_offset + 0 ].buffer_length	= sizeof( m_aux_time );

	bind_statement_parameters	( disconnect_inactive_users_statement_type, disconnect_inactive_users_bind_offset );

	m_binds[ delete_online_user_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_LONG;
	m_binds[ delete_online_user_bind_offset + 0 ].buffer			= &m_account_id;
	m_binds[ delete_online_user_bind_offset + 0 ].buffer_length	= sizeof( m_account_id );

	bind_statement_parameters	( delete_user_by_account_id_statement_type, delete_online_user_bind_offset );

	m_binds[ select_account_id_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_LONG;
	m_binds[ select_account_id_bind_offset + 0 ].buffer		= &m_session_id;
	m_binds[ select_account_id_bind_offset + 0 ].buffer_length	= sizeof( m_session_id );

	bind_statement_parameters	( select_account_id_by_session_id_statement_type, select_account_id_bind_offset );

	m_binds[ select_account_data_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_VARCHAR;
	m_binds[ select_account_data_bind_offset + 0 ].buffer			= m_account_name;
	m_binds[ select_account_data_bind_offset + 0 ].buffer_length	= max_account_name_length;
	m_binds[ select_account_data_bind_offset + 0 ].length			= &m_account_name_length;

	bind_statement_parameters	( select_account_data_by_name_statement_type, select_account_data_bind_offset );

	m_binds[ select_account_sign_info_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_VARCHAR;
	m_binds[ select_account_sign_info_bind_offset + 0 ].buffer			= m_account_name;
	m_binds[ select_account_sign_info_bind_offset + 0 ].buffer_length	= max_account_name_length;
	m_binds[ select_account_sign_info_bind_offset + 0 ].length			= &m_account_name_length;

	bind_statement_parameters	( select_account_sign_info_by_name_statement_type, select_account_sign_info_bind_offset );

	m_binds[ selected_account_result_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_LONG;
	m_binds[ selected_account_result_bind_offset + 0 ].buffer			= &m_account_id;
	m_binds[ selected_account_result_bind_offset + 0 ].buffer_length	= sizeof( m_account_id );

	bind_statement_results		( select_account_id_by_session_id_statement_type, selected_account_result_bind_offset );

	m_binds[ selected_hash_result_bind_offset + 0 ].buffer_type		= MYSQL_TYPE_BLOB;
	m_binds[ selected_hash_result_bind_offset + 0 ].buffer			= m_password_digest;
	m_binds[ selected_hash_result_bind_offset + 0 ].buffer_length	= sizeof( m_password_digest );

	bind_statement_results		( select_hash_by_account_id_statement_type, selected_hash_result_bind_offset );

	m_binds[ selected_account_data_result_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_LONG;
	m_binds[ selected_account_data_result_bind_offset + 0 ].buffer			= &m_account_id;
	m_binds[ selected_account_data_result_bind_offset + 0 ].buffer_length	= sizeof( m_account_id );
	
	m_binds[ selected_account_data_result_bind_offset + 1 ].buffer_type	= MYSQL_TYPE_BLOB;
	m_binds[ selected_account_data_result_bind_offset + 1 ].buffer			= m_password_digest;
	m_binds[ selected_account_data_result_bind_offset + 1 ].buffer_length	= sizeof( m_password_digest );

	m_binds[ selected_account_data_result_bind_offset + 2 ].buffer_type	= MYSQL_TYPE_LONG;
	m_binds[ selected_account_data_result_bind_offset + 2 ].buffer			= &m_invalid_attempts_count;
	m_binds[ selected_account_data_result_bind_offset + 2 ].buffer_length	= sizeof( m_invalid_attempts_count );

	m_binds[ selected_account_data_result_bind_offset + 3 ].buffer_type	= MYSQL_TYPE_DATETIME;
	m_binds[ selected_account_data_result_bind_offset + 3 ].buffer			= &m_aux_time;
	m_binds[ selected_account_data_result_bind_offset + 3 ].buffer_length	= sizeof( m_aux_time );

	bind_statement_results		( select_account_data_by_name_statement_type, selected_account_data_result_bind_offset );

	m_binds[ selected_count_result_bind_offset + 0 ].buffer_type	= MYSQL_TYPE_LONG;
	m_binds[ selected_count_result_bind_offset + 0 ].buffer		= &m_results_count;
	m_binds[ selected_count_result_bind_offset + 0 ].buffer_length	= sizeof( m_results_count );
	
	bind_statement_results		( select_account_sign_info_by_name_statement_type, selected_count_result_bind_offset );
}

inline void query_processor::delete_statement	( MYSQL_STMT*& statement )
{
	if ( mysql_stmt_close( statement ) )
		log_last_stmt_error		( "close statement error", statement );

	statement					= 0;
}

query_processor::~query_processor				( )
{
	for ( u32 i = 0; i < statements_types_enum_count; ++i )
		delete_statement		( m_statements[i] );
}

void query_processor::set_current_time			( MYSQL_TIME& time_to_fill )
{
	boost::posix_time::ptime const& now	= boost::posix_time::microsec_clock::local_time();
	time_to_fill.time_type		= MYSQL_TIMESTAMP_DATETIME;
	time_to_fill.year			= now.date().year();
	time_to_fill.month			= now.date().month();
	time_to_fill.day			= now.date().day();
	time_to_fill.hour			= now.time_of_day().hours();
	time_to_fill.minute			= now.time_of_day().minutes();
	time_to_fill.second			= now.time_of_day().seconds();
	time_to_fill.second_part	= u32( now.time_of_day().total_milliseconds() - now.time_of_day().total_seconds() * 1000 );
	time_to_fill.neg			= false;
}

inline void query_processor::execute_statement	( statements_types_enum const statement_type )
{
	if ( mysql_stmt_execute( m_statements[statement_type] ) )
		log_last_stmt_error		( "execute statement error", m_statements[statement_type] );
}

void query_processor::add_new_account			( sign_up_info const& info )
{
	m_sign_up_info				= info;
	m_account_name_length		= strlen( m_sign_up_info.account_name );
	m_email_length				= strlen( m_sign_up_info.email );
	fill_string_digest			( m_sign_up_info.password, m_password_digest );
	set_current_time			( m_aux_time );
	
	execute_statement			( add_new_user_statement_type );
}

void query_processor::update_sign_in_stats		( u32 const account_id, u32 const invalid_attempts_count, boost::posix_time::ptime const& next_attempt_time )
{
	m_account_id				= account_id;
	m_invalid_attempts_count	= invalid_attempts_count;

	m_aux_time.time_type		= MYSQL_TIMESTAMP_DATETIME;
	m_aux_time.year				= next_attempt_time.date().year();
	m_aux_time.month			= next_attempt_time.date().month();
	m_aux_time.day				= next_attempt_time.date().day();
	m_aux_time.hour				= next_attempt_time.time_of_day().hours();
	m_aux_time.minute			= next_attempt_time.time_of_day().minutes();
	m_aux_time.second			= next_attempt_time.time_of_day().seconds();
	m_aux_time.second_part		= u32( next_attempt_time.time_of_day().total_milliseconds() - next_attempt_time.time_of_day().total_seconds() * 1000 );
	m_aux_time.neg				= false;

	execute_statement			( update_sign_in_stats_statement_type );
}

u32 query_processor::add_new_online_account		( u32 const account_id, std::string const& ip_address )
{
	m_account_id				= account_id;
	memcpy						( m_ip_address, ip_address.c_str(), ip_address.length() * sizeof( char ) );
	m_ip_address_length			= ip_address.length();
	set_current_time			( m_aux_time );

	execute_statement			( add_new_online_user_statement_type );

	u32 const session_id		= (u32)mysql_insert_id( &m_connection );
	return						session_id;
}

void query_processor::set_last_activity_time	( u32 const session_id )
{
	m_session_id				= session_id;
	set_current_time			( m_aux_time );

	execute_statement			( set_last_activity_time_statement_type );
}

u64 query_processor::disconnect_inactive		( boost::posix_time::seconds const& interval )
{
	boost::posix_time::ptime const& collection_time	= boost::posix_time::microsec_clock::local_time() - interval;

	m_aux_time.time_type		= MYSQL_TIMESTAMP_DATETIME;
	m_aux_time.year				= collection_time.date().year();
	m_aux_time.month			= collection_time.date().month();
	m_aux_time.day				= collection_time.date().day();
	m_aux_time.hour				= collection_time.time_of_day().hours();
	m_aux_time.minute			= collection_time.time_of_day().minutes();
	m_aux_time.second			= collection_time.time_of_day().seconds();
	m_aux_time.second_part		= u32( collection_time.time_of_day().total_milliseconds() - collection_time.time_of_day().total_seconds() * 1000 );
	m_aux_time.neg				= false;

	execute_statement			( disconnect_inactive_users_statement_type );
	u64 const users_collected	= mysql_stmt_affected_rows( m_statements[disconnect_inactive_users_statement_type] );
	return						users_collected;
}

static bool store_statement_result				( MYSQL_STMT* const statement )
{
	if ( !mysql_stmt_store_result( statement ) )
		return					false;		

	log_last_stmt_error			( "store statement result error", statement );
	return						true;
}

void query_processor::remove_online_user_with_password	( u32 const account_id, pcstr const password )
{
	m_account_id				= account_id;
	execute_statement			( select_hash_by_account_id_statement_type );

	if ( store_statement_result( m_statements[select_hash_by_account_id_statement_type] ) )
		return;
	
	digest_type					password_digest;
	fill_string_digest			( password, password_digest );

	if ( !mysql_stmt_fetch( m_statements[select_hash_by_account_id_statement_type] ) )
	{
		if ( memcmp( password_digest, m_password_digest, sizeof( password_digest ) ) == 0 )
		{
			LOG					( "removing user...\r\n" );
			execute_statement	( delete_user_by_account_id_statement_type );
			LOG					( "removed!\r\n" );
		}
		else
			LOG_ERROR			( "SIGN_OUT: invalid password\r\n" );
	}
	else
		LOG						( "no user with specified account_id found\r\n" );
}

void query_processor::remove_online_user		( u32 const account_id )
{
	m_account_id				= account_id;
	execute_statement			( delete_user_by_account_id_statement_type );
}

u32 query_processor::process_sign_out			( u32 const session_id )
{
	m_session_id				= session_id;

	LOG							( "signing out...\r\n" );
	execute_statement			( select_account_id_by_session_id_statement_type );

	if ( store_statement_result( m_statements[select_account_id_by_session_id_statement_type] ) )
		return					u32(-1);

	if ( !mysql_stmt_fetch( m_statements[select_account_id_by_session_id_statement_type] ) )
		return					m_account_id;

	LOG_ERROR					( "SIGN_OUT: invalid session id\r\n" );
	return						u32(-1);
}

bool query_processor::process_sign_in			(
		pcstr const account_name,
		u32 const account_name_length,
		u32& account_id,
		u32& attempts_count,
		boost::posix_time::ptime& next_attempt_time,
		digest_type& password_digest
	)
{
	m_account_name_length		= account_name_length;
	memcpy						( m_account_name, account_name, account_name_length * sizeof( char ) );
	execute_statement			( select_account_data_by_name_statement_type );

	if ( store_statement_result( m_statements[select_account_data_by_name_statement_type] ) )
		return					false;

	if ( mysql_stmt_fetch( m_statements[select_account_data_by_name_statement_type] ) ) {
		LOG_ERROR				( "SIGN_IN: invalid user name - %s\r\n", m_account_name );
		return					false;
	}

	account_id					= m_account_id;
	attempts_count				= m_invalid_attempts_count + 1;
	boost::gregorian::date		attempt_date( (short)m_aux_time.year, (short)m_aux_time.month, (short)m_aux_time.day );
	next_attempt_time			= boost::posix_time::ptime( attempt_date, boost::posix_time::hours( m_aux_time.hour ) + boost::posix_time::minutes( m_aux_time.minute ) + boost::posix_time::seconds( m_aux_time.second ) );
	memcpy						( &password_digest, &m_password_digest, sizeof( m_password_digest ) );
	return						true;
}

bool query_processor::process_sign_up			( pcstr const account_name, u32 const account_name_length, u32& count )
{
	m_account_name_length		= account_name_length;
	memcpy						( m_account_name, account_name, account_name_length * sizeof( char ) );
	execute_statement			( select_account_sign_info_by_name_statement_type );

	if ( store_statement_result( m_statements[select_account_sign_info_by_name_statement_type] ) )
		return					false;

	if ( mysql_stmt_fetch( m_statements[select_account_sign_info_by_name_statement_type] ) )
		return					false;

	count						= m_results_count;
	return						true;
}

} // namespace xray