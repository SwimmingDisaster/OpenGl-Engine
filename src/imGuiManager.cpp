#include "mypch.h"
#include "imGuiManager.h"
#include "application.h"

void ImGuiManager::InitImGui() {
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
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