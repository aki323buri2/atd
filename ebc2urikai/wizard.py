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
def cpp(filename, s):
	make(filename, """\
//{filename}
{s}\
""".format(filename = filename, s = s))
def header(filename, s):
	make(filename, """\
//{filename}
#ifndef {guard}
#define {guard}
{s}
#endif//{guard}\
""".format(filename = filename, s = s, guard = guard(filename)))

def makefiles():
	cpp("main.cpp", """\
#include "common.h"
int main(int argc, char **argv)
{{
	int r = 0;
	return r;
}}
int frame(int argc, char **argv)
{{
	int r = 0;
	return r;
}}
int run(int argc, char **argv)
{{
	int r = 0;
	return r;
}}
""".format())
	header("common.h", """\
#include "common.h"
int main(int argc, char **argv);
int frame(int argc, char **argv);
int run(int argc, char **argv);
""".format())



if __name__ == "__main__":
	makefiles()