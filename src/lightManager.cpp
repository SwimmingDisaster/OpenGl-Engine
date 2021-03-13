#include "lightManager.h"
#include "lights.h"
#include "game.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"



LightManager::LightManager() {

}

void LightManager::Init() {
	glGenBuffers(1, &DirLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, DirLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(DirLight), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, DirLightUBO);

	glGenBuffers(1, &PointLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, PointLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, 10 * sizeof(PointLight), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, PointLightUBO);

	glGenBuffers(1, &SpotLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, SpotLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(SpotLight), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 5, SpotLightUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightManager::Update(Shader& shader) {
	shader.setFloat("dirLightNumber", (float)Game::dirLightVector.size());
	glBindBuffer(GL_UNIFORM_BUFFER, DirLightUBO);
	glBufferData(GL_UNIFORM_BUFFER,  sizeof(Game::dirLightVector.size()), &Game::dirLightVector.front(), GL_STATIC_DRAW);


	shader.setFloat("pointLightNumber", (float)Game::pointLightVector.size());
	glBindBuffer(GL_UNIFORM_BUFFER, PointLightUBO);
	glBufferData(GL_UNIFORM_BUFFER,  Game::pointLightVector.size() * sizeof(PointLight), &Game::pointLightVector.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	/*	glBindBuffer(GL_UNIFORM_BUFFER, SpotLightUBO);
		glBufferData(GL_UNIFORM_BUFFER,  sizeof(Game::spotLightVector.size()), &Game::spotLightVector.front(), GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}
LightManager::~LightManager() {

}