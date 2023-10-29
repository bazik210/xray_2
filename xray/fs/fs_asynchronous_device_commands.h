////////////////////////////////////////////////////////////////////////////
//	Created		: 07.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef ASYNCHRONOUS_DEVICE_COMMANDS_H_INCLUDED
#define ASYNCHRONOUS_DEVICE_COMMANDS_H_INCLUDED

#include <xray/one_way_threads_channel_with_response.h>
#include <xray/fs/fs_file_type.h>
#include <xray/fs/fs_virtual_path_string.h>
#include <xray/fs/fs_device_file_system_proxy.h>
#include <xray/fs/fs_physical_path_info.h>
#include <xray/threading_extensions.h>

namespace xray {
namespace fs_new {

class synchronous_device_interface;

class asynchronous_device_query : public threads_channel_query_base<asynchronous_device_query>
{
public:
							asynchronous_device_query	(memory::base_allocator * allocator) 
								: threads_channel_query_base<asynchronous_device_query>(allocator), m_device_query_result(true) {}
	virtual					~asynchronous_device_query() {}

	virtual	void			execute						() = 0;
	virtual	void			callback_if_needed			() {}

protected:
			bool			m_device_query_result;
private:
	friend class			asynchronous_device_interface;
};

class null_device_query : public asynchronous_device_query
{
public:
							null_device_query			(memory::base_allocator * allocator) 
								: asynchronous_device_query(allocator) {}
	virtual					~null_device_query			() {}

	virtual	void			execute						() {}
};

class asynchronous_device_interface;

class synchronize_device_query : public asynchronous_device_query 
{
public:
							synchronize_device_query	(asynchronous_device_interface *	device, 
														 threading::thread_id_type			synchronous_thread_id,
														 memory::base_allocator *			allocator)
								:	asynchronous_device_query	(allocator), 
									m_synchronous_thread_id		(synchronous_thread_id), 
									m_device					(device) {}

	virtual					~synchronize_device_query	() {}

	virtual	void			execute						();
	void					wait_synchronization		();
	void					on_synchronized_ended		();
private:
	asynchronous_device_interface *		m_device;
	threading::event					m_synchronization_started_event;
	threading::event					m_synchronization_ended_event;
	threading::thread_id_type			m_synchronous_thread_id;
};

typedef	boost::function< void (file_type * out_result) >	query_open_file_callback;

class open_file_device_query : public asynchronous_device_query
{
public:
					open_file_device_query		(native_path_string const &			physical_path,
												 open_file_params const &			open_file_params,
												 query_open_file_callback const &	callback,
												 memory::base_allocator *			allocator,
												 device_file_system_proxy const &	device) 
								: asynchronous_device_query(allocator), m_physical_path(physical_path), 
								m_open_file_params(open_file_params), m_callback(callback), m_device(device), 
								m_result_file(NULL) {}

	virtual			~open_file_device_query			() {}

	virtual	void	execute				();
	virtual	void	callback_if_needed	();
private:
	native_path_string			m_physical_path;
	open_file_params			m_open_file_params;
	query_open_file_callback			m_callback;
	device_file_system_proxy			m_device;
	file_type *							m_result_file;
};

class close_file_device_query : public asynchronous_device_query
{
public:
							close_file_device_query		(file_type * file, 
														 memory::base_allocator * allocator,
														 device_file_system_proxy const &	device) 
								: asynchronous_device_query(allocator), m_file(file), m_device(device) {}

	virtual					~close_file_device_query	() {}

	virtual	void			execute						();
private:
	file_type *							m_file;
	device_file_system_proxy			m_device;
	bool								m_result;
};

struct query_write_file_args
{
	typedef	boost::function< void (bool) >				callback_type;

	query_write_file_args	(file_type * file, pcvoid data, size_t size, callback_type callback)
		: file(file), data(data), size(size), callback(callback) {}

	file_type * 				file; 
	pcvoid								data;
	size_t								size;
	callback_type						callback;
};

class write_file_device_query : public asynchronous_device_query
{
public:
							write_file_device_query		(query_write_file_args const & args, 
														 memory::base_allocator * allocator,
														 device_file_system_proxy const &	device) 
								: asynchronous_device_query(allocator), m_args(args), m_device(device), m_result(false) {}

	virtual					~write_file_device_query	() {}

	virtual	void	execute				();
	virtual	void	callback_if_needed	();
private:
	query_write_file_args				m_args;
	device_file_system_proxy			m_device;
	bool								m_result;
};

struct query_read_file_args
{
	typedef	boost::function< void (bool) >				callback_type;

	query_read_file_args	(file_type * file, pvoid data, size_t size, callback_type callback)
		: file(file), data(data), size(size), callback(callback) {}

	file_type * 				file; 
	pvoid								data;
	size_t								size;
	callback_type						callback;
};

class read_file_device_query : public asynchronous_device_query
{
public:
							read_file_device_query		(query_read_file_args const & args, 
														 memory::base_allocator * allocator,
														 device_file_system_proxy const &	device) 
								: asynchronous_device_query(allocator), m_args(args), m_device(device), m_result(false) {}
	virtual					~read_file_device_query	() {}

	virtual	void	execute				();
	virtual	void	callback_if_needed	();
private:
	query_read_file_args				m_args;
	device_file_system_proxy			m_device;
	bool								m_result;
};

typedef	boost::function<void (physical_path_info const & info)>		query_physical_path_info_callback;

class query_physical_path_device_query : public asynchronous_device_query
{
public:
					query_physical_path_device_query	(native_path_string const &			path, 
														 query_physical_path_info_callback const & callback,
														 memory::base_allocator *			allocator,
														 device_file_system_proxy const &	device) 
				: asynchronous_device_query(allocator), m_path(path), m_callback(callback), m_device(device) {}
	virtual			~query_physical_path_device_query() {}

	virtual	void	execute				();
	virtual	void	callback_if_needed	();
private:
	native_path_string					m_path;
	query_physical_path_info_callback	m_callback;
	device_file_system_proxy			m_device;
	physical_path_info					m_result;
};

struct query_custom_operation_args
{
	typedef	boost::function< void (bool) >		callback_type;
	typedef	boost::function< bool (synchronous_device_interface &) > custom_operation_type;

	query_custom_operation_args	(custom_operation_type custom_operation, callback_type callback)
		: result(false), custom_operation(custom_operation), callback(callback) {}

	custom_operation_type				custom_operation;
	bool								result;
	callback_type						callback;
};

class custom_operation_query : public asynchronous_device_query
{
public:
			custom_operation_query		(query_custom_operation_args const &	args, 
										 memory::base_allocator *				allocator,
										 device_file_system_proxy const &		device) 
				: asynchronous_device_query(allocator), m_args(args), m_device(device), m_result(false) {}

	virtual		~custom_operation_query	() {}

	virtual	void	execute				();
	virtual	void	callback_if_needed	();
private:
	query_custom_operation_args			m_args;
	device_file_system_proxy			m_device;
	bool								m_result;
};

} // namespace fs_new
} // namespace xray

#endif // #ifndef ASYNCHRONOUS_DEVICE_COMMANDS_H_INCLUDED