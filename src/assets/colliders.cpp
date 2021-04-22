#include "mypch.h"
#include "assets/colliders.h"


//--------------BoxCollider--------------
REGISTERIMPL(BoxCollider);

void BoxCollider::Show() {
	ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.01f);
}
void BoxCollider::Serialize(YAML::Emitter& out) {
	out << YAML::Key << name;
	out << YAML::BeginMap;
	out << YAML::Key << "Scale" << YAML::Value << scale;
	out << YAML::EndMap;
}
void BoxCollider::Deserialize(const YAML::Node& data) {
	scale = data["Scale"].as<glm::vec3>();
}
PxGeometryHolder BoxCollider::GetGeometry() {
	return PxBoxGeometry(scale.x, scale.y, scale.z);
}
BoxCollider::BoxCollider() {
	name = "BoxCollider";
}
#ifdef SHOW_DELETED
BoxCollider::~BoxCollider() {
	Log("Deleted " << name);
}
#endif
//--------------BoxCollider--------------


//--------------SphereCollider--------------
REGISTERIMPL(SphereCollider);

void SphereCollider::Show() {
	ImGui::DragFloat("Radius", &radius, 0.01f);
}
void SphereCollider::Serialize(YAML::Emitter& out) {
	out << YAML::Key << name;
	out << YAML::BeginMap;
	out << YAML::Key << "Radius" << YAML::Value << radius;
	out << YAML::EndMap;
}
void SphereCollider::Deserialize(const YAML::Node& data) {
	radius = data["Radius"].as<float>();
}
PxGeometryHolder SphereCollider::GetGeometry() {
	return PxSphereGeometry(PxReal(radius));
}
SphereCollider::SphereCollider() {
	name = "SphereCollider";
}
#ifdef SHOW_DELETED
SphereCollider::~SphereCollider() {
	Log("Deleted " << name);
}
#endif
//--------------SphereCollider--------------