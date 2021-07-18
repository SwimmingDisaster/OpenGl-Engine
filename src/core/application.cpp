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

int Application::Init(){
    Random::Init();
    ReturnIfNotZero(Renderer::InitOpenGL("ENGINE"));
#ifndef RELEASE_BUILD
    ImGuiManager::InitImGui();
#endif
    Renderer::Init();
    PhysicsManager::InitPhysx();
    NFD_Init();
    return 0;
}

void Application::Start() {

#ifdef RELEASE_BUILD
    m_curentScene.Deserialize("other/scenes/shapes.scene");//change this so that the initial scene is set in a .project file
#endif
}

void Application::Run(){
    /*
    		static bool is = false;
    		if (isRunning && !is)
    		{
    			for (int i = 0; i <	500; i++)
    			{
    				auto newEntity = m_curentScene.AddEntityR("New Entity " + std::to_string(i));
    				auto transform = newEntity->AddComponentR<Transform>();
    				transform->position = {((int)i % 80) * 3.0f, 50.0f, (int)(i / 80.0f) * 3.0f};
    				newEntity->AddComponent<Model>();
    				auto material = newEntity->AddComponentR<Material>();
    				material->materialProperties.push_back(std::make_pair("color", glm::vec3(Random::Float(), Random::Float(), Random::Float())));
    				newEntity->AddComponent<ModelRenderer>();
    				newEntity->Start();
    			}
    			is = true;
    		}
    		*/
    while (!glfwWindowShouldClose(window)){

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

#ifndef RELEASE_BUILD
        if (!isRunning){
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
        glfwSwapBuffers(window);

        Input::Update();
        glfwPollEvents();

        EngineInfo::CalculateDeltaTime();
    }
#ifndef RELEASE_BUILD
    if (!isRunning){
        m_curentScene.Serialize("other/TEMP.scene");
    }
#endif
    m_curentScene.Clear();
}

void Application::Shutdown(){
    PhysicsManager::ShutdownPhysx();
#ifndef RELEASE_BUILD
    ImGuiManager::ShutdownImGui();
#endif
    Renderer::ShutdownOpenGL();
    NFD_Quit();
}
