#include "main_includes.h"
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
ParticleSystem Game::particleSystem;

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

Shader Game::particleShader;
Shader Game::particlesCompute;

Shader Game::glyphShader;

Shader Game::particleComputeUpdate;


//---MODELS---
Model Game::boxModel;
Model Game::ourModel;
Model Game::sphereModel;
Model Game::monkyModel;


//---CONTROLLERS---
GLFWwindow* Game::window = nullptr;
ImGuiIO* Game::ioPtr = nullptr;


//-----wierd shit varibales----
unsigned int matrixUBO;
LightManager lightManager;
glm::vec3 edgeColor = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 baseColor = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 transformColor = glm::vec3(1.0f, 0.0f, 0.0f);
float widthValue = 0.03f;
float scaleValue = 1.0f;
bool useTransparency = true;
glm::vec3 ROOOTOTTO = glm::vec3(270.0f, 0.0f, 90.0f);
int frameCountToDisplay = 0;
std::string frameImGuiText = "FPS";

Font foni;
UI_text img;

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

	glfwSetKeyCallback(window, key_callback);
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

	particleShader = Shader("res/shaders/particle.vs", "res/shaders/particle.fs");
	particlesCompute = Shader("res/shaders/compute.cs");
	glyphShader = Shader("res/shaders/glyph.vs", "res/shaders/glyph.fs");
	particleComputeUpdate = Shader("res/shaders/particleUpdate.cs");

	lightingShader1.use();





	boxModel = Model("res/box.obj");
	//Model ourModel("res/robot/nanosuuit.fbx");
	ourModel = Model("res/BOXES/BOX.obj");
	sphereModel = Model("res/sphere.fbx");
	monkyModel = Model("res/robot/nanosuuit.fbx");

	lightManager.Init();




	lightingShader1.setVec3("spotLight[0].position", Game::mainCamera.vPos);
	lightingShader1.setVec3("spotLight[0].direction", Game::mainCamera.vFront);
	lightingShader1.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
	lightingShader1.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
	lightingShader1.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
	lightingShader1.setFloat("spotLight[0].constant", 1.0f);
	lightingShader1.setFloat("spotLight[0].linear", 0.09f);
	lightingShader1.setFloat("spotLight[0].quadratic", 0.032f);
	lightingShader1.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader1.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));

	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);
}
void Game::Run() {


	foni.Init("res/fonts/IHATEYOU.fnt", "res/fonts/IHATEYOU.png");
	img.Init("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec id pretium felis, vel ornare risus. Etiam sit amet malesuada est. In mattis tempor odio vel finibus. Integer feugiat felis in mi porta, a tempor augue dapibus. Phasellus porta ac lectus vel mattis. Duis enim nisl, facilisis vel dui posuere, euismod luctus tortor. Donec tempor dui quis hendrerit consequat. Sed ac ante condimentum, porttitor orci id, imperdiet lacus. Sed interdum tincidunt eros id porttitor. Proin consectetur ut ligula et placerat. Fusce luctus risus a efficitur pretium.", &foni);

	particleComputeUpdate.use();
	particleComputeUpdate.setFloat("maxLifetime", particleSystem.maxLifetime);
	particleComputeUpdate.setFloat("minLifetime", particleSystem.minLifetime);
	particleComputeUpdate.setVec3("vPos", particleSystem.vPos);
	particleComputeUpdate.setVec3("vRot", particleSystem.vRot);

	glDispatchCompute(1, 1, 1);

	while (!glfwWindowShouldClose(window))
	{
		PROFILE_SCOPE("Game loop");
		{
			PROFILE_SCOPE("Input");
			glEnable(GL_DEPTH_TEST);
			processInput(window);
			viewMatrix = mainCamera.GetViewMatrix();
			viewProjectionMatrix = projectionMatrix * viewMatrix;


			glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);


		}

		//--------------------------Draw--------------------------
		{
			PROFILE_SCOPE("Render");
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





			lightingShader1.use();
			lightManager.Update(lightingShader1);
			lightingShader1.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
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
			for (int i = 0; i < pointLightVector.size(); i++) {
				boxModel.vPos = glm::vec3(pointLightVector[i].position_junk);
				boxShader.setVec3("color", pointLightVector[i].diffuse_linear.x, pointLightVector[i].diffuse_linear.y, pointLightVector[i].diffuse_linear.z);
				boxModel.Draw(boxShader);

			}



			if (!useTransparency) {
				coolSahder.use();

				coolSahder.setVec3("viewPos", mainCamera.vPos);
				coolSahder.setMat4("matProj", projectionMatrix);
				coolSahder.setMat4("matView", viewMatrix);
				float itme = glfwGetTime() / 2.0f;
				float ttresh = (sin(itme) + 1.0f) / 2.0f;
				coolSahder.setFloat("width", widthValue);
				coolSahder.setFloat("scale", scaleValue);
				coolSahder.setFloat("treshold", ttresh);
				coolSahder.setVec3("resolution", 2.0f, 2.0f, 2.0f);

				coolSahder.setVec3("transformColor", transformColor);
				coolSahder.setVec3("baseColor", baseColor);
				coolSahder.setVec3("edgeColor", edgeColor);
			}
			else {
				notCoolSahder.use();
				notCoolSahder.setVec3("viewPos", mainCamera.vPos);
				notCoolSahder.setMat4("matProj", projectionMatrix);
				notCoolSahder.setMat4("matView", viewMatrix);
				float itme = glfwGetTime() / 2.0f;
				float ttresh = (sin(itme) + 1.0f) / 2.0f;
				notCoolSahder.setFloat("width", widthValue);
				notCoolSahder.setFloat("scale", scaleValue);
				notCoolSahder.setFloat("treshold", ttresh);
				notCoolSahder.setVec3("resolution", 2.0f, 2.0f, 2.0f);

				notCoolSahder.setVec3("transformColor", transformColor);
				notCoolSahder.setVec3("baseColor", baseColor);
				notCoolSahder.setVec3("edgeColor", edgeColor);
			}

			sphereModel.vRot = glm::vec3(0.0f, 0.0f, 0.0f);
			sphereModel.vScale = glm::vec3(1.0f, 1.0f, 1.0f);
			sphereModel.vPos = glm::vec3(1.0f, 1.0f, 1.0f);

			monkyModel.vRot = glm::vec3(0.0f, 90.0f, 0.0f);
			monkyModel.vScale = glm::vec3(1.0f, 1.0f, 1.0f);
			monkyModel.vPos = glm::vec3(0.0f, 0.0f, 8.0f);


			if (!useTransparency) {
				monkyModel.Draw(coolSahder);
			}
			else {
				monkyModel.Draw(notCoolSahder);
			}



			{
				PROFILE_SCOPE("wow Upadte");
				particleSystem.Update(deltaTime);





				particlesCompute.use();
				particlesCompute.setFloat("deltaTime", deltaTime);



				particlesCompute.setVec4("start_color", particleSystem.start_color);
				particlesCompute.setVec4("end_color", particleSystem.end_color);
				particlesCompute.setVec3("gravity", particleSystem.gravity);


				particlesCompute.setFloat("maxLifetime", particleSystem.maxLifetime);
				particlesCompute.setFloat("minLifetime", particleSystem.minLifetime);




				glDispatchCompute(particleSystem.maxParticles / 1000, 1, 1);


				glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
				glFinish();


			}
			{
				PROFILE_SCOPE("wow Draw");
				particleSystem.Draw(particleShader);
			}

		}
		//--------------------------Draw--------------------------

		//--------------------------GUI--------------------------
		//--------------------------GUI--------------------------

		//--------------------------ImGui--------------------------
		{
			PROFILE_SCOPE("ImGui");
			ImGUI();
		}

		//--------------------------ImGui--------------------------
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


	for (int i = 0; i < pointLightVector.size(); i++) {
		if (ImGui::CollapsingHeader(("Light" + std::to_string(i)).c_str()))
		{
			ImGui::DragFloat3((std::to_string(i) + "-light pos").c_str(), glm::value_ptr(pointLightVector[i].position_junk));
			ImGui::ColorEdit3((std::to_string(i) + "-ambient color").c_str(), glm::value_ptr(pointLightVector[i].ambient_constant));
			ImGui::ColorEdit3((std::to_string(i) + "-diffuse color").c_str(), glm::value_ptr(pointLightVector[i].diffuse_linear));
			ImGui::ColorEdit3((std::to_string(i) + "-specular color").c_str(), glm::value_ptr(pointLightVector[i].specular_quadratic));
			ImGui::DragFloat3((std::to_string(i) + "-intensities").c_str(), glm::value_ptr(pointLightVector[i].intensities), 0.01f);
		}

	}
	if (ImGui::CollapsingHeader("Cool shader")) {
		ImGui::ColorEdit3("edgeColor", glm::value_ptr(edgeColor));
		ImGui::ColorEdit3("baseColor", glm::value_ptr(baseColor));
		ImGui::ColorEdit3("transformColor", glm::value_ptr(transformColor));
		ImGui::SliderFloat("width", &widthValue, 0.0f, 1.0f);
		ImGui::SliderFloat("scale", &scaleValue, 0.0f, 15.0f);
		ImGui::Checkbox ("cool?", &useTransparency);
		ImGui::DragFloat3("AAAAAAAAAAA",  glm::value_ptr(ROOOTOTTO));
	}


	particleSystem.ImGuiDraw();

	img.ImGuiDraw();
	img.Draw(glyphShader);



	ImGui::End();
	ImGui::Begin("OUTPUT");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "OUTPUT");
	ImGui::BeginChild("Output list");
	std::string line;
	std::ifstream myfile ("other/output.txt");
	if (myfile.is_open())
	{
		int i = 0;
		while ( getline (myfile, line) )
		{
			ImGui::Text(line.c_str(), i);
			i++;
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
		unsigned int n = 0;
		infile >> n;
		for (unsigned int i = 0; i < n; i++) {
			std::string listName;
			int listSize;
			infile >> listName >> listSize;
			if (listName == "PointLight") {
				pointLightVector.reserve(listSize);
			}
			if (listName == "SpotLight") {
				spotLightVector.reserve(listSize);
			}
			if (listName == "DirLight") {
				dirLightVector.reserve(listSize);
			}
		}
		std::string classType;
		while (infile >> classType) {
			if (classType == "PointLight") {
				LoadPointLight(infile);
			}
			if (classType == "SpotLight") {
				LoadSpotLight(infile);
			}
			if (classType == "DirLight") {
				LoadDirLight(infile);
			}
			if (classType == "Camera") {
				LoadCamera(infile);
			}
			if (classType == "ParticleSystem") {
				LoadParticleSystem(infile);
			}
		}
	}
}
void Game::SaveScene(const std::string& fileName) {
	std::ofstream outfile(fileName);
	outfile << 3 << "\n";
	outfile << "PointLight " << pointLightVector.size() << "\n";
	outfile << "SpotLight " << spotLightVector.size() << "\n";
	outfile << "DirLight " << dirLightVector.size() << "\n";
	for (int i = 0; i < pointLightVector.size(); i++) {
		SavePointLight(pointLightVector[i], outfile);
	}
	for (int i = 0; i < spotLightVector.size(); i++) {
		SaveSpotLight(spotLightVector[i], outfile);
	}
	for (int i = 0; i < dirLightVector.size(); i++) {
		SaveDirLight(dirLightVector[i], outfile);
	}
	SaveCamera(outfile);
	SaveParticleSystem(outfile);
}


