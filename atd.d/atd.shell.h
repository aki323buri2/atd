// atd.shell.h
#ifndef __ATD_SHELL_H__
#define __ATD_SHELL_H__
#include "atd.h"
namespace atd {;
//==============================================================
//= struct atd::shell
//==============================================================
struct shell 
{
	static strings execute(const string &command
		, const generic::notify &notify
		, const string &currentdirectory = ""
		, const string &prefix = DEFAULT_RESULT_PREFIX
		);
	static void error(const char *format, ...);
	
	static const char *DEFAULT_RESULT_PREFIX;
};
//==============================================================
}//namespace atd
#endif//__ATD_SHELL_H__