#include "mypch.h"

#include "ecs/component.h"
#include "ecs/other/componentFactory.h"

#include "assets/transform.h"


class Camera : public Component {
	REGISTER(Camera);
public:
	glm::vec3 vFront;

	glm::vec3 vUp;
	glm::vec3 vRight;
	glm::vec3 vWorldUp = {0.0f, 1.0f, 0.0f};

	float mouseSensitivity;

	bool isLocked = false;
	bool isPrimary = false;

public:
	std::shared_ptr<Transform> transform;

public:
	virtual void Start();
	virtual void Update();
	virtual void Show();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);


	Camera();
#ifdef SHOW_DELETED
	~Camera();
#endif

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(GLboolean constrainPitch = true);

	void updateCameraVectors();

};