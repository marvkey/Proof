#Vertex Shader
#version 330 core
layout(location =0) in vec3  aPos;
layout(location =2)in vec2 aTexCoord;
layout(location =3) in mat4 aTransform;

uniform mat4 u_View;
uniform mat4 u_Projection;
out vec2 TexCoord;
void main() {
	TexCoord = aTexCoord;
	gl_Position = u_Projection * u_View * aTransform * vec4(aPos,1.0);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform sampler2D texture_diffuse;
in vec2 TexCoord;
out int color;
void main() {
	FragColor = texture(texture_diffuse,TexCoord);
	if(FragColor.a <0.5)
		discard;
	// THIS IS BASICALLY SAYING LIKE IF WE HAVE A DONUT WE CAN CLICK IN HTE MIDDLE BUT
	// WILL STILL OUTPUT OUR ENTITY ID
	color = 12;
}