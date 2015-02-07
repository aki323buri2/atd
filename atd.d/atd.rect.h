//atd.rect.h
#ifndef __atd_rect_h__
#define __atd_rect_h__
#include "atd.h"
namespace atd {;
struct point;
struct size;
struct rect;
//====================================================
//= struct atd::point
//====================================================
struct point : public object, public POINT
{
	LONG &left, &top;
	point(LONG x = 0, LONG y = 0);
	point(const point &that);
	point &operator = (const point &that);
};
//====================================================
//= struct atd::size
//====================================================
struct size : public object, public SIZE
{
	LONG &width, &height, &h, &w;
	size(LONG w = 0, LONG h = 0);
	size(const size &that);
	size &operator = (const size &that);
};
//====================================================
//= struct atd::rect
//====================================================
struct rect : public object, public RECT
{
	LONG &x, &y, &r, &b;
	rect(LONG x = 0, LONG y = 0, LONG r = 0, LONG b = 0);
	rect(const rect &that);
	rect &operator = (const rect &that);
	int w() const;
	int h() const;
	int width() const;
	int height() const;
};
}//namespace atd
#endif//__atd_rect_h__