#version 450

layout(location =0) in vec2 position;
layout(location =1 )in vec3 color;

layout(push_constant) uniform Push{
	vec2 offset;
	vec3 color;
}push;
layout(set=0,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;
void main(){
    vec4 worldPos = vec4(position+push.offset,1.0,1.0);

	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * worldPos;

}