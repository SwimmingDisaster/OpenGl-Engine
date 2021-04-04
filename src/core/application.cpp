#include "mypch.h"

#include "core/shader.h"
#include "core/application.h"
#include "core/renderer.h"
#include "core/imGuiManager.h"
#include "core/input.h"

EditorCamera Application::editorCamera;

int Application::SCREEN_WIDTH = 1920;
int Application::SCREEN_HEIGHT = 1080;
float Application::deltaTime = 0.0001f;
float Application::lastFrame = 0.0f;


Scene Application::m_curentScene;
std::shared_ptr<Entity> Application::m_selectedEntity;
std::shared_ptr<Entity> Application::m_copiedEntity;
std::shared_ptr<Component> Application::m_copiedComponent;

GLFWwindow* Application::window = nullptr;

bool Application::isRunning = false;
bool Application::isRunningLast = false;
std::string Application::sceneFileName = "";

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
	Renderer::InitMatrices();
}

void Application::Run() {
	std::shared_ptr<Shader> lightingShader1 = std::make_shared<Shader>();
	std::shared_ptr<Shader> colorShader1 = std::make_shared<Shader>();

	lightingShader1->CreateVertexAndFragment("res/shaders/model.vs", "res/shaders/model.fs");
	colorShader1->CreateVertexAndFragment("res/shaders/color.vs", "res/shaders/color.fs");


	while (!glfwWindowShouldClose(window))
	{
		if (isRunningLast == false && isRunning == true) {
			m_selectedEntity = nullptr;
			m_curentScene.Serialize("other/TEMP.scene");
		}
		else if (isRunningLast == true && isRunning == false) {
			m_selectedEntity = nullptr;
			m_curentScene.Deserialize("other/TEMP.scene");
		}

		if (!isRunning) {
			editorCamera.Update();
		}
		Renderer::SetupMatrices();

		//--------------------------Draw--------------------------
		if (isRunning) {
			glClearColor(Renderer::clearColor.r, Renderer::clearColor.g, Renderer::clearColor.b, 1.0f);
		}
		else {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//--------------------------Draw--------------------------


		//--------------------------Update--------------------------
		if (isRunning) {
			m_curentScene.Update();
		}
		else {
			m_curentScene.Render();
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
	if (!isRunning) {
		m_curentScene.Serialize("other/TEMP.scene");
	}
	m_curentScene.Clear();
}

void Application::Shutdown() {
	ImGuiManager::ShutdownImGui();
	Renderer::ShutdownOpenGL();
}

