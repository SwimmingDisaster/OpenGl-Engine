#pragma once
#include "mypch.h"

#include "assets/model.h"
#include "ecs/other/componentFactory.h"



class ModelRenderer : public Component {
	REGISTER(ModelRenderer);
public:
	std::shared_ptr<Shader> m_shader;
	std::string shaderName = "res/shaders/color";
	std::shared_ptr<Model> m_modelComponent;

	glm::vec3 m_color = {1.0f, 1.0f, 1.0f}; //todo this should be in a material component


public:
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);
	virtual void Start();
	virtual void Update();
	virtual void Show();

	ModelRenderer(const ModelRenderer& other) {}

	ModelRenderer();
#ifdef SHOW_DELETED
	~ModelRenderer();
#endif

	void DrawModel(std::shared_ptr<Shader>& shader, std::shared_ptr<Model> model);
	void DrawMesh(std::shared_ptr<Shader>& shader, Mesh& mesh);
};
