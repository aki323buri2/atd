//atd.geneirc.tree.h
#ifndef __ATD_GENEIRC_TREE_H__
#define __ATD_GENEIRC_TREE_H__
#include "atd.h"
namespace atd {;
namespace generic {;
//====================================================
//= atd::generic::treenode
//====================================================
struct treenode : public object 
{
	string text;
	treenode *parent, *child, *prev, *next;
	treenode(const string &text = "");
	~treenode();
	treenode &clear();
	treenode &disconnect();
	treenode &append_to(treenode *node);
	treenode &insert_to(treenode *node);
	treenode *append(treenode *node);
	treenode *insert(treenode *node);
	treenode *lastchild() const;
	int depth() const;
};
//====================================================
//= atd::generic::tree
//====================================================
struct tree : public object 
{
	tree();
	~tree();
	treenode *root() const;
	struct iterator;
	iterator begin() const;
	iterator end  () const;
	iterator find(const string &search) const;
	iterator find(const string &search, iterator from) const;
	iterator find(const string &search, iterator from, iterator to) const;
	void dump(const generic::notify &notify) const;
protected:
	treenode *head;
	treenode *tail;
};
//====================================================
//= atd::generic::tree::iterator
//====================================================
struct tree::iterator 
{
	treenode *value;
	iterator(treenode *value = 0);
	treenode *operator ->() const;
	treenode &operator * () const;
	iterator operator ++();
	iterator operator ++(int);
	bool operator ==(const iterator &that) const;
	bool operator !=(const iterator &that) const;
};
}//namespace generic
}//namespace atd

#endif//__ATD_GENEIRC_TREE_H__