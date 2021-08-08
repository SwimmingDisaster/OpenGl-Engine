#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

#include "assets/material.h"

class DeleteAfterTouched : public Component {
	REGISTER(DeleteAfterTouched);
public:
	// variables go here
	bool isTouched = false;
	float time = 3.0f;
	std::shared_ptr<Material> material;

public:
    void Start() override;
    void Update() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;
	void OnCollision(const std::shared_ptr<Entity>& other) override;

	DeleteAfterTouched();
#ifdef SHOW_DELETED
public:
	virtual ~DeleteAfterTouched();
#else
//public:
	//virtual ~DeleteAfterTouched() {};
#endif

};
