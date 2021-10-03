#include "mypch.h"

#include "assets/modelRenderer.h"
#include "assets/material.h"
#include "core/renderer.h"


REGISTERIMPL(ModelRenderer);
GETNAMEIMPL(ModelRenderer);

void ModelRenderer::Start() {
    m_shader = Shader::shaderMap[shaderName].get();
    m_modelComponent = parentEntity->GetComponent<Model>();
    m_materialComponent = parentEntity->GetComponent<Material>();
    m_transformComponent = parentEntity->GetComponent<Transform>();
}
void ModelRenderer::Show() {
    //ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
    std::string copyString = shaderName;
    if(ImGui::InputText("Shader Name", &copyString, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue)) {
        shaderName = copyString;
    }
	ImGui::Checkbox("Is not batched", &isntBatched);
}
void ModelRenderer::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Shader name" << YAML::Value << shaderName;
    out << YAML::Key << "Isnt batched" << YAML::Value << isntBatched;

    out << YAML::EndMap;
}
void ModelRenderer::Deserialize(const YAML::Node& data) {
    shaderName = data["Shader name"].as<std::string>();
    isntBatched = data["Isnt batched"].as<bool>();
}
void ModelRenderer::Draw() {
    DrawModel();
}
void ModelRenderer::DrawModel(){
	if(isntBatched){
		if(b == nullptr){
			b = new Batch();
			b->Setup();
		}

		Shader::shaderMap[shaderName]->use();
		b->Clear();
	//	b->AddObject(m_modelComponent->vertices, m_modelComponent->indices, m_materialComponent, m_transformComponent);
		b->AddProperties(m_materialComponent.get(), m_transformComponent.get());
		b->DrawThisGeometry(Shader::shaderMap[shaderName].get(), m_modelComponent->vertices, m_modelComponent->indices);

	}
	else{
		BatchRenderer::AddObject(m_modelComponent->vertices, m_modelComponent->indices, m_materialComponent.get(), m_transformComponent.get(), shaderName);
	}
}

ModelRenderer::~ModelRenderer() {
	if(b != nullptr){
		b->Destroy();
		delete b;
	}
}

