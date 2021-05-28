#Vertex Shader
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	
	gl_Position = Projection * View * Model * vec4(aPos,1.0);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;

void main() {
	FragColor = vec4(1.0); // set alle 4 vector values to 1.0
}