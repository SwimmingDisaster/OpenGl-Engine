#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"
#include "assets/model.h"

class PlanetFaceGenerator : public Component {
	REGISTER(PlanetFaceGenerator);
	GETNAME();
public:
	// variables go here
	int resolution = 32;
	
private:
	std::shared_ptr<Model> model;

public:
    void Start() override;
    void Update() override;
    void Draw() override;
    void End() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;

	void GenerateFace();
};