void Game::LoadPointLight(std::ifstream& infile) {
	PointLight t;

	infile >> t.position_junk.x;
	infile >> t.position_junk.y;
	infile >> t.position_junk.z;
	infile >> t.ambient_constant.x;
	infile >> t.ambient_constant.y;
	infile >> t.ambient_constant.z;
	infile >> t.diffuse_linear.x;
	infile >> t.diffuse_linear.y;
	infile >> t.diffuse_linear.z;
	infile >> t.specular_quadratic.x;
	infile >> t.specular_quadratic.y;
	infile >> t.specular_quadratic.z;
	infile >> t.ambient_constant.w;
	infile >> t.diffuse_linear.w;
	infile >> t.specular_quadratic.w;
	infile >> t.intensities.x;
	infile >> t.intensities.y;
	infile >> t.intensities.z;


	pointLightVector.push_back(t);
}
void Game::SavePointLight(const PointLight& pointLight, std::ofstream& outfile) {
	outfile << "PointLight\n";
	outfile << pointLight.position_junk.x << " ";
	outfile << pointLight.position_junk.y << " ";
	outfile << pointLight.position_junk.z;
	outfile << "\n";
	outfile << pointLight.ambient_constant.x << " ";
	outfile << pointLight.ambient_constant.y << " ";
	outfile << pointLight.ambient_constant.z;
	outfile << "\n";
	outfile << pointLight.diffuse_linear.x << " ";
	outfile << pointLight.diffuse_linear.y << " ";
	outfile << pointLight.diffuse_linear.z;
	outfile << "\n";
	outfile << pointLight.specular_quadratic.x << " ";
	outfile << pointLight.specular_quadratic.y << " ";
	outfile << pointLight.specular_quadratic.z;
	outfile << "\n";
	outfile << pointLight.ambient_constant.w;
	outfile << "\n";
	outfile << pointLight.diffuse_linear.w;
	outfile << "\n";
	outfile << pointLight.specular_quadratic.w;
	outfile << "\n";
	outfile << pointLight.intensities.x << " ";
	outfile << pointLight.intensities.y << " ";
	outfile << pointLight.intensities.z;
	outfile << "\n";
}


