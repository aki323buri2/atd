//atd.network.socket.cpp
#include "atd.h"
using namespace atd;
using namespace atd::network;
#include "openssl/ssl.h"
#include "openssl/err.h"
/*
struct sockaddr_in {
	short	sin_family;
	u_short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};
struct in_addr {
	union {
		struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { u_short s_w1,s_w2; } S_un_w;
		u_long S_addr;
	} S_un;
#define s_addr  S_un.S_addr
#define s_host  S_un.S_un_b.s_b2
#define s_net   S_un.S_un_b.s_b1
#define s_imp   S_un.S_un_w.s_w2
#define s_impno S_un.S_un_b.s_b4
#define s_lh    S_un.S_un_b.s_b3
};
struct  hostent {
	char	*h_name;
	char	**h_aliases;
	short	h_addrtype;
	short	h_length;
	char	**h_addr_list;
#define h_addr h_addr_list[0]
};
*/
//====================================================
//= wsa startup
//====================================================
namespace {;
static struct wsa 
{
	wsa()
	{
		WSADATA data;
		::WSAStartup(MAKEWORD(2, 0), &data);
	}
	~wsa()
	{
		::WSACleanup();
	}
}	wsa;
}//namespace
//====================================================
//= atd::network::root::socket
//====================================================
network::root::socket::socket()
: value(INVALID_SOCKET)
{
	clear();
}

void network::root::socket::clear()
{
	value = INVALID_SOCKET;
	::memset((sockaddr_in *)this, 0, sizeof(sockaddr_in));
	sin_family	= AF_INET;
	int type	= SOCK_STREAM;
	value = ::socket(sin_family, type, 0);
}
void network::root::socket::close()
{
	if (value != INVALID_SOCKET) ::closesocket(value);
	clear();
}
string network::root::socket::ipaddress() const 
{
	return ::inet_ntoa(sin_addr);
}
unsigned long network::root::socket::nslookup(const string &host_)
{
	const char *host = &host_[0];
	unsigned long addr = ::inet_addr(host);
	if (addr == INADDR_NONE)
	{
		HOSTENT *h = ::gethostbyname(host);
		if (h)
		{
			addr = *(unsigned long *)h->h_addr;
		}
	}
	return addr;
}
//====================================================
//= atd::network::socket
//====================================================
socket::socket()
: network::root::socket()
{ }
socket::socket(const network::url &url)
: network::root::socket()
{
	connect(url.host, url.port);
}
socket::socket(const string &host, int port)
: network::root::socket()
{
	connect(host, port);
}
network::socket &socket::connect(const string &host, int port)
{
	this->host = host;
	this->port = port;
	unsigned long &sin_addr = this->sin_addr.s_addr;
	sin_addr = nslookup(host);
	sin_port = ::htons(port);

	if (sin_addr == INADDR_NONE)
	{
		throw generic::exception(
			string::format("host [%s] is unavailable!", &host[0])
		);
	}

	int code = ::connect(value
		, (sockaddr *)((sockaddr_in *)this)
		, sizeof(sockaddr_in)
	);
	if (code != 0)
	{
		throw_wsa_error(::WSAGetLastError());
	}
	return *this;
}
int socket::send(char *ptr, int size)
{
	return ::send(value, ptr, size, 0);
}
int socket::recv(char *ptr, int size)
{
	return ::recv(value, ptr, size, 0);
}

