#pragma once
#include "mypch.h"

#include "assets/model.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class Material;
class ModelRenderer : public Component{
	REGISTER(ModelRenderer);
	GETNAME();
public:
	std::string shaderName = "res/shaders/color";
	Shader* m_shader;
	Model* m_modelComponent;
	Transform* m_transformComponent;
	Material* m_materialComponent;

	bool isntBatched = false;
	int textureIndex;

	std::unordered_map<std::string, int> textureIndexMap;
	unsigned int VAO, VBO, EBO;

public:
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node &data) override;
	void Start() override;
	void Draw() override;
	void Show() override;

#ifdef SHOW_DELETED
public:
	virtual ~ModelRenderer();
#else
//public:
	//virtual ~ModelRenderer() {}
#endif
public:
	void DrawModel();
};
