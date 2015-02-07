// atd.generic.property.cpp
#include "atd.h"
using namespace atd;
using namespace atd::generic;
#include "picojson.h"
#include <fstream>
#include <sstream>
//====================================================
//= struct property
//====================================================
property::property(const string &name, const string &value)
: name(name), value(value)
{ }
bool property::isnull(const string &s)
{
	return s == "" || s == "null";
}
string property::demo(int nlen) const 
{
	return string::format("%-*s: %s"
		, nlen
		, name.c_str()
		, value.c_str()
		);
}
//====================================================
//= struct property::primitive
//====================================================
//====================================================
//= struct property::hashforlist
//====================================================
property::hashforlist::hashforlist(owner_type *owner)
: owner(owner)
{
}
property::hashforlist::owner_type::iterator 
property::hashforlist::iterator_of(const string &name)
{
	iterator i = find(name);
	if (i == end())
	{
		owner_type::iterator add = owner->insert(owner->end(), property(name, ""));
		insert(value_type(name, add));
		return add;
	}
	else 
	{
		return i->second;
	}
}
string property::hashforlist::at(const string &name, const string &backup) const
{
	const_iterator i = find(name);
	return i == end() ? backup : i->second->value;
}
//====================================================
//= struct property::hashforvector
//====================================================
property::hashforvector::hashforvector(owner_type *owner)
: owner(owner)
{
}
property::hashforvector::owner_type::iterator 
property::hashforvector::iterator_of(const string &name)
{
	iterator i = find(name);
	if (i == end())
	{
		owner_type::iterator add = owner->insert(owner->end(), property(name, ""));
		insert(value_type(name, add - owner->begin()));
		return add;
	}
	else 
	{
		return owner->begin() + i->second;
	}
}
string property::hashforvector::at(const string &name, const string &backup) const
{
	const_iterator i = find(name);
	return i == end() ? backup : (owner->begin() + i->second)->value;
}
//====================================================
//= struct property::table
//====================================================
template <typename base_t, typename hash_t>
property::table<base_t, hash_t>::table()
: hash(this)
{ }
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::clear()
{
	hash.clear();
	base_t::clear();
}
template <typename base_t, typename hash_t>
bool property::table<base_t, hash_t>::has_key(const string &name) const
{
	return hash.find(name) != hash.end();
}
template <typename base_t, typename hash_t>
string &property::table<base_t, hash_t>::operator [](const string &name)
{
	return value_of(name);
}
template <typename base_t, typename hash_t>
string &property::table<base_t, hash_t>::value_of(const string &name)
{
	return hash.iterator_of(name)->value;
}
template <typename base_t, typename hash_t>
string property::table<base_t, hash_t>::at(const string &name, const string &backup) const
{
	return hash.at(name, backup);
}
//demo
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::demo(const generic::notify &notify, int nlen) const
{
	for (typename base_t::const_iterator i = base_t::begin(), e = base_t::end()
		; i != e; ++i)
	{
		notify(i->demo(nlen));
	}
}
//====================================================
//json encode & decode
//====================================================
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::json_decode(std::istream &is)
{
	picojson::value v(picojson::object_type, false);
	is >> v;
	if (!v.is<picojson::object>()) return;
	
	picojson::object &o = v.get<picojson::object>();
	for (picojson::object::iterator i = o.begin(), e = o.end()
		; i != e; ++i)
	{
		value_of(i->first) = i->second.to_str();
	}
}
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::json_encode(std::ostream &os) const
{
	picojson::value v(picojson::object_type, false);
	picojson::object &o = v.get<picojson::object>();
	for (typename base_t::const_iterator i = base_t::begin(), e = base_t::end()
		; i != e; ++i)
	{
		o[i->name] = i->value;
	}
	os << o;
}
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::json_decode(const string &json)
{
	if (path::file_exists(json.sjis()))
	{
		std::ifstream ifs(json.sjis().c_str());
		json_decode(ifs);
	}
	else
	{
		std::istringstream iss(json);
		json_decode(iss);
	}
}
template <typename base_t, typename hash_t>
string property::table<base_t, hash_t>::json_encode() const
{
	std::ostringstream oss;
	json_encode(oss);
	return oss.str();
}
//====================================================
//load & save profile (ini)
//====================================================
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::load_profile(const string &path, const string &section)
{
	//バッファが足らなかった場合、GetPrivateProfileSection()はバッファサイズ-2を返す
	string buf(0x100, 0);
	int size = buf.size();
	while (true)
	{
		//読む
		int done = ::GetPrivateProfileSection(
			section.sjis().c_str()
			, &buf[0], size
			, path.sjis().c_str()
			);
		if (done == size - 2)
		{
			//バッファ足らないので２倍！
			buf.resize(size *= 2, 0);
			continue;
		}
		else 
		{
			//バッファ足りた！
			break;
		}
	}
	//[name]=[value]\0[name]=[value]\0...=[value]\0\0
	string name, value, *p = &name;
	int stage = 0;
	//'='で区切って'\0'でエントリして'\0\0'でループを終了する
	for (string::iterator i = buf.begin(), e = buf.end(); i != e; ++i)
	{
		uchar c = (uchar )*i;
		if (c == '=')
		{
			//'='なので積む対象を切り替える
			p = &value;
			stage++;
			//続ける
			continue;
		}
		else if (c == '\0')
		{
			//'\0\0'なのでブレイク
			if (stage == 0) break;
			//ペアをエントリする
			value_of(name.utf8()) = value.utf8();
			//メモをリセット
			name = "";
			value = "";
			p = &name;
			stage = 0;
			//続ける
			continue;
		}
		//メモに積む
		*p += c;
	}

}
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::save_profile(const string &path, const string &section) const
{
	string s;
	for (typename base_t::const_iterator i = base_t::begin(), e = base_t::end()
		; i != e; ++i)
	{
		s += i->name.sjis();
		s += '=';
		s += i->value.sjis();
		s += '\0';
	}
	s += '\0';
	//書く
	::WritePrivateProfileSection(section.sjis().c_str(), &s[0], path.sjis().c_str());
}
//====================================================
//load primitive structure
//====================================================
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::load_primitive(struct primitive *primitive)
{
	for (struct primitive *p = primitive; p->name; p++)
	{
		value_of(p->name) = p->value;
	}
}
//====================================================
//supply
//====================================================
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::supply(const table &that)
{
	for (typename base_t::const_iterator i = that.begin(), e = that.end()
		; i != e; ++i)
	{
		const string &name = i->name;
		const string &value = i->value;
		if (isnull(value)) continue;
		if (has_key(name) && !isnull(at(name))) continue;
		value_of(name) = value;
	}
}
//====================================================
//overwrap
//====================================================
template <typename base_t, typename hash_t>
void property::table<base_t, hash_t>::overwrap(const table &that)
{
	for (typename base_t::const_iterator i = that.begin(), e = that.end()
		; i != e; ++i)
	{
		const string &name = i->name;
		const string &value = i->value;
		value_of(name) = value;
	}
}
//====================================================
//explicit instatiation
//====================================================
template struct property::table<std::list<property>, property::hashforlist>;
template struct property::table<std::vector<property>, property::hashforvector>;
//====================================================
//= struct property::list
//====================================================
//====================================================
//= struct property::vector
//====================================================
//====================================================