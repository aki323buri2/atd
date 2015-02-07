// atd.fileop.h
#ifndef __ATD_FILEOP_H__
#define __ATD_FILEOP_H__
#include "atd.h"
namespace atd {;
//====================================================
//= atd::fileop
//====================================================
struct fileop
{
	struct book;
	static struct book book;
	static DWORD operation(const string &from, const string &to, UINT func);
	static DWORD move(const string &from, const string &to);
	static DWORD copy(const string &from, const string &to);
	static DWORD remove(const string &from);
};
//====================================================
//= atd::fileop::book
//====================================================
struct fileop::book : public generic::status::book
{
	book();
};
}//namespace atd
#endif//__ATD_FILEOP_H__