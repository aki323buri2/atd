# wizard.py
import sys
import os
import re
release = False
release = True
def make(filename, s):
	if release and os.path.isfile(filename):
		return
	with open(filename, "w", encoding = "utf-8") as f:
		f.write(s)
def guard(filename):
	return "__%s__" %(filename.replace(".", "_"))
def headerfile(filename, s):
	make(filename, """\
//{filename}
#ifndef {guard}
#define {guard}
{s}
#endif//{guard}\
""".format(filename = filename, s = s, guard = guard(filename)))
def cppfile(filename, s):
	make(filename, """\
//{filename}
{s}
""".format(filename = filename, s = s))
# ====================================================
ns = "aui"
names = [
	"common", 
	"application", 
	"widget", 
	"sizer", 
	"dialog", 
	"control", 
	"button", 
	"editbox", 
]
subs = {
	"widget": ["properties"]
}
classes = {
	"common": [
		"font", 
	], 
	"application": [
		"winmessage", 
	], 
	"widget": [

		"winclass", 
		"wincreator", 
		"winlistener", 
		"winchildren", 
	], 
	"sizer": [
		"boxsizer", 
		"gridsizer", 
	], 
}
bases = {
	"font"			: ["object", "LOGFONT"], 
	"winmessage"	: ["object", "MSG"], 
	"winclass"		: ["object", "WNDCLASSEX"], 
	"wincreator"	: ["object", "CREATESTRUCT"], 
	"winlistener"	: ["generic::listener<winmessage &>"], 
	"winchildren"	: ["object", "std::vector<widget *>"], 
	"boxsizer"		: ["sizer"], 
	"gridsizer"		: ["sizer"], 
	"dialog"		: ["widget"], 
	"control"		: ["widget"], 
	"button"		: ["control"], 
	"editbox"		: ["control"], 
}
declares = [
	"winchildren(widget *parent = 0);", 
	"sizer(widget *owner);", 
	"boxsizer(widget *owner);", 
	"gridsizer(widget *owner);", 
	"widget(const string &name, const string &text);", 
	"control(const string &name, const string &text);", 
	"dialog(const string &text);", 
	"button(const string &text);", 
	"editbox(const string &text);", 
"""\
font(
	  const string &name = "MS ゴシック"
	, int points = 90
	, bool bold = false 
	, bool italic = false 
//	, LONG lfHeight = 0
	, LONG lfWidth = 0
	, LONG lfEscapement = 0
	, LONG lfOrientation = 0
//	, LONG lfWeight = 0
//	, BYTE lfItalic = 0
	, BYTE lfUnderline = 0
	, BYTE lfStrikeOut = 0
	, BYTE lfCharSet = DEFAULT_CHARSET
	, BYTE lfOutPrecision = 0
	, BYTE lfClipPrecision = 0
	, BYTE lfQuality = 0
	, BYTE lfPitchAndFamily = 0
//	, TCHAR lfFaceName[LF_FACESIZE] = 0
);""", 
"""\
winmessage(
	  HWND		hwnd = 0
	, UINT		message = 0
	, WPARAM	wParam = 0
	, LPARAM	lParam = 0
	, object	*sender = 0
//	, DWORD		time = 0
//	, POINT		pt = 0
);""", 
"""\
winclass(
	  const string &name = ""
//	, UINT		cbSize = 0
	, UINT		style = 0
	, WNDPROC	lpfnWndProc = 0
	, int		cbClsExtra = 0
	, int		cbWndExtra = 0
//	, HINSTANCE	hInstance = 0
	, HICON		hIcon = 0
	, HCURSOR	hCursor = 0
	, HBRUSH	hbrBackground = 0
	, LPCTSTR	lpszMenuName = 0
//	, LPCTSTR	lpszClassName = 0
	, HICON		hIconSm = 0
);""",
"""\
wincreator(
	  const string &name = ""
	, const string &text = ""
	, LPVOID lpCreateParams = 0
//	, HANDLE hInstance = 0
	, HMENU hMenu = 0
	, HWND hwndParent = 0
	, int cy = 0
	, int cx = 0
	, int y = 0
	, int x = 0
	, LONG style = 0
//	, LPCSTR lpszName = 0
//	, LPCSTR lpszClass = 0
	, DWORD dwExStyle = 0
);""", 
]

