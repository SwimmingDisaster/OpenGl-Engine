#include "mypch.h"
#include "core/imGuiManager.h"
#include "core/application.h"
#include "ecs/entity.h"

#include "assets/transform.h"
#include "assets/mesh.h"
#include "assets/camera.h"

const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

ImGuiContext* ImGuiManager::imGuiContext;
std::string ImGuiManager::searchString;


float frameCountToDisplay = 0;
std::string frameImGuiText = "FPS";


void ImGuiManager::InitImGui() {
	IMGUI_CHECKVERSION();

	imGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	imGuiContext->IO.WantCaptureMouse = (imGuiContext->ActiveId != 0 && !imGuiContext->ActiveIdAllowOverlap) && (imGuiContext->HoveredWindow != NULL);


	ImGui_ImplGlfw_InitForOpenGL(Application::window, true);
	ImGui_ImplOpenGL3_Init("#version 430");


	ImGui::StyleColorsDark();
}
void ImGuiManager::ShutdownImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
void ImGuiManager::StartFrame() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	}
}

void ImGuiManager::EndFrame() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2((float)Application::SCREEN_WIDTH, (float)Application::SCREEN_HEIGHT);
	io.DeltaTime = Application::deltaTime;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* backup_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_context);
	}
}


void ImGuiManager::DrawEnity(std::shared_ptr<Entity>& entityToDraw) {

	if (entityToDraw == nullptr) {
		return;
	}

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	float lineHeight = ImGuiManager::imGuiContext->Font->FontSize + ImGuiManager::imGuiContext->Style.FramePadding.y * 2.0f;


	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	ImGui::Separator();
	bool entityopen = ImGui::TreeNodeEx(static_cast<void*>(&entityToDraw->name), treeNodeFlags, entityToDraw->name.c_str());
	ImGui::PopStyleVar();
	ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
	if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
	{
		ImGui::OpenPopup("ComponentSettings");
	}

	bool entityRemoveComponent = false;
	if (ImGui::BeginPopup("ComponentSettings"))
	{
		if (ImGui::MenuItem("Remove Entity"))
			entityRemoveComponent = true;

		ImGui::EndPopup();
	}


	if (entityopen)
	{
		ImGui::InputText("Name", &entityToDraw->name, ImGuiInputTextFlags_CallbackResize);
		ImGui::Text(("UIID: " + std::to_string(entityToDraw->uuid)).c_str());
		for (int i = 0; i < entityToDraw->m_components.size(); i++) {

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)i, treeNodeFlags, entityToDraw->m_components[i]->m_name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("*", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
				}
				if (ImGui::MenuItem("Move up")) {
					iter_swap(entityToDraw->m_components.begin() + i, entityToDraw->m_components.begin() + i - 1);
				}
				if (ImGui::MenuItem("Move down", "", false, i != entityToDraw->m_components.size() - 1)) {
					iter_swap(entityToDraw->m_components.begin() + i, entityToDraw->m_components.begin() + i + 1);
				}
				if (ImGui::MenuItem("Copy")) {
					Application::m_copiedComponent = entityToDraw->m_components[i];
				}
				if (ImGui::MenuItem("Paste")) {
					std::string str(typeid(*Application::m_copiedComponent).name());
					std::string last_element(str.substr(str.rfind(" ") + 1));

					Factory::copy(last_element, entityToDraw, Application::m_copiedComponent);
					//Application::m_copiedComponent = nullptr;
					entityToDraw->m_components[i]->Start();
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				entityToDraw->m_components[i]->Show();
				ImGui::TreePop();
			}

			if (removeComponent) {
				entityToDraw->m_components.erase(entityToDraw->m_components.begin() + i);
				i--;
			}
		}


		static bool showSearch = false;
		if (ImGui::InputText("##Add Component", &searchString, ImGuiInputTextFlags_CallbackAlways)) {
			showSearch = true;
		}


		if (showSearch) {
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 2.0f);
			if (ImGui::Button("CLOSE")) {
				showSearch = false;
				searchString = "";
			}
		}

		if (showSearch) {
			for (auto it = Factory::get_table().begin(); it != Factory::get_table().end(); ++it)
			{
				if (it->first.rfind(searchString, 0) == 0) {
					if (ImGui::Button(it->first.c_str())) {
						Factory::create(it->first, (std::shared_ptr<Entity>&)entityToDraw)->Start();
						showSearch = false;
					}
				}
			}

		}
		ImGui::TreePop();
	}

	if (entityRemoveComponent) {
		Application::m_curentScene.RemoveEntity(entityToDraw->name, entityToDraw->uuid);
		Application::m_selectedEntity = nullptr;
	}
}

