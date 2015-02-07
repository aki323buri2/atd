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
	string path, dirname, basename, filename, logfile;
	app()
	{
		path = path::app_path().utf8();
		dirname = path::remove_file_spec(path);
		basename = path::find_file_name(path);
		filename = path::remove_extension(basename);
		logfile = path::rename_extension(path, ".log");
	}
} app;
static void notify(const string &s)
{
	string sjis = s.sjis();
	std::ofstream ofs(app.logfile.sjis().c_str(), std::ios::app);
	ofs << "[" << datetime().strftime() << "] ";
	ofs << sjis << endl;
	cout << sjis << endl;
}
static generic::notifyf notifyf = notify;
#endif//__common_h__