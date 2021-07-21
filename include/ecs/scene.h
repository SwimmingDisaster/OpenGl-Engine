#pragma once
#include "mypch.h"
#include "entity.h"

class Scene
{
public:
	std::vector< std::shared_ptr<Entity> > m_entities;
	std::string name;


public:
	std::shared_ptr<Entity> GetEntity(std::string name) const noexcept;
	std::shared_ptr<Entity> GetEntity(long long uuid) const noexcept;
	std::shared_ptr<Entity> GetEntity(std::string name, long long uuid) const noexcept;
	std::shared_ptr<Entity>	GetEntityWithUUID(long long uuid) const noexcept;
	std::shared_ptr<Entity> GetEntityWithUUID(const std::string& uuidstring) const noexcept;

	void AddEntity(std::string name = "New Entity", long long uuid = -1);
	std::shared_ptr<Entity> AddEntityR(std::string name = "New Entity", long long uuid = -1, int tag = 0, int layer = 0);

	void RemoveEntity(std::string name) noexcept;
	void RemoveEntity(long long uuid) noexcept;
	void RemoveEntity(std::string name, long long uuid) noexcept;

	void Clear() noexcept;

	void Update() const;
	void Render() const;

	void Serialize(const std::string& filePath) const;
	void Deserialize(const std::string& filePath);

	void RemoveEntity(const std::shared_ptr<Entity>& entity);

};
