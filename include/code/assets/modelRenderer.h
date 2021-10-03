#pragma once
#include "mypch.h"

#include "assets/model.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

#include "core/batchRenderer.h"

class Material;
class ModelRenderer : public Component{
	REGISTER(ModelRenderer);
	GETNAME();
public:
	std::string shaderName = "res/shaders/color";
	Shader* m_shader;
	std::shared_ptr<Transform> m_transformComponent;
	std::shared_ptr<Material> m_materialComponent;
	Batch* b = nullptr;

	std::shared_ptr<Model> m_modelComponent;

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

public:
	virtual ~ModelRenderer();

public:
	void DrawModel();
};