void Game::LoadDirLight(std::ifstream& infile) {
	DirLight t;

	infile >> t.direction_junk.x;
	infile >> t.direction_junk.y;
	infile >> t.direction_junk.z;
	infile >> t.ambient_junk.x;
	infile >> t.ambient_junk.y;
	infile >> t.ambient_junk.z;
	infile >> t.diffuse_junk.x;
	infile >> t.diffuse_junk.y;
	infile >> t.diffuse_junk.z;
	infile >> t.specular_junk.x;
	infile >> t.specular_junk.y;
	infile >> t.specular_junk.z;
	infile >> t.intensities.x;
	infile >> t.intensities.y;
	infile >> t.intensities.z;

	dirLightVector.push_back(t);
}
void Game::SaveDirLight(const DirLight& pointLight, std::ofstream& outfile) {
	outfile << "DirLight\n";
	outfile << pointLight.direction_junk.x << " ";
	outfile << pointLight.direction_junk.y << " ";
	outfile << pointLight.direction_junk.z;
	outfile << "\n";
	outfile << pointLight.ambient_junk.x << " ";
	outfile << pointLight.ambient_junk.y << " ";
	outfile << pointLight.ambient_junk.z;
	outfile << "\n";
	outfile << pointLight.diffuse_junk.x << " ";
	outfile << pointLight.diffuse_junk.y << " ";
	outfile << pointLight.diffuse_junk.z;
	outfile << "\n";
	outfile << pointLight.specular_junk.x << " ";
	outfile << pointLight.specular_junk.y << " ";
	outfile << pointLight.specular_junk.z;
	outfile << "\n";
	outfile << pointLight.intensities.x << " ";
	outfile << pointLight.intensities.y << " ";
	outfile << pointLight.intensities.z;
	outfile << "\n";
}


