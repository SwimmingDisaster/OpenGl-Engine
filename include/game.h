#pragma once
#include  "mypch.h"


#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "texture.h"
#include "particles.h"
#include "ui.h"

#include "mesh_new.h"
#include "model.h"

#include "profiler.h"


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



	//---MODELS---
	static Model boxModel;
	static Model ourModel;
	static Model sphereModel;
	static Model monkyModel;



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
	static void LoadPointLight(std::ifstream& infile);
	static void SavePointLight(const PointLight& pointLight, std::ofstream& outfile);

	static void LoadDirLight(std::ifstream& infile);
	static void SaveDirLight(const DirLight& pointLight, std::ofstream& outfile);

	static void LoadSpotLight(std::ifstream& infile);
	static void SaveSpotLight(const SpotLight& pointLight, std::ofstream& outfile);

	static void LoadCamera(std::ifstream& infile);
	static void SaveCamera(std::ofstream& outfile);

	static void LoadParticleSystem(std::ifstream& infile);
	static void SaveParticleSystem(std::ofstream& outfile);



};