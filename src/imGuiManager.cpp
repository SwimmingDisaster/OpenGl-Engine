#include "mypch.h"
#include "imGuiManager.h"
#include "application.h"
#include "entity.h"


const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

ImGuiContext* ImGuiManager::imGuiContext;

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

void ImGuiManager::DrawEnity(const std::shared_ptr<Entity>& entityToDraw) {

	ImGui::InputText("Name", &entityToDraw->name, ImGuiInputTextFlags_CallbackResize);
	ImGui::Text(("UIID: " + std::to_string(entityToDraw->uuid)).c_str());
	for (int i = 0; i < entityToDraw->m_components.size(); i++) {
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = ImGuiManager::imGuiContext->Font->FontSize + ImGuiManager::imGuiContext->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)i, treeNodeFlags, entityToDraw->m_components[i]->m_name.c_str());
		ImGui::PopStyleVar(
		);
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("*", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
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
}