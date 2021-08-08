#include "mypch.h"
#include "core/imGuiManager.h"
#include "core/application.h"
#include "core/input.h"
#include "core/renderer.h"
#include "core/physics.h"

#include "core/tag.h"
#include "core/layer.h"

#include "ecs/entity.h"

#include "assets/transform.h"
#include "assets/mesh.h"
#include "assets/material.h"
#include "assets/modelRenderer.h"
#include "assets/rigidbody.h"



#ifndef RELEASE_BUILD
const ImGuiTreeNodeFlags treeNodeFlags =
    ImGuiTreeNodeFlags_DefaultOpen |
    ImGuiTreeNodeFlags_Framed |
    ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_AllowItemOverlap |
    ImGuiTreeNodeFlags_FramePadding;

ImGuiContext *ImGuiManager::imGuiContext;

void ImGuiManager::InitImGui()
{
    IMGUI_CHECKVERSION();

    imGuiContext = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle &style = ImGui::GetStyle();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0U)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    imGuiContext->IO.WantCaptureMouse = (imGuiContext->ActiveId != 0U && !imGuiContext->ActiveIdAllowOverlap) && (imGuiContext->HoveredWindow != nullptr);

    //io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans-Bold.ttf", 15.0f);

    ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow(), true);
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

    if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0U)
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
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0U)
    {
        GLFWwindow *backup_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_context);
    }
}

[[nodiscard]] bool ShowEntityMenuAndReturnTrueIfRemoved(std::shared_ptr<Entity> &entityToDraw)
{
    bool entityRemoveComponent = false;
    if (ImGui::BeginPopup("EntitySettings"))
    {
        if (ImGui::MenuItem("Remove Entity")) {
            entityRemoveComponent = true;
        }

        if (ImGui::MenuItem("Paste Component"))
        {
            if (Application::GetCopiedComponent() != nullptr)
            {
                Factory::copy(Application::GetCopiedComponent()->name, entityToDraw, Application::GetCopiedComponent());
                Application::GetCopiedComponent() = nullptr;
                entityToDraw->Start();
            }
            else
            {
                Error("Tried to paste a null component");
            }
        }
        if (ImGui::MenuItem("Copy Entity"))
        {
            Application::GetCopiedEntity() = entityToDraw;
        }
        if (ImGui::MenuItem("Paste Entity"))
        {
            entityToDraw->Copy(Application::GetCopiedEntity());
            entityToDraw->SetUUID(Random::Int());
            Application::GetCopiedEntity() = nullptr;
        }

        ImGui::EndPopup();
    }
    return entityRemoveComponent;
}
[[nodiscard]] bool ShowComponentMenuAndReturnTrueIfRemoved(const std::shared_ptr<Entity> &entityToDraw, std::vector<std::shared_ptr<Component>>::iterator& iter)
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
            iter_swap(iter, iter - 1);
        }
        if (ImGui::MenuItem("Move down", "", false, iter != entityToDraw->m_components.end()))
        {
            iter_swap(iter, iter + 1);
        }
        if (ImGui::MenuItem("Copy"))
        {
            Application::GetCopiedComponent() = *iter;
        }
        if (ImGui::MenuItem("Paste"))
        {
            if (Application::GetCopiedComponent() != nullptr)
            {
                Factory::copy(Application::GetCopiedComponent()->name, entityToDraw, Application::GetCopiedComponent());
                Application::GetCopiedComponent() = nullptr;
                (*iter)->Start();
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

[[nodiscard]] bool ShowHeader(const std::string& headerName, const std::string& popupName, const std::string& popupIcon)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
    ImGui::Separator();
    bool open = ImGui::TreeNodeEx((void*)&headerName, treeNodeFlags, "%s", headerName.c_str());
    ImGui::PopStyleVar();

    static const float lineHeight = ImGuiManager::imGuiContext->Font->FontSize + ImGuiManager::imGuiContext->Style.FramePadding.y * 2.0f;
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
    if (ImGui::Button(popupIcon.c_str(), ImVec2{lineHeight, lineHeight}))
    {
        ImGui::OpenPopup(popupName.c_str());
    }
    return open;
}
void ShowEntitySearchBar(const std::shared_ptr<Entity>& entityToDraw)
{
    static bool showSearch = false;
    static std::string searchString;
    if (ImGui::InputText("##Add Component", &searchString, ImGuiInputTextFlags_CallbackAlways))
    {
        showSearch = true;
    }

    if (showSearch)
    {
        static const float lineHeight = ImGuiManager::imGuiContext->Font->FontSize + ImGuiManager::imGuiContext->Style.FramePadding.y * 2.0f;
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 2.0f);
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
			ToLower(string1); ToLower(string2);
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

void ShowEntityTagAndLayer(const std::shared_ptr<Entity>& entityToDraw) {
    ImGui::TextUnformatted(("Tag: "s += TagManager::tagList[entityToDraw->GetTag()] += " ("s += std::to_string(entityToDraw->GetTag()) += ")"s).c_str());
    ImGui::SameLine();
    if(ImGui::Button("Change tag")) {
        ImGui::OpenPopup("Tag Popup");
    }
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize * 0.25f);
    if(ImGui::BeginPopupModal("Tag Popup")) {
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
        for(int i = 0; i < TagManager::tagList.size(); i++) {
            if(ImGui::MenuItem(TagManager::tagList[i].c_str())) {
                entityToDraw->SetTag(i);
            }
        }
        ImGui::EndPopup();
    }

    ImGui::TextUnformatted(("Layer: "s += LayerManager::layerList[entityToDraw->GetLayer()] += " ("s += std::to_string(entityToDraw->GetLayer()) += ")"s).c_str());
    ImGui::SameLine();
    if(ImGui::Button("Change layer")) {
        ImGui::OpenPopup("Layer Popup");
    }
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize * 0.25f);
    if(ImGui::BeginPopupModal("Layer Popup")) {
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
        for(int i = 0; i < LayerManager::layerList.size(); i++) {
            if(ImGui::MenuItem(LayerManager::layerList[i].c_str())) {
                entityToDraw->SetLayer(i);
            }
        }
        ImGui::EndPopup();
    }
}

