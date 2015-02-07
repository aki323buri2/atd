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
			, datetime().strftime().c_str()
			, e.what()
		);
	}
	return r;
}
int frame(int argc, char **argv)
{
	notifyf(">>>> %s start (%s) >>>>", app.filename.c_str(), datetime().strftime().c_str());
	int r = run(argc, argv);
	notifyf(">>>> %s e n d (%s) >>>>", app.filename.c_str(), datetime().strftime().c_str());
	return r;
}
#include "aui.h"
using namespace aui;
int run(int argc, char **argv)
{
	application app(argc, argv);
	dialog dialog(app.name());

	button *button = new struct button("button");
	button->append_to(dialog);

	dialog.show();
	return app.run();
}
