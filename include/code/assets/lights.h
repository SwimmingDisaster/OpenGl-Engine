#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

#include "assets/lightsBase.h"

class PointLight : LightBase {
	REGISTER(PointLight);
public:
	// variables go here

public:
    void Start() override;
    void Update() override;
    void Draw() override;
    void End() override;
    void Serialize(YAML::Emitter& out) override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;

	PointLight();
#ifdef SHOW_DELETED
public:
	virtual ~PointLight();
#else
public:
	virtual ~PointLight() {};
#endif

};
