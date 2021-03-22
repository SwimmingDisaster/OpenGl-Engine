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

#define STB_IMAGE_IMPLEMENTATION


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//---OBJECTS---
Camera Application::mainCamera;

//---GLLOBALS---
int Application::SCREEN_WIDTH = 1280;
int Application::SCREEN_HEIGHT = 720;
float Application::deltaTime = 0.0001f;
float Application::lastFrame = 0.0f;

//---SHADERS---
Shader lightingShader1;
//---MODELS---


std::shared_ptr<Entity> Application::m_selectedEntity;
Scene  Application::m_curentScene;

//---CONTROLLERS---
GLFWwindow* Application::window = nullptr;

int frameCountToDisplay = 0;
std::string frameImGuiText = "FPS";



void Application::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	Renderer::projectionMatrix = glm::perspective(glm::radians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}



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
	LoadScene("other/save.txt");
	lightingShader1 = Shader("res/shaders/model.vs", "res/shaders/light.fs");

	lightingShader1.use();


	Renderer::InitMatrices();
}
void Application::Run() {
	m_curentScene.AddEntity("bob");
	m_curentScene.m_entities[0]->uuid = Random::Int();
	m_curentScene.m_entities[0]->AddComponent<Transform>();
	m_curentScene.m_entities[0]->m_components[0]->Start();

	m_selectedEntity = m_curentScene.GetEntity("bob");

	m_curentScene.AddEntity("bob 2");
	m_curentScene.m_entities[1]->uuid = Random::Int();
	m_curentScene.m_entities[1]->AddComponent<Transform>();
	m_curentScene.m_entities[1]->AddComponent<Model>();
	m_curentScene.m_entities[1]->m_components[0]->Start();
	m_curentScene.m_entities[1]->m_components[1]->Start();


	m_curentScene.m_entities[1]->GetComponent<Transform>()->position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_curentScene.m_entities[1]->GetComponent<Transform>()->rotation = glm::vec3(0.0f, 90.0f, 0.0f);
	m_curentScene.m_entities[1]->GetComponent<Transform>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);

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


		//--------------------------Update--------------------------
		for (int i = 0; i < m_curentScene.m_entities.size(); i++) {
			m_curentScene.m_entities[i]->Update();
		}

		//--------------------------Update--------------------------


		//--------------------------Draw--------------------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		lightingShader1.setFloat("material.shininess", 64.0f);

		lightingShader1.use();

		ModelRenderer::DrawModel(lightingShader1, m_curentScene.m_entities[1]->GetComponent<Model>());


		//--------------------------ImGui--------------------------
		ImGUI();
		//--------------------------ImGui--------------------------


		glfwSwapBuffers(window);

		Input::Update();
		glfwPollEvents();

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	m_curentScene.Clear();
}

void Application::ImGUI() {
	ImGuiManager::StartFrame();

	ImGui::Begin("INFO");

	frameCountToDisplay++;
	if (frameCountToDisplay >= 60) {
		frameCountToDisplay = 0;
		frameImGuiText = std::to_string(1 / deltaTime);
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::TextColored(ImVec4(1, 1, 0, 1), frameImGuiText.c_str());


	ImGui::End();

	ImGui::Begin("Hierarchy");

	for (int i = 0; i < m_curentScene.m_entities.size(); i++)
		ImGuiManager::DrawEnityHierarchy(m_curentScene.m_entities[i ]);

	if (ImGui::Button("Add Entity")) {
		m_curentScene.AddEntity();
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
		m_selectedEntity.reset();
	}

	ImGui::End();

	ImGui::Begin("Properties");
	ImGuiManager::DrawEnity(m_selectedEntity);

	ImGui::End();

	ImGui::Begin("OUTPUT");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "OUTPUT");
	ImGui::BeginChild("Output list");
	std::string line;
	std::ifstream myfile ("other/output.txt");

	if (myfile.is_open())
	{
		int i = 0;
		for (int lineCount = 0; lineCount < 100; lineCount++) {
			if (getline (myfile, line))
			{
				ImGui::Text(line.c_str(), i);
				i++;
			}
			else {
				break;
			}

		}
		myfile.close();
	}
	ImGui::EndChild();
	ImGui::End();

	ImGuiManager::EndFrame();
}
void Application::Shutdown() {
	SaveScene("other/save.txt");
	//ImGui::DestroyContext();
	ImGuiManager::ShutdownImGui();
	Renderer::ShutdownOpenGL();
}


void Application::LoadScene(const std::string& fileName) {
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
void Application::SaveScene(const std::string& fileName) {
	std::ofstream outfile(fileName);
	SaveCamera(outfile);
}


void Application::LoadCamera(std::ifstream& infile) {
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
void Application::SaveCamera(std::ofstream& outfile) {
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
