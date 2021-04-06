#version 330 core

uniform vec3 Location;
void main(){
	gl_Position = vec4(Location,1.0f);
}