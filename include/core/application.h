#pragma once
#include  "mypch.h"


#include "core/shader.h"
#include "core/other/camera.h"
#include "texture.h"

#include "ecs/entity.h"
#include "ecs/scene.h"



class Application {
public:
	//---OBJECTS---
	static Camera mainCamera;

	//---GLLOBALS---
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static float deltaTime;
	static float lastFrame;

	//---CONTROLLERS---
	static GLFWwindow* window;
	static std::shared_ptr<Entity> m_selectedEntity;
	static Scene m_curentScene;


public:
	static void LoadScene(const std::string& fileName);
	static void SaveScene(const std::string& fileName);

	int Init();
	void Start();
	void Run();
	void Shutdown();

	static void framebuffer_size_callback(GLFWwindow * window, int width, int height);

private:
	static void LoadCamera(std::ifstream& infile);
	static void SaveCamera(std::ofstream& outfile);
};