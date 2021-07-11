#pragma once
#include "mypch.h"
#include "core/texture.h"
#include "ecs/component.h"
#include "ecs/entity.h"

#include "core/shader.h"
#include "assets/transform.h"
#include "ecs/other/componentFactory.h"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int ObjectIndex;
};

struct Texture {
	unsigned int id;
	std::string type;
	std:: string path;
};



class Mesh {
public:
	std::vector<Texture> textures_loaded;
	// mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	unsigned int VAO, VBO, EBO;


	std::string directory;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh() {}

	void setupMesh();
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};


