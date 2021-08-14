#version 430 core

layout (location = 0) out vec4 gColor;
layout (location = 1) out vec4 gPosition;
layout (location = 2) out vec4 gNormal;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

in vec3 FragPos;
smooth in vec3 Normal;
in vec2 TexCoords;
in flat int ObjectIndexFragment;

uniform float[1] brightnessScale;

uniform vec3 color[150];

uniform Material material;

void main(){    
    gColor = vec4(color[ObjectIndexFragment], 1.0f);
    gPosition.xyz = FragPos;
	gPosition.a = 1.0f;
    gNormal = vec4(normalize(Normal), 1.0f);
	//FragColor = vec4(color[0], 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

