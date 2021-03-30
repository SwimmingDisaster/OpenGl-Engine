#pragma once
#include  "mypch.h"

#include "core/shader.h"
#include "core/other/editorCamera.h"
#include "texture.h"

#include "ecs/entity.h"
#include "ecs/scene.h"

#include "assets/model.h"
#include "assets/modelRenderer.h"




class Application {
public:
	//---OBJECTS---
	static EditorCamera mainCamera;

	//---GLLOBALS---
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static float deltaTime;
	static float lastFrame;

	//---CONTROLLERS---
	static GLFWwindow* window;
	static std::shared_ptr<Entity> m_selectedEntity;
	static std::shared_ptr<Entity> m_copiedEntity;
	static std::shared_ptr<Component> m_copiedComponent;
	static Scene m_curentScene;

	static bool isRunning;
	static bool isRunningLast;
	static std::string sceneFileName;


public:
	int Init();
	void Start();
	void Run();
	void Shutdown();

};