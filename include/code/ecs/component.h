#pragma once
#include "mypch.h"


//#define SHOW_DELETED

class Entity;
class Component {
public:
    virtual void Start() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void End() {}
    virtual void Serialize(YAML::Emitter& out) const {}
    virtual void Deserialize(const YAML::Node& data) {}
    virtual void Show() {}
	virtual void OnCollision(const Entity* const other) {}

public:
    virtual ~Component() noexcept = default;


public:
    Entity* parentEntity;
    std::string name = "Unnamed Component";
};
