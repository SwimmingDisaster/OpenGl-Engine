#include "mypch.h"
#include "core/shader.h"



std::unordered_map<std::string, std::shared_ptr<Shader> > Shader::shaderMap;


void Shader::CreateVertexAndFragment(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode   = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (const std::exception& e)
	{
		std::cout << "Shader file not read successfully" << e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	unsigned int vertexShader, fragmentShader;


	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	CheckShaderError(vertexShader, GL_COMPILE_STATUS, false, "Vertex shader init failed");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	CheckShaderError(fragmentShader, GL_COMPILE_STATUS, false, "Fragment shader init failed");


	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	CheckShaderError(ID, GL_LINK_STATUS, true, "Program linking failed");


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	std::string fullname(vertexPath);
	size_t lastindex = fullname.find_last_of(".");
	std::string rawname = fullname.substr(0, lastindex);
	shaderMap[rawname] = shared_from_this();
}
void Shader::CreateCompute(const char* computePath) {
	std::string computeCode;
	std::ifstream computeFile;
	computeFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{

		computeFile.open(computePath);
		std::stringstream vComputeStream;

		vComputeStream << computeFile.rdbuf();

		computeFile.close();

		computeCode = vComputeStream.str();
	}
	catch (const std::exception& e)
	{
		std::cout << "Shader file not read successfully" << e.what() << std::endl;
	}
	const char* shaderCode = computeCode.c_str();


	unsigned int computeShader;


	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shaderCode, NULL);
	glCompileShader(computeShader);

	CheckShaderError(computeShader, GL_COMPILE_STATUS, false, "Compute shader init failed");



	ID = glCreateProgram();
	glAttachShader(ID, computeShader);
	glLinkProgram(ID);

	CheckShaderError(ID, GL_LINK_STATUS, true, "Program linking failed");

	glDeleteShader(computeShader);
}


Shader::Shader() {}


Shader::~Shader() {
	//Log("Deleted shader " << ID);
	//glDeleteProgram(ID);
	//auto itr = shaderMap.begin();

	//while ( itr != shaderMap.end() )
	//{
	//	if ( (*itr).second == *this )	
	//		itr = shaderMap.erase( itr );
	//	else
	//		itr++;
	//}
}

bool Shader::operator==(Shader &rhs) {
	return (*this).ID == rhs.ID;
}

void Shader::use() {
	glUseProgram(ID);
}
// ------------------------------------------------------------------------
void Shader::setBool(const char* name, bool value) const noexcept
{
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const char* name, int value) const noexcept
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const char* name, float value) const noexcept
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const char* name, const glm::vec2 &value) const noexcept
{
	glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const char* name, float x, float y) const noexcept
{
	glUniform2f(glGetUniformLocation(ID, name), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const char* name, const glm::vec3 &value) const noexcept
{
	glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const char* name, float x, float y, float z) const noexcept
{
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, const glm::vec4 &value) const noexcept
{
	glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, const glm::vec3 &value) const noexcept
{
	glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, float x, float y, float z, float w) const noexcept
{
	glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const char* name, const glm::mat2 &mat) const noexcept
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const char* name, const glm::mat3 &mat) const noexcept
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const char* name, const glm::mat4& mat) const noexcept
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}
//-------------------------------------------------------------------------


void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[512] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cout << errorMessage << ": '" << error << "'" << std::endl;
	}
}