initlists = [
	"boxsizer(...) : sizer(owner);", 
	"gridsizer(...) : sizer(owner);", 
	"control(...) : widget(name, text);", 
	"dialog(...) : widget(\"DIALOG_AUI_WINDOW\", text);", 
	"button(...) : control(\"BUTTON\", text);", 
	"editbox(...) : control(\"EDIT\", text);", 
]
# ====================================================
ss = declares
declares = {}
for s in ss:
	m = re.match(r"(.+)\s*\(", s)
	if not m: 
		continue
	name = m.groups(0)[0]
	header = s
	s = re.sub(r"\s*=\s*.+([\n,)])", r"\1", s)
	s = re.sub(r"(%s)\s*(\()" %(name), r"\1::\1\2", s)
	s = re.sub(r"(\))\s*;", r"\1", s)
	cpp = s
	declares[name] = {"header": header, "cpp": cpp}
# ====================================================
ss = initlists
initlists = {}
for s in ss:
	m = re.match(r"(.+)\s*\(\.{3}\)", s)
	if not m:
		continue
	name = m.groups(0)[0]
	s = re.sub(r"%s\s*\(\.{3}\)\s*(:.+)\s*;" %(name), r"\1", s)
	s = re.sub(r"(\))\s*,\s*", r"\1\n, ", s)
	initlists[name] = s
# ====================================================
def domake():
	
	s = "\n".join(["#include \"%s.%s.h\"" %(ns, n) for n in names])

	headerfile("%s.h" %(ns), """\
#include "atd.h"
using namespace atd;
{s}\
""".format(s = s))

	[headerandcpp(ns, n) for n in names]

def headerandcpp(ns, name):
	header(ns, name)
	cpp(ns, name)

def header(ns, name):
	
	cc = [name] + classes.get(name, [])

	p = "\n".join(["struct %s;" %(c) for c in cc])

	s = "\n".join([classdef(ns, c) for c in cc])

	headerfile("%s.%s.h" %(ns, name), """\
#include "{ns}.h"
namespace {ns} {{;
{p}
{s}
}}//namespace {ns}\
""".format(ns = ns, s = s, p = p))

def cpp(ns, name):
	
	s = "\n".join([classimpl(ns, c) for c in [name] + classes.get(name, [])])

	cppfile("%s.%s.cpp" %(ns, name), """\
#include "{ns}.h"
using namespace {ns};
{s}\
""".format(ns = ns, s = s))

	[subcpp(ns, name, sub) for sub in subs.get(name, [])]

def subcpp(ns, name, sub):

	s = ""

	cppfile("%s.%s.%s.cpp" %(ns, name, sub), """\
#include "{ns}.h"
using namespace {ns};
//====================================================
//= struct {ns}::{name}::{sub}
//====================================================
{s}\
""".format(ns = ns, name = name, sub = sub, s = s))

def classdef(ns, c):
	
	b = ["public %s" %(b) for b in bases.get(c, ["object"])]
	b = ": " + ", ".join(b) if len(b) else ""
	
	s = "{c}();".format(c = c)
	s = declares.get(c, {"header": s})["header"]
	s = re.sub(r"\n", r"\n\t", s)

	return """\
//====================================================
//= struct {ns}::{c}
//====================================================
struct {c} {b}
{{
	{s}
}};\
""".format(ns = ns, c = c, s = s, b = b)

def classimpl(ns, c):
	
	s = "{c}::{c}()".format(c = c)
	s = declares.get(c, {"cpp": s})["cpp"]

	i = initlists.get(c, "")
	i = "\n" + i if i else ""
	s += i

	s += "\n{\n}"

	return """\
//====================================================
//= struct {ns}::{c}
//====================================================
{s}\
""".format(ns = ns, c = c, s = s)

# ====================================================
if __name__ == "__main__":

	headerfile("common.h", """\
#include "atd.h"
using namespace atd;
int main(int argc, char **argv);
int frame(int argc, char **argv);
int run(int argc, char **argv);
""".format())

	cppfile("main.cpp", """\
#include "common.h"
int main(int argc, char **argv)
{{
	int r = 0;
	return r;
}}
int frame(int argc, char **argv)
{{
	int r = 0;
	return r;
}}
#include "{ns}.h"
using namespace {ns};
int run(int argc, char **argv)
{{
	int r = 0;
	return r;
}}\
""".format(ns = ns))

	domake()
# ====================================================