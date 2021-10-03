#pragma once
#include "mypch.h"
#include "ecs/component.h"

namespace YAML {
class Emitter;
}
namespace YAML {
class Node;
}

class Entity {
public:
	std::vector<std::shared_ptr<Component>> m_components;
	std::vector<std::unique_ptr<Entity>> m_children;

private:
	int tag = 0;
	int layer = 0;
	std::string name;
	std::string uuidString;
	long long uuid;

public:
	void SetName(const std::string& othername);
	void SetUUID(const long long otheruuid);
	void SetTag(const int othertag);
	void SetLayer(const int othertag);

	[[nodiscard]] const std::string& GetName() const;
	[[nodiscard]] const long long& GetUUID() const;
	[[nodiscard]] const std::string& GetUUIDString() const;
	[[nodiscard]] const int& GetTag() const;
	[[nodiscard]] const int& GetLayer() const;
	[[nodiscard]] std::string& GetNameReference();


	template<typename T> [[nodiscard]] std::shared_ptr<T> GetComponent() const noexcept;
	template<typename T> [[nodiscard]] std::shared_ptr<T> GetComponentOrMakeIfNone() noexcept;
	template<typename T> [[nodiscard]] std::shared_ptr<T> GetComponentByName(const std::string& name) const noexcept;
	template<typename T> std::shared_ptr<T> AddComponent();
	template<typename T> void RemoveComponent();

	void Start();
	void Update();
	void End();
	void Serialize(YAML::Emitter& out) const;
	void Deserialize(YAML::Node& data);
	void OnCollision(const Entity* other);

	void Copy(const Entity* const other);
	Entity() {}
#ifdef SHOW_DELETED
public:
	virtual ~Entity();
#else
public:
	virtual ~Entity() {};
#endif
};

template<typename T> [[nodiscard]] std::shared_ptr<T> Entity::GetComponent() const noexcept {
	for (auto& comp : m_components) {
		if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
		{
			return castedComp;
		}
	}
	return nullptr;
}
template<typename T> [[nodiscard]] std::shared_ptr<T> Entity::GetComponentOrMakeIfNone() noexcept {
	for (auto& comp : m_components) {
		if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
		{
			return castedComp;
		}
	}
	return AddComponent<T>();

}
template<typename T> [[nodiscard]] std::shared_ptr<T> Entity::GetComponentByName(const std::string& name) const noexcept {
	for (const auto& comp : m_components) {
		if (comp->GetName() == name)
		{
			return std::dynamic_pointer_cast<T>(comp);
		}
	}
	return nullptr;
}
template<typename T> std::shared_ptr<T> Entity::AddComponent() {
	std::shared_ptr<T> comp = std::make_shared<T>();
	comp->parentEntity = this;
	m_components.push_back(comp);
	return comp;
}
template<typename T> void Entity::RemoveComponent() {
	for (unsigned long i = 0; i < m_components.size(); i++) {
		if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( m_components[i] ) )
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}
