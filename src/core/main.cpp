#include "mypch.h"
#include "core/application.h"



int main() {
	std::freopen("other/output.txt", "w", stdout);

	Application app;
	int returnValue = app.Init();//Inits opengl and imgui
	if (returnValue != 0) {
		std::cout << returnValue << std::endl;
		return returnValue;
	}

	app.Start();//inits game elements

	app.Run(); //runs the game

	app.Shutdown();//cleanup of classes and opengl and imgui shutdown

	return 0;
}


