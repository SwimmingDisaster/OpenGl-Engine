#include "mypch.h"
#include "core/application.h"


int main() {
	std::freopen("other/output.txt", "w", stdout);

	Application app;

	ReturnIfNotZero(app.Init());

	app.Start();
	app.Run();
	app.Shutdown();

	return 0;
}


