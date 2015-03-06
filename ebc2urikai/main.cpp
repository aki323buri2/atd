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
		notify("");
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

	//コマンドライン引数でオーバーライド
	args.commandline(argc, argv);

	bool check = true;
	//絶対パスと存在チェック
	{
		string *ss[] = 
		{
			&ebc.path, 
			&fdg.path1, 
			&fdg.path2, 
			&fdg.path3, 
			0, 
		};
		check = abspath_and_existscheck(ss);
	}

	args.demo(notify);
	if (!check) throw generic::exception("args file exists check error !!");

	//invokers
	invokers invokers;
	invokers.entry(new invoker(fdg.path1));
	invokers.entry(new invoker(fdg.path2));
	invokers.entry(new invoker(fdg.path3));

	if (!invokers.size())
	{
		throw generic::exception("invokers is empty!!");
	}
	if (!invokers[0]->navigater.size())
	{
		throw generic::exception("invokers has empty navigater!!");
	}
	if (!invokers.even())
	{
		throw generic::exception("record size is uneven!!");
	}

	//レコード長
	int rsize = invokers[0]->navigater.rsize;

	//変換元ファイルを分割
	string fname = path::basename(ebc.path.sjis()).utf8();
	int64  fsize = path::filesize(ebc.path.sjis());
	int64  lines = fsize / (int64)rsize;

	notify ("**********************************");
	notify ("* EBCDIC file >>                 *");
	notifyf("*   file name : %-16s *", fname.c_str());
	notify ("*---------------------------------");
	notifyf("* record size : %10lld bytes *", rsize);
	notifyf("*   file size : %10lld bytes *", fsize);
	notifyf("*  lines count: %10lld lines *", lines);
	notify ("**********************************");

	//出力フォルダ作成
	struct { string d; } out;
	out.d = path::make_sure_directory(path::app_path("out.d").sjis()).utf8();
	invokers.apply_outd(ebc.path, out.d);
	notifyf(">> 出力フォルダ : %s", out.d.c_str());

	//読み取りファイルオープン
	std::ifstream ifs(ebc.path.sjis().c_str()
		, std::ios::binary
		| std::ios::in
	);

	//カウンタ変数
	struct { int64 bytes, lines; } done = {0};

	//進捗バー用変数
	struct { int steps; int64 step; } prog = {0};
	prog.steps = 50;
	prog.step  = lines / (int64)prog.steps;

	notifyf(">> %s input start: ", fname.c_str());

	cout << ">> loading... ";
	string line(rsize, 0);
	while (ifs.read(&line[0], line.size()))
	{
		done.lines += 1;
		done.bytes += line.size();

		//進捗バー
		if (done.lines % prog.step == 0)
		{
			cout << "#";
		}
	}
	cout << endl;

	ifs.close();

	notifyf(">> done.lines = %10lld", done.lines);
	notifyf(">> done.bytes = %10lld", done.bytes);



	//並列処理スタート＆全ｽﾚｯﾄﾞ完了待ち
	invokers.start();
	invokers.wait();

	return 0;	
}
void invoker::run()
{
	notifyf("%s's navigater(rsize: %d) have %d fields"
		, name.c_str()
		, navigater.rsize
		, navigater.size()
	);

}