#include "mypch.h"
#include "core/shader.h"



std::unordered_map<std::string, std::shared_ptr<Shader> > Shader::shaderMap;
std::vector<std::shared_ptr<Shader>> Shader::shaderList;
std::vector<std::string> Shader::shaderNames;
int Shader::BoundShaderID;

void Shader::CreateVertexAndFragment(const std::string& name) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {

        vShaderFile.open(name+ ".vs");
        fShaderFile.open(name+ ".fs");
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



    //std::string fullname(vertexPath);
    //size_t lastindex = fullname.find_last_of(".");
    //std::string rawname = fullname.substr(0, lastindex);
    //shaderMap[rawname] = shared_from_this();
    shaderMap[name] = shared_from_this();
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

const unsigned int& Shader::GetID() const {
    return ID;
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
    if (ID != BoundShaderID) {
        glUseProgram(ID);
        BoundShaderID = ID;
    }
}
// ------------------------------------------------------------------------
void Shader::setBool(const char* name, bool value) noexcept
{
    glUniform1i(GetUniformLocation(name), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const char* name, int value) noexcept
{
    glUniform1i(GetUniformLocation(name), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const char* name, float value) noexcept
{
    glUniform1f(GetUniformLocation(name), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const char* name, const glm::vec2 &value) noexcept
{
    glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const char* name, float x, float y) noexcept
{
    glUniform2f(GetUniformLocation(name), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const char* name, const glm::vec3 &value) noexcept
{
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const char* name, float x, float y, float z) noexcept
{
    glUniform3f(GetUniformLocation(name), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, const glm::vec4 &value) noexcept
{
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, const glm::vec3 &value) noexcept
{
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const char* name, float x, float y, float z, float w) noexcept
{
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const char* name, const glm::mat2 &mat) noexcept
{
    glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const char* name, const glm::mat3 &mat) noexcept
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const char* name, const glm::mat4& mat) noexcept
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
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

GLint Shader::GetUniformLocation(const char* name) {
    /*	if (uniform_cache.find(name) != uniform_cache.end()) {
    		return uniform_cache[name];
    	}*/

    GLint location = glGetUniformLocation(ID, name);
    uniform_cache[name] = location;
    return location;
}
