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


void Entity::SetName(std::string& othername){
	name = othername;
}
void Entity::SetUUID(long long otheruuid){
	uuid = otheruuid;
	uuidString = std::to_string(uuid);
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
	
std::string& Entity::GetNameReference(){
	return name;
}
