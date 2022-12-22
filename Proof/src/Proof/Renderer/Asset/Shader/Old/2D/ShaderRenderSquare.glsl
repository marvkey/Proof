#Vertex Shader
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location =1) in vec4 aColor;
layout (location =2) in vec2 aTexCoord;
layout (location =3 )in float aTexIndex;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
out vec4 m_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
layout(std140,binding=1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;

}Camera;
void main(){
			m_Color = aColor;
			v_TexCoord = aTexCoord;
			v_TexIndex = aTexIndex;
			gl_Position =Camera.ProjectionMatrix* Camera.ViewMatrix* vec4(aPos,1.0);
}

#Fragment Shader
#version 450 core
out vec4 FragColor;
in vec4 m_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
uniform sampler2D u_TextureSlot[32];
void main(){
vec4 temp =texture(u_TextureSlot[int(v_TexIndex)], v_TexCoord)* vec4(m_Color);
if(temp.a<0.1)
			discard;
			FragColor =temp;
}