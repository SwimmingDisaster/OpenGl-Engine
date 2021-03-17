#pragma once
#include "component.h"
#include <string>
#include <vector>
#include <memory>

class Entity { /*: public std::enable_shared_from_this<Entity>*/
public:
	std::vector< std::shared_ptr<Component> > m_components;
	std::string m_name;
	long long m_uuid;

public:
	virtual void Start();
	virtual void Update();
	virtual void End();
	virtual void Serialize();
	virtual void Deserialize();

	template<typename T> std::shared_ptr<T> GetComponent() {
		for (auto comp : m_components) {
			if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( comp ) )
			{
				return castedComp;
			}
		}
		return nullptr;
	}

	template<typename T> void AddComponent() {
		std::shared_ptr<T> comp = std::make_shared<T>();
		comp->m_parentEntity = std::make_shared<Entity>(*this); //shared_from_this();
		m_components.push_back(comp);
	}


	template<typename T> void RemoveComponent() {
		for (int i = 0; i < m_components.size(); i++) {
			if ( std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>( m_components[i] ) )
			{
				m_components.erase(m_components.begin() + i);
			}
		}

	}



};