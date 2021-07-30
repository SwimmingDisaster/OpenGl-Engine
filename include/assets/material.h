#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"


class Material : public Component {
    REGISTER(Material);

public:
    std::vector<std::pair<std::string, std::any>>materialProperties;

public:
    void Show() override ;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
	void SetProperty(const std::string& propertyName, const std::any value); 

    Material();
#ifdef SHOW_DELETED
public:
    virtual ~Material();
#else
public:
    virtual ~Material() {};
#endif
};
