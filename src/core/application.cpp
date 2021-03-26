#include "mypch.h"

#include "core/shader.h"
#include "core/application.h"
#include "core/texture.h"
#include "core/renderer.h"
#include "core/imGuiManager.h"
#include "core/input.h"

#include "core/other/camera.h"
#include "core/other/log.h"


#include "assets/transform.h"
#include "assets/mesh.h"

#include "ecs/other/componentFactory.h"


Camera Application::mainCamera;

int Application::SCREEN_WIDTH = 1920;
int Application::SCREEN_HEIGHT = 1080;
float Application::deltaTime = 0.0001f;
float Application::lastFrame = 0.0f;


Scene Application::m_curentScene;
std::shared_ptr<Entity> Application::m_selectedEntity;

GLFWwindow* Application::window = nullptr;


int Application::Init() {
	Random::Init();
	int exitCode = Renderer::InitOpenGL();
	if (exitCode != 0) {
		return exitCode;
	}
	ImGuiManager::InitImGui();
	return 0;
}


void Application::Start() {
	LoadScene("other/save.txt"); //todo this is old code replace this asap
	Renderer::InitMatrices();
}



void Application::Run() {
	Shader lightingShader1 = Shader("res/shaders/model.vs", "res/shaders/model.fs");
	lightingShader1.use();
	Shader colorShader1 = Shader("res/shaders/color.vs", "res/shaders/color.fs");
	colorShader1.use();


	m_curentScene.Deserialize("other/savetest.txt");


	/*	auto enttiti = m_curentScene.AddEntityR("Email");
		Factory::create("Transform", enttiti);
		Factory::create("Model", enttiti);
		Factory::create("ModelRenderer", enttiti);
		enttiti->Start();*/


	while (!glfwWindowShouldClose(window))
	{

		mainCamera.ProcessMouseMovement();
		mainCamera.ProcessKeyboard(deltaTime);
		Renderer::SetupMatrices();


		if (Input::IsKeyPressed(INPUT_KEY_ESCAPE)) {
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
				mainCamera.isLocked = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				mainCamera.isLocked = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}



		//--------------------------Draw--------------------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lightingShader1.setFloat("material.shininess", 64.0f);
		lightingShader1.use();
		//--------------------------Update--------------------------
		for (int i = 0; i < m_curentScene.m_entities.size(); i++) {
			m_curentScene.m_entities[i]->Update();
		}
		//--------------------------Update--------------------------

		//--------------------------ImGui--------------------------
		ImGuiManager::Update();
		//--------------------------ImGui--------------------------


		glfwSwapBuffers(window);

		Input::Update();
		glfwPollEvents();

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	m_curentScene.Serialize("other/savetest.txt");
	m_curentScene.Clear();
}

void Application::Shutdown() {
	SaveScene("other/save.txt");//todo this is old code replace this asap
	ImGuiManager::ShutdownImGui();
	Renderer::ShutdownOpenGL();
}



void Application::LoadScene(const std::string & fileName) {//todo this is old code replace this asap
	std::ifstream infile(fileName);

	if (infile.is_open()) {
		std::string classType;
		while (infile >> classType) {
			if (classType == "Camera") {
				LoadCamera(infile);
			}
		}
	}
}
void Application::SaveScene(const std::string & fileName) {//todo this is old code replace this asap
	std::ofstream outfile(fileName);
	SaveCamera(outfile);
}


void Application::LoadCamera(std::ifstream & infile) {//todo this is old code replace this asap
	infile >> mainCamera.vPos.x;
	infile >> mainCamera.vPos.y;
	infile >> mainCamera.vPos.z;

	infile >> mainCamera.vUp.x;
	infile >> mainCamera.vUp.y;
	infile >> mainCamera.vUp.z;

	infile >> mainCamera.vRot.y;
	infile >> mainCamera.vRot.x;
	mainCamera.updateCameraVectors();
}
void Application::SaveCamera(std::ofstream & outfile) {//todo this is old code replace this asap
	outfile << "Camera\n";
	outfile << mainCamera.vPos.x << " ";
	outfile << mainCamera.vPos.y << " ";
	outfile << mainCamera.vPos.z;
	outfile << "\n";
	outfile << mainCamera.vUp.x << " ";
	outfile << mainCamera.vUp.y << " ";
	outfile << mainCamera.vUp.z;
	outfile << "\n";
	outfile << mainCamera.vRot.y;
	outfile << "\n";
	outfile << mainCamera.vRot.x;
	outfile << "\n";
}