void ImGuiManager::DrawEnityHierarchy(const std::shared_ptr<Entity>& entt) {
	ImGuiTreeNodeFlags flags;
	if (Application::m_selectedEntity == nullptr) {
		flags = ( 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	}
	else {
		flags = ((Application::m_selectedEntity->uuid == entt->uuid) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	}

	bool opened = ImGui::TreeNodeEx((void*)entt->uuid, flags, entt->name.c_str());

	if (ImGui::IsItemClicked()) {
		Application::m_selectedEntity = entt;
	}
	if (opened) {
		ImGui::TreePop();
	}

	if (ImGui::BeginPopupContextItem()) {

		if (ImGui::MenuItem("Delete Entity")) {
			Application::m_selectedEntity.reset();
			Application::m_curentScene.m_entities.erase(std::remove(Application::m_curentScene.m_entities.begin(), Application::m_curentScene.m_entities.end(), entt), Application::m_curentScene.m_entities.end());
		}
		ImGui::EndPopup();
	}
}

void ShapeMenu(const char* name, const char* fileName) {
	if (ImGui::MenuItem(name)) {
		auto entity = Application::m_curentScene.AddEntityR((std::string("New ") + name).c_str());
		entity->AddComponent<Transform>();
		auto modelComponent = entity->AddComponentR<Model>();
		modelComponent->path = fileName;
		entity->AddComponentR<ModelRenderer>();
		entity->Start();
		Application::m_selectedEntity = entity;
	}
}

void ImGuiManager::Update() {
	ImGuiManager::StartFrame();


	ImGui::Begin("INFO");
	frameCountToDisplay += Application::deltaTime;
	if (frameCountToDisplay >= 1.0f) {
		frameCountToDisplay = 0.0f;
		frameImGuiText = std::to_string(1 / Application::deltaTime);

	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::TextColored(ImVec4(1, 1, 0, 1), frameImGuiText.c_str());
	Application::isRunningLast = Application::isRunning;
	ImGui::Checkbox("Is running", &Application::isRunning);
	ImGui::End();//INFO

	ImGui::Begin("Hierarchy");

	for (int i = 0; i < Application::m_curentScene.m_entities.size(); i++)
		ImGuiManager::DrawEnityHierarchy(Application::m_curentScene.m_entities[i]);


	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
		Application::m_selectedEntity.reset();
	}

	if (ImGui::BeginPopupContextWindow(0, 1, false)) {

		if (ImGui::BeginMenu("Add Entity")) {
			if (ImGui::MenuItem("Empty object")) {
				Application::m_curentScene.AddEntity();
			}
			if (ImGui::BeginMenu("Model")) {
				ShapeMenu("Cube", 		"res/fbx/box.fbx");
				ShapeMenu("Plane", 		"res/fbx/plane.fbx");
				ShapeMenu("Sphere", 	"res/fbx/sphere.fbx");
				ShapeMenu("Cone", 		"res/fbx/cone.fbx");
				ShapeMenu("Torus", 		"res/fbx/torus.fbx");
				ShapeMenu("Cylinder", 	"res/fbx/cylinder.fbx");

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	ImGui::End();//Hierarchy

	ImGui::Begin("Properties");
	ImGuiManager::DrawEnity(Application::m_selectedEntity);
	ImGui::End();//Properties

	ImGui::Begin("OUTPUT");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "OUTPUT");
	ImGui::BeginChild("Output list");
	std::string line;
	std::ifstream myfile ("other/output.txt");

	if (myfile.is_open())
	{
		int i = 0;
		for (int lineCount = 0; lineCount < 100; lineCount++) {
			if (getline (myfile, line))
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
	ImGui::End();//OUTPUT


	ImGuiManager::EndFrame();
}

