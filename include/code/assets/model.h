#pragma once

#include "mypch.h"
#include "assets/mesh.h"

#include "ecs/other/componentFactory.h"
#include "ecs/other/componentDefines.h"



class Model : public Component
{
	REGISTER(Model);
public:
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	unsigned int VAO, VBO, EBO;

	Transform* transform;

	std::string directory;
	std::string path = "res/fbx/box.fbx";
	//std::string path = "res/fbx/sphere.fbx";
	bool isFlipped = false;

public:
	void Start() override;
	void Serialize(YAML::Emitter& out) const override;
	void Deserialize(const YAML::Node& data) override;
	void Show() override;

	/*
	void loadModel(std::string const& path);
	void  processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
	*/

	Model();
#ifdef SHOW_DELETED
public:
	virtual ~Model();
#else
//public:
	//virtual ~Model() {};
#endif
};
