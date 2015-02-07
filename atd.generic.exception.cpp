//atd.generic.exception.cpp
#include "atd.h"
using namespace atd;
using namespace atd::generic;
//====================================================
//= atd::generic::exception
//====================================================
exception::exception(const string &what)
: generic::status(0, what, strings())
{ }
exception::exception(DWORD code, const string &what, const strings &description)
: generic::status(code, what, description)
{ }
exception::exception(struct primitive *primitive)
: generic::status(primitive)
{ }
exception::~exception() throw()
{ }
const char *exception::what() const throw()
{
	return generic::status::what.c_str();
}