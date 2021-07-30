#include "mypch.h"
#include "assets/testComponent.h"
#include "core/application.h"

REGISTERIMPL(TestComponent);


void TestComponent::Show()  {
	ImGui::InputText("Name", &entity_name, ImGuiInputTextFlags_CallbackAlways);
	ImGui::DragFloat3("Offset", glm::value_ptr(offset));
}

void TestComponent::Serialize(YAML::Emitter& out) const {
	out << YAML::Key << name;
	out << YAML::BeginMap;

	out << YAML::Key << "Name" << YAML::Value << entity_name;
	out << YAML::Key << "Offset" << YAML::Value << offset;

	out << YAML::EndMap;
}

void TestComponent::Deserialize(const YAML::Node& data) {
	entity_name = data["Name"].as<std::string>();
	offset = data["Offset"].as<glm::vec3>();
}
void TestComponent::Start() {
	entity = Application::GetScene().GetEntity(entity_name);
	transform = parentEntity->GetComponent<Transform>();
	otherTransform = entity->GetComponent<Transform>();
}

void TestComponent::Update() {
	transform->position = otherTransform->position + offset;
}


#ifdef SHOW_DELETED
TestComponent::~TestComponent() {
	Log("Deleted " << name);
}
#endif
TestComponent::TestComponent() {
	name = "TestComponent";
}
