#include "mypch.h"
#include "assets/lights.h"
#include "core/batchRenderer.h"
#include "core/input.h"


REGISTERIMPL(PointLightComponent);
GETNAMEIMPL(PointLightComponent);

void PointLightComponent::Show()  {
	//	ImGui::InputText("Shader Name", &shaderName, ImGuiInputTextFlags_CallbackResize);
	ImGui::ColorEdit3("Color", glm::value_ptr(pointLight.color));
	ImGui::DragFloat("Ambient intensity", &pointLight.ambientIntensity, 0.01f);
	ImGui::DragFloat("Diffuse intensity", &pointLight.diffuseIntensity, 0.01f);

	ImGui::DragFloat("Constant", &pointLight.constant, 0.01f);
	ImGui::DragFloat("Linear", &pointLight.linear, 0.01f);
	ImGui::DragFloat("Exp", &pointLight.exp, 0.001f);
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
void PointLightComponent::Start() {
	transform = parentEntity->GetComponent<Transform>();
}
void PointLightComponent::Update() {
	//pointLight.position = transform->GetPosition();
}
void PointLightComponent::Draw() {
	RecalculateSize();
	pointLight.position = transform->GetPosition();
	//LightsBatchRenderer::AddObject(LightsBatchRenderer::quadVertices,	LightsBatchRenderer::quadIndices, pointLight, transform, "res/shaders/pointLight");
	LightsBatchRenderer::AddObject(LightsBatchRenderer::sphereVertices,	LightsBatchRenderer::sphereIndices, pointLight, transform, "res/shaders/pointLight");
}
void PointLightComponent::End() {

}
void PointLightComponent::RecalculateSize() {
	float pointLightMax  = std::fmaxf(std::fmaxf(pointLight.color.r, pointLight.color.g), pointLight.color.b);
	float radius    = (-pointLight.linear +  sqrtf(pointLight.linear * pointLight.linear - 4 * pointLight.exp * (pointLight.constant - (256.0 / 5.0) * pointLightMax))) / (2 * pointLight.exp);
	transform->SetScale({radius, radius, radius});
}

#ifdef SHOW_DELETED
PointLightComponent::~PointLightComponent() {
	Log("Deleted " << name);
}
#endif



REGISTERIMPL(DirectionalLightComponent);
GETNAMEIMPL(DirectionalLightComponent);

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
void DirectionalLightComponent::Start() {

}
void DirectionalLightComponent::Update() {

}
void DirectionalLightComponent::Draw() {
}
void DirectionalLightComponent::End() {

}

#ifdef SHOW_DELETED
DirectionalLightComponent::~DirectionalLightComponent() {
	Log("Deleted " << name);
}
#endif
