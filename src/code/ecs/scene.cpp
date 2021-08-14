#include "mypch.h"

#include "ecs/scene.h"
#include "ecs/other/componentFactory.h"

#include "core/application.h"
#include "core/batchRenderer.h"
#include "core/physics.h"
#include "core/renderer.h"

#include "core/tag.h"
#include "core/layer.h"

#include "assets/ui/text.h"
#include "assets/transform.h"
#include "assets/mesh.h"
#include "assets/modelRenderer.h"


std::shared_ptr<Entity> Scene::GetEntity(std::string name) const noexcept{
    for (auto &entt : m_entities)
    {
        if (entt->GetName() == name)
        {
            return entt;
        }
    }
    return nullptr;
}
std::shared_ptr<Entity> Scene::GetEntity(long long uuid) const noexcept{
    for (auto &entt : m_entities)
    {
        if (entt->GetUUID() == uuid)
        {
            return entt;
        }
    }
    return nullptr;
}
std::shared_ptr<Entity> Scene::GetEntityWithUUID(long long uuid) const noexcept{
    for (auto &entt : m_entities)
    {
        if (entt->GetUUID() == uuid)
        {
            return entt;
        }
    }
    return nullptr;
}
std::shared_ptr<Entity> Scene::GetEntityWithUUID(const std::string& uuidstring) const noexcept{
    for (auto &entt : m_entities)
    {
        if (entt->GetUUIDString() == uuidstring)
        {
            return entt;
        }
    }
    return nullptr;
}
std::shared_ptr<Entity> Scene::GetEntity(std::string name, long long uuid) const noexcept{
    for (auto &entt : m_entities)
    {
        if (entt->GetName() == name && entt->GetUUID() == uuid)
        {
            return entt;
        }
    }
    return nullptr;
}

std::shared_ptr<Entity> Scene::AddEntity(std::string name, long long uuid, int tag, int layer){
    std::shared_ptr<Entity> entt = std::make_shared<Entity>();
    entt->SetName(name);
    entt->SetUUID(uuid);
    entt->SetTag(tag);
    entt->SetLayer(layer);
    m_entities.push_back(entt);
    return entt;
}

void Scene::RemoveEntity(std::string name) noexcept{
    for (unsigned long i = 0; i < m_entities.size(); i++)
    {
        if (m_entities[i]->GetName() == name)
        {
            m_entities[i]->m_components.clear();
            m_entities.erase(m_entities.begin() + i);
        }
    }
}
void Scene::RemoveEntity(long long uuid) noexcept{
    for (unsigned long i = 0; i < m_entities.size(); i++)
    {
        if (m_entities[i]->GetUUID() == uuid)
        {
            m_entities[i]->m_components.clear();
            m_entities.erase(m_entities.begin() + i);
        }
    }
}
void Scene::RemoveEntity(std::string name, long long uuid) noexcept{
    for (unsigned long i = 0; i < m_entities.size(); i++)
    {
        if (m_entities[i]->GetName() == name && m_entities[i]->GetUUID() == uuid)
        {
            m_entities[i]->m_components.clear();
            m_entities.erase(m_entities.begin() + i);
        }
    }
}

void Scene::Clear() noexcept{
    for(auto& entity : m_entities) {
        entity->End();
    }
    for (unsigned long i = 0; i < m_entities.size(); i++)
    {
        m_entities[i]->m_components.clear();
        m_entities.erase(m_entities.begin() + i);
        i--;
    }
    m_entities.clear();

    BatchRenderer::Clear();

    Shader::shaderMap.clear();
    Shader::shaderList.clear();
    Shader::shaderNames.clear();

    TagManager::tagList.clear();
    TagManager::tagList.emplace_back();

    TextureManager::textureMap.clear();
    TextureManager::textureList.clear();
}
void Scene::Update() const{
    for (unsigned long i = 0; i < m_entities.size(); i++)
    {
        m_entities[i]->Update();
    }
}
void Scene::Render() const{
    for (std::size_t i = 0; i < m_entities.size(); i++){
        auto modelRendererComponent = m_entities[i]->GetComponent<ModelRenderer>();
        if (modelRendererComponent != nullptr)
        {
            modelRendererComponent->Draw();
        }
    }
    BatchRenderer::Draw();
    for (std::size_t i = 0; i < m_entities.size(); i++) {
        auto textRendererComponent = m_entities[i]->GetComponent<Text>();
        if (textRendererComponent != nullptr)
        {
            textRendererComponent->Draw();
        }
    }
    for (std::size_t i = 0; i < m_entities.size(); i++) {
        auto lightRendererComponent = m_entities[i]->GetComponent<LightBase>();
        if (lightRendererComponent != nullptr)
        {
            lightRendererComponent->Draw();
        }
    }
	Renderer::forwardFrameBuffer.Bind(); 
	LightsBatchRenderer::Draw();
	Renderer::forwardFrameBuffer.Unbind(); 
}

