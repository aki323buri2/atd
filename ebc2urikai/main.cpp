//main.cpp
#include "common.h"
int main(int argc, char **argv)
{
	int r = 0;
	try 
	{
		r = frame(argc, argv);
	}
	catch (std::exception &e)
	{
		notifyf("!!!! %s error (%s) : %s"
			, app.basename.c_str()
			, app::now().c_str()
			, e.what()
		);
	}
	return r;
}
int frame(int argc, char **argv)
{
	notifyf(">>>> %s start (%s) >>>>", app.filename.c_str(), app::now().c_str());
	int r = run(argc, argv);
	notifyf(">>>> %s e n d (%s) >>>>", app.filename.c_str(), app::now().c_str());
	notify ("");
	return r;
}
int run(int argc, char **argv)
{
	properties args;
	struct { string &path; } ebc = 
	{
		args.value_of("ebc.path"), 
	};
	struct { string &path1, &path2, &path3; } fdg = 
	{
		args.value_of("fdg.path1"), 
		args.value_of("fdg.path2"), 
		args.value_of("fdg.path3"), 
	};
	ebc.path  = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D1.D0209";
	fdg.path1 = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\データ退避ＣＯＰＹメンバ\\TUF010.TXT";
	fdg.path2 = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\データ退避ＣＯＰＹメンバ\\TUF020.TXT";
	fdg.path3 = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\データ退避ＣＯＰＹメンバ\\TUF090.TXT";

	args.commandline(argc, argv);
	args.demo(notify);

	bool check = true;
	{
		string *ss[] = 
		{
			&ebc.path, 
			&fdg.path1, 
			&fdg.path2, 
			&fdg.path3, 
			0, 
		};
		for (string **pp = ss; *pp; pp++)
		{
			string &path = **pp;

			path = path::app_path(path);

			if (!path::file_exists(path.sjis()))
			{
				notifyf("!!!! %s is not exists !!!"
					, path::basename(path.sjis()).utf8().c_str()
				);
				check = false;
			}
		}
	}
	if (!check) throw generic::exception("args file exists check error !!");

	return 0;
}