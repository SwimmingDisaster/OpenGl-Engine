#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in int ObjectIndexVertex;

uniform mat4 matModel[150];

layout (std140, binding = 0) uniform Matrices
{
    mat4 matView;
    mat4 matProj;
};


void main()
{
    vec3 FragPos = vec3(matModel[ObjectIndexVertex] * vec4(aPos, 1.0f));
    gl_Position = matProj *  matView * vec4(FragPos, 1.0f);
}
