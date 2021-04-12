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

int Application::SCREEN_WIDTH = 1920;
int Application::SCREEN_HEIGHT = 1080;
float Application::deltaTime = 0.0001f;
float Application::lastFrame = 0.0f;


Scene Application::m_curentScene;
std::shared_ptr<Entity> Application::m_selectedEntity;
std::shared_ptr<Entity> Application::m_copiedEntity;
std::shared_ptr<Component> Application::m_copiedComponent;

GLFWwindow* Application::window = nullptr;

#ifndef RELEASE_BUILD
bool Application::isRunning = false;
bool Application::isRunningLast = false;
#endif
std::string Application::sceneFileName = "";
unsigned int Application::framebuffer;
unsigned int Application::textureColorbuffer;
unsigned int Application::rbo;

#ifndef RELEASE_BUILD
FrameBuffer Application::frameBuffer;
#endif

int Application::imguizmoType = -1;




/*
PxFoundation* mFoundation;
PxPhysics* mPhysics;
PxCooking* mCooking;
PxScene* mScene;
*/


int Application::Init() {
	Random::Init();
	int exitCode = Renderer::InitOpenGL();
	if (exitCode != 0) {
		return exitCode;
	}
#ifndef RELEASE_BUILD
	ImGuiManager::InitImGui();
#endif
	return 0;
}

void Application::Start() {
	Renderer::InitMatrices();
	PhysicsManager::InitPhysx();

}


float mAccumulator = 0.0f;
float mStepSize = 1.0f / 60.0f;

bool advance(float dt)
{
	mAccumulator  += dt;
	if (mAccumulator < mStepSize)
		return false;

	while (mAccumulator >= mStepSize) {
		mAccumulator -= mStepSize;
		PhysicsManager::mScene->simulate(mStepSize);
		PhysicsManager::mScene->fetchResults(true);
	}
	//Log(mAccumulator);
	return true;
}


void Application::Run() {


	//*-----------------PHYSX-----------------

	PxMaterial* mMaterial;
	mMaterial = PhysicsManager::mPhysics->createMaterial(1.0f, 1.0f, -50.0f);    //static friction, dynamic friction, restitution
	if (!mMaterial)
		Error("createMaterial failed!");


	glm::mat4 glmTransform = glm::translate(glm::mat4(1.0f), glm::vec3{0.0f, 150.0f, 5.0f})
	                         * glm::toMat4(glm::quat(glm::radians(glm::vec3{50.0f, 20.0f, 3.0f})))
	                         * glm::scale(glm::mat4(1.0f), glm::vec3{1.0f, 1.0f, 1.0f});

	PxMat44 physxTransform;
	GlmMat4ToPhysXMat4(glmTransform, physxTransform);

	PxRigidDynamic * aSphereActor = PxCreateDynamic(*PhysicsManager::mPhysics, PxTransform(physxTransform), PxBoxGeometry(1, 5, 2), *mMaterial, 1.0f);
	PhysicsManager::mScene->addActor(*aSphereActor);




	PxRigidStatic* plane = PxCreatePlane(*PhysicsManager::mPhysics, PxPlane(PxVec3(0, 1, 0), 0), *mMaterial);
	if (!plane)
		Error("create shape failed!");



	PhysicsManager::mScene->addActor(*plane);
	//*-----------------PHYSX-----------------


	std::shared_ptr<Shader> lightingShader1 = std::make_shared<Shader>();
	std::shared_ptr<Shader> colorShader1 = std::make_shared<Shader>();

	lightingShader1->CreateVertexAndFragment("res/shaders/model.vs", "res/shaders/model.fs");
	colorShader1->CreateVertexAndFragment("res/shaders/color.vs", "res/shaders/color.fs");


#ifdef RELEASE_BUILD
	m_curentScene.Deserialize("other/scenes/chubby.scene");
#else
	frameBuffer = FrameBuffer((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
#endif

	while (!glfwWindowShouldClose(window))
	{


#ifndef RELEASE_BUILD
		if (isRunningLast == false && isRunning == true) {
			m_selectedEntity = nullptr;
			m_curentScene.Serialize("other/TEMP.scene");
		}
		else if (isRunningLast == true && isRunning == false) {
			m_selectedEntity = nullptr;
			m_curentScene.Deserialize("other/TEMP.scene");
		}
#endif


#ifndef RELEASE_BUILD
		if (!isRunning) {
			editorCamera.Update();
		}
#endif
		Renderer::SetupMatrices();


#ifndef RELEASE_BUILD
		frameBuffer.Bind();
#endif
		//--------------------------Draw--------------------------
#ifndef RELEASE_BUILD
		if (isRunning) {
			glClearColor(Renderer::clearColor.r, Renderer::clearColor.g, Renderer::clearColor.b, 1.0f);
		}
		else {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		}
#else
		glClearColor(Renderer::clearColor.r, Renderer::clearColor.g, Renderer::clearColor.b, 1.0f);
#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//--------------------------Draw--------------------------

#ifndef RELEASE_BUILD
		if (isRunning) {
#endif
			advance(deltaTime);
			if (auto headEntity = m_curentScene.GetEntity("Head")) {
				auto transcomp = headEntity->GetComponent<Transform>();

				PxTransform t = aSphereActor->getGlobalPose();
				PxMat44 m = PxMat44(t);
				glm::mat4 newM;

				glm::vec3 translation; glm::vec3 rotation; glm::vec3 scale;
				DecomposeTransform(newM, translation, rotation, scale);
				PhysXMat4ToglmMat4(m, newM);

				transcomp->position = translation;
				transcomp->rotation = glm::degrees(rotation);
				transcomp->scale = scale;
				transcomp->scale.y = 5.0f; transcomp->scale.z = 2.0f;
			}
#ifndef RELEASE_BUILD
		}
#endif


		//--------------------------Update--------------------------

#ifndef RELEASE_BUILD
		if (isRunning) {
			m_curentScene.Update();
		}
		else {
			m_curentScene.Render();
		}
#else
		m_curentScene.Update();
#endif
		//--------------------------Update--------------------------
#ifndef RELEASE_BUILD
		frameBuffer.Unbind();
#endif

		//--------------------------ImGui--------------------------
#ifndef RELEASE_BUILD
		ImGuiManager::Update();
#endif
		//--------------------------ImGui--------------------------


		glfwSwapBuffers(window);

		Input::Update();
		glfwPollEvents();

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
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
}

