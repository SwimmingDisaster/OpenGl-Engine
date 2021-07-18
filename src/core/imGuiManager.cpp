#include "glm/fwd.hpp"
#include "mypch.h"
#include "core/imGuiManager.h"
#include "core/application.h"
#include "core/input.h"
#include "ecs/entity.h"

#include "assets/transform.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/modelRenderer.h"
#include "assets/rigidbody.h"

#include "core/renderer.h"


#ifndef RELEASE_BUILD
const ImGuiTreeNodeFlags treeNodeFlags =
    ImGuiTreeNodeFlags_DefaultOpen |
    ImGuiTreeNodeFlags_Framed |
    ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_AllowItemOverlap |
    ImGuiTreeNodeFlags_FramePadding;

ImGuiContext *ImGuiManager::imGuiContext;

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
    ImGuizmo::BeginFrame();

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    }
}
void ImGuiManager::EndFrame()
{
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)EngineInfo::SCREEN_WIDTH, (float)EngineInfo::SCREEN_HEIGHT);
    io.DeltaTime = EngineInfo::deltaTime;

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

bool ShowEntityMenuAndReturnTrueIfRemoved(std::shared_ptr<Entity> &entityToDraw)
{
    bool entityRemoveComponent = false;
    if (ImGui::BeginPopup("EntitySettings"))
    {
        if (ImGui::MenuItem("Remove Entity"))
            entityRemoveComponent = true;

        if (ImGui::MenuItem("Paste Component"))
        {
            if (Application::copiedComponent != nullptr)
            {
                Factory::copy(Application::copiedComponent->name, entityToDraw, Application::copiedComponent);
                Application::copiedComponent = nullptr;
                entityToDraw->Start();
            }
            else
            {
                Error("Tried to paste a null component");
            }
        }
        if (ImGui::MenuItem("Copy Entity"))
        {
            Application::copiedEntity = entityToDraw;
        }
        if (ImGui::MenuItem("Paste Entity"))
        {
            entityToDraw->Copy(Application::copiedEntity);
            entityToDraw->uuid = Random::Int();
            Application::copiedEntity = nullptr;
        }

        ImGui::EndPopup();
    }
    return entityRemoveComponent;
}
bool ShowComponentMenuAndReturnTrueIfRemoved(std::shared_ptr<Entity> &entityToDraw, int i)
{
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
            Application::copiedComponent = entityToDraw->m_components[i];
        }
        if (ImGui::MenuItem("Paste"))
        {
            if (Application::copiedComponent != nullptr)
            {
                Factory::copy(Application::copiedComponent->name, entityToDraw, Application::copiedComponent);
                Application::copiedComponent = nullptr;
                entityToDraw->m_components[i]->Start();
            }
            else
            {
                Error("Tried to paste a null component");
            }
        }

        ImGui::EndPopup();
    }
    return removeComponent;
}



bool ShowHeader(const std::string &headerName, const std::string &popupName, const std::string &popupIcon)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
    ImGui::Separator();
    bool open = ImGui::TreeNodeEx((void *)(&headerName), treeNodeFlags, "%s", headerName.c_str());
    ImGui::PopStyleVar();
    ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
    if (ImGui::Button(popupIcon.c_str(), ImVec2{lineHeight, lineHeight}))
    {
        ImGui::OpenPopup(popupName.c_str());
    }
    return open;
}
void ShowEntitySearchBar(std::shared_ptr<Entity> &entityToDraw, std::string &searchString)
{
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
            std::string string1 = it->first;
            std::string string2 = searchString;
            std::transform(string1.begin(), string1.end(), string1.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            std::transform(string2.begin(), string2.end(), string2.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            if (string1.rfind(string2, 0) == 0)
            {
                bool clicked = ImGui::Button(it->first.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f) );
                if (clicked)
                {
                    Factory::create(it->first, (std::shared_ptr<Entity> &)entityToDraw)->Start();
                    showSearch = false;
                }
            }
        }
    }
    ImGui::TreePop();
}

