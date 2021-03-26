#include "assets/mesh.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}


std::vector<Texture> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{	// if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}




REGISTERIMPL(Model);
Model::Model() {
	m_name = "Model";
};
Model::~Model() {
	Log("Deleted " << m_name);
};


void Model::Start() {
	transform = m_parentEntity->GetComponent<Transform>();
	loadModel(path);
}


void Model::Serialize(YAML::Emitter& out) {
	out << YAML::Key << m_name;
	out << YAML::BeginMap;

	out << YAML::Key << "Is flipped" << YAML::Value << isFlipped;
	out << YAML::Key << "Path" << YAML::Value << path;

	out << YAML::EndMap;
}

void Model::Deserialize(const YAML::Node& data) {
	isFlipped = data["Is flipped"].as<bool>();
	path = data["Path"].as<std::string>();
}

void Model::Show() {
	ImGui::Checkbox("Is flipped", &isFlipped);

	ImGui::InputText("File path", &path, ImGuiInputTextFlags_CallbackResize);

	if (ImGui::Button("Browse")) {
		path = OpenFile("3D model\0*.fbx;*.dae;*.gltf;*.glb;*.blend;*.3ds;*.ase;*.obj;*.ifc;*.xgl;*.zgl;*.ply;*.dxf;*.lwo;*.lws;*.lxo;*.stl;*.x;*.ac;*.ms3d;*.cob;*.scn;*.bvh;*.csm;*.xml;*.irrmesh;*.irr;*.mdl;*.md2;*.md3;*.pk3;*.mdc;*.md5*;*.smd;*.vta;*.ogex;*.3d;*.b3d;*.q3d;*.q3s;*.nff;*.nff;*.off;*.raw;*.ter;*.mdl;*.hmp;*.ndo\0");
		std::replace( path.begin(), path.end(), '\\', '/');
	}

	ImGui::SameLine();

	if (ImGui::Button("Reload")) {
		textures_loaded.clear();
		meshes.clear();

		Start();
	}
}

void Model::loadModel(std::string const& path)
{

	stbi_set_flip_vertically_on_load(isFlipped);


	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality | aiProcess_OptimizeMeshes);


	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Error("Assimp:  " << importer.GetErrorString());
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void  Model::processNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

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
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		if (face.mNumIndices < 3) {
			continue;
		}
		assert(face.mNumIndices == 3);
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{	// if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}



REGISTERIMPL(ModelRenderer);
ModelRenderer::ModelRenderer() {
	m_name = "ModelRenderer";
}
ModelRenderer::~ModelRenderer() {
	Log("Deleted " << m_name);
}

void ModelRenderer::Serialize(YAML::Emitter& out) {
	out << YAML::Key << m_name;
	out << YAML::BeginMap;

	out << YAML::Key << "Shader name" << YAML::Value << shaderName;
	out << YAML::Key << "Color" << YAML::Value << m_color;

	out << YAML::EndMap;
}
void ModelRenderer::Deserialize(const YAML::Node& data) {
	shaderName = data["Shader name"].as<std::string>();
	m_color = data["Color"].as<glm::vec3>();
}
void ModelRenderer::Start() {
	m_shader = Shader::shaderMap[shaderName];
	m_modelComponent = m_parentEntity->GetComponent<Model>();
}
void ModelRenderer::Update() {
	m_shader.use();
	m_shader.setVec3("color", m_color);
	DrawModel(m_shader, m_modelComponent);
}
void ModelRenderer::Show() {
	ImGui::InputInt("Shader id", (int*)&m_shader.ID);
	ImGui::ColorEdit3("Color", glm::value_ptr(m_color));
}


void ModelRenderer::DrawModel(Shader& shader, std::shared_ptr<Model> model)
{
	glm::mat4 matModel = glm::mat4(1.0f);
	matModel = glm::translate(matModel, model->transform->position);
	matModel = glm::rotate(matModel, glm::radians(model->transform->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matModel = glm::rotate(matModel, glm::radians(model->transform->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matModel = glm::rotate(matModel, glm::radians(model->transform->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	matModel = glm::scale(matModel, model->transform->scale);


	shader.setMat4("matModel", matModel);
	for (unsigned int i = 0; i < model->meshes.size(); i++) {
		DrawMesh(shader, model->meshes[i]);
	}
}

void ModelRenderer::DrawMesh(Shader& shader, Mesh& mesh)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < mesh.textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = mesh.textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++); // transfer unsigned int to stream
		}

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
	}

	// draw mesh
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

