#include "core/modelImporter.h"

std::string ModelImporter::directory;

const aiScene* ModelImporter::GetAssimpScene(const std::string& filePath, Assimp::Importer& importer){
	static const unsigned int flags = aiProcess_Triangulate
	   	| aiProcess_GenSmoothNormals
	   	| aiProcess_FlipUVs
	   	| aiProcess_CalcTangentSpace
	   	| aiProcess_ImproveCacheLocality 
		| aiProcess_OptimizeMeshes;
	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Error("Assimp:  " << importer.GetErrorString());
		return nullptr;
	}

	return scene;
}

void ModelImporter::LoadModelWithTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool isFlipped){
	stbi_set_flip_vertically_on_load(isFlipped);
	Assimp::Importer importer;
	const aiScene* scene = GetAssimpScene(filePath, importer);
	directory = filePath.substr(0, filePath.find_last_of('/'));
	ProcessNodeWithTextures(scene->mRootNode, scene, vertices, indices, textures);
}
void ModelImporter::LoadModelWithoutTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices){
	Assimp::Importer importer;
	const aiScene* scene = GetAssimpScene(filePath, importer);
	ProcessNodeWithoutTextures(scene->mRootNode, scene, vertices, indices);
}

void ModelImporter::ProcessNodeWithTextures(aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures){
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMeshWithTextures(mesh, scene, vertices, indices, textures);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNodeWithTextures(node->mChildren[i], scene, vertices, indices, textures);
	}
}
void ModelImporter::ProcessNodeWithoutTextures(aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices){
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMeshWithoutTextures(mesh, vertices, indices);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNodeWithoutTextures(node->mChildren[i], scene, vertices, indices);
	}
}

void ModelImporter::ProcessMeshWithTextures(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures){
	ProcessMeshWithoutTextures(mesh, vertices, indices);
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	ProcessTextures(textures, material);
}
void ModelImporter::ProcessMeshWithoutTextures(aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices){
	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		if (face.mNumIndices < 3) {
			continue;
		}
		assert(face.mNumIndices == 3);
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

void ModelImporter::ProcessTextures(std::vector<Texture>& textures, aiMaterial* material){
	LoadTextures(textures, material, aiTextureType_DIFFUSE, "texture_diffuse");
	LoadTextures(textures, material, aiTextureType_SPECULAR, "texture_specular");
	LoadTextures(textures, material, aiTextureType_HEIGHT, "texture_normal");
	LoadTextures(textures, material, aiTextureType_AMBIENT, "texture_height");
}
void ModelImporter::LoadTextures(std::vector<Texture>& textures, aiMaterial* mat, aiTextureType type, const std::string& typeName){
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures.size(); j++)
		{
			if (std::strcmp(textures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{	// if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures.push_back(texture);
		}
	}
}

