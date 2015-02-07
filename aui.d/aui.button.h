//aui.button.h
#ifndef __aui_button_h__
#define __aui_button_h__
#include "aui.h"
namespace aui {;
struct button;
//====================================================
//= struct aui::button
//====================================================
struct button : public control
{
	button(const string &text);
};
}//namespace aui
#endif//__aui_button_h__