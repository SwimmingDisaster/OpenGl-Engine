#include "mypch.h"
#include "entity.h"

class ImGuiManager
{
public:
	static ImGuiContext* imGuiContext;
public:
	static void InitImGui();
	static void ShutdownImGui();

	static void StartFrame();
	static void EndFrame();

	static void DrawEnity(const std::shared_ptr<Entity>& entt);
};