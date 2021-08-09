#pragma once
#include "mypch.h"

#include "core/other/editorCamera.h"
#include "core/engineInfo.h"

#include "ecs/entity.h"
#include "ecs/scene.h"


class Application
{
private:
	static Application m_instance;

	GLFWwindow *window;
	std::shared_ptr<Entity> selectedEntity;
	std::shared_ptr<Entity> copiedEntity;
	std::shared_ptr<Component> copiedComponent; // EDITOR MANAGER?

	Scene m_curentScene;
public:
#ifndef RELEASE_BUILD
	static EditorCamera editorCamera;
	static bool isRunning;
	static bool isRunningLast;
#endif

public:
	[[nodiscard]] constexpr static Application& Get();
	[[nodiscard]] constexpr static const Scene& GetScene();
	[[nodiscard]] constexpr static Scene& GetSceneModifiable();
	[[nodiscard]] constexpr static GLFWwindow* GetWindow();
	[[nodiscard]] constexpr static std::shared_ptr<Entity>& GetSelectedEntity();
	[[nodiscard]] constexpr static std::shared_ptr<Entity>& GetCopiedEntity();
	[[nodiscard]] constexpr static std::shared_ptr<Component>& GetCopiedComponent();

public:
	static int Init();
	static void Shutdown();
	void Start();
	void Run();

	Application() = default;
	Application(const Application& other) = delete;
	Application(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;


	friend class Renderer;
 };

constexpr Application& Application::Get(){
	return m_instance;
}
constexpr const Scene& Application::GetScene() {
	return Get().m_curentScene; 
}
constexpr Scene& Application::GetSceneModifiable(){
	return Get().m_curentScene;
}
constexpr GLFWwindow* Application::GetWindow(){
	return Get().window;
}
constexpr std::shared_ptr<Entity>& Application::GetSelectedEntity(){
	return Get().selectedEntity;
}
constexpr std::shared_ptr<Entity>& Application::GetCopiedEntity(){
	return Get().copiedEntity;
}
constexpr std::shared_ptr<Component>& Application::GetCopiedComponent(){
	return Get().copiedComponent;
}
