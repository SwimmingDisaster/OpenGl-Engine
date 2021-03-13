#version 330 core

in vec2 UV;
in vec4 particlecolor;

out vec4 color;


void main(){
	color = particlecolor;
}