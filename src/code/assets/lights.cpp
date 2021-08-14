#include "assets/lights.h"
#include "core/batchRenderer.h"


REGISTERIMPL(PointLightComponent);

void PointLightComponent::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
	ImGui::ColorEdit3("Color", glm::value_ptr(pointLight.color));
	ImGui::DragFloat("Ambient intensity", &pointLight.ambientIntensity);
	ImGui::DragFloat("Diffuse intensity", &pointLight.diffuseIntensity);

	ImGui::DragFloat("Constant", &pointLight.constant);
	ImGui::DragFloat("Linear", &pointLight.linear);
	ImGui::DragFloat("Exp", &pointLight.exp);
}
void PointLightComponent::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

	//	out << YAML::Key << "Position" << YAML::Value << position;
	out << YAML::Key << "Color" << YAML::Value << pointLight.color;

	out << YAML::Key << "Ambient intensity" << YAML::Value << pointLight.ambientIntensity;
	out << YAML::Key << "Diffuse intensity" << YAML::Value << pointLight.diffuseIntensity;

	out << YAML::Key << "Constant" << YAML::Value << pointLight.constant;
	out << YAML::Key << "Linear" << YAML::Value << pointLight.linear;
	out << YAML::Key << "Exp" << YAML::Value << pointLight.exp;

    out << YAML::EndMap;
}
void PointLightComponent::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
	pointLight.color = data["Color"].as<glm::vec3>();

	pointLight.ambientIntensity = data["Ambient intensity"].as<float>();
	pointLight.diffuseIntensity = data["Diffuse intensity"].as<float>();

	pointLight.constant = data["Constant"].as<float>();
	pointLight.linear = data["Linear"].as<float>();
	pointLight.exp = data["Exp"].as<float>();
}
void PointLightComponent::Start(){
	transform = parentEntity->GetComponent<Transform>();	
}
void PointLightComponent::Update(){
	pointLight.position = transform->position;
}
void PointLightComponent::Draw(){
	LightsBatchRenderer::AddObject(LightsBatchRenderer::sphereVertices,	LightsBatchRenderer::sphereIndices, pointLight, transform, "res/shaders/pointLight");
}
void PointLightComponent::End(){

}

#ifdef SHOW_DELETED
PointLightComponent::~PointLightComponent() {
    Log("Deleted " << name);
}
#endif
PointLightComponent::PointLightComponent() {
    name = "PointLightComponent";
}



REGISTERIMPL(DirectionalLightComponent);

void DirectionalLightComponent::Show()  {
    //	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
	ImGui::ColorEdit3("Color", glm::value_ptr(directionalLight.color));
	ImGui::DragFloat3("Direction", glm::value_ptr(directionalLight.direction));
	ImGui::DragFloat("Ambient intensity", &directionalLight.ambientIntensity);
	ImGui::DragFloat("Diffuse intensity", &directionalLight.diffuseIntensity);

}
void DirectionalLightComponent::Serialize(YAML::Emitter& out) const {
    out << YAML::Key << name;
    out << YAML::BeginMap;

	//	out << YAML::Key << "Position" << YAML::Value << position;
	out << YAML::Key << "Color" << YAML::Value << directionalLight.color;
	out << YAML::Key << "Direction" << YAML::Value << directionalLight.direction;


	out << YAML::Key << "Ambient intensity" << YAML::Value << directionalLight.ambientIntensity;
	out << YAML::Key << "Diffuse intensity" << YAML::Value << directionalLight.diffuseIntensity;

    out << YAML::EndMap;
}
void DirectionalLightComponent::Deserialize(const YAML::Node& data) {
    //	position = data["Position"].as<glm::vec3>();
	directionalLight.color = data["Color"].as<glm::vec3>();
	directionalLight.direction = data["Direction"].as<glm::vec3>();

	directionalLight.ambientIntensity = data["Ambient intensity"].as<float>();
	directionalLight.diffuseIntensity = data["Diffuse intensity"].as<float>();

}
void DirectionalLightComponent::Start(){

}
void DirectionalLightComponent::Update(){

}
void DirectionalLightComponent::Draw(){
}
void DirectionalLightComponent::End(){

}

#ifdef SHOW_DELETED
DirectionalLightComponent::~DirectionalLightComponent() {
    Log("Deleted " << name);
}
#endif
DirectionalLightComponent::DirectionalLightComponent() {
    name = "DirectionalLightComponent";
}
