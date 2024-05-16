// correct_hudson_custom_workspace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <string.h>

#define ASSERT(expression) if ( !(expression) ) { printf("%s assertion failed", # expression); exit(1); }
#define PATH_MAX		512

typedef char const *	pcstr;
typedef char *			pstr;
typedef unsigned int 	u32;

pcstr		open_tag_string				=	"<customWorkspace>";
u32 const	open_tag_string_length		=	strlen(open_tag_string);
pcstr		close_tag_string			=	"</customWorkspace>";
u32 const	close_tag_string_length		=	strlen(close_tag_string);


u32 const	max_file_size				=	1024 * 128;
char		file_data[max_file_size]	=	"";
u32			file_data_size				=	0;

void   get_project_config_file_path (char (&out_path_to_config)[PATH_MAX], pcstr project_name)
{
	ASSERT									(*project_name);

	strcpy_s								(out_path_to_config, "jobs\\");
	strcat_s								(out_path_to_config, project_name);
	strcat_s								(out_path_to_config, "\\config.xml");
}

void   read_file (pstr const out_file_data, u32 * out_file_size, pcstr file_name)
{
	FILE * file							=	NULL;
	errno_t const file_open_err			=	fopen_s(& file, file_name, "rb");
	ASSERT									(file_open_err == 0);

	ASSERT									(fseek(file, 0, SEEK_END) == 0);
	u32 const file_size					=	ftell(file);
	ASSERT									(file_size < max_file_size);
	ASSERT									(fseek(file, 0, SEEK_SET) == 0);

	ASSERT									(fread_s(file_data, max_file_size, 1, file_size, file) == file_size);
	file_data[file_size]				=	0;

	if ( out_file_size )
		*out_file_size					=	file_size;

	fclose									(file);
}

void   search_workspace_path (u32 * out_open_tag_pos, u32 * out_close_tag_pos, pcstr file_data, u32 const file_size)
{
	u32 open_tag_pos					=	-1;
	u32 close_tag_pos					=	-1;

	for ( u32 file_pos=0; file_pos<file_size; ++file_pos )
	{
		if ( file_pos < open_tag_string_length )
			continue;

		pcstr const search_string		=	(open_tag_pos == -1) ? open_tag_string : close_tag_string;
		u32 const search_string_length	=	(open_tag_pos == -1) ? open_tag_string_length : close_tag_string_length;

		if ( !_memicmp(file_data + file_pos - search_string_length, search_string, search_string_length) )
		{
			if ( open_tag_pos == -1 )
			{
				open_tag_pos			=	file_pos - search_string_length;
				continue;
			}
			else
			{
				close_tag_pos			=	file_pos - search_string_length;
				break;
			}
		}
	}
	ASSERT									(open_tag_pos != -1);
	ASSERT									(close_tag_pos != -1);

	ASSERT									(out_open_tag_pos);
	* out_open_tag_pos					=	open_tag_pos;
	* out_close_tag_pos					=	close_tag_pos;
}

void   get_workspace_path (char (&workspace_path)[PATH_MAX], u32 open_tag_pos, u32 close_tag_pos, pcstr file_data, u32 const file_size)
{
	u32 const workspace_path_length		=	close_tag_pos - open_tag_pos - open_tag_string_length;
	memcpy_s								(workspace_path, 
											 PATH_MAX, 
											 file_data + open_tag_pos + open_tag_string_length, 
											 workspace_path_length);
	workspace_path[workspace_path_length]	=	0;
}

pcstr   get_relative_to_hudson_project_path (char const (& workspace_path) [PATH_MAX])
{
	u32 const workspace_path_length		=	strlen(workspace_path);
	pcstr const hudson_path_string1		=	"hudson/jobs";
	pcstr const hudson_path_string2		=	"hudson\\jobs";
	u32 const hudson_path_string_length	=	strlen(hudson_path_string1);

	for ( u32 string_pos=0; string_pos<workspace_path_length; ++string_pos )
	{
		if ( string_pos < hudson_path_string_length )
			continue;

		pcstr const relative_to_hudson_project_path	=	workspace_path + string_pos;
		pcstr const string_at_pos					=	workspace_path + string_pos - hudson_path_string_length;
		if ( !_memicmp(string_at_pos, hudson_path_string1, hudson_path_string_length) ||
			 !_memicmp(string_at_pos, hudson_path_string2, hudson_path_string_length) )
			return							relative_to_hudson_project_path;
	}

	printf	("%s %s", "warning, project %s uses customWorkspace and workspace path is not at hudson dir - is this intended?");
	exit	(0);
}

void   get_hudson_home (char (& out_hudson_home) [PATH_MAX], pcstr const full_path)
{
	pcstr last_back_slash				=	strrchr(full_path, '\\');
	pcstr last_front_slash				=	strrchr(full_path, '/');

	u32 const length_to_back_slash		=	last_back_slash	 ? u32(last_back_slash - full_path) : 0;
	u32 const length_to_front_slash		=	last_front_slash ? u32(last_front_slash - full_path) : 0;

	u32 const length_to_slash			=	(length_to_back_slash > length_to_front_slash) ? length_to_back_slash : length_to_front_slash;

	memcpy									(out_hudson_home, full_path, length_to_slash);
	out_hudson_home[length_to_slash]	=	0;
	strcat_s								(out_hudson_home, "\\jobs");
}

u32 _tmain(u32 argc, _TCHAR* argv[])
{
	ASSERT									(argc > 1);

	char relative_path_to_config			[PATH_MAX];
	get_project_config_file_path			(relative_path_to_config, argv[1]);

	u32	file_size;
	read_file								(file_data, & file_size, relative_path_to_config);

	u32 open_tag_pos, close_tag_pos;
	search_workspace_path					(& open_tag_pos, & close_tag_pos, file_data, file_size);

	char workspace_path						[PATH_MAX];
	get_workspace_path						(workspace_path, open_tag_pos, close_tag_pos, file_data, file_size);

	pcstr relative_to_hudson_project_path	=	get_relative_to_hudson_project_path	(workspace_path);

	char hudson_home						[PATH_MAX];
	get_hudson_home							(hudson_home, argv[0]);

	
	char backup_config_path					[PATH_MAX];
	strcpy_s								(backup_config_path, relative_path_to_config);
	strcat_s								(backup_config_path, ".backup");
	_unlink									(backup_config_path);
	rename									(relative_path_to_config, backup_config_path);

	FILE * file							=	NULL;
	errno_t const file_open_err			=	fopen_s(& file, relative_path_to_config, "wb");
	ASSERT									(file_open_err == 0);

	fwrite									(file_data, 1, open_tag_pos + open_tag_string_length, file);
	fwrite									(hudson_home, 1, strlen(hudson_home), file);

	pcstr									after_path_part	=	file_data + open_tag_pos + open_tag_string_length + 
																strlen(workspace_path) - strlen(relative_to_hudson_project_path);

	fwrite									(after_path_part, 1, file_data + file_size - after_path_part, file);
	fclose									(file);

	return									0;
}

