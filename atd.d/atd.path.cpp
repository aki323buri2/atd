//atd.path.cpp
#include "atd.h"
using namespace atd;
#include <shlwapi.h>
//====================================================
//= atd::path
//====================================================
string path::app_path()
{
	string s(MAX_PATH + 1, 0);
	::GetModuleFileName(NULL, &s[0], s.size());
	return s.c_str();
}
string path::app_path(const string &relative)
{
	return canonicalize(combine(remove_file_spec(app_path()), relative));
}
string path::dirname(const string &path)
{
	return remove_file_spec(path);
}
string path::basename(const string &path)
{
	return find_file_name(path);
}
string path::filename(const string &path)
{
	return remove_extension(basename(path));
}
int64 path::filesize(const string &path)
{
	struct file
	{
		HANDLE handle;
		file(const string &path)
		: handle(0)
		{
			handle = ::CreateFile(path.c_str()
				, GENERIC_READ
				, FILE_SHARE_READ
				, NULL
				, OPEN_EXISTING
				, FILE_ATTRIBUTE_NORMAL
				, NULL
			);
		}
		~file()
		{
			if (handle != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(handle);
			}
		}
	};
	LARGE_INTEGER size = {0};
	::GetFileSizeEx(file(path).handle, &size);
	return size.QuadPart;
}
string path::add_backslash(const string &path)
{
	string s(MAX_PATH + 1, 0); 
	::PathAddBackslash(&s[0]);
	return s.c_str();
}
string path::add_extension(const string &path, const string &extension)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathAddExtension(&s[0], &extension[0]);
	return s.c_str();
}
string path::append(const string &path, const string &more)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathAppend(&s[0], &more[0]);
	return s.c_str();
}
string path::build_root(char drive)
{
	string s(MAX_PATH + 1, 0);
	::PathBuildRoot(&s[0], drive - 'a');
	return s.c_str();
}
string path::canonicalize(const string &path)
{
	string s(MAX_PATH + 1, 0);
	::PathCanonicalize(&s[0], &path[0]);
	return s.c_str();
}
string path::combine(const string &path, const string &more)
{
	string s(MAX_PATH + 1, 0);
	::PathCombine(&s[0], &path[0], &more[0]);
	return s.c_str();
}
string path::common_prefix(const string &path1, const string &path2)
{
	string s(MAX_PATH + 1, 0);
	::PathCommonPrefix(&path1[0], &path2[0], &s[0]);
	return s.c_str();
}
string path::compact_path(const string &path, HDC hdc, UINT dx)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathCompactPath(hdc, &s[0], dx);
	return s.c_str();
}
string path::compact_path_ex(const string &path, UINT max)
{
	string s(MAX_PATH + 1, 0);
	::PathCompactPathEx(&s[0], &path[0], max, 0);
	return s.c_str();
}
string path::create_from_url(const string &url)
{
	string s(MAX_PATH + 1, 0);
	DWORD done = s.size();
	::PathCreateFromUrl(&url[0], &s[0], &done, 0);
	return s.c_str();
}
bool path::file_exists(const string &path)
{
	return ::PathFileExists(&path[0]) != 0;
}
string path::find_extension		(const string &path) { char *r = PathFindExtension		(&path[0]); return r ? r : ""; }
string path::find_file_name		(const string &path) { char *r = PathFindFileName		(&path[0]); return r ? r : ""; }
string path::find_next_component(const string &path)
{
	wchar_t *r = PathFindNextComponent(path.mb2wc(CP_ACP).c_str());
	wstring wc = r ? r : L"";
	return wc.wc2mb(CP_ACP);
}
string path::find_on_path(const string &path, const char **otherdirs)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathFindOnPath(&s[0], otherdirs);
	return s.c_str();
}
string path::find_suffix_array(const string &path, const char **suffixes)
{
	std::vector<const char *> ss;
	for (const char **s = suffixes; *s; s++)
	{
		ss.push_back(*s);
	}
	return find_suffix_array(path, &ss[0]);
}
string path::find_on_path(const string &path, const strings &otherdirs)
{
	const strings &ss = otherdirs;
	std::vector<const char *> a;
	for (strings::const_iterator i = ss.begin(), e = ss.end(); i != e; ++i)
	{
		a.push_back(i->c_str());
	}
	a.push_back('\0');
	return find_on_path(path, &a[0]);
}
string path::find_suffix_array(const string &path, const strings &suffixes)
{
	const strings &ss = suffixes;
	std::vector<const char *> a;
	for (strings::const_iterator i = ss.begin(), e = ss.end(); i != e; ++i)
	{
		a.push_back(i->c_str());
	}
	const char *r = PathFindSuffixArray(&path[0], &a[0], a.size()); 
	return r ? r : "";
}
string path::get_args(const string &path)
{
	return ::PathGetArgs(&path[0]); 
}
UINT path::get_char_type(uchar ch)
{
	return ::PathGetCharType(ch);
}
char path::get_drive_number(const string &path)
{
	int r = ::PathGetDriveNumber(&path[0]);
	return r < 0 ? 0 : (r + 'a');
}
bool path::is_content_type(const string &path, const string &constent_type)
{
	return ::PathIsContentType(&path[0], &constent_type[0]);
}
bool path::is_directory			(const string &path) { return ::PathIsDirectory		(&path[0]) != 0; }
bool path::is_directory_empty	(const string &path) { return ::PathIsDirectoryEmpty(&path[0]) != 0; }
bool path::is_file_spec			(const string &path) { return ::PathIsFileSpec		(&path[0]) != 0; }
bool path::is_LFN_file_spec		(const string &path) { return ::PathIsLFNFileSpec	(&path[0]) != 0; }
bool path::is_network_path		(const string &path) { return ::PathIsNetworkPath	(&path[0]) != 0; }

