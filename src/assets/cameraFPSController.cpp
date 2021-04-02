#include "mypch.h"
#include "assets/cameraFPSController.h"

#include "core/input.h"
#include "core/application.h"

REGISTERIMPL(CameraFPSController);


void CameraFPSController::Start() {
	transform = m_parentEntity->GetComponent<Transform>();
	camera = m_parentEntity->GetComponent<Camera>();
}
void CameraFPSController::Update() {
	if (Input::IsKeyPressed(INPUT_KEY_ESCAPE)) {
		if (isLocked) {
			isLocked = false;
			glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			isLocked = true;
			glfwSetInputMode(Application::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}


	ProcessKeyboard(Application::deltaTime);
	ProcessMouseMovement(true);
}
void CameraFPSController::Show() {
	ImGui::DragFloat("Mouse Sensitivity", &mouseSensitivity, 0.1f);
	ImGui::DragFloat("Movement Speed", &movementSpeed, 0.001f);
	ImGui::Checkbox("Is Locked", &isLocked);
}
void CameraFPSController::Serialize(YAML::Emitter& out) {
	out << YAML::Key << m_name;
	out << YAML::BeginMap;

	out << YAML::Key << "Mouse Sensitivity" << YAML::Value << mouseSensitivity;
	out << YAML::Key << "Movement Speed" << YAML::Value << movementSpeed;

	out << YAML::EndMap;
}
void CameraFPSController::Deserialize(const YAML::Node& data) {
	mouseSensitivity = data["Mouse Sensitivity"].as<float>();
	movementSpeed = data["Movement Speed"].as<float>();
}


CameraFPSController::CameraFPSController() {
	m_name = "CameraFPSController";
}
#ifdef SHOW_DELETED
CameraFPSController::~CameraFPSController() {
	Log("Deleted " << m_name);
}
#endif



void CameraFPSController::ProcessKeyboard(float deltaTime)
{
	float velocity = movementSpeed * deltaTime;

	if (glfwGetKey(Application::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		velocity *= 3.0f;

	if (Input::IsKeyHeld(INPUT_KEY_W)) {
		transform->position += camera->vFront * velocity;
	}
	if (Input::IsKeyHeld(INPUT_KEY_S)) {
		transform->position -= camera->vFront * velocity;
	}
	if (Input::IsKeyHeld(INPUT_KEY_A)) {
		transform->position -= camera->vRight * velocity;
	}
	if (Input::IsKeyHeld(INPUT_KEY_D)) {
		transform->position += camera->vRight * velocity;
	}
}

void CameraFPSController::ProcessMouseMovement(bool constrainPitch)
{

	if (!isLocked) {

		transform->rotation.y   += Input::Get().xoffset * mouseSensitivity;
		transform->rotation.x   += Input::Get().yoffset * mouseSensitivity;

		transform->rotation.y = fmod(transform->rotation.y, 360.0f);

		if (constrainPitch)
		{
			if (transform->rotation.x > 89.0f)
				transform->rotation.x = 89.0f;
			if (transform->rotation.x < -89.0f)
				transform->rotation.x = -89.0f;
		}

		camera->updateCameraVectors();
	}
}

