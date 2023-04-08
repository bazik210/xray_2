////////////////////////////////////////////////////////////////////////////
//	Created		: 06.02.2012
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef QUERY_PROCESSOR_H_INCLUDED
#define QUERY_PROCESSOR_H_INCLUDED

#include <xray/login_server/login_structures.h>
#include <boost/program_options.hpp>

namespace xray {

class query_processor : private boost::noncopyable
{
public:
			query_processor					( MYSQL& connection, boost::program_options::variables_map const& options );
			~query_processor				( );	

	void	add_new_account					( sign_up_info const& info );
	void	update_sign_in_stats			(
				u32 account_id,
				u32 invalid_attempts_count,
				boost::posix_time::ptime const& next_attempt_time
			);
	u32		add_new_online_account			( u32 account_id, std::string const& ip_address );
	void	set_last_activity_time			( u32 session_id );
	u64		disconnect_inactive				( boost::posix_time::seconds const& interval );
	void	remove_online_user_with_password( u32 const account_id, pcstr password );
	void	remove_online_user				( u32 const account_id );
	u32		process_sign_out				( u32 const session_id );
	bool	process_sign_in					(
				pcstr account_name,
				u32 account_name_length,
				u32& account_id,
				u32& attempts_count,
				boost::posix_time::ptime& next_attempt_time,
				digest_type& password_digest
			);
	bool	process_sign_up					( pcstr account_name, u32 account_name_length, u32& count );

	static	void	fill_string_digest		( pcstr const string, digest_type& digest );

private:
	static	void	set_current_time		( MYSQL_TIME& time_to_fill );

private:
	void	bind_parameters					( );

	// IMPORTANT!!!
	// do not change order of elements in the enums below
private:
	enum statements_types_enum {
		add_new_user_statement_type,
		update_sign_in_stats_statement_type,
		add_new_online_user_statement_type,
		set_last_activity_time_statement_type,
		disconnect_inactive_users_statement_type,
		select_hash_by_account_id_statement_type,
		delete_user_by_account_id_statement_type,
		select_account_id_by_session_id_statement_type,
		select_account_data_by_name_statement_type,
		select_account_sign_info_by_name_statement_type,

		// always should be the last one
		statements_types_enum_count,
	}; // enum statements_types_enum

	enum binds_counts_enum {
		add_new_user_params_count			= 4,
		update_sign_in_stats_bind			= 3,
		add_new_online_user_bind			= 4,
		set_last_activity_time_bind			= 2,
		disconnect_inactive_users_bind		= 1,
		select_hash_bind					= 1,
		delete_online_user_bind				= 1,
		select_account_id_bind				= 1,
		select_account_data_bind			= 1,
		select_account_sign_info_bind		= 1,
		selected_account_result_bind		= 1,
		selected_hash_result_bind			= 1,
		selected_account_data_result_bind	= 4,
		selected_count_result_bind			= 1,
	}; // enum binds_counts_enum

	enum binds_offsets_enum {
		add_new_user_bind_offset					= 0,
		update_sign_in_stats_bind_offset			= add_new_user_bind_offset + add_new_user_params_count,
		add_new_online_user_bind_offset				= update_sign_in_stats_bind_offset + update_sign_in_stats_bind,
		set_last_activity_time_bind_offset			= add_new_online_user_bind_offset + add_new_online_user_bind,
		disconnect_inactive_users_bind_offset		= set_last_activity_time_bind_offset + set_last_activity_time_bind,
		select_hash_bind_offset						= disconnect_inactive_users_bind_offset + disconnect_inactive_users_bind,
		delete_online_user_bind_offset				= select_hash_bind_offset + select_hash_bind,
		select_account_id_bind_offset				= delete_online_user_bind_offset + delete_online_user_bind,
		select_account_data_bind_offset				= select_account_id_bind_offset + select_account_id_bind,
		select_account_sign_info_bind_offset		= select_account_data_bind_offset + select_account_data_bind,
		selected_account_result_bind_offset			= select_account_sign_info_bind_offset + select_account_sign_info_bind,
		selected_hash_result_bind_offset			= selected_account_result_bind_offset + selected_account_result_bind,
		selected_account_data_result_bind_offset	= selected_hash_result_bind_offset + selected_hash_result_bind,
		selected_count_result_bind_offset			= selected_account_data_result_bind_offset + selected_account_data_result_bind,

		// always should be the last one
		binds_count									= selected_count_result_bind_offset + selected_count_result_bind,
	}; // enum binds_offsets_enum

private:
	inline	MYSQL_STMT* new_statement				( );
	inline	void		delete_statement			( MYSQL_STMT*& statement );
	inline	void		prepare_statement			( statements_types_enum statement_type, pcstr const statement_string, u32 const statement_string_length );
	template < int Count >
	inline	void		prepare_statement			( statements_types_enum statement_type, char const (&statement_string)[Count] );
	inline	void		bind_statement_parameters	( statements_types_enum statement_type, binds_offsets_enum bind_offset );
	inline	void		bind_statement_results		( statements_types_enum statement_type, binds_offsets_enum bind_offset );
	inline	void		execute_statement			( statements_types_enum statement_type );

private:
	sign_up_info	m_sign_up_info;
	digest_type		m_password_digest;
	MYSQL_TIME		m_aux_time;

	MYSQL_BIND		m_binds[ binds_count ];

	MYSQL_STMT*		m_statements[ statements_types_enum_count ];

	MYSQL&			m_connection;
	
	unsigned long	m_account_name_length;
	unsigned long	m_email_length;
	unsigned long	m_ip_address_length;
	
	u32				m_account_id;
	u32				m_invalid_attempts_count;
	u32				m_squad_leader_id;
	u32				m_session_id;
	u32				m_results_count;

	char			m_ip_address[ 64 ];
	char			m_account_name[ max_account_name_length ];
}; // class query_processor

} // namespace xray

#endif // #ifndef QUERY_PROCESSOR_H_INCLUDED