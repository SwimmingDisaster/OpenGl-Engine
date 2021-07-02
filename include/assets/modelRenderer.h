#pragma once
#include "mypch.h"

#include "assets/model.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class Material;
class ModelRenderer : public Component
{
	REGISTER(ModelRenderer);

public:
	std::shared_ptr<Shader> m_shader;
	std::string shaderName = "res/shaders/color";
	std::shared_ptr<Model> m_modelComponent;
	std::shared_ptr<Material> m_materialComponent;

public:
	void Serialize(YAML::Emitter &out) override;
	void Deserialize(const YAML::Node &data) override;
	void Start() override;
	void Draw() override;
	void Show() override;

	ModelRenderer(const ModelRenderer &other) {}

	ModelRenderer();
#ifdef SHOW_DELETED
public:
	virtual ~ModelRenderer();
#else
public:
	virtual ~ModelRenderer() {}
#endif
public:
	void DrawModel(const std::shared_ptr<Shader> &shader, const std::shared_ptr<Model> model);
	void DrawMesh(const std::shared_ptr<Shader> &shader, const Mesh &mesh);
};
