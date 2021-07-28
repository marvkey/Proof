#Vertex Shader
#version 330 core
layout(location =0) in vec3  aPos;
layout(location =1)in vec2 aTexCoord;
layout(location =3) in mat4 aTransform;
layout(location = 7) in vec4 aMaterial;
uniform mat4 u_View;
uniform mat4 u_Projection;
out vec2 TexCoord;
out vec4 MaterialColour;
void main() {
	TexCoord = aTexCoord;
	MaterialColour = aMaterial;
	gl_Position = u_Projection * u_View * aTransform * vec4(aPos,1.0);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform sampler2D texture_diffuse;
in vec2 TexCoord;
out int color;
in vec4 MaterialColour;
void main() {
	FragColor = texture(texture_diffuse,TexCoord)*MaterialColour;
	if(FragColor.a <0.1)
		discard;
}