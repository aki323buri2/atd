//aui.widget.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::widget
//====================================================
widget::widget(const string &name, const string &text)
: wc(new struct winclass(name))
, creator(new struct wincreator(name, text))
, listener(new struct winlistener())
, children(new struct winchildren(this))
, font(new struct font())
{
}
widget::~widget()
{
}
HWND widget::hwnd() const
{
	return creator->hwnd;
}
void widget::show(widget *parent)
{
	HWND hwnd = creator->hwnd;
	if (!hwnd)
	{
		if (wc->query())
		{
			wc->name += "_AUI_WINDOW";
		}
		wc->lpfnWndProc = THE_CALLBACK;
		wc->registerclass();

		creator->name = wc->name;
		creator->lpCreateParams = this;
		creator->hwndParent = parent ? parent->hwnd() : 0;
		hwnd = creator->create();

		font->setfont(hwnd);
	}
	children->show();
	showwindow(hwnd);
}
#define PROPKEY "HWND2WIDGET_PROP"
widget *widget::hwnd2widget(HWND hwnd)
{
	return (widget *)::GetProp(hwnd, PROPKEY);
}
widget *widget::hwnd2widget(HWND hwnd, widget *w)
{
	return (widget *)::SetProp(hwnd, PROPKEY, (HANDLE)w);
}
#undef PROPKEY
LRESULT CALLBACK widget::THE_CALLBACK(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	widget *w = hwnd2widget(hwnd);
	if (!w
		&& (message == WM_NCCREATE)
		&& (w = (widget *)(((LPCREATESTRUCT)lparam)->lpCreateParams))
	)
	{
		w->creator->hwnd = hwnd;
		hwnd2widget(hwnd, w);
	}
	if (w && message == WM_DESTROY)
	{
		hwnd2widget(hwnd, 0);
	}
	return w ? w->process(message, wparam, lparam): 0;
}
LRESULT CALLBACK widget::process(UINT message, WPARAM wparam, LPARAM lparam)
{
	winmessage m(hwnd(), message, wparam, lparam);
	return listener->process(m) ? m.result : m.callwndproc(wc->origin, ::DefWindowProc);
}
bool widget::pretranslate(MSG *pmsg)
{
	HWND hwnd = pmsg->hwnd;
	if (classinfo(hwnd).cbWndExtra == DLGWINDOWEXTRA 
		&& ::IsDialogMessage(hwnd, pmsg)
	)
	{
		return true;
	}
	return false;
}
void widget::append(widget *child) { return children->append(child); }
void widget::append(widget &child) { return append(&child); }
void widget::append_to(widget *parent) { return parent->append(this); }
void widget::append_to(widget &parent) { return parent. append(this); }

//====================================================
//= struct aui::winclass
//====================================================
winclass::winclass(
	  const string &name
//	, UINT		cbSize
	, UINT		style
	, WNDPROC	lpfnWndProc
	, int		cbClsExtra
	, int		cbWndExtra
//	, HINSTANCE	hInstance
	, HICON		hIcon
	, HCURSOR	hCursor
	, HBRUSH	hbrBackground
	, LPCTSTR	lpszMenuName
//	, LPCTSTR	lpszClassName
	, HICON		hIconSm
)
: name(name)
, origin(0)
{
	this->cbSize		= sizeof(WNDCLASSEX);//★
	this->style			= style;
	this->lpfnWndProc	= lpfnWndProc;
	this->cbClsExtra	= cbClsExtra;
	this->cbWndExtra	= cbWndExtra;
	this->hInstance		= application::hinstance();//★
	this->hIcon			= hIcon;
	this->hCursor		= hCursor;
	this->hbrBackground	= hbrBackground;
	this->lpszMenuName	= lpszMenuName;
	this->lpszClassName	= 0;//★
	this->hIconSm		= hIconSm;
}
bool winclass::query()
{
	string sjis = name.sjis();
	bool success = ::GetClassInfoEx(NULL, sjis.c_str(), this);
	origin = success ? lpfnWndProc : 0;
	return success;
}
ATOM winclass::registerclass()
{
	string sjis = name.sjis();
	lpszClassName = sjis.c_str();
	ATOM a = ::RegisterClassEx(this);
	return a;
}
//====================================================
//= struct aui::wincreator
//====================================================
wincreator::wincreator(
	  const string &name
	, const string &text
	, LPVOID lpCreateParams
//	, HANDLE hInstance
	, HMENU hMenu
	, HWND hwndParent
	, int cy
	, int cx
	, int y
	, int x
	, LONG style
//	, LPCSTR lpszName
//	, LPCSTR lpszClass
	, DWORD dwExStyle
)
: hwnd(0)
, name(name)
, text(text)
{
	this->lpCreateParams= lpCreateParams;
	this->hInstance		= application::hinstance();//★
	this->hMenu			= hMenu;
	this->hwndParent	= hwndParent;
	this->cy			= cy;
	this->cx			= cx;
	this->y				= y;
	this->x				= x;
	this->style			= style;
	this->lpszName		= 0;//★
	this->lpszClass		= 0;//★
	this->dwExStyle		= dwExStyle;
}
HWND wincreator::create()
{
	struct { string name, text; } sjis;
	sjis.name = name.sjis();
	sjis.text = text.sjis();
	hwnd = ::CreateWindowEx(
		  dwExStyle			// 拡張ウィンドウスタイル
		, sjis.name.c_str()	// 登録されているクラス名
		, sjis.text.c_str()	// ウィンドウ名
		, style				// ウィンドウスタイル
		, x					// ウィンドウの横方向の位置
		, y					// ウィンドウの縦方向の位置
		, cx				// ウィンドウの幅
		, cy				// ウィンドウの高さ
		, hwndParent		// 親ウィンドウまたはオーナーウィンドウのハンドル
		, hMenu				// メニューハンドルまたは子識別子
		, hInstance			// アプリケーションインスタンスのハンドル
		, lpCreateParams	// ウィンドウ作成データ
	);
	return hwnd;
}
//====================================================
//= struct aui::winlistener
//====================================================
winlistener::winlistener()
{
}
bool winlistener::process(winmessage &m)
{
	fire(m.message, m);
	return m.handled;
}
//====================================================
//= struct aui::winchildren
//====================================================
winchildren::winchildren(widget *parent)
: parent(parent)
{
}
winchildren::~winchildren()
{
	for (iterator i = begin(), e = end(); i != e; ++i)
	{
		delete *i;
		*i = 0;
	}
}
void winchildren::show()
{
	for (iterator i = begin(), e = end(); i != e; ++i)
	{
		widget *w = *i;
		if (!w) continue;
		w->show(parent);
	}
}
void winchildren::append(widget *child)
{
	push_back(child);
	if (parent->hwnd()) child->show(parent);
}