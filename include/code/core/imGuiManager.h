#pragma once
#include "mypch.h"

struct ImGuiContext;
class ImGuiManager
{
#ifndef RELEASE_BUILD
public:
	static ImGuiContext* imGuiContext;

public:
	static void InitImGui();
	static void ShutdownImGui();

	static void StartFrame();
	static void EndFrame();

	static void Update();
#endif
};
