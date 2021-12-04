#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 5) in mat4 aTransform;

layout(std140,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;

void main() {
     vec4 worldPos = aTransform * vec4(aPos,1.0);

    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * worldPos;
}


#Fragment Shader
#version 450 core
out vec4 FragColor;
void main(){
    FragColor = vec4(0.160, 0.458, 0.101,1.0);
}