void DrawEnity(std::shared_ptr<Entity> &entityToDraw)
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
        ImGui::TextUnformatted(("UIID: " + std::to_string(entityToDraw->uuid)).c_str());
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
        Application::selectedEntity = nullptr;
    }
}
void DrawEnityHierarchy(const std::shared_ptr<Entity> &entt, int i)
{
    ImGuiTreeNodeFlags flags;
    if (Application::selectedEntity == nullptr)
    {
        flags = (0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    }
    else
    {
        flags = ((Application::selectedEntity->uuid == entt->uuid) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    }

    bool opened = ImGui::TreeNodeEx((void *)entt->uuid, flags, "%s", entt->name.c_str());

    if (ImGui::IsItemClicked())
    {
        Application::selectedEntity = entt;
    }
    if (opened)
    {
        ImGui::TreePop();
    }

    if (ImGui::BeginPopupContextItem())
    {

        if (ImGui::MenuItem("Delete Entity"))
        {
            Application::selectedEntity.reset();
            Application::m_curentScene.RemoveEntity(entt);
        }
        if (ImGui::MenuItem("Copy Entity"))
        {
            Application::copiedEntity = entt;
        }
        if (ImGui::MenuItem("Paste Entity"))
        {
            entt->Copy(Application::copiedEntity);
            Application::copiedEntity = nullptr;
        }
        ImGui::MenuItem("--------------");
        if (ImGui::MenuItem("Move up")) {
            iter_swap(Application::m_curentScene.m_entities.begin() + i, Application::m_curentScene.m_entities.begin() + i - 1);
        }
        if (ImGui::MenuItem("Move down")) {
            iter_swap(Application::m_curentScene.m_entities.begin() + i, Application::m_curentScene.m_entities.begin() + i + 1);
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
        entity->AddComponent<ModelRenderer>();
        entity->AddComponent<Material>();
        entity->Start();
        Application::selectedEntity = entity;
    }
}

void ShowSaveAndOpenMenuItems()
{
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
        Application::sceneFileName = OpenFile(NULL, 0);
        if (Application::sceneFileName != "")
            Application::m_curentScene.Deserialize(Application::sceneFileName);
    }
    else if (saveas)
    {
        Application::sceneFileName = SaveFile(NULL, 0);
        if (Application::sceneFileName != "")
            Application::m_curentScene.Serialize(Application::sceneFileName);
    }
    else if (save)
    {
        if (Application::sceneFileName != "")
        {
            Application::m_curentScene.Serialize(Application::sceneFileName);
            if (Application::isRunning)
            {
                Application::m_curentScene.Serialize("other/TEMP.scene");
            }
        }
    }
}
void ShowInfoPanel()
{
    ImGui::Begin("INFO");

    ImGui::InputText("Scene name", &Application::m_curentScene.name, ImGuiInputTextFlags_CallbackAlways);

    frameCountToDisplay += EngineInfo::deltaTime;
    if (frameCountToDisplay >= 1.0f)
    {
        frameCountToDisplay = 0.0f;
        frameImGuiText = std::to_string(1 / EngineInfo::deltaTime);
    }
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", frameImGuiText.c_str());

    Application::isRunningLast = Application::isRunning;
    ImGui::Checkbox("Is running", &Application::isRunning);

    ImGui::End(); //INFO
}
void ShowHierarchyPanel()
{
    ImGui::Begin("Hierarchy");
    for (int i = 0; i < Application::m_curentScene.m_entities.size(); i++)
        DrawEnityHierarchy(Application::m_curentScene.m_entities[i], i);

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
        Application::selectedEntity.reset();
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
        if (ImGui::MenuItem("Paste Entity"))
        {
            if (Application::copiedEntity != nullptr)
            {
                auto copiedEntity = Application::m_curentScene.AddEntityR();
                copiedEntity->Copy(Application::copiedEntity);
                copiedEntity->uuid = Random::Int();
                Application::copiedEntity = nullptr;
            }
            else
            {
                Error("Tried to paste a null entity");
            }
        }
        ImGui::EndPopup();
    }

    if (Input::IsKeyPressed(INPUT_KEY_DELETE))
    {
        Application::m_curentScene.RemoveEntity(Application::selectedEntity);
        Application::selectedEntity.reset();
    }
    if (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_C))
    {
        Application::copiedEntity = Application::selectedEntity;
    }
    if (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_V))
    {
        if (Application::copiedEntity)
        {
            if (Application::selectedEntity)
            {
                Application::selectedEntity->Copy(Application::copiedEntity);
                Application::selectedEntity->uuid = Random::Int();
                Application::copiedEntity = nullptr;
            }
            else
            {
                auto copiedEntity = Application::m_curentScene.AddEntityR();
                copiedEntity->Copy(Application::copiedEntity);
                copiedEntity->uuid = Random::Int();
                Application::copiedEntity = nullptr;
            }
        }
        else
        {
            Error("Tried to paste a null entity");
        }
    }
    ImGui::End(); //Hierarchy
}
void ShowPropertiesPanel()
{
    ImGui::Begin("Properties");
    DrawEnity(Application::selectedEntity);
    ImGui::End(); //Properties
}
void ShowOutputPanel()
{
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
void ShowImGuizmo()
{
    ImGui::Begin("Scene");

    if (Input::IsKeyPressed(INPUT_KEY_W))
    {
        Application::imguizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_E))
    {
        Application::imguizmoType = ImGuizmo::OPERATION::SCALE;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_R))
    {
        Application::imguizmoType = ImGuizmo::OPERATION::ROTATE;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_T))
    {
        Application::imguizmoType = -1;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_Q))
    {
        Application::imguizmoType = -2;
    }

    Renderer::SetWindowSize(Application::window, (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

    ImVec4 borderColor;
    if (Application::isRunning)
    {
        borderColor = ImVec4(1, 0, 0, 1);
    }
    else
    {
        borderColor = ImVec4(0, 1, 1, 1);
    }
    ImGui::Image((void*)(uintptr_t)Renderer::frameBuffer.GetData(), ImVec2(EngineInfo::SCREEN_WIDTH - 2.0f, EngineInfo::SCREEN_HEIGHT - 2.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1, 1, 1, 1), borderColor);

    if (Application::selectedEntity && Application::imguizmoType != -2)
    {
        auto tc = Application::selectedEntity->GetComponent<Transform>();
        if (tc)
        {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            glm::mat4 viewMat = Renderer::viewMatrix;
            glm::mat4 projMat = Renderer::projectionMatrix;
            glm::mat4 transformMat = tc->GetTransform();

            ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), (ImGuizmo::OPERATION)Application::imguizmoType, ImGuizmo::LOCAL, glm::value_ptr(transformMat));

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                DecomposeTransform(transformMat, translation, rotation, scale);
                tc->position = translation;
                tc->scale = scale;
                auto rc = Application::selectedEntity->GetComponent<Rigidbody>();
                if (rc && Application::isRunning) {
                    rc->aDynamicActor->setGlobalPose({tc->position.x, tc->position.y, tc->position.z}, false);
                }

                glm::vec3 deltaRot = glm::degrees(rotation) - tc->rotation;
                tc->rotation += deltaRot;
            }
        }
    }
    ImGui::End();
}

