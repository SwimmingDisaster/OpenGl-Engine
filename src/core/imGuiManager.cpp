#include "mypch.h"
#include "core/imGuiManager.h"
#include "core/application.h"
#include "core/input.h"
#include "ecs/entity.h"

#include "assets/transform.h"
#include "assets/mesh.h"
//#include "assets/camera.h"
#include "assets/material.h"
#include "assets/modelRenderer.h"
//#include "assets/cameraFPSController.h"

const ImGuiTreeNodeFlags treeNodeFlags =
    ImGuiTreeNodeFlags_DefaultOpen |
    ImGuiTreeNodeFlags_Framed |
    ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_AllowItemOverlap |
    ImGuiTreeNodeFlags_FramePadding;

ImGuiContext* ImGuiManager::imGuiContext;

float frameCountToDisplay = 0;
std::string frameImGuiText = "FPS";
std::string searchString;


ImVec2 contentRegionAvailable;
float lineHeight;


void ImGuiManager::InitImGui()
{
	IMGUI_CHECKVERSION();

	imGuiContext = ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	imGuiContext->IO.WantCaptureMouse = (imGuiContext->ActiveId != 0 && !imGuiContext->ActiveIdAllowOverlap) && (imGuiContext->HoveredWindow != NULL);

	//io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans-Bold.ttf", 15.0f);

	ImGui_ImplGlfw_InitForOpenGL(Application::window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	ImGui::StyleColorsDark();
}
void ImGuiManager::ShutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
void ImGuiManager::StartFrame()
{
	ImGuiIO &io = ImGui::GetIO();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	}
}
void ImGuiManager::EndFrame()
{
	ImGuiIO &io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)Application::SCREEN_WIDTH, (float)Application::SCREEN_HEIGHT);
	io.DeltaTime = Application::deltaTime;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow *backup_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_context);
	}
}

bool ShowEntityMenuAndReturnTrueIfRemoved(std::shared_ptr<Entity>& entityToDraw) {
	bool entityRemoveComponent = false;
	if (ImGui::BeginPopup("EntitySettings"))
	{
		if (ImGui::MenuItem("Remove Entity"))
			entityRemoveComponent = true;

		if (ImGui::MenuItem("Paste Component")) {
			if (Application::m_copiedComponent != nullptr) {
				std::string str(typeid(*Application::m_copiedComponent).name());
				std::string last_element(str.substr(str.rfind(" ") + 1));

				Factory::copy(last_element, entityToDraw, Application::m_copiedComponent);
				Application::m_copiedComponent = nullptr;
				entityToDraw->Start();
			}
			else {
				Error("Tried to paste a null component");
			}
		}
		if (ImGui::MenuItem("Copy Entity"))
		{
			Application::m_copiedEntity = entityToDraw;
		}
		if (ImGui::MenuItem("Paste Entity"))
		{
			entityToDraw->Copy(Application::m_copiedEntity);
			entityToDraw->uuid = Random::Int();
			Application::m_copiedEntity = nullptr;
		}

		ImGui::EndPopup();
	}
	return entityRemoveComponent;
}
bool ShowComponentMenuAndReturnTrueIfRemoved(std::shared_ptr<Entity>& entityToDraw, int i) {
	bool removeComponent = false;
	if (ImGui::BeginPopup("ComponentSettings"))
	{
		if (ImGui::MenuItem("Remove Component"))
		{
			removeComponent = true;
		}
		if (ImGui::MenuItem("Move up"))
		{
			iter_swap(entityToDraw->m_components.begin() + i, entityToDraw->m_components.begin() + i - 1);
		}
		if (ImGui::MenuItem("Move down", "", false, i != entityToDraw->m_components.size() - 1))
		{
			iter_swap(entityToDraw->m_components.begin() + i, entityToDraw->m_components.begin() + i + 1);
		}
		if (ImGui::MenuItem("Copy"))
		{
			Application::m_copiedComponent = entityToDraw->m_components[i];
		}
		if (ImGui::MenuItem("Paste"))
		{
			if (Application::m_copiedComponent != nullptr) {
				std::string str(typeid(*Application::m_copiedComponent).name());
				std::string last_element(str.substr(str.rfind(" ") + 1));

				Factory::copy(last_element, entityToDraw, Application::m_copiedComponent);
				Application::m_copiedComponent = nullptr;
				entityToDraw->m_components[i]->Start();
			}
			else {
				Error("Tried to paste a null component");
			}
		}

		ImGui::EndPopup();
	}
	return removeComponent;
}

