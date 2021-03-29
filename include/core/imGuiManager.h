#include "mypch.h"
#include "ecs/entity.h"

class ImGuiManager
{
public:
	static std::string searchString;
	static ImGuiContext* imGuiContext;

public:
	static void InitImGui();
	static void ShutdownImGui();

	static void StartFrame();
	static void EndFrame();

	static void DrawEnity(std::shared_ptr<Entity>& entityToDraw);
	static void DrawEnityHierarchy(const std::shared_ptr<Entity>& entt);

	static void Update();

};