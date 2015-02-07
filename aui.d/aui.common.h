//aui.common.h
#ifndef __aui_common_h__
#define __aui_common_h__
#include "aui.h"
namespace aui {;
struct common;
struct font;
//====================================================
//= struct aui::common
//====================================================
struct common : public object
{
	common();
};
//====================================================
//= struct aui::font
//====================================================
struct font : public object, public LOGFONT
{
	HFONT hfont;
	string name;
	int points;
	bool bold;
	bool italic;

	font(
		  const string &name = "MS ゴシック"
		, int points = 90
		, bool bold = false 
		, bool italic = false 
	//	, LONG lfHeight = 0
		, LONG lfWidth = 0
		, LONG lfEscapement = 0
		, LONG lfOrientation = 0
	//	, LONG lfWeight = 0
	//	, BYTE lfItalic = 0
		, BYTE lfUnderline = 0
		, BYTE lfStrikeOut = 0
		, BYTE lfCharSet = DEFAULT_CHARSET
		, BYTE lfOutPrecision = 0
		, BYTE lfClipPrecision = 0
		, BYTE lfQuality = 0
		, BYTE lfPitchAndFamily = 0
	//	, TCHAR lfFaceName[LF_FACESIZE] = 0
	);
	~font();
	void create();
	void destroy();
	HFONT setfont(HWND hwnd);
};
}//namespace aui
#endif//__aui_common_h__