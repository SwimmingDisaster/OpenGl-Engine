#version 330 core

layout (location = 0) out vec4 gColor;
layout (location = 1) out vec4 gPosition;
layout (location = 2) out vec3 gNormal;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

uniform float width = 0.50;
uniform float edge = 0.1;

void main()
{    
	float distance = texture(text, TexCoords).a;
	float alpha = smoothstep(width, width + edge, distance); 
    vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    gColor = vec4(textColor, 1.0) * sampled;
//    gNormal = normalize(Normal);
}  
