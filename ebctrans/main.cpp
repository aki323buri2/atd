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

	cout << rsize << endl;

	//データレコード数算出
	int64 fsize	= path::filesize(ebc.path.sjis());
	int64 rows	= fsize / (int64)rsize;
	cout << rows << endl;
	if (!rows) throw generic::exception("record count is zero!");

	//進捗
	int blocks = 40;
	int64 step = (rows + 1) / blocks;

	//map
	struct navmap : public std::map<uchar, fdg::navigater *>
	{
		fdg::navigater *navof(uchar key) const
		{
			const_iterator i = find(key);
			return i == end() ? 0 : i->second;
		}
		void addnav(uchar key, fdg::navigater *nav)
		{
			insert(value_type(key, nav));
		}
	};
	navmap map;
	map.addnav('1', &navs[0]);
	map.addnav('2', &navs[1]);
	map.addnav('3', &navs[2]);

	std::ifstream ifs(ebc.path.sjis().c_str(), std::ios::binary);
	string buf(rsize, 0);

	for (int64 i = 0; i < rows; i++)
	{
		::memset(&buf[0], 0, buf.size());
		ifs.read(&buf[0], buf.size());

		uchar rid = *(buf.rbegin());
		rid = ebcdic::tosjis(rid);
		
		fdg::navigater *nav = map.navof(rid);


		generic::property::vector conv;
		int offset = 0;
		string ebc(0x100, 0);
		string sjis;
		string utf8;
		for (fdg::fields::iterator i = nav->begin(), e = nav->end()
			; i != e; ++i)
		{
			fdg::field &field = *i;
			string &name = field.name;
			int real = field.real;
			ebc.resize(real, 0);
			::memcpy(&ebc[0], &buf[offset], real);
			sjis = field.translate(ebc);
			utf8 = sjis.utf8();
			
			offset += real;
		}

		//progress
		if ((i % step) == 0)
		{
			cout << "#";
		}
	}
	cout << endl;
	ifs.close();

	

	return 0;
}