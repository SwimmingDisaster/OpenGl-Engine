#include "ecs/scene.h"


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