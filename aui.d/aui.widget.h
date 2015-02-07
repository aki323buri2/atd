//aui.widget.h
#ifndef __aui_widget_h__
#define __aui_widget_h__
#include "aui.h"
namespace aui {;
struct widget;
struct winclass;
struct wincreator;
struct winlistener;
struct winchildren;
//====================================================
//= struct aui::widget
//====================================================
struct widget : public object
{
	struct winclass *wc;
	struct wincreator *creator;
	struct winlistener *listener;
	struct winchildren *children;
	struct font *font;

	widget(const string &name, const string &text);
	~widget();
	HWND hwnd() const;
	void show(widget *parent = 0);
	static widget *hwnd2widget(HWND hwnd);
	static widget *hwnd2widget(HWND hwnd, widget *w);
	static LRESULT CALLBACK THE_CALLBACK(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK process(UINT message, WPARAM wparam, LPARAM lparam);

	static bool pretranslate(MSG *pmsg);
	void append(widget *child);
	void append(widget &child);
	void append_to(widget *parent);
	void append_to(widget &parent);

	//properties --static--
	static LRESULT send(HWND hwnd, UINT message, WPARAM wparam = 0, LPARAM lparam = 0);
	static bool post(HWND hwnd, UINT message, WPARAM wparam = 0, LPARAM lparam = 0);
	static DWORD processid(HWND hwnd);
	static bool iswindow(HWND hwnd);
	static bool isvisible(HWND hwnd);
	static bool isenabled(HWND hwnd);
	static bool isactive(HWND hwnd);
	static bool isfocused(HWND hwnd);
	static void showwindow(HWND hwnd, int sw = SW_SHOW);
	static HWND activate(HWND hwnd);
	static HWND focus(HWND hwnd);
	static void hide(HWND hwnd);
	static void close(HWND hwnd);
	static void destroy(HWND hwnd);
	static rect windowrect(HWND hwnd);
	static rect clientrect(HWND hwnd);
	static point screen2client(HWND hwnd, const point &pt);
	static point client2screen(HWND hwnd, const point &pt);
	static rect moveto(HWND hwnd, const point &pt);
	static rect extend(HWND hwnd, const size &sz);
	static rect move(HWND hwnd, const rect &rc);
	static string classname(HWND hwnd);
	static WNDCLASSEX classinfo(HWND hwnd);

	//properties --cdcall--
	LRESULT send(UINT message, WPARAM wparam = 0, LPARAM lparam = 0);
	bool post(UINT message, WPARAM wparam = 0, LPARAM lparam = 0);
	DWORD processid();
	bool iswindow();
	bool isvisible();
	bool isenabled();
	bool isactive();
	bool isfocused();
	void showwindow(int sw = SW_SHOW);
	HWND activate();
	HWND focus();
	void hide();
	void close();
	void destroy();
	rect windowrect();
	rect clientrect();
	point screen2client(const point &pt);
	point client2screen(const point &pt);
	rect moveto(const point &pt);
	rect extend(const size &sz);
	rect move(const rect &rc);
	string classname();
	WNDCLASSEX classinfo();

};
//====================================================
//= struct aui::winclass
//====================================================
struct winclass : public object, public WNDCLASSEX
{
	string name;
	WNDPROC origin;

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
	);
	bool query();
	ATOM registerclass();
};
//====================================================
//= struct aui::wincreator
//====================================================
struct wincreator : public object, public CREATESTRUCT
{
	HWND hwnd;
	string name;
	string text;

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
	);
	HWND create();
};
//====================================================
//= struct aui::winlistener
//====================================================
struct winlistener : public generic::listener<winmessage &>
{
	winlistener();
	bool process(winmessage &m);
};
//====================================================
//= struct aui::winchildren
//====================================================
struct winchildren : public object, public std::vector<widget *>
{
	widget *parent;

	winchildren(widget *parent = 0);
	~winchildren();
	void show();
	void append(widget *child);
};
}//namespace aui
#endif//__aui_widget_h__