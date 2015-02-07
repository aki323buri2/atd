//atd.generic.status.h
#ifndef __ATD_GENERIC_STATUS_H__
#define __ATD_GENERIC_STATUS_H__
#include "atd.h"
namespace atd {;
namespace generic {; 
//====================================================
//= atd::generic::status
//====================================================
struct status : public object
{
	DWORD code;
	string what;
	strings description;
	struct primitive;

	status(DWORD code, const string &what, const strings &description);
	status(const primitive *primitive);

	static status empty();
	static status lasterror();
	static status lasterror(DWORD code);

	struct primitive 
	{
		DWORD code;
		const char *what;
		const char *description[100];
	};

	//★
	void display(const generic::notify &notify) const;

	//====================================================
	//= atd::generic::status::book
	//====================================================
	struct book : public object, public std::vector<status>
	{
		book();
		book(const status::primitive *primitive);
		book &append(const status::primitive *primitive);
		status &operator [](DWORD code);
		status &push(const struct status &status);
		status pop(DWORD code, const status &backup = status::empty());
		status status_of(DWORD code, const status &backup = status::empty()) const;
		void clear();
	protected:
		struct keys : public object, public std::map<DWORD, size_t>
		{
		};
		keys keys;
	public:
		//★
		void display(const generic::notify &notify) const;
	};
};
}//namespace generic
}//namespace atd
#endif//__ATD_GENERIC_STATUS_H__