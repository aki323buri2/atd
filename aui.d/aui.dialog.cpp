//aui.dialog.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::dialog
//====================================================
dialog::dialog(const string &text)
: widget("DIALOG_AUI_WINDOW", text)
{
	wc->hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wc->hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc->cbWndExtra = DLGWINDOWEXTRA;
	creator->style = WS_OVERLAPPEDWINDOW;
	creator->cx = 500;
	creator->cy = 500;

	listener->add(WM_COMMAND, &dialog::oncommand, this);
}
void dialog::oncommand(winmessage &m)
{
	switch (m.wParam)
	{
	case IDOK: onok(); break;
	case IDCANCEL: oncan(); break;
	}
}
void dialog::onok()
{
}
void dialog::oncan()
{
	close();
}