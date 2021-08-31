#include "mypch.h"
#include "core/modelImporter.h"

static const unsigned int assimpFlags = aiProcess_CalcTangentSpace
                                        | aiProcess_JoinIdenticalVertices
                                        | aiProcess_Triangulate
                                        | aiProcess_GenNormals
                                        | aiProcess_FlipUVs
                                        | aiProcess_ImproveCacheLocality
                                        | aiProcess_OptimizeMeshes;

static const unsigned int physxFlags = aiProcess_Triangulate
                                       | aiProcess_JoinIdenticalVertices;


auto ModelImporter::GetAssimpScene(const std::string& filePath, Assimp::Importer& importer, unsigned int flags) -> const aiScene* {
    const aiScene* scene = importer.ReadFile(filePath, flags);

    if ((scene == nullptr) || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || (scene->mRootNode == nullptr))
    {
        Error("Assimp:  " << importer.GetErrorString());
        return nullptr;
    }

    return scene;
}

void ModelImporter::LoadModelWithTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool isFlipped, bool loadBinaryVersion, bool makeBinaryVersion) {
	if(loadBinaryVersion){
		if(LoadBinaryModelWithTextures(filePath + ".wt", vertices, indices, textures)){
			return;
		}
	}
    vertices.clear();
    indices.clear();
    textures.clear();
    stbi_set_flip_vertically_on_load(static_cast<int>(isFlipped));
    Assimp::Importer importer;
    const aiScene* scene = GetAssimpScene(filePath, importer, assimpFlags);
    std::string directory = filePath.substr(0, filePath.find_last_of('/'));
    ProcessNodeWithTextures(scene->mRootNode, scene, vertices, indices, textures, directory);
	if(makeBinaryVersion){
		SaveBinaryModelWithTextures(filePath + ".wt", vertices, indices, textures);
	}
}
void ModelImporter::LoadModelWithoutTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, bool loadBinaryVersion, bool makeBinaryVersion) {
	if(loadBinaryVersion){
		if(LoadBinaryModelWithoutTextures(filePath + ".wot", vertices, indices)){
//			Log("Loaded model without textures at path: " << filePath << " with " << vertices.size() << " vertices and " << indices.size() << " indices.");
			return;
		}
	}
    vertices.clear();
    indices.clear();
    Assimp::Importer importer;
    const aiScene* scene = GetAssimpScene(filePath, importer, assimpFlags);
    ProcessNodeWithoutTextures(scene->mRootNode, scene, vertices, indices);
	if(makeBinaryVersion){
		SaveBinaryModelWithoutTextures(filePath + ".wot", vertices, indices);
	}
//	Log("Loaded model without textures at path: " << filePath << " with " << vertices.size() << " vertices and " << indices.size() << " indices.");
}
void ModelImporter::LoadModelBasic(const std::string& filePath, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, bool loadBinaryVersion, bool makeBinaryVersion) {
	if(loadBinaryVersion){
		if(LoadBinaryModelBasic(filePath + ".bs", vertices, indices)){
			return;
		}
	}
    vertices.clear();
    indices.clear();
    Assimp::Importer importer;
    const aiScene* scene = GetAssimpScene(filePath, importer, physxFlags);
    ProcessNodeBasic(scene->mRootNode, scene, vertices, indices);
	if(makeBinaryVersion){
		SaveBinaryModelBasic(filePath + ".bs", vertices, indices);
	}
}

void ModelImporter::ProcessNodeWithTextures(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, const std::string& directory) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMeshWithTextures(mesh, scene, vertices, indices, textures, directory);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNodeWithTextures(node->mChildren[i], scene, vertices, indices, textures, directory);
    }
}
void ModelImporter::ProcessNodeWithoutTextures(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
/*
	if(strcmp(node->mName.C_Str(), "RootNode")){
		std::string name = node->mName.C_Str();
		Log(name);
	}
	*/
	//Log(aiNode->mTransformation);
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
void ModelImporter::ProcessNodeBasic(const aiNode* node, const aiScene* scene, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMeshBasic(mesh, vertices, indices);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNodeBasic(node->mChildren[i], scene, vertices, indices);
    }
}
void ModelImporter::ProcessMeshWithTextures(const aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, const std::string& directory) {
    ProcessMeshWithoutTextures(mesh, vertices, indices);
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    ProcessTextures(textures, material, directory);
}
void ModelImporter::ProcessMeshWithoutTextures(const aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	const std::size_t numVertices = vertices.size();
	//const std::size_t numIndices = indices.size();
	//
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
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

		vertex.ObjectIndex = 0;

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
        for (GLuint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j] + numVertices);
        }
    }
}
void ModelImporter::ProcessMeshBasic(const aiMesh* mesh, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) {
    // walk through each of the mesh's vertices
    //assert(!mesh->HasNormals());

    Warn("Mesh has normals");

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertices.push_back(vector);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices < 3) {
            continue;
        }
        assert(face.mNumIndices == 3);
        for (GLuint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void ModelImporter::ProcessTextures(std::vector<Texture>& textures, const aiMaterial* material, const std::string& directory) {
    LoadTextures(textures, material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    LoadTextures(textures, material, aiTextureType_SPECULAR, "texture_specular", directory);
    LoadTextures(textures, material, aiTextureType_HEIGHT, "texture_normal", directory);
    LoadTextures(textures, material, aiTextureType_AMBIENT, "texture_height", directory);
}
void ModelImporter::LoadTextures(std::vector<Texture>& textures, const aiMaterial* mat, const aiTextureType type, const std::string& typeName, const std::string& directory) {
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
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures.push_back(texture);
        }
    }
}

