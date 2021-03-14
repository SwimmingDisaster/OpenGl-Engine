#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh_new.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "texture.h"

class Model
{
public:
    Model();
    Model(std::string const& path, bool gamma = false);

    void Draw(Shader& shader);

private:
    void loadModel(std::string const& path);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);


public:
    std::vector<Mesh>    meshes;
    glm::vec3 vPos;
    glm::vec3 vRot;
    glm::vec3 vScale;
private:
    std::string directory;
    bool gammaCorrection;
};

