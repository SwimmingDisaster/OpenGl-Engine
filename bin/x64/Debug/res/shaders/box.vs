
#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 matModel;
layout (std140, binding = 0) uniform Matrices
{
    mat4 matView;
    mat4 matProj;
};


void main()
{
    vec3 FragPos = vec3(matModel * vec4(aPos, 1.0));

    gl_Position = matProj * matView * vec4(FragPos, 1.0);
}