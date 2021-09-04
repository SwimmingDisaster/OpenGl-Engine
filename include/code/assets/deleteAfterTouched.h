#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentDefines.h"

class Entity;
class Material;
namespace YAML { class Emitter; }
namespace YAML { class Node; }

class DeleteAfterTouched : public Component {
	REGISTER(DeleteAfterTouched);
	GETNAME();
public:
	// variables go here
	bool isTouched = false;
	float time = 3.0f;
	Material* material;

public:
    void Start() override;
    void Update() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;
	void OnCollision(const Entity* const other) override;

#ifdef SHOW_DELETED
public:
	virtual ~DeleteAfterTouched();
#else
//public:
	//virtual ~DeleteAfterTouched() {};
#endif

};
