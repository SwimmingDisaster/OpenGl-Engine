#pragma once
#include "mypch.h"

#include "core/shader.h"
#include "core/other/editorCamera.h"
#include "texture.h"

#include "ecs/entity.h"
#include "ecs/scene.h"

#include "assets/model.h"
#include "assets/modelRenderer.h"
#include "assets/colliders.h"
#include "assets/rigidbody.h"
#include "assets/material.h"
#include "core/framebuffer.h"

#include "core/engineInfo.h"

class Application
{
public:
	//---OBJECTS---
#ifndef RELEASE_BUILD
	static EditorCamera editorCamera;
#endif

	//---GLLOBALS---
	//static EngineInfo Info;

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
