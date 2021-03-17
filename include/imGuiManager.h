#include "mypch.h"

class ImGuiManager
{
public:
	static void InitImGui();
	static void ShutdownImGui();

	static void StartFrame();
	static void EndFrame();
};