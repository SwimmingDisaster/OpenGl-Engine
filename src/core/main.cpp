#include "mypch.h"
#include "core/application.h"


int main(int argc, char *argv[]) {
#if !__linux__
	std::freopen("other/output.txt", "w", stdout);
#endif

	Application app;

	ReturnIfNotZero(app.Init());

	app.Start();
	app.Run();
	app.Shutdown();

	return 0;
}


