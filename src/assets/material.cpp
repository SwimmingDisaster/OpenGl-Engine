#include "mypch.h"
#include "assets/material.h"


class ModelRenderer;
REGISTERIMPL(Material);

void Material::Show()  {
    ImGui::ColorEdit3("Color", glm::value_ptr(color));

    for(int i = 0; i < materialProperties.size(); i++) {
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);
        ImGui::InputText(std::to_string(i).c_str(), &materialProperties[i].first, ImGuiInputTextFlags_CallbackResize);
        ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
		if(materialProperties[i].first.find("olor") != std::string::npos){
            ImGui::ColorEdit3(("###" + materialProperties[i].first).c_str(), glm::value_ptr(*std::any_cast<glm::vec3>(&materialProperties[i].second)));
		}
		else if(materialProperties[i].second.type() == typeid(float)) {
            ImGui::DragFloat(("###" + materialProperties[i].first).c_str(), std::any_cast<float>(&materialProperties[i].second));
        }
		else if(materialProperties[i].second.type() == typeid(int)) {
            ImGui::DragInt(("###" + materialProperties[i].first).c_str(), std::any_cast<int>(&materialProperties[i].second));
        }
		else if(materialProperties[i].second.type() == typeid(glm::vec3)) {
            ImGui::DragFloat3(("###" + materialProperties[i].first).c_str(), glm::value_ptr(*std::any_cast<glm::vec3>(&materialProperties[i].second)));
        }
		else if(materialProperties[i].second.type() == typeid(glm::vec4)) {
            ImGui::DragFloat4(("###" + materialProperties[i].first).c_str(), glm::value_ptr(*std::any_cast<glm::vec4>(&materialProperties[i].second)));
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
            materialProperties.push_back(std::make_pair("Float", 0.0f));
        }
        if (ImGui::MenuItem("Add an int")) {
            materialProperties.push_back(std::make_pair("Int", 0));
        }
        if (ImGui::MenuItem("Add a vec3")) {
            materialProperties.push_back(std::make_pair("Vec3", glm::vec3(0.0f)));
        }
        if (ImGui::MenuItem("Add a vec4")) {
            materialProperties.push_back(std::make_pair("Vec4", glm::vec4(0.0f)));
        }
        ImGui::EndPopup();
    }

}

void Material::Serialize(YAML::Emitter& out) {
    out << YAML::Key << name;
    out << YAML::BeginMap;

    out << YAML::Key << "Color" << YAML::Value << color;

	for(int i = 0; i < materialProperties.size(); i++){
		if(materialProperties[i].second.type() == typeid(float)) {
			out << YAML::Key << ("f" + materialProperties[i].first) << YAML::Value << std::any_cast<float>(materialProperties[i].second);
        }
		else if(materialProperties[i].second.type() == typeid(int)) {
			out << YAML::Key << ("i" + materialProperties[i].first) << YAML::Value << std::any_cast<int>(materialProperties[i].second);
        }
		else if(materialProperties[i].second.type() == typeid(glm::vec3)) {
			out << YAML::Key << ("3" + materialProperties[i].first)<< YAML::Value << std::any_cast<glm::vec3>(materialProperties[i].second);
        }
		else if(materialProperties[i].second.type() == typeid(glm::vec4)) {
			out << YAML::Key << ("4" + materialProperties[i].first) << YAML::Value << std::any_cast<glm::vec4>(materialProperties[i].second);
        }
	}

    out << YAML::EndMap;
}

void Material::Deserialize(const YAML::Node& data) {
    color = data["Color"].as<glm::vec3>();
	for (YAML::const_iterator property=data.begin(); property != data.end();++property){
		std::string name = property->first.as<std::string>();
		char firstLetter = name[0];
		if(firstLetter == 'f'){
			materialProperties.push_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<float>()));
		}
		if(firstLetter == 'i'){
			materialProperties.push_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<int>()));
		}
		if(firstLetter == '3'){
			materialProperties.push_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<glm::vec3>()));
		}
		if(firstLetter == '4'){
			materialProperties.push_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<glm::vec4>()));
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
