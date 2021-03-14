#include "mypch.h"

#include "particles.h"
#include "game.h"
#include "shader.h"
#include "model.h"
#include "random.h"

#define PI 3.14159265

ParticleSystem::~ParticleSystem() {
	delete[] g_particule_data;
	glDeleteBuffers(1, &billboard_vertex_buffer);
}


void ParticleSystem::Init() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	static const GLfloat g_vertex_buffer_data[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	for (int i = 0; i < maxParticles; i++) {
		g_particule_data[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		g_particule_data[i].pos_size.w = 0.0f;
		g_particule_data[i].speed_life.w = ((Random::Float() * 10.0f) - 5.0f);
	}


	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, maxParticles * sizeof(Test), g_particule_data, GL_STREAM_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO);


}




int ParticleSystem::FindUnusedParticle() {
	for (int i = lastUsedParticle; i < maxParticles; i++) {
		if (g_particule_data[i].speed_life.w <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (g_particule_data[i].speed_life.w <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	return 0;
}



void ParticleSystem::Update(float deltaTime) {
	/*
		if (cleared) {
			cleared = false;
		} else {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
			glGetBufferSubData(	GL_SHADER_STORAGE_BUFFER, 0, maxParticles * sizeof(Test), g_particule_data);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO);
		}
	*/


	newparticles = (int)(deltaTime * spawnrate) * 5;
	if (newparticles > (int)(0.016f * spawnrate))
		newparticles = (int)(0.016f * spawnrate);

	/*	if (newparticles > (int)maxParticles * deltaTime / maxLifetime) {
			newparticles = maxParticles * deltaTime / maxLifetime;
		}
	*/


	for (int i = 0; i < newparticles; i++) {
		int afdsf = FindUnusedParticle();
	}


}

void ParticleSystem::Draw(Shader& shader) {




	GLuint CameraRight_worldspace_ID  = glGetUniformLocation(shader.ID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID  = glGetUniformLocation(shader.ID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(shader.ID, "VP");
	shader.use();


	shader.setFloat("maxLifetime", maxLifetime);
	shader.setFloat("minLifetime", minLifetime);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glUniform3f(CameraRight_worldspace_ID, Game::viewMatrix[0][0], Game::viewMatrix[1][0], Game::viewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID   , Game::viewMatrix[0][1], Game::viewMatrix[1][1], Game::viewMatrix[2][1]);
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &Game::viewProjectionMatrix[0][0]);



	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(0, 0);


	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxParticles);


	glDisableVertexAttribArray(0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::ImGuiDraw() {
	if (ImGui::CollapsingHeader("ParticleSystem")) {
		ImGui::DragFloat3("Position",  glm::value_ptr(vPos));
		ImGui::SliderFloat3("Angle",  glm::value_ptr(vRot), 0.0f, 360.0f);
		ImGui::SliderFloat3("Gravity",  glm::value_ptr(gravity), -10.0f, 10.0f);
		ImGui::ColorEdit4("Start color", glm::value_ptr(start_color));
		ImGui::ColorEdit4("End color", glm::value_ptr(end_color));
		ImGui::SliderFloat("Anbgle", &angle, 0.0f, 360.0f);
		ImGui::DragFloat("MinLifetime",  &minLifetime, 0.05f);
		ImGui::DragFloat("MaxLifetime",  &maxLifetime, 0.05f);
		ImGui::SliderFloat("Spawnrate",  &spawnrate, 60.0f, 100000.0f);
		if (ImGui::Button("Reset particles"))
		{

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 2);
			glBufferData(GL_SHADER_STORAGE_BUFFER, maxParticles * sizeof(Test), g_particule_data, GL_STREAM_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO);


		}
	}
}
