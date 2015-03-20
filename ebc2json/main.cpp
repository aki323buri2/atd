// main.cpp
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
		notifyf("!!!! %s stop because error : %s", app.basename.c_str(), e.what());
	}
	return r;
}
int frame(int argc, char **argv)
{
	string name = app.filename;
	notifyf(">>>> %s start (%s) >>>>", name.c_str(), datetime().strftime().c_str());
	int r = run(argc, argv);
	notifyf(">>>> %s e n d (%s) >>>>", name.c_str(), datetime().strftime().c_str());
	return r;
}
int run(int argc, char **argv)
{
	namevalue manifest;
	struct { string &path; } ebc  = { manifest.value_of("ebc.path" ) };
	struct { string &path; } fdg  = { manifest.value_of("fdg.path" ) };
	struct { string &json; } tran = { manifest.value_of("tran.json") };
	namevalue backup;
	backup["ebc.path"] = "..\\ebc\\TMASAPF.RDMLIB";
	backup["ebc.path"] = "..\\ebc\\ZAIKOPF.KCRDMLB";
	backup["ebc.path"] = "..\\ebc\\FAXFILPF.KA4RDMLB";
	backup["ebc.path"] = "..\\ebc\\FFX14052014025490";
	backup["ebc.path"] = "..\\ebc\\FFX141026185451P1S4SERVE";
	backup["ebc.path"] = "..\\ebc\\TDBK4D1.D0209";//債権債務得意先Ｍ（関西）
	backup["fdg.path"] = "..\\fdg\\TXF010.TXT";//債権債務得意先ＭのＦＤＧ
	manifest.supply(backup);
	manifest.supply(namevalue(path::rename_extension(app.path, ".hack.json")));
	manifest.commandline(argc, argv);	

	if (namevalue::isnull(fdg.path))
	{
		fdg.path = util::find_fdg_or_die(ebc.path);
	}
	if (namevalue::isnull(tran.json))
	{
		string folder = path::app_path("tran");
		path::make_sure_directory(folder.sjis());
		tran.json = path::combine(folder, path::find_file_name(ebc.path) + ".tran.json");
	}

	ebc.path  = path::app_path(ebc.path);
	fdg.path  = path::app_path(fdg.path);
	tran.json = path::app_path(tran.json);

	manifest.demo(notify);

	util::exists_or_die(ebc.path);
	util::exists_or_die(fdg.path);

	//================================================
	//= FDG（COBOLイメージ）のパース
	//================================================
	fdg::fields fields;
	fields << fdg.path;
	if (!fields.size())
	{
		util::error("fdg parse failed : %s", fdg.path.c_str());
	}
	//================================================
	//= FDGナビゲータ生成（集団項目およびOCCURS展開）
	//================================================
	fdg::navigater navigater;
	navigater << fields;
	if (!navigater.size())
	{
		util::error("fdg expand faild : %s", fdg.path.c_str());
	}
	//================================================
	//= 変換JSONの出力ストリーム作成
	//================================================
	std::ofstream ofs(tran.json.sjis().c_str());
	//================================================
	//= トランスレータ生成
	//================================================
	struct translator translator(ofs, navigater);
	//================================================
	//= EBCDICファイルを変換
	//================================================
	translator << ebc.path;
	//================================================
	//= 変換後ファイルのパスをコンソール出力
	//================================================
	notifyf("<<<result>>>{\"%s\": \"%s\"}", "tran.json", tran.json.c_str());
	//================================================

	return 0;
}