bool ShowHeader(const std::string& headerName, const std::string& popupName, const std::string& popupIcon) {
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
	ImGui::Separator();
	bool open = ImGui::TreeNodeEx((void*)(&headerName), treeNodeFlags, headerName.c_str());
	ImGui::PopStyleVar();
	ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
	if (ImGui::Button(popupIcon.c_str(), ImVec2{lineHeight, lineHeight}))
	{
		ImGui::OpenPopup(popupName.c_str());

	}
	return open;
}
void ShowEntitySearchBar(std::shared_ptr<Entity>& entityToDraw, std::string& searchString) {
	static bool showSearch = false;
	if (ImGui::InputText("##Add Component", &searchString, ImGuiInputTextFlags_CallbackAlways))
	{
		showSearch = true;
	}

	if (showSearch)
	{
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 2.0f);
		if (ImGui::Button("CLOSE"))
		{
			showSearch = false;
			searchString = "";
		}
	}

	if (showSearch)
	{
		for (auto it = Factory::get_table().begin(); it != Factory::get_table().end(); ++it)
		{
			if (it->first.rfind(searchString, 0) == 0)
			{
				if (ImGui::Button(it->first.c_str()))
				{
					Factory::create(it->first, (std::shared_ptr<Entity> &)entityToDraw)->Start();
					showSearch = false;
				}
			}
		}
	}
	ImGui::TreePop();
}

