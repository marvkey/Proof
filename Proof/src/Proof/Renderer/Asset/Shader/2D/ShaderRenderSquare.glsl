#Vertex Shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location =1) in vec4 aColor;
layout (location =2) in vec2 aTexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
out vec4 m_Color;
out vec2 v_TexCoord;
void main(){
			m_Color = aColor;
			v_TexCoord = aTexCoord;
			gl_Position =u_ViewProjection* u_View * vec4(aPos,1.0);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
in vec4 m_Color;
in vec2 v_TexCoord;
uniform sampler2D m_TextureSlot;
void main(){
			FragColor =texture(m_TextureSlot, v_TexCoord)* vec4(m_Color);
}

