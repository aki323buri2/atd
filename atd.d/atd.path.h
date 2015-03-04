//atd.path.h
#ifndef __ATD_PATH_H__
#define __ATD_PATH_H__
#include "atd.h"
namespace atd {;
//====================================================
//= atd::path
//====================================================
struct path : public object 
{
	static string app_path();
	static string app_path(const string &relative);

	static string dirname(const string &path);
	static string basename(const string &path);
	static string filename(const string &path);

	static string add_backslash			(const string &path);
	static string add_extension			(const string &path, const string &extension);
	static string append				(const string &path, const string &more);
	static string build_root			(char drive);
	static string canonicalize			(const string &path);
	static string combine				(const string &path, const string &more);
	static string common_prefix			(const string &path1, const string &path2);
	static string compact_path			(const string &path, HDC hdc, UINT dx);
	static string compact_path_ex		(const string &path, UINT max);
	static string create_from_url		(const string &url);
	static bool   file_exists			(const string &path);
	static string find_extension		(const string &path);
	static string find_file_name		(const string &path);
	static string find_next_component	(const string &path);
	static string find_on_path			(const string &path, const char **otherdirs);
	static string find_suffix_array		(const string &path, const char **suffixes);
	static string find_on_path			(const string &path, const strings &otherdirs);
	static string find_suffix_array		(const string &path, const strings &suffixes);
	static string get_args				(const string &path);
	static UINT   get_char_type			(uchar ch);
	/*
	Return code		Description
	========================================================
	GCT_INVALID		The character is not valid in a path.
	GCT_LFNCHAR		The character is valid in a long file name.
	GCT_SEPARATOR	The character is a path separator.
	GCT_SHORTCHAR	The character is valid in a short (8.3) file name.
	GCT_WILD		The character is a wildcard character.
	*/
	static char   get_drive_number		(const string &path);
	static bool   is_content_type		(const string &path, const string &constent_type);
	static bool   is_directory			(const string &path);
	static bool   is_directory_empty	(const string &path);
	static bool   is_file_spec			(const string &path);
	static bool   is_LFN_file_spec		(const string &path);
	static bool   is_network_path		(const string &path);
	
	static bool   is_prefix				(const string &path, const string &prefix);	
	static bool   is_relative			(const string &path);						
	static bool   is_root				(const string &path);						
	static bool   is_same_root			(const string &path1, const string &path2);	
	static bool   is_system_folder		(const string &path);						
	static bool   is_UNC				(const string &path);						
	static bool   is_UNC_server			(const string &path);						
	static bool   is_UNC_server_share	(const string &path);						
	static bool   is_URL				(const string &path);						
	static string make_pretty			(const string &path);						
	static bool   make_system_folder	(const string &path);						
	static bool   match_spec			(const string &path, const string &spec);	
	static string parse_icon_location	(const string &path, int &index);
	static string quote_spaces			(const string &path);
	static string relative_path_to		(const string &from, const string &to);
	static string remove_args			(const string &path);
	static string remove_backslash		(const string &path);
	static string remove_blanks			(const string &path);
	static string remove_extension		(const string &path);
	static string remove_file_spec		(const string &path);
	static string rename_extension		(const string &path, const string &extension);
	static string search_and_qualify	(const string &path);
	static string set_dlg_item_path		(const string &path, HWND hwnd, int id);
	static string skip_root				(const string &path);
	static string strip_path			(const string &path);
	static string strip_to_root			(const string &path);
	static string undecorate			(const string &path);
	static string un_expand_env_strings	(const string &path);
	static bool   unmake_system_folder	(const string &path);
	static string unquote_spaces		(const string &path);

	//================================================
	//= path::make_sure_directory()
	//================================================
	static string make_sure_directory(const string &path);
};
}//namespace atd
#endif//__ATD_PATH_H__