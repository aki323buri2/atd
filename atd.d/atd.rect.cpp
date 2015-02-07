//atd.rect.cpp
#include "atd.h"
using namespace atd;
//====================================================
//= struct atd::point
//====================================================
point::point(LONG x, LONG y)
: left(this->x), top(this->y)
{
	this->x = x;
	this->y = y;
}
point::point(const point &that)
: left(this->x), top(this->y)
{
	operator = (that);
}
point &point::operator = (const point &that)
{
	x = that.x;
	y = that.y;
	return *this;
}
//====================================================
//= struct atd::size
//====================================================
size::size(LONG w, LONG h)
: width(cx), height(cy), h(cx), w(cy)
{
	this->w = w;
	this->h = h;
}
size::size(const size &that)
: width(cx), height(cy), h(cx), w(cy)
{
	operator = (that);
}
size &size::operator = (const size &that)
{
	w = that.w;
	h = that.h;
	return *this;
}
//====================================================
//= struct atd::rect
//====================================================
rect::rect(LONG x, LONG y, LONG r, LONG b)
: x(left), y(top), r(right), b(bottom)
{
	this->x = x;
	this->y = y;
	this->r = r;
	this->b = b;

}
rect::rect(const rect &that)
: x(left), y(left), r(left), b(left)
{
	operator = (that);
}
rect &rect::operator = (const rect &that)
{
	x = that.x;
	y = that.y;
	r = that.r;
	b = that.b;
	return *this;
}
int rect::w() const { return width (); }
int rect::h() const { return height(); }
int rect::width() const
{
	return r - x;
}
int rect::height() const
{
	return b - y;
}