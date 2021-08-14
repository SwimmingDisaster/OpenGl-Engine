#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

#include "assets/lightsBase.h"
#include "assets/transform.h"

struct DirectionalLight
{
	glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
	glm::vec3 direction;
};

struct PointLight
{
	glm::vec3 color;
    float ambientIntensity;
	glm::vec3 position;
    float diffuseIntensity;
    float constant;
    float linear;
    float exp;
};

struct SpotLight
{
	glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
	glm::vec3 direction;
    float cutoff;
};

class PointLightComponent : public LightBase {
	REGISTER(PointLightComponent);
public:
	// variables go here
	PointLight pointLight;
	std::shared_ptr<Transform> transform;

public:
    void Start() override;
    void Update() override;
    void Draw() override;
    void End() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;

	PointLightComponent();
#ifdef SHOW_DELETED
public:
	virtual ~PointLightComponent();
#else
public:
	virtual ~PointLightComponent() {};
#endif

};

class DirectionalLightComponent : public LightBase {
	REGISTER(DirectionalLightComponent);
public:
	// variables go here
	DirectionalLight directionalLight;

public:
    void Start() override;
    void Update() override;
    void Draw() override;
    void End() override;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
    void Show() override;

	DirectionalLightComponent();
#ifdef SHOW_DELETED
public:
	virtual ~DirectionalLightComponent();
#else
public:
	virtual ~DirectionalLightComponent() {};
#endif

};
