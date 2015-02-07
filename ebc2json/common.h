// common.h
#ifndef __common_h__
#define __common_h__
#include "atd.h"
using namespace atd;
#include <fstream>
#include <sstream>
#include <set>
#include "fdg.h"
#include "ebcdic.h"
#include "translator.h"
static struct app 
{
	string path, dirname, basename, extension, filename;
	string log;
	app()
	{
		path = path::app_path();
		dirname = path::remove_file_spec(path);
		basename = path::find_file_name(path);
		extension = path::find_extension(basename);
		filename = path::remove_extension(basename);

		log = path::rename_extension(path, ".log");
	}
} app;
void notify(const string &s)
{
	string sjis = s.sjis();
	std::ofstream ofs(app.log.sjis().c_str(), std::ios::app);
	ofs << datetime().strftime() << " -- ";
	ofs << sjis << endl;
	cout << sjis << endl;
}
generic::notifyf notifyf = notify;
int frame(int argc, char **argv);
int run(int argc, char **argv);
//====================================================
//= utility functions
//====================================================
struct util 
{
	static bool exists(const string &path)
	{
		return path::file_exists(path.sjis());
	}
	static void exists_or_die(const string &path)
	{
		if (!exists(path))
		{
			error("file or directory not exists : %s", path.c_str());
		}
	}
	static string find_fdg_or_die(const string &ebc)
	{
		string fdg = fdg::search(ebc);
		if (fdg == "")
		{
			error("no fdg for %s", ebc.double_quote().c_str());
		}
		return fdg;
	}
	static void error(const char *format, ...)
	{
		va_list va;
		va_start(va, format);
		string s(::_vscprintf(format, va) + 1, 0);
		::vsprintf(&s[0], format, va);
		va_end(va);
		throw generic::exception(s.c_str());
	}
};
//====================================================
//= struct namevalue 
//====================================================
struct namevalue : public generic::property::list
{
	static bool isnull(const string &s)
	{
		return generic::property::isnull(s);
	}
	namevalue()
	{ }
	namevalue(const string &json)
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
#endif//__common_h__