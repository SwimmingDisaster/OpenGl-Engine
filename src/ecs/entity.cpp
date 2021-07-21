#include "ecs/entity.h"


void Entity::Start() {
	for (auto& comp : m_components) {
		comp->Start();
	}
}
void Entity::Update() {
	for (auto& comp : m_components) {
		comp->Update();
	}
}
void Entity::End() {
	for (auto& comp : m_components) {
		comp->End();
	}
}
void Entity::Serialize(YAML::Emitter& out) {
	for (auto& comp : m_components) {
		comp->Serialize(out);
	}

}
void Entity::Deserialize(YAML::Node& data) {
	for (auto& comp : m_components) {
		comp->Deserialize(data);
	}
}
void Entity::OnCollision(const std::shared_ptr<Entity>& other){
	for (auto& comp : m_components) {
		comp->OnCollision(other);
	}
}


void Entity::SetName(const std::string& othername){
	name = othername;
}
void Entity::SetUUID(const long long otheruuid){
	uuid = otheruuid;
	uuidString = std::to_string(uuid);
}
void Entity::SetTag(const int othertag){
	tag = othertag;
}
void Entity::SetLayer(const int otherlayer){
	layer = otherlayer;
}

const std::string& Entity::GetName(){
	return name;
}
const long long& Entity::GetUUID(){
	return uuid;
}
const std::string& Entity::GetUUIDString(){
	return uuidString;
}
const int& Entity::GetTag(){
	return tag;
}
const int& Entity::GetLayer(){
	return layer;
}
	
std::string& Entity::GetNameReference(){
	return name;
}