void Scene::Serialize(const std::string &filePath) const{
#ifdef SHOW_SCENE_SERIALISATION
    Log("Serialized: " << filePath);
#endif

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << Application::GetScene().name;

    out << YAML::Key << "Shaders" << YAML::Value << YAML::BeginSeq;
    for (auto shaderName : Shader::shaderNames)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Shader Path" << YAML::Value << shaderName;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
    for (auto textureName : TextureManager::textureList)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Texture Path" << YAML::Value << textureName;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Tags" << YAML::Value << YAML::BeginSeq;
    for (unsigned long i = 1; i < TagManager::tagList.size(); i++)
    {
        std::string& tagName = TagManager::tagList[i];
        out << YAML::BeginMap;
        out << YAML::Key << "Tag Name" << YAML::Value << tagName;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Layers" << YAML::Value << YAML::BeginSeq;
    for (auto layerName : LayerManager::layerList)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Layer Name" << YAML::Value << layerName;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Collision layer mask" << YAML::Value << YAML::BeginSeq;
    for (auto layerName : PhysicsManager::collisionLayerMask)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Layer Name" << YAML::Value << layerName;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Notify layer mask" << YAML::Value << YAML::BeginSeq;
    for (auto layerName : PhysicsManager::collisionLayerMask)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Layer Name" << YAML::Value << layerName;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    for (auto entity : m_entities)
    {
        out << YAML::BeginMap;

        out << YAML::Key << "Entity" << YAML::Value << entity->GetName();
        out << YAML::Key << "UUID" << YAML::Value << entity->GetUUID();
        out << YAML::Key << "Tag" << YAML::Value << entity->GetTag();
        out << YAML::Key << "Layer" << YAML::Value << entity->GetLayer();

        out << YAML::Flow;
        out << YAML::Key << "Components" << YAML::Flow << YAML::BeginSeq;
        for (auto component : entity->m_components)
            out << component->name;
        out << YAML::EndSeq;

        entity->Serialize(out);

        out << YAML::EndMap;
    }

    out << YAML::EndSeq;

#ifndef RELEASE_BUILD
    out << YAML::Key << "Editor Camera" << YAML::Value << YAML::BeginSeq;
    out << YAML::BeginMap;
    out << YAML::Key << "Camera vPos" << YAML::Value << Application::editorCamera.vPos;
    out << YAML::Key << "Camera vUp" << YAML::Value << Application::editorCamera.vUp;
    out << YAML::Key << "Camera vRot" << YAML::Value << Application::editorCamera.vRot;
    out << YAML::EndMap;
    out << YAML::EndSeq;
#endif

    out << YAML::EndMap;

    std::fstream fout;
    fout.open(filePath, std::fstream::out);
    fout << out.c_str();
}
void Scene::Deserialize(const std::string &filePath){

#ifdef SHOW_SCENE_SERIALISATION
    Log("Deserialized: " << filePath);
#endif

    Clear();
    std::ifstream stream(filePath);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());

    name = data["Scene"].as<std::string>();


    const YAML::Node &shaders = data["Shaders"];
    for (auto shader : shaders) {
        std::string shaderName = shader["Shader Path"].as<std::string>();
        Shader::shaderList.push_back(std::make_shared<Shader>());
        Shader::shaderNames.push_back(shaderName);
        Shader::shaderList[Shader::shaderList.size() - 1]->CreateVertexAndFragment(shaderName);
    }

    const YAML::Node &textures = data["Textures"];
    for (auto texture : textures) {
        std::string textureName = texture["Texture Path"].as<std::string>();
        TextureManager::textureList.push_back(textureName);
        TextureManager::textureMap[textureName] = (int)loadTexture(textureName.c_str());
    }

    const YAML::Node &tags = data["Tags"];
    for (auto tag : tags) {
        std::string tagName = tag["Tag Name"].as<std::string>();
        TagManager::tagList.push_back(tagName);
    }

    const YAML::Node &layers = data["Layers"];
    int j = 0;
    for (auto layer : layers) {
        std::string layerName = layer["Layer Name"].as<std::string>();
        LayerManager::layerList[j] = layerName;
        j++;
    }

    const YAML::Node &collisionLayerMaskValues = data["Collision layer mask"];
    int k = 0;
    for (auto collisionLayerMaskValue : collisionLayerMaskValues) {
        int layerName = collisionLayerMaskValue["Layer Name"].as<int>();
        PhysicsManager::collisionLayerMask[k] = layerName;
        k++;
    }

    const YAML::Node &notifyLayerMaskValues = data["Notify layer mask"];
    int l = 0;
    for (auto notifyLayerMaskValue : notifyLayerMaskValues) {
        int layerName = notifyLayerMaskValue["Layer Name"].as<int>();
        PhysicsManager::notifyLayerMask[l] = layerName;
        l++;
    }


    const YAML::Node &entities = data["Entities"];
    if (entities)
    {
        for (auto entity : entities)
        {
            std::string name = entity["Entity"].as<std::string>();
            long long uuid = entity["UUID"].as<long long>();
            int tag = entity["Tag"].as<int>();
            int layer = entity["Layer"].as<int>();

            std::vector<std::string> componentNames = entity["Components"].as<std::vector<std::string>>();

            std::shared_ptr<Entity> newEntity = AddEntity(name, uuid, tag, layer);
            for (unsigned long i = 0; i < componentNames.size(); i++)
            {
                const YAML::Node &componentData = entity[componentNames[i]];
                if (componentData)
                {
                    auto mrc = Factory::create(componentNames[i], newEntity);
                    mrc->Deserialize(componentData);
                }
            }

            newEntity->Start();
        }
    }



#ifndef RELEASE_BUILD
    const YAML::Node &mainCameraData = data["Editor Camera"];
    Application::editorCamera.vPos = mainCameraData[0]["Camera vPos"].as<glm::vec3>();
    Application::editorCamera.vUp = mainCameraData[0]["Camera vUp"].as<glm::vec3>();
    Application::editorCamera.vRot = mainCameraData[0]["Camera vRot"].as<glm::vec3>();
    Application::editorCamera.updateCameraVectors();
#endif
}

void Scene::RemoveEntity(const std::shared_ptr<Entity> &entity){
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
}
