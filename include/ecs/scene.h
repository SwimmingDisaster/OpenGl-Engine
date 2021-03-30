#pragma once
#include "mypch.h"
#include "entity.h"

class Scene
{
public:
	std::vector< std::shared_ptr<Entity> > m_entities;
	std::string name;

	std::shared_ptr<Entity>  GetEntity(std::string name);
	std::shared_ptr<Entity>  GetEntity(long long uuid);
	std::shared_ptr<Entity>  GetEntity(std::string name, long long uuid);

	void AddEntity(std::string name = "New Entity", long long uuid = -1);

	std::shared_ptr<Entity> AddEntityR(std::string name = "New Entity", long long uuid = -1);

	void RemoveEntity(std::string name);
	void RemoveEntity(long long uuid);
	void RemoveEntity(std::string name, long long uuid);

	void Clear();

	void Serialize(const std::string& filePath);
	void Deserialize(const std::string& filePath);

	void RemoveEntity(const std::shared_ptr<Entity>& entity);

	~Scene();
};