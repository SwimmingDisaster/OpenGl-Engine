#pragma once
#include  "mypch.h"


#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "texture.h"

#include "mesh_new.h"
#include "model.h"

#include "entity.h"
#include "transform.h"


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


public:
	static void LoadScene(const std::string& fileName);
	static void SaveScene(const std::string& fileName);

	int Init();
	void Start();
	void Run();
	void Shutdown();
	void ImGUI();

	static void framebuffer_size_callback(GLFWwindow * window, int width, int height);

private:
	static void LoadCamera(std::ifstream& infile);
	static void SaveCamera(std::ofstream& outfile);
};