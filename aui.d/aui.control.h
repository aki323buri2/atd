//aui.control.h
#ifndef __aui_control_h__
#define __aui_control_h__
#include "aui.h"
namespace aui {;
struct control;
//====================================================
//= struct aui::control
//====================================================
struct control : public widget
{
	control(const string &name, const string &text);
};
}//namespace aui
#endif//__aui_control_h__