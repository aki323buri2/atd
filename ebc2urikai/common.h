//common.h
#ifndef __common_h__
#define __common_h__
#include "atd.h"
#include <fstream>
using namespace atd;
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
//= 
//====================================================
//====================================================
#endif//__common_h__