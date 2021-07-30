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
void Entity::Serialize(YAML::Emitter& out) const{
    for (auto& comp : m_components) {
        comp->Serialize(out);
    }

}
void Entity::Deserialize(YAML::Node& data) {
    for (auto& comp : m_components) {
        comp->Deserialize(data);
    }
}
void Entity::OnCollision(const std::shared_ptr<Entity>& other) {
    for (auto& comp : m_components) {
        comp->OnCollision(other);
    }
}


void Entity::SetName(const std::string& othername) {
    name = othername;
}
void Entity::SetUUID(const long long otheruuid) {
    uuid = otheruuid;
    uuidString = std::to_string(uuid);
}
void Entity::SetTag(const int othertag) {
    tag = othertag;
}
void Entity::SetLayer(const int otherlayer) {
    layer = otherlayer;
}

[[nodiscard]] const std::string& Entity::GetName() const{
    return name;
}
[[nodiscard]] const long long& Entity::GetUUID() const{
    return uuid;
}
[[nodiscard]] const std::string& Entity::GetUUIDString() const{
    return uuidString;
}
[[nodiscard]] const int& Entity::GetTag() const{
    return tag;
}
[[nodiscard]] const int& Entity::GetLayer() const{
    return layer;
}
[[nodiscard]] std::string& Entity::GetNameReference(){
    return name;
}

void Entity::Copy(const std::shared_ptr<Entity>& other) {
    name = other->name;
    uuid = other->uuid;

    for (auto& component : other->m_components) {
        Factory::copy(component.get()->name, shared_from_this(), component);
    }
    for (auto& component : m_components) {
        component->name = component.get()->name;
        component->parentEntity = shared_from_this();
    }
    Start();

}


#ifdef SHOW_DELETED
~Entity() 	{
    Log("Removed Entity " << name);
}
#endif
