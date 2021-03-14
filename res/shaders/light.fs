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


uniform Material material;

void main()
{    
    FragColor =vec4(texture(material.texture_diffuse1, TexCoords).xyz, 1.0f);
}

