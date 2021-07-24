#pragma once
#include "mypch.h"

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
	std::string path;
};

class ModelImporter {
private:
	static const aiScene* GetAssimpScene(const std::string& filePath, Assimp::Importer& importer);
public:	
	static void LoadModelWithTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool isFlipped = false);
	static void LoadModelWithoutTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	static void ProcessNodeWithTextures(aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	static void ProcessNodeWithoutTextures(aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	static void ProcessMeshWithTextures(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	static void ProcessMeshWithoutTextures(aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	static void ProcessTextures(std::vector<Texture>& textures, aiMaterial* material);
	static void LoadTextures(std::vector<Texture>& textures, aiMaterial* mat, aiTextureType type, const std::string& typeName);

private:
	static std::string directory;
};
