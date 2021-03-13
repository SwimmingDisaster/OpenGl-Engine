#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "shader.h"
#include "model.h"

struct Test {
	glm::vec4 pos_size;
	glm::vec4 color;
	glm::vec4 speed_life;
};


class ParticleSystem {
public:
	void Init();
	int FindUnusedParticle();
	void Update(float deltaTime);
	void Draw(Shader& shader);
	void ImGuiDraw();
	~ParticleSystem();




private:
	unsigned int VAO, VBO, SSBO;
	static const GLfloat g_vertex_buffer_data[12];
	GLuint billboard_vertex_buffer;

	int lastUsedParticle = 0;

public:
	static const int maxParticles = 100000;
	int newparticles = 0;
	Test* g_particule_data = new Test[maxParticles];

	glm::vec3 vPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vRot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec4 start_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 end_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float angle = 30.0f;
	float minLifetime = 4.75f, maxLifetime = 5.0f;
	bool cleared = false;
	float spawnrate = 2000.0f;
};