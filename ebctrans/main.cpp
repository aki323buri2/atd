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
#include "translator.h"
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

	//処理対象ファイル名
	string filename = path::filename(ebc.path.sjis()).utf8();
	
	//navigater map　
	navmap map;
	strings ss = fdg.path.explode(";");
	map.addnav('1', path::app_path(ss[0]));
	map.addnav('2', path::app_path(ss[1]));
	map.addnav('3', path::app_path(ss[2]));
	
	
	//レコード長（一番長いやつ）
	if (!map.rsizecheck())
	{
		throw generic::exception("fdg record sizes do not have the same value!");
	}
	int rsize = map.begin()->second.nav.rsize;
	if (!rsize) throw generic::exception("record size is zero!");

	//出力フォルダ作成
	string outd = path::make_sure_directory(path::app_path("output.d"));

	//データレコード数算出
	int64 fsize	= path::filesize(ebc.path.sjis());
	int64 rows	= fsize / (int64)rsize;
	cout << rows << endl;
	if (!rows) throw generic::exception("record count is zero!");

	//進捗バー用
	int blocks = 40;
	int64 step = (rows + 1) / blocks;


	//ファイルオープン
	std::ifstream ifs(ebc.path.sjis().c_str(), std::ios::binary);
	string buf(rsize, 0);

	for (navmap::iterator i = map.begin(), e = map.end()
		; i != e; ++i)
	{
		navitem &item = i->second;
		string name = filename + "." + item.name;
		string path = path::combine(outd, name);
		cout << path << endl;
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
	//ファイル分割処理
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

	//分割ファイルを一旦クローズ
	map.filecloseall();

	//分割ファイル読み取り＆書き込みストリームオープン
	for (navmap::iterator i = map.begin(), e = map.end()
		; i != e; ++i)
	{
		navitem &item = i->second;
		const string &name = filename + "." + item.name;
		std::ifstream *&ifs = item.ifs;
		std::ofstream *&ofs = item.ofs;
		struct { string path; } input = { path::combine(outd, name) };
		struct { string path; } output = { path::combine(outd, name+".json") };

		ifs = new std::ifstream(input.path.sjis().c_str()
			, std::ios::binary | std::ios::in
		);
		ofs = new std::ofstream(output.path.sjis().c_str()
			, std::ios::out
		);
	}

	map.invoke();
	notify("done");

	return 0;
}
void navitem::action()
{
	int rsize = nav.rsize;
	string line(rsize, 0);
	string ebc(rsize, 0);
	string sjis;
	string utf8;
	struct { int64 lines; } done = {0};

	std::ofstream &ofs = *(this->ofs);

	ofs << "[\n";
	while (ifs->read(&line[0], line.size()))
	{
		ofs << "\t";
		ofs << (done.lines++ ? ", " : "  ");
		ofs << "{\n";
		int offset = 0;
		for (fdg::navigater::iterator b = nav.begin(), i = b, e = nav.end()
			; i != e; ++i)
		{
			ofs << "\t\t";
			ofs << (i - b ? ", " : "  ");

			fdg::field &field = *i;
			int real = field.real;
			const string &name = field.name;
			ebc.resize(real, 0);
			::memcpy(&ebc[0], &line[offset], real);
			sjis = field.translate(ebc);
			utf8 = sjis.utf8();

			ofs << translator::json_escape(name).double_quote();
			ofs << ": ";
			ofs << translator::json_escape(utf8).double_quote();
			ofs << "\n";

			offset += real;
		}
		ofs << "}\n";
	}
	ofs << "]";
	cout << name << " done : " << done.lines << "lines" << endl;
}
void navmap::cleanup()
{
	for (iterator i = begin(), e = end(); i != e; ++i)
	{
		navitem &item = i->second;
		delete item.ifs;
		delete item.ofs;
		delete item.thread;
		delete item.event;
	}
}
void navmap::invoke()
{
	for (iterator i = begin(), e = end(); i != e; ++i)
	{
		navitem &item = i->second;
		item.start();
	}

	while (true)
	{
		if (!running()) break;
		app::sleep(1);
	}
}