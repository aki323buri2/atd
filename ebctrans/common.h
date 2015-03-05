//common.h
#ifndef __common_h__
#define __common_h__
#include "atd.h"
using namespace atd;
#include <fstream>
int main(int argc, char **argv);
int frame(int argc, char **argv);
int run(int argc, char **argv);
static struct app
{
	string path, dirname, basename, filename, log;
	app()
	{
		path = path::app_path().utf8();
		dirname = path::dirname(path);
		basename = path::basename(path);
		filename = path::filename(path);
		log = path::rename_extension(path, ".log");
	}
	static string now()
	{
		return datetime().strftime();
	}
	static void sleep(DWORD milliseconds)
	{
		::Sleep(milliseconds);
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

struct properties : public generic::property::list
{
	properties()
	{
	}
	properties(const string &json)
	{
		json_decode(json);
	}
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
//=　threading
//====================================================
#include <process.h>
struct threading
{
	struct object;
	struct event;
	struct thread;
};
//====================================================
//=　threading::object
//====================================================
struct threading::object : public atd::object
{
	HANDLE handle;
	object(HANDLE handle = 0);
	DWORD wait(DWORD milliseconds = INFINITE);
};
//====================================================
//=　threading::event
//====================================================
struct threading::event : public threading::object
{
	event();
	void set();
	void reset();
};
//====================================================
//=　threading::thread
//====================================================
struct threading::thread : public threading::object
{
	typedef generic::function<void, void> action;
	action callback;
	unsigned int id;

	thread(const action &callback);
	void start();
	unsigned __stdcall kernel();
	void suspend();
	void resume();
	bool running() const;
};

//====================================================
//= navigater container class
//====================================================
#include "fdg.h"
struct navitem : public object
{
	string name;
	string path;
	fdg::navigater nav;
	int count;
	std::ifstream *ifs;
	std::ofstream *ofs;
	threading::thread *thread;
	threading::event *event;

	navitem(
		  const string &path = ""
	)
	: name(path::filename(path.sjis()).utf8())
	, path(path)
	, count(0)
	, ifs(0)
	, ofs(0)
	, thread(0)
	, event(0)
	{
		//FDGロード
		fdg::fields fd;
		fd << path;
		nav << fd;
	}
	void start()
	{
		thread = new threading::thread(
			threading::thread::action(&navitem::action, this)
		);
		thread->start();
	}
	DWORD wait(DWORD milliseconds)
	{
		return thread->wait(milliseconds);
	}
	bool running() const 
	{
		return thread->running();
	}
	void action();//★

};

struct navmap : public std::map<uchar, navitem>
{
	navitem &navof(uchar key)
	{
		iterator i = find(key);
		if (i == end())
		{
			insert(value_type(key, navitem()));
			i = find(key);
		}
		return i->second;
	}
	void addnav(uchar key, const string &fdg)
	{
		insert(value_type(key, navitem(fdg)));
	}
	~navmap()
	{
		cleanup();
	}
	void filecloseall()
	{
		for (iterator i = begin(), e = end(); i != e; ++i)
		{
			std::ofstream *&ofs = i->second.ofs;
			ofs->close();
			//再利用のため破棄＆初期化
			delete ofs;
			ofs = 0;
		}
	}
	int running() const
	{
		int running = 0;
		for (const_iterator i = begin(), e = end(); i != e; ++i)
		{
			const navitem &item = i->second;
			if (item.running()) running++;
		}
		return running;
	}
	bool rsizecheck() const
	{

		int rsize = 0;
		for (const_iterator i = begin(), e = end(); i != e; ++i)
		{
			int newvalue = i->second.nav.rsize;
			if (!rsize)
			{
				rsize = newvalue;
			}
			if (!rsize) return false;
			if (rsize != newvalue) return false;
		}
		return true;
	}

	void cleanup();//★
	void invoke();//★

};

//====================================================
//=　threading classes implementations
//====================================================
inline threading::object::object(HANDLE handle)
: handle(handle)
{ }
inline DWORD threading::object::wait(DWORD milliseconds)
{
	return ::WaitForSingleObject(handle, milliseconds);
}
inline threading::event::event()
: threading::object(::CreateEvent(NULL, FALSE, FALSE, NULL))
{ }
inline void threading::event::set()
{
	::SetEvent(handle);
}
inline void threading::event::reset()
{
	::ResetEvent(handle);
}
inline threading::thread::thread(const action &callback)
: callback(callback), id(0)
{ }
inline void threading::thread::start()
{
	struct call
	{
		static unsigned __stdcall back(void *data)
		{
			return ((thread *)data)->kernel();
		}
	};
	handle = (HANDLE)::_beginthreadex(NULL, 0, call::back, (void *)this, 0, &id);
}
inline unsigned __stdcall threading::thread::kernel()
{
	callback();
	::CloseHandle(handle);
	handle = 0;
	return 0;
}
inline void threading::thread::suspend()
{ 
	::SuspendThread(handle);
}
inline void threading::thread::resume()
{
	::ResumeThread(handle);
}
bool threading::thread::running() const 
{
	return handle != 0;
}
#endif//__common_h__