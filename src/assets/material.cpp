#include "mypch.h"
#include "assets/material.h"


class ModelRenderer;
REGISTERIMPL(Material);

void Material::Show()  {
	ImGui::ColorEdit3("Color", glm::value_ptr(color));
}

void Material::Serialize(YAML::Emitter& out) {
	out << YAML::Key << name;
	out << YAML::BeginMap;

	out << YAML::Key << "Color" << YAML::Value << color;

	out << YAML::EndMap;
}

void Material::Deserialize(const YAML::Node& data) {
	color = data["Color"].as<glm::vec3>();
}



#ifdef SHOW_DELETED
Material::~Material() {
	Log("Deleted " << m_name);
}
#endif
Material::Material() {
	name = "Material";
}