void ShowEntity(const std::shared_ptr<Entity>& entityToDraw) {
    ImGui::InputText("Name", &entityToDraw->GetNameReference(), ImGuiInputTextFlags_CallbackResize);
    ImGui::TextUnformatted(("UIID: " + std::to_string(entityToDraw->GetUUID())).c_str());

    ShowEntityTagAndLayer(entityToDraw);

    for (auto component = entityToDraw->m_components.begin(); component != entityToDraw->m_components.end(); ++component)
    {

        bool isComponentOpen = ShowHeader((*component)->name, "ComponentSettings", "*");
        bool isComponentRemoved = ShowComponentMenuAndReturnTrueIfRemoved(entityToDraw, component);

        if (isComponentOpen)
        {
            (*component)->Show();
            ImGui::TreePop();
        }
        if (isComponentRemoved)
        {
            entityToDraw->m_components.erase(component);
            component--;
        }
    }
    ShowEntitySearchBar(entityToDraw);
}

void DrawEntity(std::shared_ptr<Entity> &entityToDraw)
{

    if (entityToDraw == nullptr) {
        return;
    }

    static const ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
    static const float lineHeight = ImGuiManager::imGuiContext->Font->FontSize + ImGuiManager::imGuiContext->Style.FramePadding.y * 2.0f;

    bool isEntityopen = ShowHeader(entityToDraw->GetName(), "EntitySettings", "-");
    bool isEntityRemoved = ShowEntityMenuAndReturnTrueIfRemoved(entityToDraw);

    if (isEntityopen)
    {
        ShowEntity(entityToDraw);
    }

    if (isEntityRemoved)
    {
        Application::GetSceneModifiable().RemoveEntity(entityToDraw->GetName(), entityToDraw->GetUUID());
        Application::GetSelectedEntity() = nullptr;
    }
}
void DrawEntityHierarchy(const std::shared_ptr<Entity> &entt, int i)
{
    ImGuiTreeNodeFlags flags = 0U;
    if (Application::GetSelectedEntity() == nullptr)
    {
        flags = (0U) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    }
    else
    {
        flags = ((Application::GetSelectedEntity()->GetUUID() == entt->GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0U) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    }

    bool opened = ImGui::TreeNodeEx((void *)entt->GetUUID(), flags, "%s", entt->GetName().c_str());

    if (ImGui::IsItemClicked())
    {
        Application::GetSelectedEntity() = entt;
    }
    if (opened)
    {
        ImGui::TreePop();
    }

    if (ImGui::BeginPopupContextItem())
    {

        if (ImGui::MenuItem("Delete Entity"))
        {
            Application::GetSelectedEntity().reset();
            Application::GetSceneModifiable().RemoveEntity(entt);
        }
        if (ImGui::MenuItem("Copy Entity"))
        {
            Application::GetCopiedEntity() = entt;
        }
        if (ImGui::MenuItem("Paste Entity"))
        {
            entt->Copy(Application::GetCopiedEntity());
            Application::GetCopiedEntity() = nullptr;
        }
        ImGui::MenuItem("--------------");
        if (ImGui::MenuItem("Move up")) {
            iter_swap(Application::GetSceneModifiable().m_entities.begin() + i, Application::GetSceneModifiable().m_entities.begin() + i - 1);
        }
        if (ImGui::MenuItem("Move down")) {
            iter_swap(Application::GetSceneModifiable().m_entities.begin() + i, Application::GetSceneModifiable().m_entities.begin() + i + 1);
        }
        ImGui::EndPopup();
    }
}

void ShapeMenu(const char *name, const char *fileName)
{
    if (ImGui::MenuItem(name))
    {
        auto entity = Application::GetSceneModifiable().AddEntity(std::string("New ") + name);
        entity->AddComponent<Transform>();
        auto modelComponent = entity->AddComponentR<Model>();
        modelComponent->path = fileName;
        entity->AddComponent<ModelRenderer>();
        entity->AddComponent<Material>();
        entity->Start();
        Application::GetSelectedEntity() = entity;
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

    static std::string sceneFileName;

    if (openScene)
    {
        nfdfilteritem_t filterItemList[1] = { { "Scene", "scene,.scene" } };
        sceneFileName = OpenFile(filterItemList, 1);
        if (!sceneFileName.empty()) {
            Application::GetSceneModifiable().Deserialize(sceneFileName);
        }
    }
    else if (saveas)
    {
        nfdfilteritem_t filterItemList[1] = { { "Scene", "scene" } };
        sceneFileName = SaveFile(filterItemList, 1);
        if (!sceneFileName.empty()) {
            Application::GetSceneModifiable().Serialize(sceneFileName);
        }
    }
    else if (save)
    {
        if (sceneFileName.empty())
        {
            Application::GetSceneModifiable().Serialize(sceneFileName);
            if (Application::isRunning)
            {
                Application::GetSceneModifiable().Serialize("other/TEMP.scene");
            }
        }
    }
}
void ShowInfoPanel()
{
    ImGui::Begin("INFO");

    ImGui::InputText("Scene name", &Application::GetSceneModifiable().name, ImGuiInputTextFlags_CallbackAlways);

    static float frameCountToDisplay = 0;
    static std::string frameImGuiText = "FPS";
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

void HandleHierarchyKeyboardShortcuts() {
    if (Input::IsKeyPressed(INPUT_KEY_DELETE))
    {
        Application::GetSceneModifiable().RemoveEntity(Application::GetSelectedEntity());
        Application::GetSelectedEntity().reset();
    }
    if (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_C))
    {
        Application::GetCopiedEntity() = Application::GetSelectedEntity();
    }
    if (Input::IsKeyHeld(INPUT_KEY_LEFT_CONTROL) && Input::IsKeyPressed(INPUT_KEY_V))
    {
        if (Application::GetCopiedEntity())
        {
            if (Application::GetSelectedEntity())
            {
                Application::GetSelectedEntity()->Copy(Application::GetCopiedEntity());
                Application::GetSelectedEntity()->SetUUID(Random::Int());
                Application::GetCopiedEntity() = nullptr;
            }
            else
            {
                auto copiedEntity = Application::GetSceneModifiable().AddEntity();
                copiedEntity->Copy(Application::GetCopiedEntity());
                copiedEntity->SetUUID(Random::Int());
                Application::GetCopiedEntity() = nullptr;
            }
        }
        else
        {
            Error("Tried to paste a null entity");
        }
    }
}


void ShowRightClickHierarchyContextMenu() {
    if (ImGui::BeginPopupContextWindow(nullptr, 1, false))
    {

        if (ImGui::BeginMenu("Add Entity"))
        {
            if (ImGui::MenuItem("Empty object"))
            {
                Application::GetSceneModifiable().AddEntity();
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
            if (Application::GetCopiedEntity() != nullptr)
            {
                auto copiedEntity = Application::GetSceneModifiable().AddEntity();
                copiedEntity->Copy(Application::GetCopiedEntity());
                copiedEntity->SetUUID(Random::Int());
                Application::GetCopiedEntity() = nullptr;
            }
            else
            {
                Error("Tried to paste a null entity");
            }
        }
        ImGui::EndPopup();
    }
}
void ShowHierarchyPanel()
{
    ImGui::Begin("Hierarchy");
    for (int i = 0; i < Application::GetScene().m_entities.size(); i++) {
        DrawEntityHierarchy(Application::GetScene().m_entities[i], i);
    }

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
        Application::GetSelectedEntity().reset();
    }

    ShowRightClickHierarchyContextMenu();
    HandleHierarchyKeyboardShortcuts();

    ImGui::End(); //Hierarchy
}
void ShowPropertiesPanel()
{
    ImGui::Begin("Properties");
    DrawEntity(Application::GetSelectedEntity());
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
    static int imguizmoType;

    if (Input::IsKeyPressed(INPUT_KEY_W))
    {
        imguizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_E))
    {
        imguizmoType = ImGuizmo::OPERATION::SCALE;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_R))
    {
        imguizmoType = ImGuizmo::OPERATION::ROTATE;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_T))
    {
        imguizmoType = -1;
    }
    else if (Input::IsKeyPressed(INPUT_KEY_Q))
    {
        imguizmoType = -2;
    }

    Renderer::SetWindowSize(Application::GetWindow(), (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

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

    if (Application::GetSelectedEntity() && imguizmoType != -2)
    {
        auto tc = Application::GetSelectedEntity()->GetComponent<Transform>();
        if (tc)
        {

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            glm::mat4 viewMat = Renderer::viewMatrix;
            glm::mat4 projMat = Renderer::projectionMatrix;
            glm::mat4 transformMat = tc->GetTransform();

            ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), (ImGuizmo::OPERATION)imguizmoType, ImGuizmo::LOCAL, glm::value_ptr(transformMat));

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation;
                glm::vec3 rotation;
                glm::vec3 scale;
                DecomposeTransform(transformMat, translation, rotation, scale);
                tc->position = translation;
                tc->scale = scale;
                auto rc = Application::GetSelectedEntity()->GetComponent<Rigidbody>();
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
    bool open = ImGui::TreeNodeEx((void *)"Shaders", treeNodeFlags ^ ImGuiTreeNodeFlags_DefaultOpen, "%s", "Shaders");
    if(open) {
        for(int i = 0; i < Shader::shaderNames.size(); i++) {
            ImGui::InputText(("Shader Name " + std::to_string(i)).c_str(), &Shader::shaderNames[i], ImGuiInputTextFlags_CallbackResize);
            ImGui::SameLine();
            if (ImGui::Button(("Browse##" + std::to_string(i)).c_str())) {
                std::string& shaderName = Shader::shaderNames[i];
                shaderName = OpenFile(nullptr, 0);
                shaderName = shaderName.substr(0, shaderName.size() - 3);
            }
        }


        if (ImGui::Button("Add new shader")) {
            Shader::shaderNames.emplace_back("");
        }


        static int indexToDelete;
        ImGui::InputInt("Delete index", &indexToDelete);
        if (ImGui::Button("Delete")) {
            Shader::shaderNames.erase(Shader::shaderNames.begin() + indexToDelete);
        }
        ImGui::TreePop();
    }
}

void ShowTexturesPanel() {
    bool open = ImGui::TreeNodeEx((void *)"Textures", treeNodeFlags ^ ImGuiTreeNodeFlags_DefaultOpen, "%s", "Textures");
    if(open) {
        for(int i = 0; i < TextureManager::textureList.size(); i++) {
            ImGui::InputText(("Texture Name " + std::to_string(i)).c_str(), &TextureManager::textureList[i], ImGuiInputTextFlags_CallbackResize);
            ImGui::SameLine();
            if (ImGui::Button(("Browse##" + std::to_string(i)).c_str())) {
                TextureManager::textureList[i] = OpenFile(nullptr, 0);
            }
        }

        if (ImGui::Button("Add new texture")) {
            TextureManager::textureList.emplace_back("");
        }

        static int indexToDelete;
        ImGui::InputInt("Delete index", &indexToDelete);
        if (ImGui::Button("Delete")) {
            TextureManager::textureList.erase(TextureManager::textureList.begin() + indexToDelete);
        }
        ImGui::TreePop();
    }
}

void ShowTagsPanel() {
    bool open = ImGui::TreeNodeEx((void *)"Tags", treeNodeFlags ^ ImGuiTreeNodeFlags_DefaultOpen, "%s", "Tags");
    if(open) {
        for(int i = 1; i < TagManager::tagList.size(); i++) {
            ImGui::InputText(("Tag " + std::to_string(i)).c_str(), &TagManager::tagList[i], ImGuiInputTextFlags_CallbackResize);
        }

        if (ImGui::Button("Add new tag")) {
            TagManager::tagList.emplace_back("");
        }
        static int indexToDelete;
        ImGui::InputInt("Delete index", &indexToDelete);
        if (ImGui::Button("Delete")) {
            TagManager::tagList.erase(TagManager::tagList.begin() + indexToDelete);
        }
        ImGui::TreePop();
    }
}
void ShowLayersPanel() {
    bool open = ImGui::TreeNodeEx((void *)"Layers", treeNodeFlags ^ ImGuiTreeNodeFlags_DefaultOpen, "%s", "Layers");
    if(open) {
        for(int i = 0; i < LayerManager::layerList.size(); i++) {
            ImGui::InputText(("Layer " + std::to_string(i)).c_str(), &LayerManager::layerList[i], ImGuiInputTextFlags_CallbackResize);
        }
        ImGui::TreePop();
    }
}


void ShowPhysicsMask(std::array<int, 32>& arr, const std::string& name1, const std::string& name2) {
    bool open = ImGui::TreeNodeEx((void *)&name1, treeNodeFlags ^ ImGuiTreeNodeFlags_DefaultOpen, "%s",name1.c_str());
    static bool showAll = false;
    if(open) {
        ImGui::TextUnformatted(name2.c_str());
        ImGui::SameLine();
        ImGui::Checkbox("Show all", &showAll);
        static int clickedi = 0;
        for(int i = 0; i < LayerManager::layerList.size(); i++) {
            if(!showAll && LayerManager::layerList[i].empty()) {
                continue;
            }
            if(ImGui::Button(LayerManager::layerList[i].c_str())) {
                clickedi = i;
                ImGui::OpenPopup(name2.c_str());
            }
        }
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize * 0.25f);
        if(ImGui::BeginPopupModal(name2.c_str())) {
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }
            for(int j = 0; j < LayerManager::layerList.size(); j++) {
                bool checkboxbool = ((arr[clickedi]) & (1<<(j))) != 0;
                bool tempbool = checkboxbool;
                ImGui::Checkbox(LayerManager::layerList[j].c_str(), &checkboxbool);
                if(checkboxbool != tempbool) {
                    arr[clickedi] ^= 1UL << j;
                    if(j != clickedi) {
                        arr[j] ^= 1UL << clickedi;
                    }
                }
            }
            ImGui::EndPopup();
        }
        ImGui::TreePop();
    }
}
void ShowPhysicsPanel() {
    ShowPhysicsMask(PhysicsManager::collisionLayerMask, "Physics - collision mask", "Change collision mask for:");
    ShowPhysicsMask(PhysicsManager::notifyLayerMask, "Physics - notify mask", "Change notify mask for:");
}
void ShowEnvironmentPanel() {
    ImGui::Begin("Environment");

    ShowShadersPanel();
    ShowTexturesPanel();
    ShowTagsPanel();
    ShowLayersPanel();
    ShowPhysicsPanel();

    ImGui::End();
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
    ShowEnvironmentPanel();

    EndFrame();
}
#endif
