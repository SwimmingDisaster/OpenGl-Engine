#include "mypch.h"

#include "assets/modelRenderer.h"
#include "assets/material.h"
#include "core/renderer.h"

#include "core/batchRenderer.h"

REGISTERIMPL(ModelRenderer);
ModelRenderer::ModelRenderer() {
    name = "ModelRenderer";
}
#ifdef SHOW_DELETED
ModelRenderer::~ModelRenderer() {
    Log("Deleted " << name);
}
#endif
void ModelRenderer::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Shader name" << YAML::Value << shaderName;

    out << YAML::EndMap;
}
void ModelRenderer::Deserialize(const YAML::Node& data) {
    shaderName = data["Shader name"].as<std::string>();
}
void ModelRenderer::Start() {
    m_shader = Shader::shaderMap[shaderName];
    m_modelComponent = parentEntity->GetComponent<Model>();
    m_materialComponent = parentEntity->GetComponent<Material>();
}
void ModelRenderer::Draw() {
    DrawModel(m_shader, m_modelComponent);
}
void ModelRenderer::Show() {
    //ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
    std::string copyString = shaderName;
    if(ImGui::InputText("Shader Name", &copyString, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue)) {
		shaderName = copyString;
    }
}

void ModelRenderer::DrawModel(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model> model)
{
    //shader->use();
    //shader->setMat4("matModel", model->transform->GetTransform());
    //shader->setVec3("color", m_materialComponent->color);

    BatchRenderer::AddObject(model->vertices, model->indices, m_materialComponent, model->transform, shaderName);
    //DrawMesh(shader, model->meshes[i]);
}
void ModelRenderer::DrawMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh)
{
    // bind appropriate textures
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr = 0;
    unsigned int heightNr = 0;
    for (unsigned int i = 0; i < mesh.textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = mesh.textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular") {
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        }
        else if (name == "texture_normal") {
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        }
        else if (name == "texture_height") {
            number = std::to_string(heightNr++); // transfer unsigned int to stream
        }

        // now set the sampler to the correct texture unit
        //glUniform1i(glGetUniformLocation(shader->ID, ("material." + name + number).c_str()), i);

        shader->setInt(("material." + name + number).c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }

    // draw mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, (const void *)0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

