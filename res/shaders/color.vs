#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in int ObjectIndexVertex;

out vec3 FragPos;
out vec2 TexCoords;  
smooth out vec3 Normal;
out flat int ObjectIndexFragment;

uniform mat4 matModel[150];

layout (std140, binding = 0) uniform Matrices
{
    mat4 matView;
    mat4 matProj;
};


void main()
{
	ObjectIndexFragment = ObjectIndexVertex;
    FragPos = vec3(matModel[ObjectIndexVertex] * vec4(aPos, 1.0f));
    Normal = aNormal;  
    TexCoords = aTexCoords;

    gl_Position = matProj *  matView * vec4(FragPos, 1.0f);
}
