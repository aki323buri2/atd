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
#include "ebcdic.h"
#include <fstream>
int run(int argc, char **argv)
{
	properties args;
	struct { string &path; } ebc = { args.value_of("ebc.path") };
	struct { string &path; } fdg = { args.value_of("fdg.path") };
	properties backup;
	backup["ebc.path"] = "\\\\pc3000002\\data2\\債権債務\\GS債権債務データ移行\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D1.D0209";
	backup["ebc.path"] = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D2.D0209";
	backup["ebc.path"] = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D3.D0209";
	backup["ebc.path"] = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D4.D0209";
	backup["ebc.path"] = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D5.D0209";
	backup["ebc.path"] = "D:\\data2\\saikensaimu\\債権債務サンプルデータ\\債権債務サンプルデータ（2015.02.09）\\TDBK1D1.D0209";
	strings fdgs(
		  "..\\fdg\\データ退避ＣＯＰＹメンバ\\TUF010.TXT"
		, "..\\fdg\\データ退避ＣＯＰＹメンバ\\TUF020.TXT"
		, "..\\fdg\\データ退避ＣＯＰＹメンバ\\TUF090.TXT"
	);
	backup["fdg.path"] = fdgs.implode(";");

	args.supply(backup);
	args.supply(properties(path::rename_extension(app.path, ".back.json")));
	args.commandline(argc, argv);

	//フルパス
	ebc.path	= path::app_path(ebc.path);
	//FDGロード
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
	//レコード長
	int rsize = 0;
	for (std::vector<fdg::navigater>::iterator i = navs.begin(), e = navs.end()
		; i != e; ++i)
	{
		fdg::navigater &nav = *i;
		if (nav.rsize > rsize) rsize = nav.rsize;
	}
	
	if (!rsize) throw generic::exception("record size is zero!");

	//出力フォルダ
	string outd = path::make_sure_directory(path::app_path("output.d"));

	//データレコード数算出
	int64 fsize	= path::filesize(ebc.path.sjis());
	int64 rows	= fsize / (int64)rsize;
	cout << rows << endl;
	if (!rows) throw generic::exception("record count is zero!");



	//進捗
	int blocks = 40;
	int64 step = (rows + 1) / blocks;

	//navigater map　
	navmap map;
	map.addnav('1', "TUF010", &navs[0]);
	map.addnav('2', "TUF020", &navs[1]);
	map.addnav('3', "TUF090", &navs[2]);

	//ファイルオープン
	std::ifstream ifs(ebc.path.sjis().c_str(), std::ios::binary);
	string buf(rsize, 0);

	for (navmap::iterator i = map.begin(), e = map.end()
		; i != e; ++i)
	{
		navitem &item = i->second;
		string name = item.name;
		string path = path::combine(outd, name);
		//ファイルクリア
		{
			std::ofstream ofs(path.sjis().c_str(), std::ios::out);
		}
		//ファイルオープン（追記＆バイナリ）
		item.ofs = new std::ofstream(path.sjis().c_str()
			, std::ios::app 
			| std::ios::binary
		);
	}

	for (int64 i = 0; i < rows; i++)
	{
		::memset(&buf[0], 0, buf.size());
		ifs.read(&buf[0], buf.size());

		//末尾１バイトでFDG判定
		uchar rid = *(buf.rbegin());
		rid = ebcdic::tosjis(rid);
		navitem &nav = map.navof(rid);

		//分割ファイルに書き込み
		nav.ofs->write(&buf[0], buf.size());
		nav.count++;

		//progress
		if ((i % step) == 0)
		{
			cout << "#";
		}
	}
	cout << endl;
	ifs.close();

	for (navmap::iterator i = map.begin(), e = map.end()
		; i != e; ++i)
	{
		uchar rid = i->first;
		navitem nav = i->second;
		cout << rid << " : " << nav.count << endl;
	}

	

	return 0;
}