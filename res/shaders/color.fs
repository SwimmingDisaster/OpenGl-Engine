#version 430 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in flat int ObjectIndexFragment;


uniform vec3 color[150];

uniform Material material;

void main()
{    
    FragColor = vec4(color[ObjectIndexFragment], 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

