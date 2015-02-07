//aui.application.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::application
//====================================================
application::application(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		command.entry(string::utf8(*(argv + i)));
	}
}
HINSTANCE application::hinstance()
{
	return ::GetModuleHandle(NULL);
}
DWORD application::processid()
{
	return ::GetCurrentProcessId();
}
string application::name()
{
	return path::remove_extension(path::remove_file_spec(path()));
}
string application::path()
{
	return path::app_path().utf8();
}
string application::path(const string &relative)
{
	return path::app_path(relative).utf8();
}
bool application::nowindows()
{
	struct check
	{
		static BOOL CALLBACK func(HWND hwnd, LPARAM lparam)
		{
			bool &exists = *((bool *)lparam);
			exists = processid() == widget::processid(hwnd);
			exists = exists && widget::isvisible(hwnd);
			return !exists;
		}
	};
	bool exists = false;
	::EnumDesktopWindows(NULL, check::func, (LPARAM)&exists);
	return !exists;
}
void application::quit(int code)
{
	::PostQuitMessage(code);
}
void application::sleep(DWORD milliseconds)
{
	::Sleep(milliseconds);
}
int application::run()
{
	winmessage m;
	while (true)
	{
		if (m.peek())
		{
			if (!m.get()) break;
			if (m.pre()) continue;
			m.dispatch();
		}
		else
		{
			if (nowindows()) quit(0);
			sleep(1);
		}
	}
	return m.wParam;
}
//====================================================
//= struct aui::winmessage
//====================================================
winmessage::winmessage(
	  HWND		hwnd
	, UINT		message
	, WPARAM	wParam
	, LPARAM	lParam
	, object	*sender
//	, DWORD		time
//	, POINT		pt
)
: sender(sender)
, result(0)
, handled(false)
{
	this->hwnd = hwnd;
	this->message = message;
	this->wParam = wParam;
	this->lParam = lParam;
	this->time = 0;//★
	this->pt.x = 0;//★
	this->pt.y = 0;//★
}

bool winmessage::peek(UINT remove)
{
	return ::PeekMessage(this, NULL, 0, 0, remove) != 0;
}
bool winmessage::get()
{
	return ::GetMessage(this, NULL, 0, 0) > 0;
}
bool winmessage::pre()
{
	if (widget::pretranslate(this)) return true;
	return false;
}
void winmessage::dispatch()
{
	::TranslateMessage(this);
	::DispatchMessage(this);
}
LRESULT CALLBACK winmessage::callwndproc(WNDPROC p, WNDPROC b)
{
	p = p ? p : b;
	return p ? ::CallWindowProc(p, hwnd, message, wParam, lParam) : 0;
}