#include "mypch.h"
#include "core/batchRenderer.h"
#include "assets/mesh.h"
#include "assets/material.h"

std::vector<Vertex> BatchRenderer::vertices;
std::vector<unsigned int> BatchRenderer::indices;

std::vector<glm::mat4> BatchRenderer::matrixList(150);
std::vector<glm::vec3> BatchRenderer::colorList(150);

unsigned int BatchRenderer::VAO;
unsigned int BatchRenderer::VBO;
unsigned int BatchRenderer::EBO;
int BatchRenderer::index = 0;


/*
void RenderBatch::Draw(const std::shared_ptr<Shader>& shader){
	if (vertices.size() == 0) {
		return;
	}

	shader->use();
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "matModel"), 150, GL_FALSE, &matrixList[0][0][0]);
	glUniform3fv(glGetUniformLocation(shader->ID, "color"), 150, &colorList[0][0]);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (const void *)0);
	glBindVertexArray(0);
}
*/

void BatchRenderer::Setup() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, ObjectIndex));


	glBindVertexArray(0);
}

void BatchRenderer::Clear() {
	vertices.clear();
	indices.clear();
	index = 0;
}

void BatchRenderer::AddObject(Mesh& mesh, std::shared_ptr<Material>& material, std::shared_ptr<Transform>& transform, const std::string& shaderName)  {
	int numNewVertices = mesh.vertices.size();
	int numNewIndices = mesh.indices.size();

	int numVertices = vertices.size();
	int numIndices = indices.size();

	vertices.reserve(numVertices + numNewVertices);
	indices.reserve(numIndices + numNewIndices);

	vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
	indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());

	matrixList[index] = transform->GetTransform();
	colorList[index] = material->color;

	for (int i = numVertices; i < numVertices + numNewVertices; i++) {
		vertices[i].ObjectIndex = index;
	}

	for (int i = numIndices; i < numIndices + numNewIndices; i++) {
		indices[i] += numVertices;
	}
	index++;
}

void BatchRenderer::Draw(const std::shared_ptr<Shader>& shader) {

	if (vertices.size() == 0) {
		return;
	}

	shader->use();
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "matModel"), 150, GL_FALSE, &matrixList[0][0][0]);
	glUniform3fv(glGetUniformLocation(shader->ID, "color"), 150, &colorList[0][0]);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (const void *)0);
	glBindVertexArray(0);
}