bool ModelImporter::LoadBinaryModelWithTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, bool isFlipped){
	std::ifstream in(filePath, std::ios::binary);
	if(!in){
		return false;
	}
    decltype(vertices.size()) verticesSize;
	decltype(indices.size()) indicesSize;
	decltype(textures.size()) texturesSize;
    in.read(reinterpret_cast<char*>(&verticesSize), sizeof(verticesSize));
    in.read(reinterpret_cast<char*>(&indicesSize), sizeof(indicesSize));
    in.read(reinterpret_cast<char*>(&texturesSize), sizeof(texturesSize));
    vertices.resize(verticesSize); indices.resize(indicesSize); textures.resize(texturesSize); 

    in.read(reinterpret_cast<char*>(&vertices[0]), sizeof(vertices[0])*vertices.size());
    in.read(reinterpret_cast<char*>(&indices[0]), sizeof(indices[0])*indices.size());
    in.read(reinterpret_cast<char*>(&textures[0]), sizeof(textures[0])*textures.size());
	in.close();
	if(!in){
		ErrorAtPos("Reading from binary file: " << filePath << " failed");
		return false;
	}

    return true;
}
bool ModelImporter::LoadBinaryModelWithoutTextures(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices){
	std::ifstream in(filePath, std::ios::binary);
	if(!in){
		return false;
	}
    decltype(vertices.size()) verticesSize;
	decltype(indices.size()) indicesSize;

    in.read(reinterpret_cast<char*>(&verticesSize), sizeof(verticesSize));
    in.read(reinterpret_cast<char*>(&indicesSize), sizeof(indicesSize));

    vertices.resize(verticesSize); indices.resize(indicesSize);

    in.read(reinterpret_cast<char*>(&vertices[0]), sizeof(vertices[0])*vertices.size());
    in.read(reinterpret_cast<char*>(&indices[0]), sizeof(indices[0])*indices.size());

	in.close();
	if(!in){
		ErrorAtPos("Reading from binary file: " << filePath << " failed");
		return false;
	}

    return true;
}
bool ModelImporter::LoadBinaryModelBasic(const std::string& filePath, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices){
	std::ifstream in(filePath, std::ios::binary);
	if(!in){
		return false;
	}
    decltype(vertices.size()) verticesSize;
	decltype(indices.size()) indicesSize;

    in.read(reinterpret_cast<char*>(&verticesSize), sizeof(verticesSize));
    in.read(reinterpret_cast<char*>(&indicesSize), sizeof(indicesSize));

    vertices.resize(verticesSize); indices.resize(indicesSize);

    in.read(reinterpret_cast<char*>(&vertices[0]), sizeof(vertices[0])*vertices.size());
    in.read(reinterpret_cast<char*>(&indices[0]), sizeof(indices[0])*indices.size());

	in.close();
	if(!in){
		ErrorAtPos("Reading from binary file: " << filePath << " failed");
		return false;
	}

    return true;
}

void ModelImporter::SaveBinaryModelWithoutTextures(const std::string& filePath, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices){
	std::ofstream out(filePath, std::ios::binary);
    auto verticesSize = vertices.size();
	auto indicesSize = indices.size();

    out.write(reinterpret_cast<const char*>(&verticesSize), sizeof(verticesSize));
    out.write(reinterpret_cast<const char*>(&indicesSize), sizeof(indicesSize));

    out.write(reinterpret_cast<const char*>(&vertices[0]), sizeof(vertices[0])*vertices.size());
    out.write(reinterpret_cast<const char*>(&indices[0]), sizeof(indices[0])*indices.size());
	out.close();
	if(!out){
		ErrorAtPos("Writing to binary file: " << filePath << " failed");
	}
}

void ModelImporter::SaveBinaryModelWithTextures(const std::string& filePath, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, bool isFlipped){
	std::ofstream out(filePath, std::ios::binary);
    auto verticesSize = vertices.size();
	auto indicesSize = indices.size();
	auto texturesSize = textures.size();

    out.write(reinterpret_cast<const char*>(&verticesSize), sizeof(verticesSize));
    out.write(reinterpret_cast<const char*>(&indicesSize), sizeof(indicesSize));
    out.write(reinterpret_cast<const char*>(&texturesSize), sizeof(texturesSize));

    out.write(reinterpret_cast<const char*>(&vertices[0]), sizeof(vertices[0])*vertices.size());
    out.write(reinterpret_cast<const char*>(&indices[0]), sizeof(indices[0])*indices.size());
    out.write(reinterpret_cast<const char*>(&textures[0]), sizeof(textures[0])*textures.size());
	out.close();
	if(!out){
		ErrorAtPos("Writing to binary file: " << filePath << " failed");
	}
}
void ModelImporter::SaveBinaryModelBasic(const std::string& filePath, const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices){
	std::ofstream out(filePath, std::ios::binary);
    auto verticesSize = vertices.size();
	auto indicesSize = indices.size();

    out.write(reinterpret_cast<const char*>(&verticesSize), sizeof(verticesSize));
    out.write(reinterpret_cast<const char*>(&indicesSize), sizeof(indicesSize));

    out.write(reinterpret_cast<const char*>(&vertices[0]), sizeof(vertices[0])*vertices.size());
    out.write(reinterpret_cast<const char*>(&indices[0]), sizeof(indices[0])*indices.size());
	out.close();
	if(!out){
		ErrorAtPos("Writing to binary file: " << filePath << " failed");
	}
}
