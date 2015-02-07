# -*- coding:utf-8 -*-
import sys, os, shutil
import json
from collections import namedtuple
from collections import OrderedDict
# PDF出力モジュール
from pdfutil import cell, point
#=====================================================
# コマンドライン引数
#=====================================================
args = sys.argv
args.extend(["", ""])
py = args[0] # スクリプトパス
js = args[1] # JSONファイルパス
od = args[2] # PDF出力フォルダ

print("* py : %s" % py)
print("* js : %s" % js)
print("* od : %s" % od)

#=====================================================
# JSONパス補完（テスト用）★★★★★★★★
#=====================================================
if js == "":
	js = os.path.abspath("json/FAXFILPF.KA4RDMLB.json")
	js = os.path.abspath("json/FFX140906103106P1N1NMVC.tran.json")
	js = os.path.abspath("json/FFX140902194145P1N1NMVC.tran.json")
	js = os.path.abspath("json/FFX140929120850P1.tran.json")
	js = os.path.abspath("json/FFX140920135408P1NOFILEIDT.tran.json")
	js = os.path.abspath("json/FFX-test1.tran.json")
	js = os.path.abspath("json/FFX141026185451P1S4SERVE.tran.json")
	js = os.path.abspath("json/FFX141024195524P1S1SMDFAXT.tran.json")
	js = os.path.abspath("json/FFX141010091201P1N1JUSCO.tran.json")

#=====================================================
# パス分解
#=====================================================
class pathinfo:
	def __init__(self, full):
		self.full = full
		self.dirname, self.basename = os.path.split(full)
		self.filename, self.extension = os.path.splitext(self.basename)
pi = pathinfo(js)

#=====================================================
# JSONをロード
#=====================================================
f = open(js, "r", encoding="utf-8")
loaded = json.load(f)
f.close()
kensu = len(loaded)
print("JSONデータ件数 : %d" % kensu)
if kensu == 0:
	sys.exit()


#=====================================================
# フォルダ名生成
#=====================================================
faxid = pi.filename
row = loaded[0]
dirname = "[%s]-[%s]-[%s]-[%s]-%s(%s--)" % (
	faxid
	, row["SYOZOK"]
	, row["FAXNO"]
	, row["DENYMD"]
	, row["TOKUME"]
	, row["KNRINO"]
	)
dirname = dirname.replace("\u3000", "　")
#=====================================================
# フォルダ作成
#=====================================================
if od == "" : od = os.path.dirname(__file__) + "\\pdf"
full = namedtuple("full", "dirname, filename")
full.dirname = "%s\\%s" % (od, dirname)
if os.path.isdir(full.dirname):
	shutil.rmtree(full.dirname)
os.makedirs(full.dirname)

print("<<<result>>>%s" % full.dirname)
#=====================================================
#= ユーティリティ関数
#=====================================================
from datetime import datetime
import locale
locale.setlocale(locale.LC_TIME, "jpn")
class util:
	def ymd(s):
		d = datetime.strptime(s, "%y%m%d")
		# 注意！datetime.weekday()は月曜がゼロ！
		# YYMMDD形式の日付を読み込む
		return \
			"%02d年%02d月%02d日 (%s)" % \
			(d.year % 100, d.month, d.day
				, ["日", "月", "火", "水", "木", "金", "土"][(d.weekday()+1)%7]
				)
	def his(s):
		d = datetime.strptime(s, "%H%M%S")
		return \
			"%02d時%02d分%02d秒" % \
			(d.hour, d.minute, d.second)

	def faxtime(s):
		d = datetime.strptime(s, "%y%m%d%H%M%S")
		return \
			"%04d.%02d.%02d (%s) %02d:%02d:%02d" % \
			(d.year, d.month, d.day
				, ["日", "月", "火", "水", "木", "金", "土"][(d.weekday()+1)%7]
				, d.hour, d.minute, d.second
				)
#=====================================================
#= 日配？
#=====================================================
def isnippai(syozok):
	for sz in ["134", "160", "161", "620", "840", "930"]:
		if sz == syozok:
			return True
	return False

nippai = isnippai(row["SYOZOK"])
# nippai = False