bool path::is_prefix			(const string &path, const string &prefix)	 { return ::PathIsPrefix		(&prefix[0], &path[0] 	) != 0; }
bool path::is_relative			(const string &path)						 { return ::PathIsRelative		(&path[0]				) != 0; }
bool path::is_root				(const string &path)						 { return ::PathIsRoot			(&path[0]				) != 0; }
bool path::is_same_root			(const string &path1, const string &path2)	 { return ::PathIsSameRoot		(&path1[0], &path2[0]	) != 0; }
bool path::is_system_folder		(const string &path)						 { return ::PathIsSystemFolder	(&path[0], 0			) != 0; }
bool path::is_UNC				(const string &path)						 { return ::PathIsUNC			(&path[0]				) != 0; }
bool path::is_UNC_server		(const string &path)						 { return ::PathIsUNCServer		(&path[0]				) != 0; }
bool path::is_UNC_server_share	(const string &path)						 { return ::PathIsUNCServerShare(&path[0]				) != 0; }
bool path::is_URL				(const string &path)						 { return ::PathIsURL			(&path[0]				) != 0; }
bool path::match_spec			(const string &path, const string &spec)	 { return ::PathMatchSpec		(&path[0], &spec[0]		) != 0; }
string path::make_pretty(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathMakePretty(&s[0]);
	return s.c_str();
}
bool path::make_system_folder(const string &path)
{
	string s =  path;
	return ::PathMakeSystemFolder(&s[0]) != 0;
}

string path::parse_icon_location(const string &path, int &index)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	index = ::PathParseIconLocation(&s[0]);
	return s.c_str();
}
string path::quote_spaces(const string &path)
{
	string s = path;
	::PathQuoteSpaces(&s[0]);
	return s.c_str();
}
string path::relative_path_to(const string &from, const string &to)
{
	struct 
	{
		DWORD from, to;
	} attr = 
	{
		(DWORD)(is_directory(from) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL), 
		(DWORD)(is_directory(to  ) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL), 
	};
	string s(MAX_PATH + 1, 0);
	::PathRelativePathTo(&s[0], &from[0], attr.from, &to[0], attr.to);
	return s.c_str();
}
string path::remove_args(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathRemoveArgs(&s[0]);
	return s.c_str();
}
string path::remove_backslash	(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathRemoveBackslash(&s[0]);
	return s.c_str();
}
string path::remove_blanks		(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathRemoveBlanks(&s[0]);
	return s.c_str();
}
string path::remove_extension	(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathRemoveExtension(&s[0]);
	return s.c_str();
}
string path::remove_file_spec	(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathRemoveFileSpec(&s[0]);
	return s.c_str();
}
string path::rename_extension(const string &path, const string &extension)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathRenameExtension(&s[0], &extension[0]);
	return s.c_str();
}
string path::search_and_qualify(const string &path)
{
	string s(MAX_PATH + 1, 0);
	PathSearchAndQualify(&path[0], &s[0], s.size());
	return s.c_str();
}
string path::set_dlg_item_path(const string &path, HWND hwnd, int id)
{
	::PathSetDlgItemPath(hwnd, id, &path[0]); 
	string s(MAX_PATH + 1, 0);
	::SendMessage(::GetDlgItem(hwnd, id), WM_GETTEXT, 0, (WPARAM)&s[0]);
	return s.c_str();
}
string path::skip_root(const string &path)
{
	char *r = ::PathSkipRoot(&path[0]);
	return r ? r : "";
}
string path::strip_path(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathStripPath(&s[0]);
	return s.c_str();
}
string path::strip_to_root(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathStripToRoot(&s[0]);
	return s.c_str();
}
string path::undecorate(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathUndecorate(&s[0]);
	return s.c_str();
}
string path::un_expand_env_strings(const string &path)
{
	string s(MAX_PATH + 1, 0);
	::PathUnExpandEnvStrings(&path[0], &s[0], s.size());
	return s.c_str();
}
bool path::unmake_system_folder(const string &path)
{
	string s = path;
	return ::PathUnmakeSystemFolder(&s[0]) != 0;
}
string path::unquote_spaces(const string &path)
{
	string s = path;
	s.resize(MAX_PATH + 1, 0);
	::PathUnquoteSpaces(&s[0]);
	return s.c_str();
}

#include <shlobj.h>
//====================================================
//= path::make_sure_directory()
//====================================================
string path::make_sure_directory(const string &path)
{
	::SHCreateDirectoryEx(NULL, path.c_str(), NULL);
	return path;
}