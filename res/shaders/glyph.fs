#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

uniform float width = 0.50;
uniform float edge = 0.1;

void main()
{    
	float distance = texture(text, TexCoords).a;
	float alpha = smoothstep(width, width + edge, distance); 
    vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    color = vec4(textColor, 1.0) * sampled;
}  