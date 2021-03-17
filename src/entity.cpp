#include "entity.h"


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
void Entity::Serialize() {
	for (auto& comp : m_components) {
		comp->Serialize();
	}
}
void Entity::Deserialize() {
	for (auto& comp : m_components) {
		comp->Deserialize();
	}
}