#=====================================================
#= フォーマット的に単価なし判定（イズミヤ夜間とか
#=====================================================
hidetanka = (row["LSTFLG"] == "KAR") #★

#=====================================================
#= 処理情報解析用正規表現コンパイル（事前コンパイル
#=====================================================

import re
pattern = ".{11}(.{4}).+([0-9]{9})([0-9]{4})([0-9]{6})([0-9]{6})[0-9]{2}$"
regex = re.compile(pattern)
syorin, seqno, seqnor, ymd, his = ("", "", "", "", "")
# 待避ファイルの場合
pattern = "^(.{8})(.{2})(.{2})(.{8})([0-9]{6})([0-9]{6})(.{4})"
rege2 = re.compile(pattern)
cl, gmid, gmcd, fileid, ymd, his, syorin = ("", "", "", "", "", "", "")

# 上記のパターンのどれも当てはまらなかった場合のためにタイムスタンプを準備しておく
# まずはファイル名（FAXID）から読み取る
filename = pi.filename
pattern = ".*([0-9]{6})([0-9]{6})"
r = re.compile(pattern)
m = r.match(filename)
if not m == None:
	ymd, his = m.groups()
else:
	# 読み取れなかったら
	#　しょうがないのでフォルダのタイムスタンプ
	stat = os.stat(full.dirname)
	mtime = datetime.fromtimestamp(stat.st_mtime)
	ymd, his = (
		  mtime.strftime("%y%m%d")
		, mtime.strftime("%H%M%S")
		)	
stime = namedtuple("stime", "ymd his")
stime = stime(ymd, his)
#=====================================================
#= ページヘッダ出力関数
#=====================================================
def pageheader(cell, printed, row):
	printed.page += 1

	fontsize = 14
	lineheight = fontsize + 3
	title = "発　注　書　（誓　約　確　認　書）" if not nippai else \
			"　   発　 　 注  　　書 　　  "

	cell.setfont(size=fontsize)
	cell.reset()
	cell.home().move(160, 0).draw(title)
	cell.cr(lineheight)

	fontsize = 11
	lineheight = fontsize + 1
	charspace = 3.0
	cell.setfont(size=fontsize)
	cell.setcharspace(charspace)
	cell.draw("")

	cell.cr(lineheight)
	cell.home().move(200, 0).draw(
		"" if nippai else "%s 店着分" % util.ymd(row["DENYMD"]) 
		)

	cell.cr()
	cell.home().draw(row["HATMEI"])
	cell.home().move(160, 0).draw("御中")
	cell.home().move(200, 0).draw(
		"%s 納入分" % util.ymd(row["URIYMD"]) 
		)
	cell.end ().move(-50, 0).draw("%d / %d" % (printed.page, printed.pages))
	cell.cr()

	#FAX番号の字下げ
	faxoffset = " "
	cell.home().move( 0, 0).draw(faxoffset+row["HFAXNO"])
	cell.cr()
	cell.home().move(300, 0).draw("株式会社ショクリュー　" + row["BUKAME"])
	cell.cr().home().move(300, 0).draw(faxoffset+row["TELNO"])
	cell.cr().home().move(300, 0).draw(faxoffset+row["FAXNO"])

	cell.cr().home().draw("得意先")
	cell.cr().home()
	cell.move( 20, 0).draw("商品名")
	cell.move(120, 0).draw("" if nippai else "産地・鮮冷")
	cell.move(100, 0).draw("区分")

	#右詰め数量との調整
	suryotitleoffset = "      "
	cell.move( 40, 0).draw(suryotitleoffset + "入数")
	cell.move( 80, 0).draw(suryotitleoffset + "数量")
	cell.move( 80, 0).draw(suryotitleoffset + ("--" if hidetanka else "単価"))

	#=================================================
	#= FAX処理情報取得（空き領域の解析）
	#=================================================
	# 作成日時取得
	aki = row["AKI"]

	global stime, ymd, his, faxtime
	ymd, his = ("", "")
	# 空き領域解析
	m = regex.search(aki)
	if not m is None:
		syorin, seqno, seqnor, ymd, his = m.groups()
	else:
		#NAKAFIL3 待避ファイルバージョン？
		m = rege2.search(aki)
		if not m is None:
			cl, gmid, gmcd, fileid, ymd, his, syorin = m.groups()
	
	# 空き領域から読めなければタイムスタンプを使う
	if ymd == "": ymd = stime.ymd
	if his == "": his = stime.his
	# 日付・時刻の書式
	faxtime = ""
	if not ymd == "" and not his == "": 
		faxtime = util.faxtime(ymd+""+his)
	if not ymd == "": ymd = util.ymd(ymd)
	if not his == "": his = util.his(his)	
