#pragma once
#include "mypch.h"


#include "ecs/scene.h"
#include "core/engineInfo.h"
class Component;
class EditorCamera;
class Entity;


class Application{
private:
	static Application m_instance;

	GLFWwindow *window;

	Entity* selectedEntity;
	Entity* copiedEntity;
	Component* copiedComponent;

	Scene m_curentScene;
public:
#ifndef RELEASE_BUILD
	static EditorCamera editorCamera;
	static bool isRunning;
	static bool isRunningLast;
#endif

public:
	[[nodiscard]] static Application& Get();
	[[nodiscard]] static const Scene& GetScene();
	[[nodiscard]] static Scene& GetSceneModifiable();
	[[nodiscard]] static GLFWwindow* GetWindow();
	[[nodiscard]] static Entity*& GetSelectedEntity();
	[[nodiscard]] static Entity*& GetCopiedEntity();
	[[nodiscard]] static Component*& GetCopiedComponent();

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

