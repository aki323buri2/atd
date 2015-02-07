// atd.shell.cpp
#include "atd.h"
using namespace atd;
//====================================================
//= struct atd::shell
//====================================================
const char *shell::DEFAULT_RESULT_PREFIX = "<<<result>>>";
strings shell::execute(const string &command
	, const generic::notify &notify
	, const string &currentdirectory
	, const string &prefix
	)
{
	strings results;
	generic::notifyf notifyf = notify;


	struct handle
	{
		HANDLE h;
		handle(HANDLE h = 0) : h(h) { }
		~handle() { close(); }
		void close() { if (h) ::CloseHandle(h); h = 0; }
		operator HANDLE() const { return h; }
		HANDLE *operator &() { return &h; }
		handle &operator = (HANDLE that) { h = that; return *this; }
	};
	struct { struct handle read, write; } pipe;
	struct { struct handle handle; } process;

	//================================================
	//= create pipe
	//================================================
	SECURITY_ATTRIBUTES sa = {0};
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	if (!::CreatePipe(&pipe.read, &pipe.write, &sa, 0))
	{
		generic::status::lasterror().display(notify);
		error("create pipe error");
	}
	//================================================
	//= 0create process
	//================================================
	//カレンドディレクトリ
	const char *cur = currentdirectory.length() ? currentdirectory.c_str() : NULL;
	STARTUPINFO si = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput	= ::GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput	= pipe.write;//★
//	si.hStdOutput	= ::GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError	= ::GetStdHandle(STD_ERROR_HANDLE);
	PROCESS_INFORMATION pi = {0};
	if (!::CreateProcess(
		  NULL					//LPCTSTR					// 実行可能モジュールの名前
		, (char *)&command[0]	//LPTSTR					// コマンドラインの文字列
		, NULL					//LPSECURITY_ATTRIBUTES		// セキュリティ記述子
		, NULL					//LPSECURITY_ATTRIBUTES		// セキュリティ記述子
		, TRUE					//BOOL						// ハンドルの継承オプション
		, 0						//DWORD						// 作成のフラグ
		, NULL					//LPVOID					// 新しい環境ブロック
		, cur					//LPCTSTR					// カレントディレクトリの名前
		, &si					//LPSTARTUPINFO				// スタートアップ情報
		, &pi					//LPPROCESS_INFORMATION		// プロセス情報
		))
	{
		generic::status::lasterror().display(notify);
		error("create pipe error");
	}
	//================================================
	//= process the process
	//================================================
	process.handle = pi.hProcess;
	while (true)
	{
		//wait for 
		DWORD wait = ::WaitForSingleObject(process.handle, 50);
		bool timeout  = wait == WAIT_TIMEOUT;
		bool complete = wait == WAIT_OBJECT_0;
		if (!timeout && !complete)
		{
			notifyf("illegal wait result : 0x%x", wait);
			break;
		}
		//peek pipe
		while (true)
		{
			DWORD read = 0;
			::PeekNamedPipe(pipe.read, NULL, 0, NULL, &read, NULL);
			if (read > 0)
			{
				string s(read, 0);
				::ReadFile(pipe.read, &s[0], s.size(), &read, NULL);
				s = string::utf8(s.c_str());
				strings ss = s.explode(CRLF);
				ss.erase(ss.rbegin().base());//末尾のCRLFを取り除く
				for (strings::const_iterator i = ss.begin(), e = ss.end()
					; i != e; ++i)
				{
					const string &s = *i;
					//callback
					notify(s);
					//result prefix check
					int len = prefix.length();
					if (!len) continue;
					if (s.substr(0, len) == prefix)
					{
						results.entry(s.substr(len));
					}
				}
			}
			else
			{
				break;
			}
		}
		if (timeout ) continue;
		if (complete) break;
	}
	//================================================

	return results;
}
//====================================================
//= shell::error()
//====================================================
void shell::error(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	string s(::_vscprintf(format, va) + 1, 0);
	::vsprintf(&s[0], format, va);
	va_end(va);
	throw generic::exception(s.c_str());
}
//====================================================