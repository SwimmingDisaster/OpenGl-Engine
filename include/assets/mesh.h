#pragma once
#include "mypch.h"
#include "core/texture.h"
#include "ecs/component.h"
#include "ecs/entity.h"
#include "utils/fileUtils.h"

#include "core/shader.h"
#include "assets/transform.h"
#include "ecs/other/componentFactory.h"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
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
	~Model();

	virtual void Start();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);
	virtual void Show();

	void loadModel(std::string const& path);
	void  processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

};



class ModelRenderer : public Component {
	REGISTER(ModelRenderer);
public:
	Shader m_shader;
	std::string shaderName = "res/shaders/color";
	std::shared_ptr<Model> m_modelComponent;

	glm::vec3 m_color = {1.0f, 1.0f, 1.0f}; //todo this should be in a material component


public:
	ModelRenderer();
	~ModelRenderer();
	virtual void Serialize(YAML::Emitter& out);
	virtual void Deserialize(const YAML::Node& data);
	virtual void Start();
	virtual void Update();
	virtual void Show();


	void DrawModel(Shader& shader, std::shared_ptr<Model> model);
	void DrawMesh(Shader& shader, Mesh& mesh);
};

