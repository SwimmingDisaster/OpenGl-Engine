#pragma once
#include <string>

class EngineInfo {
public:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static float deltaTime;
	static float lastFrame;

	static std::string name;
	static int swapInterval;


public:
	static void CalculateDeltaTime();
	static void Resize(int w, int h);
};
