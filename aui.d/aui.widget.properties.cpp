//aui.widget.properties.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::widget::properties
//====================================================
//properties --static--
LRESULT widget::send(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return ::SendMessage(hwnd, message, wparam, lparam);
}
bool widget::post(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return ::PostMessage(hwnd, message, wparam, lparam) != 0;
}
DWORD widget::processid(HWND hwnd)
{
	DWORD id; ::GetWindowThreadProcessId(hwnd, &id); return id;
}
bool widget::iswindow(HWND hwnd)
{
	return ::IsWindow(hwnd) != 0;
}
bool widget::isvisible(HWND hwnd)
{
	return ::IsWindowVisible(hwnd) != 0;
}
bool widget::isenabled(HWND hwnd)
{
	return ::IsWindowEnabled(hwnd) != 0;
}
bool widget::isactive(HWND hwnd)
{
	return hwnd == ::GetActiveWindow();
}
bool widget::isfocused(HWND hwnd)
{
	return hwnd == ::GetFocus();
}
void widget::showwindow(HWND hwnd, int sw)
{
	::ShowWindow(hwnd, sw);
}
HWND widget::activate(HWND hwnd)
{
	return ::SetActiveWindow(hwnd);
}
HWND widget::focus(HWND hwnd)
{
	return ::SetFocus(hwnd);
}
void widget::hide(HWND hwnd)
{
	showwindow(hwnd, SW_HIDE);
}
void widget::close(HWND hwnd)
{
	send(hwnd, WM_CLOSE);
}
void widget::destroy(HWND hwnd)
{
	::DestroyWindow(hwnd);
}
rect widget::windowrect(HWND hwnd)
{
	rect r; ::GetWindowRect(hwnd, &r); return r;

}
rect widget::clientrect(HWND hwnd)
{
	rect r; ::GetClientRect(hwnd, &r); return r;
}
point widget::screen2client(HWND hwnd, const point &pt)
{
	point p = pt; ::ScreenToClient(hwnd, &p); return p;
}
point widget::client2screen(HWND hwnd, const point &pt)
{
	point p = pt; ::ClientToScreen(hwnd, &p); return p;
}
rect widget::moveto(HWND hwnd, const point &pt)
{
	::SetWindowPos(hwnd, NULL, pt.x, pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	return windowrect(hwnd);
}
rect widget::extend(HWND hwnd, const size &sz)
{
	::SetWindowPos(hwnd, NULL, 0, 0, sz.w, sz.h, SWP_NOZORDER | SWP_NOMOVE);
	return windowrect(hwnd);
}
rect widget::move(HWND hwnd, const rect &rc)
{
	::SetWindowPos(hwnd, NULL, rc.x, rc.y, rc.w(), rc.h(), SWP_NOZORDER | SWP_NOMOVE);
	return windowrect(hwnd);
}
string widget::classname(HWND hwnd)
{
	string s(0x100, 0);
	::GetClassName(hwnd, &s[0], s.size());
	return string::utf8(s.c_str());
}
WNDCLASSEX widget::classinfo(HWND hwnd)
{
	string name = classname(hwnd);
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	::GetClassInfoEx(application::hinstance(), name.sjis().c_str(), &wc);
	return wc;
}

//properties --cdcall--
LRESULT widget::send(UINT message, WPARAM wparam, LPARAM lparam)
{
	return send(hwnd(), message, wparam, lparam);
}
bool widget::post(UINT message, WPARAM wparam, LPARAM lparam)
{
	return post(hwnd(), message, wparam, lparam);
}
DWORD widget::processid() { return processid(hwnd()); }
bool widget::iswindow() { return iswindow(hwnd()); }
bool widget::isvisible() { return isvisible(hwnd()); }
bool widget::isenabled() { return isenabled(hwnd()); }
bool widget::isactive() { return isactive(hwnd()); }
bool widget::isfocused() { return isfocused(hwnd()); }
void widget::showwindow(int sw) { return showwindow(hwnd(), sw); }
HWND widget::activate() { return activate(hwnd()); }
HWND widget::focus() { return focus(hwnd()); }
void widget::hide() { return hide(hwnd()); }
void widget::close() { return close(hwnd()); }
void widget::destroy() { return destroy(hwnd()); }
rect widget::windowrect() { return windowrect(hwnd()); }
rect widget::clientrect() { return clientrect(hwnd()); }
point widget::screen2client(const point &pt) { return screen2client(hwnd(), pt); }
point widget::client2screen(const point &pt) { return client2screen(hwnd(), pt); }
rect widget::moveto(const point &pt) { return moveto(hwnd(), pt); }
rect widget::extend(const size &sz) { return extend(hwnd(), sz); }
rect widget::move(const rect &rc) { return move(hwnd(), rc); }
string widget::classname() { return classname(hwnd()); }
WNDCLASSEX widget::classinfo() { return classinfo(hwnd()); }