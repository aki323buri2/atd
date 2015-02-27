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
#endif//__common_h__