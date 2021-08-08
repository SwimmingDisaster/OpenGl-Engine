/*
#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class Animator : public Component {
	REGISTER(Animator);
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

	Animator();
#ifdef SHOW_DELETED
public:
	virtual ~Animator();
#else
public:
	virtual ~Animator() {};
#endif

};
*/
