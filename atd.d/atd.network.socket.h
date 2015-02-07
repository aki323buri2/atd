//atd.network.socket.h
#include "atd.h"
#include <winsock2.h>
namespace atd {;
namespace network {;
namespace root {;
//====================================================
//= atd::network::root::socket
//====================================================
struct socket : public object, public sockaddr_in 
{
	SOCKET value;
	string host;
	int port;

	socket();
	void clear();
	void close();
	string ipaddress() const;
	
	static unsigned long nslookup(const string &host);

	virtual socket &connect(const string &host, int port) = 0;
	virtual int send(char *ptr, int size) = 0;
	virtual int recv(char *ptr, int size) = 0;

	void throw_error(const generic::status &error);
	void throw_wsa_error(DWORD code);
};
}//namespace root
//====================================================
//= atd::network::socket
//====================================================
struct socket : public root::socket
{
	socket();
	socket(const network::url &url);
	socket(const string &host, int port);
	socket &connect(const string &host, int port);
	int send(char *ptr, int size);
	int recv(char *ptr, int size);

	string ipaddr() const;
};
namespace ssl {;
//====================================================
//= atd::network::ssl::socket
//====================================================
struct socket : public network::socket
{
	socket();
	socket(const network::url &url, const string &crt = "");
	socket(const string &host, int port, const string &crt = "");
	socket &connect(const string &host, int port, const string &crt = "");
	int send(char *ptr, int size);
	int recv(char *ptr, int size);

	void throw_ssl_error(DWORD code);

	static string verify_result_description(DWORD code);
protected:
	struct sslinfo;
	struct sslinfo *ssli;
	managed::objects garbage;

private:
	socket &operator = (const socket &that);
};
}//namespace ssl
}//namespace network
}//namespace atd