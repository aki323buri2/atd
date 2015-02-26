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
	notify("");
	return r;
}

#include "fdg.h"
int run(int argc, char **argv)
{
	properties args;
	struct { string &path; } ebc = { args.value_of("ebc.path") };
	struct { string &path; } fdg = { args.value_of("fdg.path") };
	properties backup;
	backup["ebc.path"] = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D1.D0209";
	backup["ebc.path"] = "\\\\pc3000002\\data2\\債権債務\\GS債権債務データ移行\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D1.D0209";
	strings fdgs(
		  "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\データ退避ＣＯＰＹメンバ\\TUF010.TXT"
		, "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\データ退避ＣＯＰＹメンバ\\TUF020.TXT"
		, "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\データ退避ＣＯＰＹメンバ\\TUF090.TXT"
	);
	backup["fdg.path"] = fdgs.implode(";");

	args.supply(backup);
	args.supply(properties(path::rename_extension(app.path, ".back.json")));
	args.commandline(argc, argv);

	fdgs.clear();
	fdgs = fdg.path.explode(";");

	std::vector<fdg::navigater> navs;
	for (strings::iterator i = fdgs.begin(), e = fdgs.end()
		; i != e; ++i)
	{
		fdg.path = *i;
		fdg.path = path::app_path(fdg.path);
		fdg::fields fd;
		fdg::navigater nav;
		fd << fdg.path;
		nav << fd;
		navs.push_back(nav);
	}
	for (std::vector<fdg::navigater>::iterator i = navs.begin(), e = navs.end()
		; i != e; ++i)
	{
		fdg::navigater &nav = *i;
		cout << nav.rsize << endl;
	}

	navs[1].demo(notify);

	ebc.path = path::app_path(ebc.path);
	int rsize = 209;
	int64 fsize = path::filesize(ebc.path.sjis());
	int64 rows = fsize / (int64)rsize;
	cout << rows << endl;
	

	return 0;
}