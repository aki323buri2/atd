//aui.sizer.h
#ifndef __aui_sizer_h__
#define __aui_sizer_h__
#include "aui.h"
namespace aui {;
struct sizer;
struct boxsizer;
struct gridsizer;
//====================================================
//= struct aui::sizer
//====================================================
struct sizer : public object
{
	sizer(widget *owner);
};
//====================================================
//= struct aui::boxsizer
//====================================================
struct boxsizer : public sizer
{
	boxsizer(widget *owner);
};
//====================================================
//= struct aui::gridsizer
//====================================================
struct gridsizer : public sizer
{
	gridsizer(widget *owner);
};
}//namespace aui
#endif//__aui_sizer_h__