void ShowShadersPanel() {
    ImGui::Begin("Shaders");
    for(int i = 0; i < Shader::shaderNames.size(); i++) {
        ImGui::InputText(("Shader Name " + std::to_string(i)).c_str(), &Shader::shaderNames[i], ImGuiInputTextFlags_CallbackResize);
        ImGui::SameLine();
        if (ImGui::Button(("Browse##" + std::to_string(i)).c_str())) {
			std::string& shaderName = Shader::shaderNames[i];
            shaderName = OpenFile(NULL, 0);
            shaderName = shaderName.substr(0, shaderName.size() - 3);
        }
    }


    if (ImGui::Button("Add new shader")) {
        Shader::shaderNames.push_back("");
    }


    static int indexToDelete;
    ImGui::InputInt("Delete index", &indexToDelete);
    if (ImGui::Button("Delete")) {
        Shader::shaderNames.erase(Shader::shaderNames.begin() + indexToDelete);
    }
    ImGui::End(); //Shaders
}

void ShowTexturesPanel() {
    ImGui::Begin("Textures");
    for(int i = 0; i < TextureManager::textureList.size(); i++) {
        ImGui::InputText(("Texture Name " + std::to_string(i)).c_str(), &TextureManager::textureList[i], ImGuiInputTextFlags_CallbackResize);
        ImGui::SameLine();
        if (ImGui::Button(("Browse##" + std::to_string(i)).c_str())) {
            TextureManager::textureList[i] = OpenFile(NULL, 0);
        }
    }

    if (ImGui::Button("Add new texture")) {
        TextureManager::textureList.push_back("");
    }

    static int indexToDelete;
    ImGui::InputInt("Delete index", &indexToDelete);
    if (ImGui::Button("Delete")) {
        TextureManager::textureList.erase(TextureManager::textureList.begin() + indexToDelete);
    }
    ImGui::End(); //Textures
}

void ImGuiManager::Update()
{
    StartFrame();

    ShowSaveAndOpenMenuItems();
    ShowImGuizmo();
    ShowInfoPanel();
    ShowHierarchyPanel();
    ShowPropertiesPanel();
    ShowOutputPanel();
    ShowShadersPanel();
    ShowTexturesPanel();

    EndFrame();
}
#endif
