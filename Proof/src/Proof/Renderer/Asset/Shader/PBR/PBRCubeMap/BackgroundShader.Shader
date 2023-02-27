#Vertex Shader

#version 450
//https://gist.github.com/keijiro/01dc44ad440623a5d30c
layout(location = 0) in vec3 aPos;
layout(location = 0) out vec3 outWorldPos;
layout(set = 0, binding = 0) uniform CameraData
{
    mat4 Projection;
    mat4 View;
    vec3 Position;
}CameraUBO;

// roating the cubemap
// can hvave rotating 
//https://gist.github.com/onedayitwillmake/3288507

void main()
{
    outWorldPos = aPos;

    mat4 rotView = mat4(mat3(CameraUBO.View));
    vec4 clipPos = CameraUBO.Projection * rotView * vec4(outWorldPos, 1.0);
    gl_Position = clipPos.xyww;
}

#Fragment Shader
#version 450


layout(set = 0, binding = 1) uniform  samplerCube environmentMap;
layout(location = 0) in vec3 WorldPos;      
layout(location = 0) out vec4 outFragColor;
 
float exposure = 1.0f; // around a maximum of 500 is just plain white
vec3 tintColor = vec3(1);
void main()
{

    vec3 envColor = textureLod(environmentMap, WorldPos,0).rgb;
    envColor *= tintColor;
    envColor *= exposure;
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));
    outFragColor = vec4(envColor,  1.0);
}

