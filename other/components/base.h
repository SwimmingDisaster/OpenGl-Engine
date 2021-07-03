#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class COMPONENTNAME : public Component {
	REGISTER(COMPONENTNAME);
public:
	// variables go here

public:
	void Show() override;
	void Serialize(YAML::Emitter& out) override;
	void Deserialize(const YAML::Node& data) override;

	
    void Start() override;
    void Update() override;
    void Draw() override;
    void End() override;
    void Serialize(YAML::Emitter& out) override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;

	COMPONENTNAME();
#ifdef SHOW_DELETED
public:
	virtual ~COMPONENTNAME();
#else
public:
	virtual ~COMPONENTNAME() {};
#endif

};
