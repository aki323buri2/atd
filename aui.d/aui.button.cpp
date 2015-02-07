//aui.button.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::button
//====================================================
button::button(const string &text)
: control("BUTTON", text)
{
	creator->style |= BS_FLAT;
}