string socket::ipaddr() const
{
	return ::inet_ntoa(sin_addr);
}
//====================================================
//= atd::network::ssl::socket::ssl
//====================================================
struct ssl::socket::sslinfo : public object
{
	SSL_CTX *ctx;
	SSL *ssl;
	string crt;
	sslinfo(const string &crt);
	~sslinfo();
};
ssl::socket::sslinfo::sslinfo(const string &crt)
: ctx(0), ssl(0)
{
	::SSL_load_error_strings();
	::SSL_library_init();
	ctx = ::SSL_CTX_new(::SSLv23_client_method());
	if (crt.length())
	{
		::SSL_CTX_load_verify_locations(ctx, &crt[0], NULL);
		::SSL_CTX_set_verify_depth(ctx, SSL_VERIFY_PEER);
	}
	ssl = ::SSL_new(ctx);
}
ssl::socket::sslinfo::~sslinfo()
{
	::SSL_free(ssl);
	::SSL_CTX_free(ctx);
	::ERR_free_strings();
}
//====================================================
//= atd::network::ssl::socket
//====================================================
ssl::socket::socket()
: network::socket(), ssli(new struct sslinfo("")), garbage(ssli)
{
}
ssl::socket::socket(const network::url &url, const string &crt)
: network::socket(), ssli(new struct sslinfo(crt)), garbage(ssli)
{
	connect(url.host, url.port);
}
ssl::socket::socket(const string &host, int port, const string &crt)
: network::socket(), ssli(new struct sslinfo(crt)), garbage(ssli)
{
	connect(host, port); 
}
ssl::socket &ssl::socket::connect(const string &host, int port, const string &crt)
{

	SSL *& ssl = ssli->ssl;

	::SSL_set_fd(ssl, value);

	//connect
	network::socket::connect(host, port);
	
	//ssl_connect
	int code = ::SSL_connect(ssl);	
	if (code != 1)//★
	{
		throw_ssl_error(::SSL_get_error(ssl, code));
	}

	//cpypher
	struct { string name, ver; } cipher;
	cipher.name = ::SSL_get_cipher(ssl);
	cipher.ver  = ::SSL_get_cipher_version(ssl);
	cout << "cipher.name : " << cipher.name << endl;
	cout << "cipher.ver  : " << cipher.ver  << endl;

	//verify
	struct { long result; } verify;
	verify.result = ::SSL_get_verify_result(ssl);

	cout << verify_result_description(verify.result).sjis() << endl;

	return *this;
}
int ssl::socket::send(char *ptr, int size)
{
	return ::SSL_write(ssli->ssl, ptr, size);
}
int ssl::socket::recv(char *ptr, int size)
{
	return ::SSL_read(ssli->ssl, ptr, size);
}
//====================================================
//= throw_error()
//====================================================
void network::root::socket::throw_error(const generic::status &error)
{
	strings ss;
	ss.push_back(string::format("host : %s", host.c_str()));
	ss.push_back(string::format("port : %d", port));
	ss.push_back(string::format("ipaddress : %s", ipaddress().c_str()));
	ss.push_back(string::format("error.code : %d", error.code));
	ss.push_back(string::format("error.what : %s", error.what.c_str()));
	throw generic::exception(ss.implode("\n"));
}
void network::root::socket::throw_wsa_error(DWORD code)
{
	throw_error(generic::status::lasterror(code));
}
void ssl::socket::throw_ssl_error(DWORD code)
{
	static generic::status::primitive primitive[100] = 
	{
		{
			SSL_ERROR_NONE, 
			"No error to report. This is set when the value of the ret parameter is greater than 0.", 
		}, 
		{
			SSL_ERROR_SSL, 
			"An error occurred in the SSL library.", 
		}, 
		{
			SSL_ERROR_WANT_READ, 
			"Processing was not completed successfully because there was no data available for reading, and the socket available for the SSL session is in nonblocking mode. Try the function again at a later time.", 
		}, 
		{
			SSL_ERROR_WANT_WRITE, 
			"Processing was not completed successfully because the socket associated with the SSL session is blocked from sending data. Try the function again at a later time.", 
		}, 
		{
			SSL_ERROR_SYSCALL, 
			"An I/O error occurred. Issue the sock_errno function to determine the cause of the error.", 
		}, 
		{
			SSL_ERROR_ZERO_RETURN, 
			"The remote application shut down the SSL connection normally. Issue the SSL_shutdown function to shut down data flow for an SSL session.", 
		}, 
		{
			SSL_ERROR_WANT_CONNECT, 
			"Processing was not completed successfully because the SSL session was in the process of starting the session, but it has not completed yet. Try the function again at a later time.", 
		}, 
	};
	static generic::status::book book(primitive);

	throw_error(book.status_of(code));
}
//====================================================
//= ssl verify_result_description
//====================================================
string ssl::socket::verify_result_description(DWORD code)
{
	//>> http://www.geocities.co.jp/SiliconValley-PaloAlto/4182/Openssl_content14.html
	static generic::status::primitive primitive[100] = 
	{
		/*値	   define											 結果*/
		{/*0 */X509_V_OK										, "ハンドシェイク・認証成功", }, 
		{/*2 */X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT				, "issure証明書を取得できなかった", }, 
		{/*3 */X509_V_ERR_UNABLE_TO_GET_CRL						, "証明書のCRLが取得できなかった", }, 
		{/*4 */X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE		, "証明書を復号することが出来なかった", }, 
		{/*5 */X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE		, "CRLファイルを復号することが出来なかった", }, 
		{/*6 */X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY	, "サーバから送られてきた共通鍵を復号できなかった", }, 
		{/*7 */X509_V_ERR_CERT_SIGNATURE_FAILURE				, "証明書の署名は無効である", }, 
		{/*8 */X509_V_ERR_CRL_SIGNATURE_FAILURE					, "CRLファイルの署名は無効の為使わなかった", }, 
		{/*9 */X509_V_ERR_CERT_NOT_YET_VALID					, "証明書の開始期限がまだ来ていない", }, 
		{/*10*/X509_V_ERR_CERT_HAS_EXPIRED						, "証明書の有効期限切れ", }, 
		{/*11*/X509_V_ERR_CRL_NOT_YET_VALID						, "CRLの開始期限がまだ来ていないので使わなかった。", }, 
		{/*12*/X509_V_ERR_CRL_HAS_EXPIRED						, "CRLの有効期限が切れていたので使わなかった。", }, 
		{/*13*/X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD		, "証明書の開始期限フォーマットの不正", }, 
		{/*14*/X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD			, "証明書の終了期限のフォーマットの不正", }, 
		{/*15*/X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD		, "CRLのlastUpdateのフォーマット不正", }, 
		{/*16*/X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD		, "CRLのnextUpdateのフォーマット不正", }, 
		{/*17*/X509_V_ERR_OUT_OF_MEM							, "メモリ不足（これは決して発生してはならない）", }, 
		{/*18*/X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT			, "渡されたルートCA証明書は信頼された証明書リストに存在しない", }, 
		{/*19*/X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN				, "チェーン証明書内の検証は成功したが、ルートCA証明書は信頼されたリスト内に存在しなかった。", }, 
		{/*20*/X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY		, "信頼された機関から発行された証明書ではない", }, 
		{/*21*/X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE		, "チェーン証明書を含んでいるがルートCAを含んでいない為認証できない", }, 
		{/*22*/X509_V_ERR_CERT_CHAIN_TOO_LONG					, "サポート範囲以上の証明書チェーンが送られてきた", }, 
		{/*23*/X509_V_ERR_CERT_REVOKED							, "証明書が無効（ちょっと自信なし...それにこれじゃ意味わかんないし）", }, 
		{/*24*/X509_V_ERR_INVALID_CA							, "無効なCA証明書（ちょっと自信なし...それにこれじゃ意味わかんないし）", }, 
		{/*25*/X509_V_ERR_PATH_LENGTH_EXCEEDED					, "pathの長さが制限を上回った。（これもちょっとなぁ。。。）", }, 
	};
	static generic::status::book book(primitive);

	return string::format("%d : %s", code, book.status_of(code).what.c_str());
}
/*
*/