/*#include "mesh.h"
#include "shader.h"


#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>



Mesh::Mesh(std::vector<float> c_vertices, glm::vec3 c_vPos, glm::vec3 c_vRot):
	vertices(c_vertices), vPos(c_vPos), vRot(c_vRot)
{}

void Mesh::Draw(glm::mat4& matView, glm::mat4& matProj, Shader& shader, unsigned int lineSize, std::vector<int> offsets) {

	glm::mat4 matVertex = glm::mat4(1.0f);
	glm::mat4 matModel = glm::mat4(1.0f);
	matModel = glm::translate(matModel, vPos);
	matModel = glm::rotate(matModel, glm::radians(vRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matModel = glm::rotate(matModel, glm::radians(vRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matModel = glm::rotate(matModel, glm::radians(vRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	matVertex = matProj * matView * matModel;

	shader.setMat4("matVertex", matVertex);


	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	for (int i = 0; i <= offsets.size() / 2; i += 2) {
		glVertexAttribPointer(i, offsets[i], GL_FLOAT, GL_FALSE, lineSize * sizeof(float), (void*)((offsets[i + 1])* sizeof(float)));
		glEnableVertexAttribArray(i / 2);
	}
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / lineSize);


	for (int i = 0; i < offsets.size() / 2; i++) {
		glDisableVertexAttribArray(i);
	}
}

void Mesh::DrawAll(glm::mat4& matView, glm::mat4& matProj, Shader& shader, unsigned int lineSize, std::vector<int> offsets) {

	glm::mat4 matVertex = glm::mat4(1.0f);
	glm::mat4 matModel = glm::mat4(1.0f);
	matModel = glm::translate(matModel, vPos);
	if (lineSize == 8) {
		matModel = glm::scale(matModel, glm::vec3(10.0f, 10.0f, 5.0f));
	}
	matModel = glm::rotate(matModel, glm::radians(vRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matModel = glm::rotate(matModel, glm::radians(vRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matModel = glm::rotate(matModel, glm::radians(vRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	if (lineSize == 8) {
		matVertex = matProj * matView * matModel;
	}
	else {
		matVertex = matProj * matView * matModel;
	}

	shader.setMat4("matModel", matModel);
	shader.setMat4("matProj", matProj);
	shader.setMat4("matView", matView);


	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	for (int i = 0; i < offsets.size() / 2; i++) {
		glVertexAttribPointer(i, offsets[i * 2], GL_FLOAT, GL_FALSE, lineSize * sizeof(float), (void*)((offsets[(i * 2) + 1])* sizeof(float)));
		glEnableVertexAttribArray(i);
	}
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / lineSize);


	for (int i = 0; i < offsets.size() / 2; i++) {
		glDisableVertexAttribArray(i);
	}
}*/