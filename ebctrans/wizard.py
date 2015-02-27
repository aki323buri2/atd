# wizard.py
import sys
import os
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
	pass

if __name__ == "__main__":
	makefiles()