//aui.dialog.h
#ifndef __aui_dialog_h__
#define __aui_dialog_h__
#include "aui.h"
namespace aui {;
struct dialog;
//====================================================
//= struct aui::dialog
//====================================================
struct dialog : public widget
{
	dialog(const string &text);
	void oncommand(winmessage &m);
	virtual void onok();
	virtual void oncan();
};
}//namespace aui
#endif//__aui_dialog_h__