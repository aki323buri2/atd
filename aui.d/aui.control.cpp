//aui.control.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::control
//====================================================
control::control(const string &name, const string &text)
: widget(name, text)
{
	creator->style = WS_CHILD | WS_VISIBLE;
	creator->cx = 100;
	creator->cy = 20;
}
