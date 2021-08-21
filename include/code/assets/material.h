#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

typedef std::variant<float, int, glm::vec3, glm::vec4, TextureInfo> MaterialPropertyType;

class Material : public Component {
    REGISTER(Material);

public:
    std::vector<std::pair<std::string, MaterialPropertyType>> materialProperties;

public:
    void Show() override ;
    void Serialize(YAML::Emitter& out) const override;
    void Deserialize(const YAML::Node& data) override;
	void SetProperty(const std::string& propertyName, const MaterialPropertyType& value); 

	void ShowProperties();
	void AddAndDeleteProperties();
	void SaveAndLoadFromFile();

    Material();
#ifdef SHOW_DELETED
public:
    virtual ~Material();
#else
//public:
    //virtual ~Material() {};
#endif
};
