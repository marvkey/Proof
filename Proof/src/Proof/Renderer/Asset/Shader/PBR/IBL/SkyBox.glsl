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
layout(set = 0, binding = 2) uniform SkyBoxData
{
    vec3 TintColor;
    float Lod;
    float Exposure;
    float Roation;
}u_SkyBoxInfo;
vec4 RotateAroundYInDegrees(vec4 vertex, float degrees)
{
float angle = degrees * 3.14 / 180.0;
	float sina=sin(angle);
	float cosa=cos(angle);
	mat2 m = mat2(cosa, -sina, sina, cosa);
	return vec4(m*vertex.xz, vertex.yw).xzyw;
}
// roating the cubemap
// can hvave rotating 
//https://gist.github.com/onedayitwillmake/3288507

void main()
{
    mat4 inverseProjectionViewMatrix = inverse(CameraUBO.Projection * CameraUBO.View);
    
    vec4 position = vec4(aPos.xy,1.0,1.0);
    gl_Position = position;
    
    outWorldPos = (inverseProjectionViewMatrix * position).xyz;
}

#Fragment Shader
#version 450


layout(set = 0, binding = 1) uniform  samplerCube u_EnvironmentMap;
layout(location = 0) in vec3 WorldPos;      
layout(location = 0) out vec4 outFragColor;
layout(set = 0, binding = 2) uniform SkyBoxData
{
    vec3 TintColor;
    float Lod;
    float Exposure;
    float Roation;
}u_SkyBoxInfo;

const float PI = 3.1415926535897932384626433832795f;
void main()
{

    vec3 envColor = textureLod(u_EnvironmentMap, normalize(WorldPos),u_SkyBoxInfo.Lod).rgb;
    envColor *= u_SkyBoxInfo.TintColor; 
    envColor *= u_SkyBoxInfo.Exposure;
    //// HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));
    outFragColor = vec4(envColor,  1.0);
}

