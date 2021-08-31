#pragma once
#include "mypch.h"
#include "component.h"

#include "ecs/other/componentFactory.h"

class Entity{
public:
	std::vector<std::unique_ptr<Component>> m_components;

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


	template<typename T> [[nodiscard]] T* GetComponent() const noexcept;
	template<typename T> [[nodiscard]] T* GetComponentOrMakeIfNone() noexcept;
	template<typename T> [[nodiscard]] T* GetComponentByName(const std::string& name) const noexcept;
	template<typename T> T* AddComponent();
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

template<typename T> [[nodiscard]] T* Entity::GetComponent() const noexcept {
	for (const auto& comp : m_components) {
		if ( T* const castedComp = dynamic_cast<T*>(comp.get()))
		{
			return castedComp;
		}
	}
	return nullptr;
}
template<typename T> [[nodiscard]]  T* Entity::GetComponentOrMakeIfNone() noexcept {
	for (const auto& comp : m_components) {
		if ( T* const castedComp = dynamic_cast<T*>(comp.get()))
		{
			return castedComp;
		}
	}
	return AddComponent<T>();

}
template<typename T> [[nodiscard]] T* Entity::GetComponentByName(const std::string& name) const noexcept{
	for (const auto& comp : m_components) {
		if (comp->GetName() == name)
		{
			return static_cast<T*>(comp.get());
		}
	}
	return nullptr;
}
template<typename T> T* Entity::AddComponent() {
	const auto& comp = m_components.emplace_back(new T);
	comp->parentEntity = this;
	return static_cast<T*>(comp.get());
}
template<typename T> void Entity::RemoveComponent() {
	for (unsigned long i = 0; i < m_components.size(); i++) {
		if ( T* const castedComp = std::dynamic_pointer_cast<T>( m_components[i] ) )
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}
