//aui.editbox.h
#ifndef __aui_editbox_h__
#define __aui_editbox_h__
#include "aui.h"
namespace aui {;
struct editbox;
//====================================================
//= struct aui::editbox
//====================================================
struct editbox : public control
{
	editbox(const string &text);
};
}//namespace aui
#endif//__aui_editbox_h__