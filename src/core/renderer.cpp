#include "mypch.h"
#include "core/renderer.h"
#include "core/application.h"
#include "core/input.h"

unsigned int Renderer::matrixUBO;
glm::mat4 Renderer::projectionMatrix;
glm::mat4 Renderer::viewMatrix;
glm::mat4 Renderer::viewProjectionMatrix;
glm::vec3 Renderer::clearColor = {0.1f, 0.1f, 0.1f};


#ifndef RELEASE_BUILD
FrameBuffer Renderer::frameBuffer;
#endif

int Renderer::InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	Application::window = glfwCreateWindow(EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT, "ENGINE", NULL, NULL);
	if (Application::window == nullptr) {
		Error("Failed to create a GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Application::window);
	glfwShowWindow(Application::window);

	glfwSetKeyCallback(Application::window, Input::KeyInputCallback);
	glfwSetFramebufferSizeCallback(Application::window, ResizeCallback);
	glfwSetCursorPosCallback(Application::window, Input::MouseCallback);
	glfwSetMouseButtonCallback(Application::window, Input::MouseButtonCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Error("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT);
	glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEBUG_OUTPUT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glfwSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	return 0;
}

void Renderer::Init() {
	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)EngineInfo::SCREEN_WIDTH / (float)EngineInfo::SCREEN_HEIGHT, 0.1f, 1000.0f);


	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);



#ifndef RELEASE_BUILD
	frameBuffer = FrameBuffer((float)EngineInfo::SCREEN_WIDTH, (float)EngineInfo::SCREEN_HEIGHT);
#endif
}

void Renderer::SetupMatrices() {

#ifndef RELEASE_BUILD
	if (!Application::isRunning) {
		viewMatrix = Application::editorCamera.GetViewMatrix();
	}
#endif
	viewProjectionMatrix = projectionMatrix * viewMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::ShutdownOpenGL() {
	glfwTerminate();
}


void Renderer::ResizeCallback(GLFWwindow * window, int width, int height)
{
#ifdef RELEASE_BUILD
	SetWindowSize(window, width, height);
#endif

}

void Renderer::SetWindowSize(GLFWwindow * window, int width, int height) {
	if ((EngineInfo::SCREEN_WIDTH != width) || (EngineInfo::SCREEN_HEIGHT != height)) {

		EngineInfo::Resize(width, height);

#ifndef RELEASE_BUILD
		frameBuffer.Resize(width, height);
#endif

		float ratio = 1.0f;
		if (EngineInfo::SCREEN_HEIGHT != 0.0f)
			ratio = (float)EngineInfo::SCREEN_WIDTH / (float)EngineInfo::SCREEN_HEIGHT;
		projectionMatrix = glm::perspective(glm::radians(90.0f), ratio, 0.05f, 1000.0f);
		glViewport(0, 0, EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT);
	}
}

void Renderer::StartFrame() {
#ifndef RELEASE_BUILD
	frameBuffer.Bind();
#endif
	//--------------------------Draw--------------------------
#ifndef RELEASE_BUILD
	if (Application::isRunning) {
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	}
	else {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}
#else
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--------------------------Draw----------------------

}

void Renderer::EndFrame() {
#ifndef RELEASE_BUILD
	frameBuffer.Unbind();
#endif
}


