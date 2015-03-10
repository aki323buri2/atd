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


	//読み取りファイルオープン
	std::ifstream ifs(ebc.path.sjis().c_str()
		, std::ios::binary
		| std::ios::in
	);
	
	//出力フォルダ作成
	struct { string d; } out;
	out.d = path::make_sure_directory(path::app_path("out.d").sjis()).utf8();
	notifyf(">> 出力フォルダ : %s", out.d.c_str());

	//EBCDICファイル振り分け用ファイルオープン
	string basename = path::basename(ebc.path.sjis()).utf8();
	for (invokers::iterator i = invokers.begin(), e = invokers.end()
		; i != e; ++i)
	{
		invoker &invoker = **i;
		std::ofstream &ofs	= invoker.ofs;
		string &name		= invoker.name;
		string &ebc			= invoker.ebc;
		string &json		= invoker.json;
		ebc  = path::combine(out.d, basename + "." + name);
		json = path::combine(out.d, basename + "." + name + ".json");
		//クリア
		ofs.open(ebc.sjis().c_str()
			, std::ios::binary
			| std::ios::out
		);
		ofs.close();
		//追記
		ofs.open(ebc.sjis().c_str()
			, std::ios::binary
			| std::ios::app
		);
	}

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
		//末尾１バイトで判定
		uchar judge = *(line.rbegin()); 
		//EBCDIC変換
		judge = ebcdic::tosjis(judge);
		//'1' or '2' or ....
		size_t offset = judge - '1';
		//invokersのイテレータ取得
		invokers::iterator i = invokers.invoker_of(offset);
		if (i == invokers.end())
		{
			notifyf("!!!! '%c' is unknown judgement flag !!!!", judge);
			throw generic::exception("invalid judgement flag !");
		}

		//振り分けファイルへの追記処理
		invoker &invoker = **i;
		std::ofstream &ofs = invoker.ofs;
		ofs.write(&line[0], line.size());

		//振り分け行数インクリメント
		invoker.judge = judge;
		invoker.lines.total++;

		//進捗バー
		done.lines += 1;
		done.bytes += line.size();
		if (done.lines % prog.step == 0)
		{
			cout << "#";
		}
	}
	cout << endl;

	//ファイルクローズ
	ifs.close();
	invokers.ofclose();
	notifyf(">> done.lines = %10lld", done.lines);
	notifyf(">> done.bytes = %10lld", done.bytes);

	//振り分け結果表示
	for (invokers::iterator i = invokers.begin(), e = invokers.end()
		; i != e; ++i)
	{
		invoker &invoker = **i;
		uchar judge = invoker.judge;
		int64 lines = invoker.lines.total;
		int64 bytes = path::filesize(invoker.ebc.sjis());
		string basename = path::basename(invoker.ebc.sjis()).utf8();
		notifyf("-- '%c' : %10s %10lld lines / %10lld bytes"
			, judge
			, basename.c_str()
			, lines
			, bytes
		);
	}

	//並列処理進捗通知ボードを初期化
	boards.init(invokers);

	//並列処理スタート＆全ｽﾚｯﾄﾞ完了待ち
	cout << "...";
	invokers.start();
	invokers.wait();
	cout << endl;

	//変換JSONファイルのサイズを表示
	for (invokers::iterator i = invokers.begin(), e = invokers.end()
		; i != e; ++i)
	{
		invoker &invoker = **i;
		uchar judge = invoker.judge;
		int64 bytes = path::filesize(invoker.json.sjis());
		string basename = path::basename(invoker.json.sjis()).utf8();
		notifyf("-- '%c' : %10s %10lld bytes"
			, judge
			, basename.c_str()
			, bytes
		);
	}

	return 0;	
}
//====================================================
//= 並列処理の内容
//====================================================
void invoker::run()
{
	ifs.open(ebc.sjis().c_str()
		, std::ios::binary
		| std::ios::in
	);
	ofs.open(json.sjis().c_str()
		, std::ios::binary
		| std::ios::out
	);
	int64 total = lines.total;
	int64 &done = lines.done;

	ofs << "[\n";

	//通知ボード取得
	board::page &board = boards.find(judge)->second;

	//パーセント進捗
	struct { int pre, now, step; } percent = {0};
	int &pre = percent.pre;
	int &now = percent.now;
	int &step = percent.step;

	step = 1;

	string line(navigater.rsize, 0);
	string buf(0x100, 0);
	string sjis, utf8;

	while (ifs.read(&line[0], line.size()))
	{
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		ofs << (done ? ", " : "  ");
		ofs << "{\n";
		
		int offset = 0;
		for (fdg::navigater::iterator 
			  b = navigater.begin()
			, i = b
			, e = navigater.end()
			; i != e
			; ++i
		)
		{
		//	ofs << "\t";
			ofs << (i - b ? ", " : "  ");

			fdg::field &field = *i;
			int real = field.real;
			const string &name = field.name;
			buf.resize(real, 0);
			::memcpy(&buf[0], &line[offset], real);
			sjis = field.translate(buf);
			utf8 = sjis.utf8();

			ofs << translator::json_escape(name).double_quote();
			ofs << ": ";
			ofs << translator::json_escape(utf8).double_quote();
			ofs << "\n";

			offset += real;
		}
		ofs << "}\n";
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		done++;
		now = (done * (int64)100) / total;
		if ((now - pre) < step) continue;

		pre = now;
		board.percent = percent.now;
		boards.update();//通知★
	}
	//ラスト
	now = (done * (int64)100) / total;
	boards.update();

	ofs << "]";
	ifs.close();
	ofs.close();
}