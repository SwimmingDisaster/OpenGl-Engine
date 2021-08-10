#pragma once
#include "mypch.h"
#include "component.h"

#include "ecs/other/componentFactory.h"

class Entity : public std::enable_shared_from_this<Entity> {
public:
	std::vector< std::shared_ptr<Component> > m_components;

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
	
	void Start();
	void Update();
	void End();
	void Serialize(YAML::Emitter& out) const;
	void Deserialize(YAML::Node& data);
	void OnCollision(const std::shared_ptr<Entity>& other);

	Entity() {}
#ifdef SHOW_DELETED
public:
	virtual ~Entity();
#else
public:
	virtual ~Entity() {};
#endif

public:

	void Copy(const std::shared_ptr<Entity>& other);

	template<typename T> [[nodiscard]] constexpr std::shared_ptr<T> GetComponent() const noexcept;
	template<typename T> [[nodiscard]] constexpr std::shared_ptr<T> GetComponentOrMakeIfNone() noexcept;
	template<typename T> [[nodiscard]] constexpr std::shared_ptr<T> AddComponentR();
	template<typename T> constexpr void AddComponent();
	template<typename T> void RemoveComponent();
};

template<typename T> [[nodiscard]] constexpr std::shared_ptr<T> Entity::GetComponent() const noexcept {
    for (auto& comp : m_components) {
        if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
        {
            return castedComp;
        }
    }
    return nullptr;
}


template<typename T> [[nodiscard]] constexpr std::shared_ptr<T> Entity::GetComponentOrMakeIfNone() noexcept{
    for (auto& comp : m_components) {
        if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
        {
            return castedComp;
        }
    }
    return AddComponentR<T>();

}


template<typename T> [[nodiscard]] constexpr std::shared_ptr<T> Entity::AddComponentR() {
    std::shared_ptr<T> comp = std::make_shared<T>();
    comp->parentEntity = shared_from_this();
    m_components.push_back(comp);
    return comp;
}



template<typename T> constexpr void Entity::AddComponent() {
    std::shared_ptr<T> comp = std::make_shared<T>();
    comp->parentEntity = shared_from_this();
    m_components.push_back(comp);
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