#=====================================================
#= セクションヘッダ出力関数
#=====================================================
def sectionheader(cell, row):
	fontsize = 11
	lineheight = fontsize + 1
	charspace = 3.0
	cell.setfont(size=fontsize)
	cell.setcharspace(charspace)
	cell.draw("")

	cell.cr(lineheight).home().draw("")
	cell.cr().home().draw(row["TOKUME"])
#	cell.move(150, 0).draw("(%s)" % row["TOKUNO"])
	cell.cr()
#=====================================================
#= 明細出力関数
#=====================================================
def detail(cell, row, data):
	fontsize = 11
	lineheight = fontsize + 1
	charspace = 2.0
	cell.setfont(size=fontsize)
	cell.setcharspace(charspace)
	cell.draw("")

	cell.cr(lineheight).home().draw("")
	cell.home()
	cell.draw(row["SHNM1"])
	cell.move(150, 0).draw("" if nippai else "<       |       >")
	cell.move( 90, 0).draw("特売" if row["URIKU"] == "9" else "定番")

	fontname = cell.fontname
	cell.setfont(name="ＭＳ 明朝")
	cell.draw("")
	cell.move( 40, 0).draw("%9.2f" % data["SIRIME"])
	cell.move( 80, 0).draw("%9.2f" % data["SKOSU"])
	cell.move( 80, 0).draw("" if hidetanka else ("%9.2f" % data["STANKA"]))
	cell.setfont(name=fontname)
	# 罫線
	cell.home()
	nw = cell.where.copy()
	nw.y += 2
	se = nw.copy()
	se.x += 500
	cell.home().line(nw, se, 0.3, [1, 3])
#=====================================================
#= ファイルフッター出力関数
#=====================================================
def filefooter(cell, row, printed, faxid):
	fontsize = 11
	lineheight = fontsize + 1
	charspace = 1.0
	cell.setfont(size=fontsize)
	cell.setcharspace(charspace)

	cell.cr(lineheight)
	cell.home().draw(syorin)
	cell.cr()
	cell.home().draw("以上、%d枚よろしくお願いいたします。" % printed.page)
	cell.cr()
	if not nippai:
		cell.cr().home().draw("上記内容を確認していただき相違なければ、ご署名の上FAXにて送信お願いいたします。")
		cell.cr().home().draw("相違ある場合、修正していただき返信お願いいたします。")
		cell.cr()
		cell.cr()
		cell.home().draw("　貴社名：")
		cell.home().move(250, 0).draw("氏名：")
		cell.cr()
	else:
		cell.cr()
		cell.cr()
		cell.cr()
		cell.cr()
		cell.cr()
		
	cell.cr()
#	cell.home().draw(faxid)
	cell.home().move(350, 0).draw(faxtime)


#=====================================================
# ファイル＆セクション振り分け
#=====================================================
files = []
memo = namedtuple("name", "file, sect, body")
index = 0
index -= 1
for row in loaded:
	index += 1
	uriymd = row["URIYMD"]
	hatno = row["HATNO"]
	tokuno = row["TOKUNO"]
	shcds = row["SHCDS"]
	uriku = row["URIKU"]
	sirime = row["SIRIME"]
	stanka = row["STANKA"]

	if memo.file != (uriymd, hatno):
		memo.file = (uriymd, hatno)
		add = OrderedDict()
		add["where"] = index
		add["sect"] = sect = []
		files.append(add)

	if memo.sect != (hatno, tokuno):
		memo.sect = (hatno, tokuno)
		add = OrderedDict()
		add["where"] = index
		add["body"] = body = []
		sect.append(add)

	if memo.body != (hatno, tokuno, shcds, uriku, sirime, stanka):
		memo.body = (hatno, tokuno, shcds, uriku, sirime, stanka)
		add = OrderedDict()
		add["where"] = index
		add["data"] = data = OrderedDict()
		data["STANKA"] = 0.0
		data["SIRIME"] = 0.0
		data["SKOSU"] = 0.0
		data["SSURYO"] = 0.0
		body.append(add)

	data["STANKA"] += float(row["STANKA"])
	data["SIRIME"] += float(row["SIRIME"])
	data["SKOSU"] += float(row["SKOSU"])
	data["SSURYO"] += float(row["SSURYO"])

