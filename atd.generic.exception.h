//atd.generic.exception.h
#ifndef __ATD_GENERIC_EXCEPTION_H__
#define __ATD_GENERIC_EXCEPTION_H__
#include "atd.h"
namespace atd {;
namespace generic {;
//====================================================
//= atd::generic::exception
//====================================================
struct exception : public generic::status, public std::exception
{
	exception(const string &what);
	exception(DWORD code, const string &what, const strings &description);
	exception(struct primitive *primitive);
	~exception() throw();
	const char *what() const throw();
};
}//namespace generic
}//namespace atd
#endif//__ATD_GENERIC_EXCEPTION_H__