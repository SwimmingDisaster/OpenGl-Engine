#include "mypch.h"

#include "core/shader.h"
#include "core/application.h"
#include "core/renderer.h"
#include "core/imGuiManager.h"
#include "core/input.h"
#include "core/physics.h"

#ifndef RELEASE_BUILD
EditorCamera Application::editorCamera;
#endif //EngineInfo Application::Info;

Scene Application::m_curentScene;
std::shared_ptr<Entity> Application::m_selectedEntity;
std::shared_ptr<Entity> Application::m_copiedEntity;
std::shared_ptr<Component> Application::m_copiedComponent;

GLFWwindow *Application::window = nullptr;

#ifndef RELEASE_BUILD
bool Application::isRunning = false;
bool Application::isRunningLast = false;
#endif
std::string Application::sceneFileName = "";

int Application::imguizmoType = -1;

float Application::mAccumulator = 0.0f;
float Application::mStepSize = 1.0f / 60.0f;

int Application::Init()
{
	Random::Init();
	int exitCode = Renderer::InitOpenGL();
	if (exitCode != 0)
	{
		return exitCode;
	}
#ifndef RELEASE_BUILD
	ImGuiManager::InitImGui();
#endif

	Renderer::Init();
	PhysicsManager::InitPhysx();
	NFD_Init();

	return 0;
}

void Application::Start()
{
	Log("this is cool");
}

bool Application::advance(float dt) // TODO PUT THIS IN A BETTER PLACE ASAP
{
	mAccumulator += dt;
	if (mAccumulator < mStepSize)
		return false;

	while (mAccumulator >= mStepSize)
	{
		mAccumulator -= mStepSize;
		PhysicsManager::mScene->simulate(mStepSize);
		PhysicsManager::mScene->fetchResults(true);
	}
	return true;
}

void Application::Run()
{

	std::shared_ptr<Shader> lightingShader1 = std::make_shared<Shader>();
	std::shared_ptr<Shader> colorShader1 = std::make_shared<Shader>();

	lightingShader1->CreateVertexAndFragment("res/shaders/model.vs", "res/shaders/model.fs");
	colorShader1->CreateVertexAndFragment("res/shaders/color.vs", "res/shaders/color.fs");

#ifdef RELEASE_BUILD
	m_curentScene.Deserialize("other/scenes/chubby.scene");
#endif

	while (!glfwWindowShouldClose(window))
	{

#ifndef RELEASE_BUILD
		if (isRunningLast == false && isRunning == true)
		{
			m_selectedEntity = nullptr;
			m_curentScene.Serialize("other/TEMP.scene");
		}
		else if (isRunningLast == true && isRunning == false)
		{
			m_selectedEntity = nullptr;
			m_curentScene.Deserialize("other/TEMP.scene");
		}
#endif

#ifndef RELEASE_BUILD
		if (!isRunning)
		{
			editorCamera.Update();
		}
#endif

		Renderer::SetupMatrices();
		Renderer::StartFrame();

		PhysicsManager::Update();

#ifndef RELEASE_BUILD
		if (isRunning)
		{
			m_curentScene.Update();
		}
		else
		{
			m_curentScene.Render();
		}
#else
		m_curentScene.Update();
#endif
		Renderer::EndFrame();

#ifndef RELEASE_BUILD
		ImGuiManager::Update();
#endif

		glfwSwapBuffers(window);

		Input::Update();
		glfwPollEvents();

		EngineInfo::CalculateDeltaTime();
	}
#ifndef RELEASE_BUILD
	if (!isRunning)
	{
		m_curentScene.Serialize("other/TEMP.scene");
	}
#endif
	m_curentScene.Clear();
}

void Application::Shutdown()
{
	PhysicsManager::ShutdownPhysx();

#ifndef RELEASE_BUILD
	ImGuiManager::ShutdownImGui();
#endif
	Renderer::ShutdownOpenGL();
	NFD_Quit();
}
