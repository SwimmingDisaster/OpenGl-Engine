#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  3.5f;
const float SENSITIVITY =  0.3f;
const float ZOOM        =  45.0f;


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

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(GLboolean constrainPitch = true);

	void updateCameraVectors();
};