#=====================================================
#= 出力処理
#=====================================================
seq = 0
cell = cell()
#=====================================================
#= ファイル出力ループ
#=====================================================
for file in files:
	where = file["where"]
	row = loaded[where]
	# ファイル名生成
	seq += 1
	filename = "[%s]-[%s]-[%s]-%s(%s--).pdf" % (
		"%03d" % seq
		, row["HFAXNO"]
		, row["HATNO"]
		, row["HATMEI"]
		, row["KNRINO"]
		)
	filename = filename.replace("\u3000", "　")

	# PDFファイル作成
	full.filename = "%s\\%s" % (full.dirname, filename)
	cell.create(full.filename, margin = point(50, 50))

	
	
	# フッタの印字開始位置設定
	bottom = cell.pagesize.h - 200#★

	#=================================================
	#= PDFファイル生成関数ここから
	#=================================================
	sectno = 0
	def filerender(row):
		#---------------------------------------------
		# ページヘッダ出力
		pageheader(cell, printed, row)
		sectno = 0
		#---------------------------------------------

		#=============================================
		#= セクション出力ループ
		#=============================================
		for sect in file["sect"]:
			where = sect["where"]
			row = loaded[where]

			sectno += 1

			# 印刷可能な残りエリアの高さ
			left = bottom - cell.where.y
			# セクションヘッダ2行＋明細行数 x 行高
			lineheight = cell.lineheight
			howmany = len(sect["body"])
			need = lineheight * (2 + howmany)
			# セクション全体を印刷できるのか？
			if left < need:

				if sectno > 1:
					#-------------------------------------
					# あふれたので改ページ
					cell.showPage()
					#-------------------------------------
					#-------------------------------------
					#リセット
					cell.reset()
					#-------------------------------------
					#-------------------------------------
					# ページヘッダ出力
					pageheader(cell, printed, row)
					#-------------------------------------

			#-----------------------------------------
			# セクションヘッダ出力
			sectionheader(cell, row)
			#-----------------------------------------

			#=========================================
			#= 明細出力ループ
			#=========================================
			for body in sect["body"]:
				where = body["where"]
				data = body["data"]
				row = loaded[where]

			    #-------------------------------------
			    #- あふれチェック
				if (cell.where.y + cell.lineheight) >= bottom:
				    # カイページ
				    cell.showPage()
				    cell.reset()
				    pageheader(cell, printed, row)
				    sectionheader(cell, row)
			    #-------------------------------------
			    #-------------------------------------
				# 明細出力
				detail(cell, row, data)
			    #-------------------------------------



		#---------------------------------------------
		# フッタ開始位置へ
		cell.moveto(0, bottom)
		#---------------------------------------------
		# ファイルフッタ
		filefooter(cell, row, printed, faxid)
		#---------------------------------------------

		#---------------------------------------------
		# 改ページ
		cell.showPage()
		#---------------------------------------------
	#=================================================
	#= PDFファイル生成関数ここまで
	#=================================================

	# ページ出力カウンタ
	printed = namedtuple("printed", "page pages")
	printed.page = 0
	printed.pages = 0

	# 練習モードで出力
	cell.practice = True
	filerender(row)

	# 総ページ数取得
	printed.pages = printed.page
	printed.page = 0

	# 本番モードで再度出力
	cell.practice = False
	cell.reset()
	filerender(row)
	#-------------------------------------------------
	# ファイル保存
	cell.save()
	#-------------------------------------------------
	print("<<<result>>>%s" % filename)


