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
	string path, basename, filename, logpath;
	app()
	{
		path = path::app_path().utf8();
		basename = path::basename(path);
		filename = path::filename(path);
		logpath = path::rename_extension(path, ".log");
	}
} app;
static string systime()
{
	return datetime().strftime();
}
static void notify(const string &s)
{
	string sjis = s.sjis();
	std::ofstream ofs(app.logpath.sjis().c_str(), std::ios::app);
	ofs << "[" << systime() << "] ";
	ofs << sjis << endl;
	cout << sjis << endl;
}
static generic::notifyf notifyf = notify;
#endif//__common_h__