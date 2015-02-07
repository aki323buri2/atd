//aui.sizer.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::sizer
//====================================================
sizer::sizer(widget *owner)
{
}
//====================================================
//= struct aui::boxsizer
//====================================================
boxsizer::boxsizer(widget *owner)
: sizer(owner)
{
}
//====================================================
//= struct aui::gridsizer
//====================================================
gridsizer::gridsizer(widget *owner)
: sizer(owner)
{
}
