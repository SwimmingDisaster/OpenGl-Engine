#pragma once
#include "mypch.h"
#include "entity.h"

class Scene
{
public:
	std::vector<std::unique_ptr<Entity> > m_entities;
	std::string name;


public:
	[[nodiscard]] Entity* GetEntity(std::string name) const noexcept;
	[[nodiscard]] Entity* GetEntity(long long uuid) const noexcept;
	[[nodiscard]] Entity* GetEntity(std::string name, long long uuid) const noexcept;
	[[nodiscard]] Entity* GetEntityWithUUID(long long uuid) const noexcept;
	[[nodiscard]] Entity* GetEntityWithUUID(const std::string& uuidstring) const noexcept;

	Entity* AddEntity(std::string name = "New Entity", long long uuid = -1, int tag = 0, int layer = 0);

	void RemoveEntity(std::string name) noexcept;
	void RemoveEntity(long long uuid) noexcept;
	void RemoveEntity(std::string name, long long uuid) noexcept;

	void Clear() noexcept;

	void Update() const;
	void Render() const;

	void Serialize(const std::string& filePath) const;
	void Deserialize(const std::string& filePath);

	void RemoveEntity(const Entity* entity);
};
