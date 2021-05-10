#include "mypch.h"

#include "core/shader.h"
#include "core/application.h"
#include "core/renderer.h"
#include "core/imGuiManager.h"
#include "core/input.h"
#include "core/physics.h"

#ifndef RELEASE_BUILD
EditorCamera Application::editorCamera;
#endif

Scene Application::m_curentScene;
std::shared_ptr<Entity> Application::selectedEntity;
std::shared_ptr<Entity> Application::copiedEntity;
std::shared_ptr<Component> Application::copiedComponent;

GLFWwindow *Application::window = nullptr;

#ifndef RELEASE_BUILD
bool Application::isRunning = false;
bool Application::isRunningLast = false;
#endif
std::string Application::sceneFileName = "";

int Application::imguizmoType = -1;

int Application::Init()
{
	Random::Init();
	ReturnIfNotZero(Renderer::InitOpenGL());
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
}

void Application::Run()
{
	//change this so that shaders can be added in the engine and saved in a .project file.
	std::shared_ptr<Shader> lightingShader1 = std::make_shared<Shader>();
	std::shared_ptr<Shader> colorShader1 = std::make_shared<Shader>();

	lightingShader1->CreateVertexAndFragment("res/shaders/model.vs", "res/shaders/model.fs");
	colorShader1->CreateVertexAndFragment("res/shaders/color.vs", "res/shaders/color.fs");

#ifdef RELEASE_BUILD
	m_curentScene.Deserialize("other/scenes/chubby.scene");//change this so that the initial scene is set in a .project file
#endif


	while (!glfwWindowShouldClose(window))
	{



		Renderer::renderID = 0;


#ifndef RELEASE_BUILD
		if (isRunningLast == false && isRunning == true)
		{
			selectedEntity = nullptr;
			m_curentScene.Serialize("other/TEMP.scene");
		}
		else if (isRunningLast == true && isRunning == false)
		{
			selectedEntity = nullptr;
			m_curentScene.Deserialize("other/TEMP.scene");
		}
#endif

		static bool is = false;
		if (isRunning && !is)
		{
			for (int i = 0; i < 1000; i++)
			{
				auto ant = m_curentScene.AddEntityR("New Entity " + std::to_string(i));
				auto aaaaaa = ant->AddComponentR<Transform>();
				aaaaaa->position = {((int)i % 80) * 3.0f, 50.0f, (int)(i / 80.0f) * 3.0f};
				ant->AddComponent<Model>();
				auto mmmmmm = ant->AddComponentR<Material>();
				mmmmmm->color = {Random::Float(), Random::Float(), Random::Float()};
				ant->AddComponent<ModelRenderer>();
				ant->Start();
			}
			is = true;
		}

#ifndef RELEASE_BUILD
		if (!isRunning)
		{
			editorCamera.Update();
		}
#endif

		Renderer::SetupMatrices();
		Renderer::StartFrame();

		PhysicsManager::Update();


		glm::vec4 vic[1000];
		int j = 0;
		for (int i = 0; i < m_curentScene.m_entities.size(); i++)
		{
			auto mat = m_curentScene.m_entities[i]->GetComponent<Material>();
			if (mat) {
				j++;
				vic[j] = glm::vec4(mat->color, 1.0f);
			}
		}

		colorShader1->use();
		glUniform4fv(glGetUniformLocation(colorShader1->ID, "colorList"), 1023, glm::value_ptr(vic[0]));


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
