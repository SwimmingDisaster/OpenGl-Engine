#pragma once
#include "mypch.h"
#include "component.h"

#include "ecs/other/componentFactory.h"

class Entity : public std::enable_shared_from_this<Entity> {
public:
	std::vector< std::shared_ptr<Component> > m_components;

private:
	std::string name;
	std::string uuidString;
	long long uuid;

public:
	void SetName(std::string& othername);
	void SetUUID(long long otheruuid);

	const std::string& GetName();
	const long long& GetUUID();
	const std::string& GetUUIDString();

	std::string& GetNameReference();
	
	virtual void Start();
	virtual void Update();
	virtual void End();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(YAML::Node& data);

	Entity() {}
public:
	virtual ~Entity(){};

public:

	void Copy(const std::shared_ptr<Entity>& other) {
		name = other->name;
		uuid = other->uuid;

		for (auto& component : other->m_components) {
			Factory::copy(component.get()->name, shared_from_this(), component);
		}
		for (auto& component : m_components) {
			component->name = component.get()->name;
			component->parentEntity = shared_from_this();
		}
		Start();

	}

	template<typename T> std::shared_ptr<T> GetComponent() const noexcept {
		for (auto comp : m_components) {
			if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
			{
				return castedComp;
			}
		}
		return nullptr;
	}


	template<typename T> [[nodiscard]] std::shared_ptr<T> GetComponentOrMakeIfNone() {
		for (auto comp : m_components) {
			if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
			{
				return castedComp;
			}
		}
		return AddComponentR<T>();

	}


	template<typename T> [[nodiscard]] std::shared_ptr<T> AddComponentR() {
		std::shared_ptr<T> comp = std::make_shared<T>();
		comp->parentEntity = shared_from_this();
		m_components.push_back(comp);
		return comp;
	}



	template<typename T> void AddComponent()  {
		std::shared_ptr<T> comp = std::make_shared<T>();
		comp->parentEntity = shared_from_this();
		m_components.push_back(comp);
	}


	template<typename T> void RemoveComponent()  {
		for (int i = 0; i < m_components.size(); i++) {
			if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( m_components[i] ) )
			{
				m_components.erase(m_components.begin() + i);
				i--;
			}
		}

	}


#ifdef SHOW_DELETED
	~Entity() 	{
		Log("Removed Entity " << name);
	}
#endif
};
