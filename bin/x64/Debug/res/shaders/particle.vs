#version 430 core

layout(location = 0) in vec3 squareVertices;
/* layout(location = 1) in vec4 xyzs;
layout(location = 2) in vec4 color; */

out vec2 UV;
out vec4 particlecolor;


uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

uniform float maxLifetime;
uniform float minLifetime;

struct Particle {
	vec3 pos;
	vec3 speed;
	float r, g, b, a;
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; 
};	

struct Test {
	vec4 pos_size;
	vec4 color;
	vec4 speed;
};


layout(std430, binding = 2) buffer ParticleSSBO
{
    Test[100000] sizeList;
};



void main()
{
	float particleSize = sizeList[gl_InstanceID].pos_size.w * sizeList[gl_InstanceID].speed.w / maxLifetime * 0.25f; 

	vec3 particleCenter_wordspace = sizeList[gl_InstanceID].pos_size.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace* squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position =VP * vec4(vertexPosition_worldspace, 1.0f);// VP * vec4(vertexPosition_worldspace, 1.0f);

	
	particlecolor = sizeList[gl_InstanceID].color;
}
	