void Game::LoadSpotLight(std::ifstream& infile) {
	SpotLight t;

	infile >> t.position_cutOff.x;
	infile >> t.position_cutOff.y;
	infile >> t.position_cutOff.z;
	infile >> t.direction_outerCutOff.x;
	infile >> t.direction_outerCutOff.y;
	infile >> t.direction_outerCutOff.z;
	infile >> t.ambient_constant.x;
	infile >> t.ambient_constant.y;
	infile >> t.ambient_constant.z;
	infile >> t.diffuse_linear.x;
	infile >> t.diffuse_linear.y;
	infile >> t.diffuse_linear.z;
	infile >> t.specular_quadratic.x;
	infile >> t.specular_quadratic.y;
	infile >> t.specular_quadratic.z;
	infile >> t.ambient_constant.w;
	infile >> t.diffuse_linear.w;
	infile >> t.specular_quadratic.w;
	infile >> t.position_cutOff.w;
	infile >> t.direction_outerCutOff.w;
	infile >> t.intensities.x;
	infile >> t.intensities.y;
	infile >> t.intensities.z;

	spotLightVector.push_back(t);
}
void Game::SaveSpotLight(const SpotLight& pointLight, std::ofstream& outfile) {
	outfile << "PointLight\n";
	outfile << pointLight.position_cutOff.x << " ";
	outfile << pointLight.position_cutOff.y << " ";
	outfile << pointLight.position_cutOff.z;
	outfile << "\n";
	outfile << pointLight.direction_outerCutOff.x << " ";
	outfile << pointLight.direction_outerCutOff.y << " ";
	outfile << pointLight.direction_outerCutOff.z;
	outfile << "\n";
	outfile << pointLight.ambient_constant.x << " ";
	outfile << pointLight.ambient_constant.y << " ";
	outfile << pointLight.ambient_constant.z;
	outfile << "\n";
	outfile << pointLight.diffuse_linear.x << " ";
	outfile << pointLight.diffuse_linear.y << " ";
	outfile << pointLight.diffuse_linear.z;
	outfile << "\n";
	outfile << pointLight.specular_quadratic.x << " ";
	outfile << pointLight.specular_quadratic.y << " ";
	outfile << pointLight.specular_quadratic.z;
	outfile << "\n";
	outfile << pointLight.ambient_constant.w;
	outfile << "\n";
	outfile << pointLight.diffuse_linear.w;
	outfile << "\n";
	outfile << pointLight.specular_quadratic.w;
	outfile << "\n";
	outfile << pointLight.position_cutOff.w;
	outfile << "\n";
	outfile << pointLight.direction_outerCutOff.w;
	outfile << "\n";
	outfile << pointLight.intensities.x << " ";
	outfile << pointLight.intensities.y << " ";
	outfile << pointLight.intensities.z;
	outfile << "\n";

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


void Game::LoadParticleSystem(std::ifstream& infile) {
	infile >> particleSystem.vPos.x;
	infile >> particleSystem.vPos.y;
	infile >> particleSystem.vPos.z;

	infile >> particleSystem.vRot.x;
	infile >> particleSystem.vRot.y;
	infile >> particleSystem.vRot.z;

	infile >> particleSystem.gravity.x;
	infile >> particleSystem.gravity.y;
	infile >> particleSystem.gravity.z;

	infile >> particleSystem.start_color.x;
	infile >> particleSystem.start_color.y;
	infile >> particleSystem.start_color.z;
	infile >> particleSystem.start_color.w;

	infile >> particleSystem.end_color.x;
	infile >> particleSystem.end_color.y;
	infile >> particleSystem.end_color.z;
	infile >> particleSystem.end_color.w;


	infile >> particleSystem.angle;
	infile >> particleSystem.minLifetime;
	infile >> particleSystem.maxLifetime;
	infile >> particleSystem.spawnrate;
	particleSystem.Init();
}
void Game::SaveParticleSystem(std::ofstream& outfile) {
	outfile << "ParticleSystem\n";
	outfile << particleSystem.vPos.x << " ";
	outfile << particleSystem.vPos.y << " ";
	outfile << particleSystem.vPos.z;
	outfile << "\n";
	outfile << particleSystem.vRot.x << " ";
	outfile << particleSystem.vRot.y << " ";
	outfile << particleSystem.vRot.z;
	outfile << "\n";
	outfile << particleSystem.gravity.x << " ";
	outfile << particleSystem.gravity.y << " ";
	outfile << particleSystem.gravity.z;
	outfile << "\n";
	outfile << particleSystem.start_color.x << " ";
	outfile << particleSystem.start_color.y << " ";
	outfile << particleSystem.start_color.z << " ";
	outfile << particleSystem.start_color.w;
	outfile << "\n";
	outfile << particleSystem.end_color.x << " ";
	outfile << particleSystem.end_color.y << " ";
	outfile << particleSystem.end_color.z << " ";
	outfile << particleSystem.end_color.w;

	outfile << "\n";
	outfile << particleSystem.angle;
	outfile << "\n";
	outfile << particleSystem.minLifetime;
	outfile << "\n";
	outfile << particleSystem.maxLifetime;
	outfile << "\n";
	outfile << particleSystem.spawnrate;
	outfile << "\n";
}