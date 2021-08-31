#include "mypch.h"
#include "assets/material.h"
#include "utils/fileUtils.h"
#include "core/texture.h"


class ModelRenderer;
REGISTERIMPL(Material);
GETNAMEIMPL(Material);

void Material::ShowProperties(){
	std::size_t i = 0;
	for(auto& [propertyName, property] : materialProperties) {
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);
		ImGui::InputText(std::to_string(i).c_str(), &propertyName, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_EnterReturnsTrue);
		i++;
		ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.75f);
		const auto& materialType = property.index();
		if(propertyName.find("olor") != std::string::npos) {
			ImGui::ColorEdit3(("###" + propertyName).c_str(), glm::value_ptr(std::get<glm::vec3>(property)));
			ImGui::PopItemWidth();
			continue;
		}
		switch(materialType) {
		case 0:
			ImGui::DragFloat(("###" + propertyName).c_str(), &std::get<float>(property));
			break;
		case 1:
			ImGui::DragInt(("###" + propertyName).c_str(), &std::get<int>(property));
			break;
		case 2:
			ImGui::DragFloat3(("###" + propertyName).c_str(), glm::value_ptr(std::get<glm::vec3>(property)));
			break;
		case 3:
			ImGui::DragFloat4(("###" + propertyName).c_str(), glm::value_ptr(std::get<glm::vec4>(property)));
			break;
		case 4:
			std::string& textureName = std::get<TextureInfo>(property).name;
			ImGui::InputText(("###" + propertyName).c_str(), &textureName, ImGuiInputTextFlags_CallbackResize);
			ImGui::SameLine();
			if (ImGui::Button("Browse")) {
				textureName = OpenFile(nullptr, 0);
			}
			break;
		}
		ImGui::PopItemWidth();
	}
}
void Material::AddAndDeleteProperties(){
	if (ImGui::Button("Add")){
		ImGui::OpenPopup("MaterialPopup");
	}
	if (ImGui::BeginPopup("MaterialPopup")){
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
}
void Material::SaveAndLoadFromFile(){
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

void Material::Show()  {
	ShowProperties();
	AddAndDeleteProperties();
	SaveAndLoadFromFile();
}
void Material::Serialize(YAML::Emitter& out) const {
	out << YAML::Key << name;
	out << YAML::BeginMap;

	for(const auto& [propertyName, property] : materialProperties) {
		const auto& materialType = property.index();
		switch(materialType) {
		case 0:
			out << YAML::Key << ("f" + propertyName) << YAML::Value << std::get<float>(property);
			break;
		case 1:
			out << YAML::Key << ("i" + propertyName) << YAML::Value << std::get<int>(property);
			break;
		case 2:
			out << YAML::Key << ("3" + propertyName)<< YAML::Value << std::get<glm::vec3>(property);
			break;
		case 3:
			out << YAML::Key << ("4" + propertyName) << YAML::Value << std::get<glm::vec4>(property);
			break;
		case 4:
			out << YAML::Key << ("t" + propertyName) << YAML::Value << std::get<TextureInfo>(property);
			break;
		}
	}

	out << YAML::EndMap;
}
void Material::Deserialize(const YAML::Node& data) {
	for (YAML::const_iterator property=data.begin(); property != data.end(); ++property) {
		auto name = property->first.as<std::string>();
		const char& firstLetter = name[0];
		switch(firstLetter) {
		case 'f':
			materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<float>()));
			break;
		case 'i':
			materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<int>()));
			break;
		case '3':
			materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<glm::vec3>()));
			break;
		case '4':
			materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<glm::vec4>()));
			break;
		case 't':
			materialProperties.emplace_back(std::make_pair(name.substr(1, name.size() - 1), property->second.as<TextureInfo>()));
			break;
		}
	}
}
void Material::SetProperty(const std::string& propertyName, const MaterialPropertyType& value) {
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
