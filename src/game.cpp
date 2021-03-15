#include "mypch.h"
#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "game.h"
#include "texture.h"
#include "log.h"
#include "mesh_new.h"
#include "model.h"

#include "input.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


//-----GAME VARIABLES----
//---LISTS---
std::vector<PointLight> Game::pointLightVector;
std::vector<SpotLight> Game::spotLightVector;
std::vector<DirLight> Game::dirLightVector;

//---OBJECTS---
Camera Game::mainCamera;

//---PROJECTIONS---
glm::mat4 Game::viewMatrix = glm::mat4(1.0f);
glm::mat4 Game::projectionMatrix = glm::mat4(1.0f);
glm::mat4 Game::viewProjectionMatrix = glm::mat4(1.0f);

//---GLLOBALS---
int Game::SCREEN_WIDTH = 1280;
int Game::SCREEN_HEIGHT = 720;
float Game::deltaTime = 0.0001f;
float Game::lastFrame = 0.0f;

//---SHADERS---
Shader Game::lightingShader1;

Shader Game::coolSahder;
Shader Game::notCoolSahder;
Shader Game::boxShader;
Shader Game::glyphShader;

//---CONTROLLERS---
GLFWwindow* Game::window = nullptr;
ImGuiIO* Game::ioPtr = nullptr;


std::unordered_map<int, bool> Game::isPressed1;
std::unordered_map<int, bool> Game::isPressed2;

//-----wierd shit varibales----
unsigned int matrixUBO;
glm::vec3 edgeColor = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 baseColor = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 transformColor = glm::vec3(1.0f, 0.0f, 0.0f);
float widthValue = 0.03f;
float scaleValue = 1.0f;
bool useTransparency = true;
glm::vec3 ROOOTOTTO = glm::vec3(270.0f, 0.0f, 90.0f);
int frameCountToDisplay = 0;
std::string frameImGuiText = "FPS";


int Game::Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	SCREEN_WIDTH = 1920;
	SCREEN_HEIGHT = 1080;
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPENGL4.3 GAME", NULL, NULL);
	if (window == nullptr) {
		Error("Failed to create a GLFW window");
		glfwTerminate();
		return -1;
	}



	glfwMakeContextCurrent(window);
	glfwShowWindow(window);

	glfwSetKeyCallback(window, Input::KeyInputCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Error("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	stbi_set_flip_vertically_on_load(false);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT );
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	glfwSwapInterval(1);
	srand (time(NULL));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	//-----------imgui-----------
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ioPtr = &ImGui::GetIO();
	ImGuiIO& io = *ioPtr; (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	ImGui::StyleColorsDark();
	//-----------imgui-----------
	return 0;
}
void Game::Start() {
	LoadScene("other/save.txt");
	lightingShader1 = Shader("res/shaders/model.vs", "res/shaders/light.fs");
	coolSahder = Shader("res/shaders/model.vs", "res/shaders/cool.fs");
	notCoolSahder = Shader("res/shaders/model.vs", "res/shaders/notcool.fs");
	boxShader = Shader("res/shaders/model.vs", "res/shaders/box.fs");

	glyphShader = Shader("res/shaders/glyph.vs", "res/shaders/glyph.fs");

	lightingShader1.use();


	ourModel = Model("res/BOXES/BOX.obj");
	monkyModel = Model("res/robot/nanosuuit.fbx");



	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);
}
void Game::Run() {


	while (!glfwWindowShouldClose(window))
	{


		mainCamera.ProcessKeyboard(deltaTime);
		viewMatrix = mainCamera.GetViewMatrix();
		viewProjectionMatrix = projectionMatrix * viewMatrix;

		glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);




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





		lightingShader1.setFloat("material.shininess", 64.0f);

		lightingShader1.use();
		ourModel.vRot = glm::vec3(0.0f, 90.0f, 0.0f);
		ourModel.vScale = glm::vec3(10.0f, 1.0f, 10.0f);
		ourModel.vPos = glm::vec3(0.0f, 0.0f, 25.0f);

		ourModel.Draw(lightingShader1);
		monkyModel.vRot = glm::vec3(0.0f, 90.0f, 0.0f);
		monkyModel.vScale = glm::vec3(1.0f, 1.0f, 1.0f);
		monkyModel.vPos = glm::vec3(0.0f, 0.0f, 0.0f);
		monkyModel.Draw(lightingShader1);

		boxShader.use();
		boxModel.vRot = glm::vec3(0.0f, 0.0f, 0.0f);
		boxModel.vScale = glm::vec3(1.0f, 1.0f, 1.0f);




		//--------------------------ImGui--------------------------
		ImGUI();

		//--------------------------ImGui--------------------------


		Input::Update();
		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

	}
}
void Game::ImGUI() {
	ImGuiIO& io = *ioPtr; (void)io;


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();



	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	ImGui::Begin("LIGHTS");

	frameCountToDisplay++;
	if (frameCountToDisplay >= 60) {
		frameCountToDisplay = 0;
		frameImGuiText = std::to_string(1 / deltaTime);
	}



	ImGui::TextColored(ImVec4(1, 1, 0, 1), frameImGuiText.c_str());


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
			if ( getline (myfile, line) )
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


	io.DisplaySize = ImVec2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	io.DeltaTime = deltaTime;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* backup_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_context);
	}
}
void Game::Shutdown() {
	SaveScene("other/save.txt");


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}


void Game::LoadScene(const std::string& fileName) {
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
void Game::SaveScene(const std::string& fileName) {
	std::ofstream outfile(fileName);
	SaveCamera(outfile);
}


void Game::LoadCamera(std::ifstream& infile) {
	infile >> mainCamera.vPos.x;
	infile >> mainCamera.vPos.y;
	infile >> mainCamera.vPos.z;

	infile >> mainCamera.vUp.x;
	infile >> mainCamera.vUp.y;
	infile >> mainCamera.vUp.z;

	infile >> mainCamera.fYaw;
	infile >> mainCamera.fPitch;
	mainCamera.updateCameraVectors();
}
void Game::SaveCamera(std::ofstream& outfile) {
	outfile << "Camera\n";
	outfile << mainCamera.vPos.x << " ";
	outfile << mainCamera.vPos.y << " ";
	outfile << mainCamera.vPos.z;
	outfile << "\n";
	outfile << mainCamera.vUp.x << " ";
	outfile << mainCamera.vUp.y << " ";
	outfile << mainCamera.vUp.z;
	outfile << "\n";
	outfile << mainCamera.fYaw;
	outfile << "\n";
	outfile << mainCamera.fPitch;
	outfile << "\n";
}
