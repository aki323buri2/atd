# wizard.py 
import sys
import os
import re
release = False
release = True
def make(filename, s):
	if release and os.path.isfile(filename):
		return
	with open(filename, "w", encoding = "utf-8") as f:
		f.write(s)

def guard(filename):
	return "__%s__" %(filename.replace(".", "_"))

def headerfile(filename, s):
	make(filename, """\
//{filename}
#ifndef {guard}
#define {guard}
{s}
#endif//{guard}\
""".format(filename = filename, s = s, guard = guard(filename)))

def cppfile(filename, s):
	make(filename, """\
//{filename}
{s}\
""".format(filename = filename, s = s))

# ====================================================
ns = "atd"
names = [
	"rect", 
]
classes = {
	"rect": [
		"point", 
		"size", 
		"rect", 
	], 
}
bases = {
	"rect": ["object", "RECT"], 
	"size": ["object", "SIZE"], 
	"point": ["object", "POINT"], 
}
# ====================================================
def domake():
	[headerandcpp(ns, n) for n in names]
def headerandcpp(ns, name):
	header(ns, name)
	cpp(ns, name)
def header(ns, name):
	cc = classes.get(name, [name])
	p = "\n".join(["struct %s;" %(c) for c in cc])
	s = "\n".join([classdef(ns, c) for c in cc])
	headerfile("%s.%s.h" %(ns, name), """\
#include "{ns}.h"
namespace {ns} {{;
{p}
{s}
}}//namespace {ns}\
""".format(ns = ns, name = name, s = s, p = p))
def classdef(ns, c):
	b = ["public %s" %(b) for b in bases.get(c, ["object"])]
	b = ": " + ", ".join(b) if len(b) else ""
	s = ""
	return """\
//====================================================
//= struct {ns}::{c}
//====================================================
struct {c} {b}
{{
}};\
""".format(ns = ns, c = c, s = s, b = b)

def cpp(ns, name):
	pass


# ====================================================
if __name__ == "__main__":
	domake()
# ====================================================