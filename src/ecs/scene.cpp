#include "ecs/scene.h"
#include "mypch.h"

#include "assets/transform.h"
#include "assets/mesh.h"
#include "ecs/other/componentFactory.h"

#include "core/application.h"

Scene::~Scene() {
	Clear();
}

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


void Scene::AddEntity(std::string name, long long uuid) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = name;
	if (uuid != -1)
		entt->uuid = uuid;
	else
		entt->uuid = Random::Int();
	m_entities.push_back(entt);
}


std::shared_ptr<Entity> Scene::AddEntityR(std::string name, long long uuid) {
	std::shared_ptr<Entity> entt = std::make_shared<Entity>();
	entt->name = name;
	if (uuid != -1)
		entt->uuid = uuid;
	else
		entt->uuid = Random::Int();
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
		i--;
	}
	m_entities.clear();
}

void Scene::Serialize(const std::string & filePath) {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << Application::m_curentScene.name;
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto entity : m_entities) {
		out << YAML::BeginMap;

		out << YAML::Key << "Entity" <<  YAML::Value << entity->name;
		out << YAML::Key << "UUID" <<  YAML::Value << entity->uuid;


		out << YAML::Flow;
		out << YAML::Key << "Components" << YAML::Flow << YAML::BeginSeq;
		for (auto component : entity->m_components)
			out << component->m_name;
		out << YAML::EndSeq;

		entity->Serialize(out);

		out << YAML::EndMap;
	}

	out << YAML::EndSeq;

	out << YAML::Key << "Editor Camera" << YAML::Value << YAML::BeginSeq;
	out << YAML::BeginMap;
	out << YAML::Key << "Camera vPos" <<  YAML::Value << Application::mainCamera.vPos;
	out << YAML::Key << "Camera vUp" <<  YAML::Value << Application::mainCamera.vUp;
	out << YAML::Key << "Camera vRot" <<  YAML::Value << Application::mainCamera.vRot;
	out << YAML::EndMap;
	out << YAML::EndSeq;

	out << YAML::EndMap;

	std::fstream fout;
	fout.open(filePath, std::fstream::out);
	fout << out.c_str();
}
void Scene::Deserialize(const std::string & filePath) {
	std::ifstream stream(filePath);
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());

	Application::m_curentScene.name = data["Scene"].as<std::string>();

	/*
	data["Scene"] = "Me and ju";
	*/


	const YAML::Node& entities = data["Entities"];
	if (entities) {
		for (auto entity : entities)
		{
			std::string name = entity["Entity"].as<std::string>();
			long long uuid = entity["UUID"].as<long long>();

			std::vector<std::string> componentNames = entity["Components"].as<std::vector<std::string> >();


			std::shared_ptr<Entity> newEntity = AddEntityR(name, uuid);
			for (int i = 0; i < componentNames.size(); i++)
			{
				const YAML::Node& componentData = entity[componentNames[i]];
				if (componentData) {
					auto mrc = Factory::create(componentNames[i], newEntity);
					mrc->Deserialize(componentData);
				}
			}

			newEntity->Start();
		}
	}

	const YAML::Node& mainCameraData = data["Editor Camera"];
	Application::mainCamera.vPos = 	mainCameraData[0]["Camera vPos"].as<glm::vec3>();
	Application::mainCamera.vUp  = 	mainCameraData[0]["Camera vUp" ].as<glm::vec3>();
	Application::mainCamera.vRot = 	mainCameraData[0]["Camera vRot"].as<glm::vec3>();
	Application::mainCamera.updateCameraVectors();

	/*
		std::fstream fout;
		fout.open(filePath, std::fstream::out);
		fout << data;
	*/
}