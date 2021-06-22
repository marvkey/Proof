#Vertex Shader
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1)in vec2 aTexCoords;
layout(location = 2)in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
out vec2 TexCoord;
void main() {
	TexCoord = aTexCoords;
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos,1.0);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform vec4 u_Color;
in vec2 TexCoord;
uniform sampler2D u_Texutre[32];


void main() {
	FragColor =texture(u_Texutre[0],TexCoord)* vec4(u_Color);
}