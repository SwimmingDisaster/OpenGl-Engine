#include "ecs/scene.h"
#include "mypch.h"

#include "assets/transform.h"
#include "assets/mesh.h"

std::shared_ptr<Entity> Scene::GetEntity(std::string name) {
	for (std::shared_ptr<Entity>& entt : m_entities) {
		if (entt->name == name) {
			return entt;
		}
	}
	return nullptr;
}
std::shared_ptr<Entity> Scene::GetEntity(long long uuid) {
	for (std::shared_ptr<Entity>& entt : m_entities) {
		if (entt->uuid == uuid) {
			return entt;
		}
	}
	return nullptr;
}
std::shared_ptr<Entity> Scene::GetEntity(std::string name, long long uuid) {
	for (std::shared_ptr<Entity>& entt : m_entities) {
		if (entt->name == name && entt->uuid == uuid) {
			return entt;
		}
	}
	return nullptr;
}

void Scene::AddEntity() {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = "New Entity";
	entt->uuid = Random::Int();
	m_entities.push_back(entt);
}
void Scene::AddEntity(std::string name) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = name;
	entt->uuid = Random::Int();
	m_entities.push_back(entt);
}
void Scene::AddEntity(long long uuid) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = "New Entity";
	entt->uuid = uuid;
	m_entities.push_back(entt);
}
void Scene::AddEntity(std::string name, long long uuid) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = name;
	entt->uuid = uuid;
	m_entities.push_back(entt);
}


std::shared_ptr<Entity> Scene::AddEntityR() {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = "New Entity";
	entt->uuid = Random::Int();
	m_entities.push_back(entt);
	return entt;
}
std::shared_ptr<Entity> Scene::AddEntityR(std::string name) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = name;
	entt->uuid = Random::Int();
	m_entities.push_back(entt);
	return entt;
}
std::shared_ptr<Entity> Scene::AddEntityR(long long uuid) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = "New Entity";
	entt->uuid = uuid;
	m_entities.push_back(entt);
	return entt;
}
std::shared_ptr<Entity> Scene::AddEntityR(std::string name, long long uuid) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = name;
	entt->uuid = uuid;
	m_entities.push_back(entt);
	return entt;
}


void Scene::RemoveEntity(std::string name) {
	for (int i = 0; i < m_entities.size(); i++) {
		if (m_entities[i]->name == name) {
			m_entities[i]->m_components.clear();
			m_entities.erase(m_entities.begin() + i);
		}
	}
}
void Scene::RemoveEntity(long long uuid) {
	for (int i = 0; i < m_entities.size(); i++) {
		if (m_entities[i]->uuid == uuid) {
			m_entities[i]->m_components.clear();
			m_entities.erase(m_entities.begin() + i);
		}
	}
}
void Scene::RemoveEntity(std::string name, long long uuid) {
	for (int i = 0; i < m_entities.size(); i++) {
		if (m_entities[i]->name == name && m_entities[i]->uuid == uuid) {
			m_entities[i]->m_components.clear();
			m_entities.erase(m_entities.begin() + i);
		}
	}
}

void Scene::Clear() {
	for (int i = 0; i < m_entities.size(); i++) {
		m_entities[i]->m_components.clear();
		m_entities.erase(m_entities.begin() + i);
	}
}

void Scene::Serialize(const std::string& filePath) {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Unnamed scene";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto entity : m_entities) {
		out << YAML::BeginMap;

		out << YAML::Key << "Entity" <<  YAML::Value << entity->name;
		out << YAML::Key << "UUID" <<  YAML::Value << entity->uuid;

		entity->Serialize(out);

		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::fstream fout;
	fout.open(filePath, std::fstream::out);
	fout << out.c_str();
}
void Scene::Deserialize(const std::string& filePath) {
	std::ifstream stream(filePath);
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());

	std::string sceneName = data["Scene"].as<std::string>();
	auto entities = data["Entities"];
	if (entities) {
		for (auto entity : entities)
		{
			std::string name = entity["Entity"].as<std::string>();
			long long uuid = entity["UUID"].as<long long>();


			std::shared_ptr<Entity> newEntity = AddEntityR(name, uuid);


			auto transformComponent = entity["Transform"];
			if (transformComponent) {
				auto tc = newEntity->AddComponentR<Transform>();
				tc->position = transformComponent["Position"].as<glm::vec3>();
				tc->rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc->scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto ModelComponent = entity["Model"];
			if (ModelComponent) {
				auto mc = newEntity->AddComponentR<Model>();
				mc->isFlipped = ModelComponent["Is flipped"].as<bool>();
				mc->path = ModelComponent["Path"].as<std::string>();
			}

			auto ModelRendererComponent = entity["ModelRenderer"];
			if (ModelRendererComponent) {
				auto mrc = newEntity->AddComponentR<ModelRenderer>();
				mrc->shaderName = ModelRendererComponent["Shader name"].as<std::string>();
				mrc->m_color = ModelRendererComponent["Color"].as<glm::vec3>();
			}

			newEntity->Start();
		}
	}
}