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

	virtual const std::string& GetName() = 0;

public:
    virtual ~Component() noexcept = default;

public:
    Entity* parentEntity;
private:
    static std::string name;
};
