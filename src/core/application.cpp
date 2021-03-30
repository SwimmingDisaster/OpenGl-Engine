#include "mypch.h"

#include "core/shader.h"
#include "core/application.h"
#include "core/texture.h"
#include "core/renderer.h"
#include "core/imGuiManager.h"
#include "core/input.h"

#include "core/other/editorCamera.h"
#include "core/other/log.h"


#include "assets/transform.h"
#include "assets/mesh.h"
#include "assets/model.h"
#include "assets/modelRenderer.h"
#include "assets/camera.h"

#include "ecs/other/componentFactory.h"


EditorCamera Application::mainCamera;

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
std::string Application::sceneFileName;

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

	lightingShader1->use();
	colorShader1->use();

	//m_curentScene.Deserialize("other/savetest.txt");


	while (!glfwWindowShouldClose(window))
	{

		if (Shader::shaderMap["res/shaders/color"]->ID != 6)
			Log(Shader::shaderMap["res/shaders/color"]->ID);

		if (isRunningLast == false && isRunning == true) {
			m_curentScene.Serialize("other/TEMP.txt");
		}
		else if (isRunningLast == true && isRunning == false) {
			m_selectedEntity = nullptr;
			m_curentScene.Deserialize("other/TEMP.txt");
		}


		mainCamera.ProcessMouseMovement();
		mainCamera.ProcessKeyboard(deltaTime);
		Renderer::SetupMatrices();



		//todo this shouldnt be here
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


		lightingShader1->setFloat("material.shininess", 64.0f);
		lightingShader1->use();
		//--------------------------Draw--------------------------


		//--------------------------Update--------------------------
		if (isRunning) {
			for (int i = 0; i < m_curentScene.m_entities.size(); i++) {
				m_curentScene.m_entities[i]->Update();
			}
		}
		else {
			for (int i = 0; i < m_curentScene.m_entities.size(); i++) {
				auto modelRendererComponent = m_curentScene.m_entities[i]->GetComponent<ModelRenderer>();
				if (modelRendererComponent != nullptr) {
					modelRendererComponent->Update();
				}
			}
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
		m_curentScene.Serialize("other/TEMP.txt");
	}
	m_curentScene.Clear();
}

void Application::Shutdown() {
	ImGuiManager::ShutdownImGui();
	Renderer::ShutdownOpenGL();
}

