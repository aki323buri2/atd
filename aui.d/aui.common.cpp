//aui.common.cpp
#include "aui.h"
using namespace aui;
//====================================================
//= struct aui::common
//====================================================
common::common()
{
}
//====================================================
//= struct aui::font
//====================================================
font::font(
	  const string &name
	, int points
	, bool bold
	, bool italic
//	, LONG lfHeight
	, LONG lfWidth
	, LONG lfEscapement
	, LONG lfOrientation
//	, LONG lfWeight
//	, BYTE lfItalic
	, BYTE lfUnderline
	, BYTE lfStrikeOut
	, BYTE lfCharSet
	, BYTE lfOutPrecision
	, BYTE lfClipPrecision
	, BYTE lfQuality
	, BYTE lfPitchAndFamily
//	, TCHAR lfFaceName[LF_FACESIZE]
)
: hfont(0)
, name(name)
, points(points)
, bold(bold)
, italic(italic)
{
	this->lfHeight			= 0;//★
	this->lfWidth			= lfWidth;
	this->lfEscapement		= lfEscapement;
	this->lfOrientation		= lfOrientation;
	this->lfWeight			= 0;//★
	this->lfItalic			= 0;//★
	this->lfUnderline		= lfUnderline;
	this->lfStrikeOut		= lfStrikeOut;
	this->lfCharSet			= lfCharSet;
	this->lfOutPrecision	= lfOutPrecision;
	this->lfClipPrecision	= lfClipPrecision;
	this->lfQuality			= lfQuality;
	this->lfPitchAndFamily	= lfPitchAndFamily;
	::memset(lfFaceName, 0, sizeof(lfFaceName));

	create();
}
font::~font()
{
	destroy();
}
void font::create()
{
	struct hdc
	{
		HDC v;
		hdc() : v(::GetDC(NULL)) { }
		~hdc() { ::ReleaseDC(NULL, v); }
	};
	struct min
	{
		static size_t compare(size_t a, size_t b) { return a < b ? a : b; }
	};

	destroy();

	//point -> twip
	lfHeight = ::MulDiv(points, ::GetDeviceCaps(hdc().v, LOGPIXELSY), 720);

	//bold and italic
	lfWeight = bold ? FW_BOLD : FW_NORMAL;
	lfItalic = italic ? 1 : 0;
	
	//face name
	string sjis = name.sjis();
	::memcpy(lfFaceName, &sjis[0], min::compare(sizeof(lfFaceName), sjis.size()));

	hfont = ::CreateFontIndirect(this);
}
void font::destroy()
{
	if (hfont) ::DeleteObject(hfont);
	hfont = 0;
}
HFONT font::setfont(HWND hwnd)
{
	return (HFONT)widget::send(hwnd, WM_SETFONT, (WPARAM)hfont, TRUE);
}