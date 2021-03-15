#pragma once
#include  "mypch.h"


#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "texture.h"

#include "mesh_new.h"
#include "model.h"



class Game {
public:
	//---LISTS---
	static std::vector<PointLight> pointLightVector;
	static std::vector<SpotLight> spotLightVector;
	static std::vector<DirLight> dirLightVector;

	//---OBJECTS---
	static Camera mainCamera;


	//---PROJECTIONS---
	static glm::mat4 viewMatrix;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewProjectionMatrix;


	//---SHADERS---
	static Shader lightingShader1;

	static Shader coolSahder;
	static Shader notCoolSahder;
	static Shader boxShader;


	static Shader glyphShader;


	static std::unordered_map<int, bool> isPressed1;
	static std::unordered_map<int, bool> isPressed2;



	//---MODELS---
	Model boxModel;
	Model ourModel;
	Model sphereModel;
	Model monkyModel;



	//---GLLOBALS---
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static float deltaTime;
	static float lastFrame;

	//---CONTROLLERS---
	static GLFWwindow* window;
	static ImGuiIO* ioPtr;


public:
	static void LoadScene(const std::string& fileName);
	static void SaveScene(const std::string& fileName);

	int Init();
	void Start();
	void Run();
	void Shutdown();
	void ImGUI();


private:
	static void LoadCamera(std::ifstream& infile);
	static void SaveCamera(std::ofstream& outfile);
};