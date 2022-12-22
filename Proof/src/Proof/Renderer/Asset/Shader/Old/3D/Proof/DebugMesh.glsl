#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;

layout(std140,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {

   gl_Position = projection * view * model * vec4(aPos, 1.0f);
}


#Fragment Shader
#version 450 core
out vec4 FragColor;
void main(){
    FragColor = vec4(0.161,1.00,0,1.0);
}