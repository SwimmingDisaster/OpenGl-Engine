#pragma once
#include "mypch.h"
#include "ecs/component.h"
#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"

class Transform : public Component {
	REGISTER(Transform);
	GETNAME();
private:
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};
	bool isCacheValid = false;
	glm::mat4 cachedTransform;


public:
	void Show() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;

	[[nodiscard]] glm::mat4 GetTransform();
	[[nodiscard]] glm::mat4 GetTransformWithNoScale();
	[[nodiscard]] glm::mat3 GetTransformForNormals();

	[[nodiscard]] const glm::vec3& GetPosition() const;  
	[[nodiscard]] const glm::vec3& GetRotation() const;  
	[[nodiscard]] const glm::vec3& GetScale() const;  

	void SetPosition(glm::vec3 other);  
	void SetRotation(glm::vec3 other);  
	void SetScale(glm::vec3 other);  

	void SetRotationX(float other);  
	void SetRotationY(float other);  
	void SetRotationZ(float other);  
};


