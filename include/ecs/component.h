#pragma once
#include "mypch.h"


//#define SHOW_DELETED


class Entity;
class Component {
public:
	virtual void Start() {}
	virtual void Update() {}
	virtual void End() {}
	virtual void Serialize(YAML::Emitter& out) {}
	virtual void Deserialize(const YAML::Node& data) {}
	virtual void Show() {}


public:
	std::shared_ptr<Entity> m_parentEntity;
	std::string m_name = "Unnamed Component";
};