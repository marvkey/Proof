#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 aColor;
out vec3 OurColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Scale;
void main(){
	gl_Position = Projection * View * Model * Scale * vec4(apos,1.0);
	OurColor =	aColor;
}

