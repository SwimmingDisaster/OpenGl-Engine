#include "mypch.h"
#include "transform.h"
#include "entity.h"


void Transform::Start()  {
	position = {1.0f, 10.0f, 1.0f};
}
void Transform::Show()  {
	ImGui::DragFloat3("position", glm::value_ptr(position));
}
/*void Transform::Update()  {
}
void Transform::End()  {
}
void Transform::Serialize()  {
}
void Transform::Deserialize()  {
}*/
Transform::~Transform() {
	Log("Deleted Transform");
}
Transform::Transform() {
	m_name = "Transform";
}

