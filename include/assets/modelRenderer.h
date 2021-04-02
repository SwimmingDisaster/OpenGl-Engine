#pragma once
#include "mypch.h"

#include "assets/model.h"
//#include "assets/material.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"


class Material;
class ModelRenderer : public Component {
	REGISTER(ModelRenderer);
public:
	std::shared_ptr<Shader> m_shader;
	std::string shaderName = "res/shaders/color";
	std::shared_ptr<Model> m_modelComponent;
	std::shared_ptr<Material> m_materialComponent;

	//glm::vec3 m_color = {1.0f, 1.0f, 1.0f}; //todo this should be in a material component


public:
	void Serialize(YAML::Emitter& out) override;
	void Deserialize(const YAML::Node& data) override;
	void Start() override;
	void Update() override;
	void Show() override;

	ModelRenderer(const ModelRenderer& other) {}

	ModelRenderer();
#ifdef SHOW_DELETED
public:
	virtual ~ModelRenderer();
#else
public:
	virtual ~ModelRenderer() {}
#endif

	void DrawModel(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model> model);
	void DrawMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh);
};
