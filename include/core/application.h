#pragma once
#include "mypch.h"

#include "core/other/editorCamera.h"
#include "core/engineInfo.h"

#include "ecs/entity.h"
#include "ecs/scene.h"


class Application
{
public:
	//---OBJECTS---
#ifndef RELEASE_BUILD
	static EditorCamera editorCamera;
#endif
	//---CONTROLLERS---
	static GLFWwindow *window;

	static int imguizmoType;
	static std::shared_ptr<Entity> selectedEntity;
	static std::shared_ptr<Entity> copiedEntity;
	static std::shared_ptr<Component> copiedComponent; // EDITOR MANAGER?

	static Scene m_curentScene;

#ifndef RELEASE_BUILD
	static bool isRunning;
	static bool isRunningLast;
#endif
	static std::string sceneFileName;

public:
	int Init();
	void Start();
	void Run();
	void Shutdown();
};
