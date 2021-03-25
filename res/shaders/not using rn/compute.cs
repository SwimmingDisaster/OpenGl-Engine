#version 430 core


struct Test {
	vec4 pos_size;
	vec4 color;
	vec4 speed;
};

uniform vec4 start_color;
uniform vec4 end_color;
uniform vec3 gravity;

uniform float maxLifetime;
uniform float minLifetime;

layout(std430, binding = 2) buffer ParticleSSBO
{
	Test[100000] sizeList;
};

uniform float deltaTime;


layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	uint id = gl_GlobalInvocationID.x;
	sizeList[id].speed.x += gravity.x * deltaTime;
	sizeList[id].speed.y += gravity.y * deltaTime;
	sizeList[id].speed.z += gravity.z * deltaTime;
	//sizeList[id].speed.w -= deltaTime;
	//sizeList[id].speed.w = clamp(sizeList[id].speed.w, 0.0f, 10000.0f);
	sizeList[id].pos_size.x += sizeList[id].speed.x * deltaTime;
	sizeList[id].pos_size.y += sizeList[id].speed.y * deltaTime;
	sizeList[id].pos_size.z += sizeList[id].speed.z * deltaTime;
	sizeList[id].color = mix(start_color,end_color, 1.0f - (sizeList[id].pos_size.w  - 1.0f)/3.1f);
}