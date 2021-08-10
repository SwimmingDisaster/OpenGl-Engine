#include "mypch.h"
#include "assets/material.h"
#include "utils/fileUtils.h"
#include "core/texture.h"


class ModelRenderer;
REGISTERIMPL(Material);

void Material::Show()  {

    for(unsigned long i = 0; i < materialProperties.size(); i++) {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);
        ImGui::InputText(std::to_string(i).c_str(), &materialProperties[i].first, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::SameLine();
        ImGui::PopItemWidth();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
        const auto& materialType = materialProperties[i].second.type();
        if(materialProperties[i].first.find("olor") != std::string::npos) {
            ImGui::ColorEdit3(("###" + materialProperties[i].first).c_str(), glm::value_ptr(*std::any_cast<glm::vec3>(&materialProperties[i].second)));
        }
        else if(materialType == typeid(float)) {
            ImGui::DragFloat(("###" + materialProperties[i].first).c_str(), std::any_cast<float>(&materialProperties[i].second));
        }
        else if(materialType == typeid(int)) {
            ImGui::DragInt(("###" + materialProperties[i].first).c_str(), std::any_cast<int>(&materialProperties[i].second));
        }
        else if(materialType == typeid(glm::vec3)) {
            ImGui::DragFloat3(("###" + materialProperties[i].first).c_str(), glm::value_ptr(*std::any_cast<glm::vec3>(&materialProperties[i].second)));
        }
        else if(materialType == typeid(glm::vec4)) {
            ImGui::DragFloat4(("###" + materialProperties[i].first).c_str(), glm::value_ptr(*std::any_cast<glm::vec4>(&materialProperties[i].second)));
        }
        else if(materialType == typeid(TextureInfo)) {
            std::string& textureName = (*std::any_cast<TextureInfo>(&materialProperties[i].second)).name;
            ImGui::InputText(("###" + materialProperties[i].first).c_str(), &textureName, ImGuiInputTextFlags_CallbackResize);
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {
                textureName = OpenFile(nullptr, 0);
            }
        }
        ImGui::PopItemWidth();
    }

    if (ImGui::Button("Add"))
    {
        ImGui::OpenPopup("MaterialPopup");
    }
    if (ImGui::BeginPopup("MaterialPopup"))
    {
        if (ImGui::MenuItem("Add a float")) {
            materialProperties.emplace_back(std::make_pair("Float", 0.0f));
        }
        if (ImGui::MenuItem("Add an int")) {
            materialProperties.emplace_back(std::make_pair("Int", 0));
        }
        if (ImGui::MenuItem("Add a vec3")) {
            materialProperties.emplace_back(std::make_pair("Vec3", glm::vec3(0.0f)));
        }
        if (ImGui::MenuItem("Add a vec4")) {
            materialProperties.emplace_back(std::make_pair("Vec4", glm::vec4(0.0f)));
        }
        if (ImGui::MenuItem("Add a texture")) {
            materialProperties.emplace_back(std::make_pair("Texture", TextureInfo()));
        }
        ImGui::EndPopup();
    }

    static int indexToDelete;
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);
    ImGui::InputInt("Delete index", &indexToDelete);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        materialProperties.erase(materialProperties.begin() + indexToDelete);
    }

    if (ImGui::Button("Save to file")) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        Serialize(out);

        std::fstream fout;
        std::string filePath = SaveFile(nullptr, 0);//"other/materials/test.material";
        fout.open(filePath, std::fstream::out);
        fout << out.c_str();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load from file")) {
        std::string filePath = OpenFile(nullptr, 0);//"other/materials/test.material";
        std::ifstream stream(filePath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        const YAML::Node &materialNode= data["Material"];

        materialProperties.clear();
        Deserialize(materialNode);
    }

}

void Material::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    for(const auto& materialProperty : materialProperties) {
        const auto& materialType = materialProperty.second.type();
        if(materialType == typeid(float)) {
            out << YAML::Key << ("f" + materialProperty.first) << YAML::Value << std::any_cast<float>(materialProperty.second);
        }
        else if(materialType == typeid(int)) {
            out << YAML::Key << ("i" + materialProperty.first) << YAML::Value << std::any_cast<int>(materialProperty.second);
        }
        else if(materialType == typeid(glm::vec3)) {
            out << YAML::Key << ("3" + materialProperty.first)<< YAML::Value << std::any_cast<glm::vec3>(materialProperty.second);
        }
        else if(materialType == typeid(glm::vec4)) {
            out << YAML::Key << ("4" + materialProperty.first) << YAML::Value << std::any_cast<glm::vec4>(materialProperty.second);
        }
        else if(materialType == typeid(TextureInfo)) {
            out << YAML::Key << ("t" + materialProperty.first) << YAML::Value << std::any_cast<TextureInfo>(materialProperty.second);
        }
    }

    out << YAML::EndMap;
}

void Material::Deserialize(const YAML::Node& data) {
    for (YAML::const_iterator property=data.begin(); property != data.end(); ++property) {
        auto name = property->first.as<std::string>();
        const char& firstLetter = name[0];
        if(firstLetter == 'f') {
            materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<float>()));
        }
        if(firstLetter == 'i') {
            materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<int>()));
        }
        if(firstLetter == '3') {
            materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<glm::vec3>()));
        }
        if(firstLetter == '4') {
            materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<glm::vec4>()));
        }
        if(firstLetter == 't') {
            materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<TextureInfo>()));
        }
    }
}


void Material::SetProperty(const std::string& propertyName, const std::any& value) {
    for(auto& propertyPair : materialProperties) {
        if(propertyPair.first == propertyName) {
            propertyPair.second = value;
        }
    }
}

#ifdef SHOW_DELETED
Material::~Material() {
    Log("Deleted " << name);
}
#endif
Material::Material() {
    name = "Material";
}
