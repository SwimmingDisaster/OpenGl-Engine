#pragma once

namespace YAML { class Emitter; }
namespace YAML { class Node; }

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


constexpr float YAW         = -90.0f;
constexpr float PITCH       =  0.0f;
constexpr float SPEED       =  3.5f;
constexpr float SENSITIVITY =  0.3f;
constexpr float ZOOM        =  45.0f;


class EditorCamera
{
public:

	glm::vec3 vPos;
	glm::vec3 vRot;
	glm::vec3 vFront;

	glm::vec3 vUp;
	glm::vec3 vRight;
	glm::vec3 vWorldUp;

	float movementSpeed;
	float mouseSensitivity;

	bool isLocked = false;


public:
	EditorCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	EditorCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


	glm::mat4 GetViewMatrix();

	void ProcessKeyboard();
	void ProcessMouseMovement(GLboolean constrainPitch = true);

	void Update();
	void Serialize(YAML::Emitter& out);
	void Deserialize(const YAML::Node& data);
	void ShowOrHideMouse();

	void updateCameraVectors();
};