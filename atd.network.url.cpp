//atd.network.url.cpp
#include "atd.h"
using namespace atd;
using namespace atd::network;

#include <wininet.h>
/*
typedef struct {
	DWORD dwStructSize;
	LPWSTR lpszScheme;
	DWORD dwSchemeLength;
	INTERNET_SCHEME nScheme;
	LPWSTR lpszHostName;
	DWORD dwHostNameLength;
	INTERNET_PORT nPort;
	LPWSTR lpszUserName;
	DWORD dwUserNameLength;
	LPWSTR lpszPassword;
	DWORD dwPasswordLength;
	LPWSTR lpszUrlPath;
	DWORD dwUrlPathLength;
	LPWSTR lpszExtraInfo;
	DWORD dwExtraInfoLength;
} URL_COMPONENTSW,*LPURL_COMPONENTSW;
*/
//====================================================
//= atd::network::url
//====================================================
url::url(const string &url)
: port(0)
{
	operator =(url);
}
url &url::operator = (const string &that)
{
	const string &url = that;

	URL_COMPONENTS uc = {0};
	uc.dwStructSize = sizeof(uc);
	
	//バッファ用意
	struct
	{
		string scheme	;
		string host		;
		string user		;
		string pass		;
		string path		;
		string extra	;
	}	buffer = 
	{
		string(0x100, 0), //scheme
		string(0x100, 0), //host
		string(0x100, 0), //user
		string(0x100, 0), //pass
		string(0x400, 0), //path
		string(0x100, 0), //extra
	};
	//バッファのポインタを構造体にセット
	uc.lpszScheme = &buffer.scheme[0];
	uc.lpszHostName = &buffer.host[0];
	uc.lpszUserName = &buffer.user[0];
	uc.lpszPassword = &buffer.pass[0];
	uc.lpszUrlPath = &buffer.path[0];
	uc.lpszExtraInfo = &buffer.extra[0];
	//バッファのサイズを構造体にセット
	uc.dwSchemeLength = buffer.scheme.size();
	uc.dwHostNameLength = buffer.host.size();
	uc.dwUserNameLength = buffer.user.size();
	uc.dwPasswordLength = buffer.pass.size();
	uc.dwUrlPathLength = buffer.path.size();
	uc.dwExtraInfoLength = buffer.extra.size();

	::InternetCrackUrl(&url[0], 0, 0, &uc);

	port = uc.nPort;
	//バッファから代入
	scheme	= buffer.scheme.c_str();
	host	= buffer.host.c_str();
	user	= buffer.user.c_str();
	pass	= buffer.pass.c_str();
	path	= buffer.path.c_str();
	extra	= buffer.extra.c_str();

	return *this;
}