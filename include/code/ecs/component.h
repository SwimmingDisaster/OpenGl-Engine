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
	virtual void OnCollision(const std::shared_ptr<Entity>& other) {}

public:
    virtual ~Component() noexcept = default;


public:
    std::shared_ptr<Entity> parentEntity;
    std::string name = "Unnamed Component";
};
