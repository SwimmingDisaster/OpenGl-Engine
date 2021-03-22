#include "mypch.h"
#include "core/imGuiManager.h"
#include "core/application.h"
#include "ecs/entity.h"

#include "assets/transform.h"



const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

ImGuiContext* ImGuiManager::imGuiContext;
std::string ImGuiManager::searchString;

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


std::string sTransfom = "Transform";


void ImGuiManager::DrawEnity(const std::shared_ptr<Entity>& entityToDraw) {

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
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

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
			if (sTransfom.rfind(searchString, 0) == 0) {
				if (ImGui::Button(sTransfom.c_str())) {
					entityToDraw->AddComponent<Transform>();
					showSearch = false;
				}
			}
		}
		ImGui::TreePop();
	}


	/*
		ImGui::InputText("Search Text", &searchString, ImGuiInputTextFlags_CallbackAlways);*/
	/*	if (ImGui::Button("Add component")) {
			ImGui::OpenPopup("AddComponent");
		}*/








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


}