void DrawEnity(std::shared_ptr<Entity>& entityToDraw)
{

	if (entityToDraw == nullptr)
	{
		return;
	}

	contentRegionAvailable = ImGui::GetContentRegionAvail();
	lineHeight = ImGuiManager::imGuiContext->Font->FontSize + ImGuiManager::imGuiContext->Style.FramePadding.y * 2.0f;


	bool isEntityopen = ShowHeader(entityToDraw->name, "EntitySettings", "-");
	bool isEntityRemoved = ShowEntityMenuAndReturnTrueIfRemoved(entityToDraw);

	if (isEntityopen)
	{

		ImGui::InputText("Name", &entityToDraw->name, ImGuiInputTextFlags_CallbackResize);
		ImGui::Text(("UIID: " + std::to_string(entityToDraw->uuid)).c_str());
		for (int i = 0; i < entityToDraw->m_components.size(); i++)
		{

			bool isComponentOpen = ShowHeader(entityToDraw->m_components[i]->name, "ComponentSettings", "*");
			bool isComponentRemoved = ShowComponentMenuAndReturnTrueIfRemoved(entityToDraw, i);

			if (isComponentOpen)
			{
				entityToDraw->m_components[i]->Show();
				ImGui::TreePop();
			}
			if (isComponentRemoved)
			{
				entityToDraw->m_components.erase(entityToDraw->m_components.begin() + i);
				i--;
			}
		}
		ShowEntitySearchBar(entityToDraw, searchString);
	}

	if (isEntityRemoved)
	{
		Application::m_curentScene.RemoveEntity(entityToDraw->name, entityToDraw->uuid);
		Application::m_selectedEntity = nullptr;
	}
}
void DrawEnityHierarchy(const std::shared_ptr<Entity> &entt)
{
	ImGuiTreeNodeFlags flags;
	if (Application::m_selectedEntity == nullptr)
	{
		flags = (0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	}
	else
	{
		flags = ((Application::m_selectedEntity->uuid == entt->uuid) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	}

	bool opened = ImGui::TreeNodeEx((void *)entt->uuid, flags, entt->name.c_str());

	if (ImGui::IsItemClicked())
	{
		Application::m_selectedEntity = entt;
	}
	if (opened)
	{
		ImGui::TreePop();
	}

	if (ImGui::BeginPopupContextItem())
	{

		if (ImGui::MenuItem("Delete Entity"))
		{
			Application::m_selectedEntity.reset();
			Application::m_curentScene.RemoveEntity(entt);
		}
		if (ImGui::MenuItem("Copy Entity"))
		{
			Application::m_copiedEntity = entt;
		}
		if (ImGui::MenuItem("Paste Entity"))
		{
			entt->Copy(Application::m_copiedEntity);
			Application::m_copiedEntity = nullptr;
		}
		ImGui::EndPopup();
	}
}

void ShapeMenu(const char *name, const char *fileName)
{
	if (ImGui::MenuItem(name))
	{
		auto entity = Application::m_curentScene.AddEntityR((std::string("New ") + name).c_str());
		entity->AddComponent<Transform>();
		auto modelComponent = entity->AddComponentR<Model>();
		modelComponent->path = fileName;
		entity->AddComponentR<ModelRenderer>();
		entity->AddComponentR<Material>();
		entity->Start();
		Application::m_selectedEntity = entity;
	}
}


void ShowSaveAndOpenMenuItems() {
	bool openScene = (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_O)) && !Application::isRunning;
	bool save = (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_S));
	bool saveas = (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyHeld(INPUT_KEY_LEFT_SHIFT) && Input::IsKeyPressed(INPUT_KEY_S)) && !Application::isRunning;


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Open scene", "CTRL+O", false, !Application::isRunning))
			{
				openScene = true;
			}
			else if (ImGui::MenuItem("Save", "CTRL+S", false, true))
			{
				save = true;
			}
			else if (ImGui::MenuItem("Save as", "CTRL+SHIFT", false, !Application::isRunning))
			{
				saveas = true;
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();


	if (openScene)
	{
		Application::sceneFileName = OpenFile("All files\0*.*\0");
		std::replace( Application::sceneFileName.begin(), Application::sceneFileName.end(), '\\', '/');
		if (Application::sceneFileName != "")
			Application::m_curentScene.Deserialize(Application::sceneFileName);
	}
	else if (saveas)
	{
		Application::sceneFileName = SaveFile("All files\0*.*\0");
		std::replace( Application::sceneFileName.begin(), Application::sceneFileName.end(), '\\', '/');
		if (Application::sceneFileName != "")
			Application::m_curentScene.Serialize(Application::sceneFileName);
	}
	else if (save)
	{
		if (Application::sceneFileName != "") {
			Application::m_curentScene.Serialize(Application::sceneFileName);
			if (Application::isRunning) {
				Application::m_curentScene.Serialize("other/TEMP.scene");
			}
		}

	}
}
void ShowInfoPanel() {
	ImGui::Begin("INFO");

	ImGui::InputText("Scene name", &Application::m_curentScene.name, ImGuiInputTextFlags_CallbackAlways);


	frameCountToDisplay += Application::deltaTime;
	if (frameCountToDisplay >= 1.0f)
	{
		frameCountToDisplay = 0.0f;
		frameImGuiText = std::to_string(1 / Application::deltaTime);
	}
	ImGui::TextColored(ImVec4(1, 1, 0, 1), frameImGuiText.c_str());

	Application::isRunningLast = Application::isRunning;
	ImGui::Checkbox("Is running", &Application::isRunning);

	ImGui::End(); //INFO
}
void ShowHierarchyPanel() {
	ImGui::Begin("Hierarchy");

	for (int i = 0; i < Application::m_curentScene.m_entities.size(); i++)
		DrawEnityHierarchy(Application::m_curentScene.m_entities[i]);

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		Application::m_selectedEntity.reset();
	}

	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{

		if (ImGui::BeginMenu("Add Entity"))
		{
			if (ImGui::MenuItem("Empty object"))
			{
				Application::m_curentScene.AddEntity();
			}
			if (ImGui::BeginMenu("Model"))
			{
				ShapeMenu("Cube", "res/fbx/box.fbx");
				ShapeMenu("Plane", "res/fbx/plane.fbx");
				ShapeMenu("Sphere", "res/fbx/sphere.fbx");
				ShapeMenu("Cone", "res/fbx/cone.fbx");
				ShapeMenu("Torus", "res/fbx/torus.fbx");
				ShapeMenu("Cylinder", "res/fbx/cylinder.fbx");

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Paste Entity")) {
			if (Application::m_copiedEntity != nullptr) {
				auto copiedEntity = Application::m_curentScene.AddEntityR();
				copiedEntity->Copy(Application::m_copiedEntity);
				copiedEntity->uuid = Random::Int();
				Application::m_copiedEntity = nullptr;
			}
			else {
				Error("Tried to paste a null entity");
			}
		}
		ImGui::EndPopup();
	}

	if (Input::IsKeyPressed(INPUT_KEY_DELETE)) {
		Application::m_curentScene.RemoveEntity(Application::m_selectedEntity);
		Application::m_selectedEntity.reset();
	}
	if (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_C)) {
		Application::m_copiedEntity = Application::m_selectedEntity;
	}
	if (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_V)) {
		if (Application::m_copiedEntity) {
			if (Application::m_selectedEntity) {
				Application::m_selectedEntity->Copy(Application::m_copiedEntity);
				Application::m_selectedEntity->uuid = Random::Int();
				Application::m_copiedEntity = nullptr;
			}
			else {
				auto copiedEntity = Application::m_curentScene.AddEntityR();
				copiedEntity->Copy(Application::m_copiedEntity);
				copiedEntity->uuid = Random::Int();
				Application::m_copiedEntity = nullptr;
			}
		}
		else {
			Error("Tried to paste a null entity");
		}
	}


	ImGui::End(); //Hierarchy
}
void ShowPropertiesPanel() {
	ImGui::Begin("Properties");
	DrawEnity(Application::m_selectedEntity);
	ImGui::End(); //Properties
}
void ShowOutputPanel() {
	ImGui::Begin("OUTPUT");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "OUTPUT");
	ImGui::BeginChild("Output list");
	std::string line;
	std::ifstream myfile("other/output.txt");

	if (myfile.is_open())
	{
		int i = 0;
		for (int lineCount = 0; lineCount < 100; lineCount++)
		{
			if (getline(myfile, line))
			{
				ImGui::Text(line.c_str(), i);
				i++;
			}
			else
			{
				break;
			}
		}
		myfile.close();
	}
	ImGui::EndChild();
	ImGui::End(); //OUTPUT
}

void ImGuiManager::Update()
{
	StartFrame();

	ShowSaveAndOpenMenuItems();
	ShowInfoPanel();
	ShowHierarchyPanel();
	ShowPropertiesPanel();
	ShowOutputPanel();

	EndFrame();
}

