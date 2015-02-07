// atd.generic.property.h
#ifndef __ATD_GENERIC_PROPERTY_H__
#define __ATD_GENERIC_PROPERTY_H__
#include "atd.h"
#include <list>
namespace atd {;
namespace generic {;
//====================================================
//= struct property
//====================================================
struct property : public object
{
	string name, value;
	property(const string &name, const string &value);

	static bool isnull(const string &s);
	string demo(int nlen = 15) const;

	struct primitive;
	struct hashforlist;
	struct hashforvector;
	template <typename base_t, typename hash_t> struct table;
	struct list;
	struct vector;
};
//====================================================
//= struct property::primitive
//====================================================
struct property::primitive
{
	const char *name, *value;
};
//====================================================
//= struct property::hashforlist
//====================================================
struct property::hashforlist 
: public object, public std::map<string, std::list<property>::iterator>
{
	typedef std::list<property> owner_type;
	owner_type *owner;
	hashforlist(owner_type *owner);
	owner_type::iterator iterator_of(const string &name);
	string at(const string &name, const string &backup = "") const;
};
//====================================================
//= struct property::hashforvector
//====================================================
struct property::hashforvector
: public object, public std::map<string, int>
{
	typedef std::vector<property> owner_type;
	owner_type *owner;
	hashforvector(owner_type *owner);
	owner_type::iterator iterator_of(const string &name);
	string at(const string &name, const string &backup = "") const;
};
//====================================================
//= struct property::table
//====================================================
template <typename base_t, typename hash_t> struct property::table
: public object, public base_t
{
	hash_t hash;
	table();
	void clear();
	bool has_key(const string &name) const;
	string &operator [](const string &name);
	string &value_of(const string &name);
	string at(const string &name, const string &backup = "") const;

	void demo(const generic::notify &notify, int nlen = 15) const;

	void json_decode(std::istream &is);
	void json_encode(std::ostream &os) const;
	void json_decode(const string &json);
	string json_encode() const;

	void load_profile(const string &path, const string &section);
	void save_profile(const string &path, const string &section) const;

	void load_primitive(struct primitive *primitive);

	void supply(const table &that);
	void overwrap(const table &that);
};
//====================================================
//= struct property::list
//====================================================
struct property::list : public table<std::list<property>, hashforlist>
{
};
//====================================================
//= struct property::vector
//====================================================
struct property::vector : public table<std::vector<property>, hashforvector>
{
};
//====================================================
}//namespace atd
}//namespace generic
#endif//__ATD_GENERIC_PROPERTY_H__