#include "mypch.h"
#include "core/renderer.h"
#include "core/application.h"
#include "core/input.h"

unsigned int Renderer::matrixUBO;
glm::mat4 Renderer::projectionMatrix;
glm::mat4 Renderer::viewMatrix;
glm::mat4 Renderer::viewProjectionMatrix;


int Renderer::InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Application::SCREEN_WIDTH = 1920;
	Application::SCREEN_HEIGHT = 1080;
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	Application::window = glfwCreateWindow(Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT, "OPENGL4.3 GAME", NULL, NULL);
	if (Application::window == nullptr) {
		Error("Failed to create a GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Application::window);
	glfwShowWindow(Application::window);

	glfwSetKeyCallback(Application::window, Input::KeyInputCallback);
	glfwSetFramebufferSizeCallback(Application::window, Application::framebuffer_size_callback);
	glfwSetCursorPosCallback(Application::window, Input::MouseCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Error("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT);
	glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	stbi_set_flip_vertically_on_load(false);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEBUG_OUTPUT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	glfwSwapInterval(1);
	srand (time(NULL));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	return 0;
}

void Renderer::InitMatrices() {
	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)Application::SCREEN_WIDTH / (float)Application::SCREEN_HEIGHT, 0.1f, 1000.0f);

	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);
}

void Renderer::SetupMatrices() {
	viewMatrix = Application::mainCamera.GetViewMatrix();
	viewProjectionMatrix = projectionMatrix * viewMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::ShutdownOpenGL() {
	glfwTerminate();
}