//common.h
#ifndef __common_h__
#define __common_h__
#include "atd.h"
#include <fstream>
using namespace atd;
#include "ebcdic.h"
#include "fdg.h"
#include "translator.h"
int main(int argc, char **argv);
int frame(int argc, char **argv);
int run(int argc, char **argv);
static struct app
{
	string path, dirname, basename, filename, log;
	app()
	{
		path = path::app_path().utf8();
		dirname = path::dirname(path.sjis()).utf8();
		basename = path::basename(path.sjis()).utf8();
		filename = path::filename(path.sjis()).utf8();
		log = path::rename_extension(path.sjis(), ".log").utf8();
	}
	static string now()
	{
		return datetime().strftime();
	}
	static void sleep(DWORD milliseconds)
	{
		return ::Sleep(milliseconds);
	}
} app;
static void notify(const string &s)
{
	string sjis = s.sjis();
	std::ofstream ofs(app.log.sjis().c_str(), std::ios::app);
	ofs << "[" << app::now() << "] ";
	ofs << sjis << endl;
	cout << sjis << endl;
}
static generic::notifyf notifyf = notify;
//====================================================
//= properties
//====================================================
struct properties : public generic::property::list
{
	void commandline(int argc, char **argv)
	{
		iterator i = begin(), e = end();
		for (int c = 1; c < argc && i != e; ++c, ++i)
		{
			i->value = *(argv + c);
		}
	}
};
//====================================================
//= file exists check
//====================================================
bool abspath_and_existscheck(string **files)
{
	bool check = true;
	for (string **pp = files; *pp; pp++)
	{
		string &path = **pp;
		path = path::app_path(path);
		if (!path::file_exists(path.sjis()))
		{
			notifyf("!!! %s is not exists !!!"
				, path::basename(path.sjis()).utf8().c_str()
			);
			check = false;
		}
	}
	return check;
}
//====================================================
//= threding
//====================================================
#include <process.h>
struct threading
{
	struct object;
	struct thread;
};
struct threading::object : public atd::object
{
	HANDLE handle;
	object(HANDLE handle = 0) : handle(handle)
	{
	}
	~object()
	{
		close();
	}
	void close()
	{
		if (handle) ::CloseHandle(handle);
		handle = 0;
	}
	DWORD wait(DWORD milliseconds)
	{
		return ::WaitForSingleObject(handle, milliseconds);
	}
};
struct threading::thread : public threading::object
{
	virtual void run() = 0;
	unsigned int id;
	thread() : id(0)
	{
	}
	void start()
	{
		struct the
		{
			static unsigned __stdcall cb(void *data)
			{
				return ((thread *)data)->kernel();
			}
		};
		handle = (HANDLE)::_beginthreadex(NULL, 0, the::cb, (void *)this, 0, &id);
	}
	unsigned __stdcall kernel()
	{
		run();
		close();
		return 0;
	}
	bool running() const 
	{
		return handle != 0;
	}
};
//====================================================
//= struct invoker
//====================================================
struct invoker : public threading::thread
{
	string fdg;
	string name;
	string ebc;
	string json;
	fdg::navigater navigater;
	std::ifstream ifs;
	std::ofstream ofs;
	uchar judge;
	struct { int64 total, done; } lines;

	invoker(const string &fdg)
	: fdg(fdg)
	, judge(0)
	{
		lines.total = 0;
		lines.done  = 0;
		name = path::filename(fdg.sjis()).utf8();

		fdg::fields fields;
		fields << fdg;
		navigater << fields;
	}
	void run();//★
};
struct invokers : public std::vector<invoker *>
{
	~invokers()
	{
		clear();
	}
	void entry(invoker *invoker)
	{
		push_back(invoker);
	}
	void start()
	{
		for (iterator i = begin(), e = end(); i != e; ++i)
		{
			(*i)->start();
		}
	}
	void wait()
	{
		while (true)
		{
			bool done = true;
			for (iterator i = begin(), e = end(); i != e; ++i)
			{
				if ((*i)->running()) 
				{
					done = false;
					break;
				}
			}
			if (done) break;
		}
	}
	void clear()
	{
		for (iterator i = begin(); i != end(); )
		{
			delete *i;
			i = erase(i);
		}
	}
	//レコード長の不揃いをチェック
	bool even() const
	{
		//レコード長が揃っているか？
		struct { int rsize; } memo = {0};
		for (const_iterator i = begin(), e = end(); i != e; ++i)
		{
			int rsize = (*i)->navigater.rsize;
			if (memo.rsize == 0) 
			{
				memo.rsize = rsize;
			}
			else if (memo.rsize != rsize)
			{
				return false;
			}
		}
		return true;
	}

	iterator invoker_of(size_t offset)
	{
		return BETWEEN(0, offset, size() - 1) ? begin() + offset : end();
	}
	//ファイルクローズ
	void fclose()
	{
		for (iterator i = begin(), e = end(); i != e; ++i)
		{
			(*i)->ifs.close();
			(*i)->ofs.close();
		}
	}
	void ifclose()
	{
		for (iterator i = begin(), e = end(); i != e; ++i)
		{
			(*i)->ifs.close();
		}
	}
	void ofclose()
	{
		for (iterator i = begin(), e = end(); i != e; ++i)
		{
			(*i)->ofs.close();
		}
	}
};
//====================================================
//= 並列処理進捗通知ボード
//====================================================
struct board 
{
	struct page;
	struct list;
};
struct board::page : public object
{
	string name;
	int percent;
	page() : percent(0)
	{
	}
};
struct board::list : public std::map<uchar, board::page>
{
	void init(const invokers &invokers)
	{
		clear();
		for (invokers::const_iterator i = invokers.begin(), e = invokers.end()
			; i != e; ++i)
		{
			invoker &invoker = **i;

			insert(value_type(invoker.judge, page()));
			page &page = find(invoker.judge)->second;
			page.name = invoker.name;
		}
	}
	string status() const 
	{
		strings ss;
		for (const_iterator i = begin(), e = end(); i != e; ++i)
		{
			const page &page = i->second;
			const string &name = page.name;
			int percent = page.percent;
			string s = string::format("%-10s : %3d%%"
				, name.c_str()
				, percent
			);
			ss.entry(s);
		}
		return ss.implode(" / ");
	}
	void update() const
	{
		static struct mutex
		{
			CRITICAL_SECTION cs;
			mutex()
			{
				::InitializeCriticalSection(&cs);
			}
			~mutex()
			{
				::DeleteCriticalSection(&cs);
			}
			void enter()
			{
				::EnterCriticalSection(&cs);
			}
			void leave()
			{
				::LeaveCriticalSection(&cs);
			}
		} mutex;
		struct lock
		{
			struct mutex &mutex;
			lock(struct mutex &mutex) : mutex(mutex)
			{
				mutex.enter();
			}
			~lock()
			{
				mutex.leave();
			}
		} lock(mutex);
		cout << "\r";
		cout << status();
	}
};
static board::list boards;
#endif//__common_h__