
#include "mypch.h"
#include "assets/camera.h"
#include "core/application.h"
#include "core/input.h"
#include "core/renderer.h"



REGISTERIMPL(Camera);
Camera::Camera() {
	m_name = "Camera";
}


#ifdef SHOW_DELETED
Camera::~Camera() {
	Log("Deleted " << m_name);
}
#endif


void Camera::Start() {
	transform = m_parentEntity->GetComponent<Transform>();
}
void Camera::Update() {
	updateCameraVectors();
	Renderer::viewMatrix = GetViewMatrix();
}
void Camera::Show() {
}

void Camera::Serialize(YAML::Emitter& out) {
	out << YAML::Key << m_name;
	out << YAML::BeginMap;
	out << YAML::EndMap;
}

void Camera::Deserialize(const YAML::Node& data) {

}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(transform->position, transform->position + vFront, vUp);
}


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(transform->rotation.y)) * cos(glm::radians(transform->rotation.x));
	front.y = sin(glm::radians(transform->rotation.x));
	front.z = sin(glm::radians(transform->rotation.y)) * cos(glm::radians(transform->rotation.x));

	vFront = glm::normalize(front);


	vRight = glm::normalize(glm::cross(vFront, vWorldUp));
	vUp    = glm::normalize(glm::cross(vRight, vFront));
	vUp    = glm::rotate(vUp, glm::radians(transform->rotation.z), {0.0f, 0.0f, 1.0f});
}
