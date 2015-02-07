//atd.network.url.h
#include "atd.h"
namespace atd {;
namespace network {;
//====================================================
//= atd::network::url
//====================================================
struct url : public object
{
	string scheme	;
	string host		;
	string user		;
	string pass		;
	string path		;
	string extra	;

	int port;

	url(const string &url);
	url &operator = (const string &that);
};
}//namespace network
}//namespace atd