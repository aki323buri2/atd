//main.cpp
#include "common.h"
int main(int argc, char **argv)
{
	int r = 0;
	try
	{
		r = frame(argc, argv);
	}
	catch (std::exception &e)
	{
		notifyf("!!!! %s error (%s) : %s"
			, app.basename.c_str()
			, systime().c_str()
			, e.what()
		);
	} 
	return r;
}
int frame(int argc, char **argv)
{
	notifyf(">>>> %s start (%s) >>>>", app.filename.c_str(), systime().c_str());
	int r = run(argc, argv);
	notifyf(">>>> %s e n d (%s) >>>>", app.filename.c_str(), systime().c_str());
	return r;
}
int run(int argc, char **argv)
{
	int r = 0;

	return r; 
}
