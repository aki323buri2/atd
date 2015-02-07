//atd.generic.tree.cpp
#include "atd.h"
using namespace atd;
using namespace atd::generic;
//====================================================
//= atd::generic::treenode
//====================================================
treenode::treenode(const string &text)
: text(text)
, parent(0)
, child(0)
, prev(0)
, next(0)
{ }
treenode::~treenode()
{
	// cout << "!!! " << this << ": " << text << endl;
}
treenode &treenode::clear()
{
	for (treenode *node = child; node; )
	{
		treenode *next = node->next;
		node->clear();
		delete node;
		node = next;
	}
	return *this;
}
treenode &treenode::disconnect()
{
	if (prev) prev->next = next;
	if (next) next->prev = prev;
	if (!prev && parent) parent->child = next;
	parent = prev = next = 0;
	return *this;
}
treenode &treenode::append_to(treenode *node)
{
	if (node == this) return *this;
	disconnect();
	parent = node;
	prev = parent->lastchild();
	if (prev)
	{
		prev->next = this;
	}
	else 
	{
		parent->child = this;
	}
	return *this;
}
treenode &treenode::insert_to(treenode *node)
{
	if (node == this) return *this;
	disconnect();
	parent = node->parent;
	prev = node->prev;
	next = node;
	if (prev) prev->next = this;
	if (next) next->prev = this;
	if (!prev && parent) parent->child = this;
	return *this;
}
treenode *treenode::append(treenode *node)
{
	node->append_to(this);
	return node;
}
treenode *treenode::insert(treenode *node)
{
	node->insert_to(this);
	return node;
}
treenode *treenode::lastchild() const
{
	treenode *last = child;
	for (; last && last->next; last = last->next);
	return last;
}
int treenode::depth() const
{
	int depth = 0;
	for (treenode *node = (treenode *)this; node && node->parent; node = node->parent)
	{
		depth++;
	}
	return depth;
}
//====================================================
//= atd::generic::tree
//====================================================
tree::tree()
: head(new treenode("__head__"))
, tail(new treenode("__tail__"))
{
	head->next = tail;
	tail->prev = head;
	tail->insert(new treenode("root"));
}
tree::~tree()
{
	treenode *root = head->next;
	if (root != tail) 
	{
		root->clear();
		delete root;
	}
	delete head;
	delete tail;
}
treenode *tree::root() const { return head->next; }
tree::iterator tree::begin() const { return root(); }
tree::iterator tree::end  () const { return tail; }
tree::iterator tree::find(const string &search) const
{
	return find(search, begin(), end());
}
tree::iterator tree::find(const string &search, iterator from) const
{
	return find(search, from, end());
}
tree::iterator tree::find(const string &search, iterator from, iterator to) const
{
	for (iterator i = from, e = to; i != e; ++i)
	{
		if (i->text == search) return i;
	}
	return to;
}
void tree::dump(const generic::notify &notify) const
{
	notify("=== Tree dump ===");
	for (iterator i = begin(), e = end(); i != e; ++i)
	{
		string s;
		for (int d = 0, depth = i->depth(); d < depth; d++)
		{
			s += "    ";
		}
		// s += string::format("0x%04x: %s", i.value, i->text.c_str());
		s += string::format("%s", i->text.c_str());
		notify(s);
	}
	notify("");
}
//====================================================
//= atd::generic::tree::iterator
//====================================================
tree::iterator::iterator(treenode *value)
: value(value)
{ }
treenode *tree::iterator::operator ->() const { return  value; }
treenode &tree::iterator::operator * () const { return *value; }
tree::iterator tree::iterator::operator ++()
{
	if (value->child)
	{
		value = value->child;
	}
	else 
	{
		while (value && !value->next)
		{
			value = value->parent;
		}
		if (value) value = value->next;
	}
	return *this;
}
tree::iterator tree::iterator::operator ++(int)
{
	iterator memo = *this;
	++(*this);
	return memo;
}
bool tree::iterator::operator ==(const iterator &that) const
{
	return value == that.value;
}
bool tree::iterator::operator !=(const iterator &that) const
{
	return value != that.value;
}
/*
*/