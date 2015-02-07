//aui.application.h
#ifndef __aui_application_h__
#define __aui_application_h__
#include "aui.h"
namespace aui {;
struct application;
struct winmessage;
//====================================================
//= struct aui::application
//====================================================
struct application : public object
{
	strings command;

	application(int argc, char **argv);
	static HINSTANCE hinstance();
	static DWORD processid();
	static string name();
	static string path();
	static string path(const string &relative);
	static bool nowindows();
	static void quit(int code = 0);
	static void sleep(DWORD milliseconds);
	static int run();
};
//====================================================
//= struct aui::winmessage
//====================================================
struct winmessage : public object, public MSG
{
	object *sender;
	LRESULT result;
	bool handled;

	winmessage(
		  HWND		hwnd = 0
		, UINT		message = 0
		, WPARAM	wParam = 0
		, LPARAM	lParam = 0
		, object	*sender = 0
	//	, DWORD		time = 0
	//	, POINT		pt = 0
	);
	bool peek(UINT remove = PM_NOREMOVE);
	bool get();
	bool pre();
	void dispatch();
	LRESULT CALLBACK callwndproc(WNDPROC p, WNDPROC b = 0);
};
}//namespace aui
#endif//__aui_application_h__