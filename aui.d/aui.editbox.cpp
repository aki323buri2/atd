//aui.editbox.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::editbox
//====================================================
editbox::editbox(const string &text)
: control("EDIT", text)
{
	creator->style |= ES_AUTOHSCROLL;
}
