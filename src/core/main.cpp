#include "mypch.h"
#include "core/application.h"


int main() {
	std::freopen("other/output.txt", "w", stdout);

	Application app;
	int returnValue = app.Init();
	if (returnValue != 0) {
		Error(returnValue);
		return returnValue;
	}

	app.Start();
	app.Run();
	app.Shutdown();

	return 0;
}


