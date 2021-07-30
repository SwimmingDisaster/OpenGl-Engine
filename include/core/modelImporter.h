#pragma once
#include "mypch.h"

using namespace std::string_literals;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int ObjectIndex;

	Vertex(const glm::vec3& otherPos)
		:Position(otherPos) {}
	Vertex(){}
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class ModelImporter {
private:
	[[nodiscard]] static const aiScene* GetAssimpScene(const std::string& filePath, Assimp::Importer& importer, unsigned int flags);
public:	
	static void LoadModelWithTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool isFlipped = false);
	static void LoadModelWithoutTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	static void LoadModelBasic(const std::string& filePath, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);

	static void ProcessNodeWithTextures(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, const std::string& directory);
	static void ProcessNodeWithoutTextures(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	static void ProcessNodeBasic(const aiNode* node, const aiScene* scene, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);

	static void ProcessMeshWithTextures(const aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, const std::string& directory);
	static void ProcessMeshWithoutTextures(const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	static void ProcessMeshBasic(const aiMesh* mesh, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);

	static void ProcessTextures(std::vector<Texture>& textures, const aiMaterial* material, const std::string& directory);
	static void LoadTextures(std::vector<Texture>& textures, const aiMaterial* mat, const aiTextureType type, const std::string& typeName, const std::string& directory);
};


