#version 330 core
layout (location = 0) in vec2 vertex; 
layout (location = 1) in vec2 texture;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 rotation;
uniform vec2 position;

void main(){	

    gl_Position =  projection * rotation* vec4(vertex + position, 0.0, 1.0);
    TexCoords = texture;
}  
