#include "mypch.h"
#include "core/application.h"


int main(int argc, char *argv[]) {

//#if !__linux__
	std::freopen("other/output.txt", "w", stdout);
	std::freopen("other/erroroutput.txt", "w", stderr);
//#endif

	Application& app = Application::Get();

	ReturnIfNotZero(app.Init());

	app.Start();
	app.Run();
	app.Shutdown();

	return 0;
}


