#include "mypch.h"

#include "core/shader.h"
#include "core/application.h"
#include "core/renderer.h"
#include "core/imGuiManager.h"
#include "core/input.h"
#include "core/physics.h"
#include "core/framebuffer.h"
#include "core/batchRenderer.h"

#include "assets/model.h"
#include "assets/modelRenderer.h"
#include "assets/colliders.h"
#include "assets/rigidbody.h"
#include "assets/material.h"


Application Application::m_instance;

#ifndef RELEASE_BUILD
EditorCamera Application::editorCamera;
bool Application::isRunning = false;
bool Application::isRunningLast = false;
#endif

int Application::Init() {
	Random::Init();
	ReturnIfNotZero(Renderer::InitOpenGL());
#ifndef RELEASE_BUILD
	ImGuiManager::InitImGui();
#endif
	Renderer::Init();
	PhysicsManager::InitPhysx();
	NFD_Init();
	LightsBatchRenderer::Init();
	return 0;
}
void Application::Start() {

#ifdef RELEASE_BUILD
	m_curentScene.Deserialize("other/scenes/shapes.scene");//change this so that the initial scene is set in a .project file
#endif
}
void Application::Run() {
	while (glfwWindowShouldClose(window) == 0) {

		static bool is = false;
		constexpr float sz = 40.0f;
		constexpr float sc = 1.0f;
		constexpr int squareCount = 500;
		constexpr int lightCount = 64;
		if (isRunning && !is) {
			for (int i = 0; i < 100; i++) {
				Log(Random::Float());
			}
			for (int i = 0; i < squareCount; i++){
				auto newEntity = m_curentScene.AddEntity("New Entity " + std::to_string(i), Random::Int());
				auto transform = newEntity->AddComponent<Transform>();
				transform->position = {Random::Float() * sz, Random::Float() * sz, Random::Float() * sz};
				transform->scale = glm::vec3(sc, sc, sc);
				newEntity->AddComponent<Model>();
				auto material = newEntity->AddComponent<Material>();
				//material->materialProperties.push_back(std::make_pair("color", glm::vec3(Random::Float(), Random::Float(), Random::Float())));
				material->materialProperties.push_back(std::make_pair("color", glm::vec3(1.0f, 1.0f, 1.0f)));
				newEntity->AddComponent<ModelRenderer>();
				newEntity->Start();
			}
		///*
			for (int i = 0; i <	lightCount; i++){
				auto newEntity = m_curentScene.AddEntity("New Light " + std::to_string(i), Random::Int());
				auto transform = newEntity->AddComponent<Transform>();
				transform->position = glm::vec3(Random::Float() * sz - sz * 0.5f, 1.0f, Random::Float() * sz - sz * 0.5f);
				auto light = newEntity->AddComponent<PointLightComponent>();
				//light->pointLight.color = glm::vec3(Random::Float(), Random::Float(), Random::Float());
				light->pointLight.color = glm::vec3(Random::Float(), 0.8f, 0.8f);
				light->pointLight.ambientIntensity = 0.0f;
				light->pointLight.diffuseIntensity = 1.0f;
				
				light->pointLight.constant = 1.0f;
				light->pointLight.linear = 0.2f;
				light->pointLight.exp = 0.0032f;
				newEntity->Start();
			}
		//*/
			is = true;
		}

#ifndef RELEASE_BUILD
		if (!isRunningLast && isRunning)
		{
			selectedEntity = nullptr;
			m_curentScene.Serialize("other/TEMP.scene");
		}
		else if (isRunningLast && !isRunning)
		{
			selectedEntity = nullptr;
			m_curentScene.Deserialize("other/TEMP.scene");
		}
#endif


#ifndef RELEASE_BUILD
		if (!isRunning) {
			editorCamera.Update();
		}
#endif
		Renderer::SetupMatrices();
		Renderer::StartFrame();

		PhysicsManager::Update();


#ifndef RELEASE_BUILD
		if (isRunning)	{
			m_curentScene.Update();
		}
#else
		m_curentScene.Update();
#endif
		m_curentScene.Render();
		Renderer::EndFrame();

#ifndef RELEASE_BUILD
		ImGuiManager::Update();
#endif

		BatchRenderer::Clear();
		LightsBatchRenderer::Clear();
		glfwSwapBuffers(window);

		Input::Update();
		glfwPollEvents();

		EngineInfo::CalculateDeltaTime();
	}
#ifndef RELEASE_BUILD
	if (!isRunning) {
		m_curentScene.Serialize("other/TEMP.scene");
	}
#endif
	m_curentScene.Clear();
}
void Application::Shutdown() {
	PhysicsManager::ShutdownPhysx();
#ifndef RELEASE_BUILD
	ImGuiManager::ShutdownImGui();
#endif
	Renderer::ShutdownOpenGL();
	NFD_Quit();
}

/*
Scene Application::m_curentScene;
std::shared_ptr<Entity> Application::selectedEntity;
std::shared_ptr<Entity> Application::copiedEntity;
std::shared_ptr<Component> Application::copiedComponent;

GLFWwindow *Application::window = nullptr;

    		*/
Application& Application::Get(){
	return m_instance;
}
const Scene& Application::GetScene() {
	return Get().m_curentScene; 
}
Scene& Application::GetSceneModifiable(){
	return Get().m_curentScene;
}
GLFWwindow* Application::GetWindow(){
	return Get().window;
}
Entity*& Application::GetSelectedEntity(){
	return Get().selectedEntity;
}
Entity*& Application::GetCopiedEntity(){
	return Get().copiedEntity;
}
Component*& Application::GetCopiedComponent(){
	return Get().copiedComponent;
}
