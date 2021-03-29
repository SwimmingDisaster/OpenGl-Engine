#pragma once

#include "mypch.h"
#include "assets/mesh.h"


#include "ecs/other/componentFactory.h"



class Model : public Component
{
	REGISTER(Model);
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;

	std::shared_ptr<Transform> transform;

	std::string directory;
	std::string path = "res/fbx/box.fbx";
	bool isFlipped = false;

public:
	Model();

#ifdef SHOW_DELETED
	~Model();
#endif

	virtual void Start();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);
	virtual void Show();

	void loadModel(std::string const& path);
	void  processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

};