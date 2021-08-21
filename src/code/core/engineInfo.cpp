#include "mypch.h"
#include "core/engineInfo.h"

int EngineInfo::SCREEN_WIDTH = 1920;
int EngineInfo::SCREEN_HEIGHT = 1080;
float EngineInfo::deltaTime = 0.0001f;
float EngineInfo::lastFrame = 0.0f;

std::string EngineInfo::name = "ENGINE";
int EngineInfo::swapInterval = 0;

void EngineInfo::CalculateDeltaTime()
{
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void EngineInfo::Resize(int w, int h)
{
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}
