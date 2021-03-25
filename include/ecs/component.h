#pragma once
#include "mypch.h"

class Entity;
class Component {
public:
	virtual void Start() {}
	virtual void Update() {}
	virtual void End() {}
	virtual void Serialize(YAML::Emitter& out) {}
	virtual void Deserialize() {}
	virtual void Show() {}

public:
	std::shared_ptr<Entity> m_parentEntity;
	std::string m_name;
};