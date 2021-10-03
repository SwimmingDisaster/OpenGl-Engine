#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class COMPONENTNAME : public Component {
	REGISTER(COMPONENTNAME);
	GETNAME();
public:
	// variables go here

public:
    void Start() override;
    void Update() override;
    void Draw() override;
    void End() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;

#ifdef SHOW_DELETED
public:
	virtual ~COMPONENTNAME();
#else
//public:
	//virtual ~COMPONENTNAME() {};
#endif

};
