#Vertex Shader

#version 450
//https://gist.github.com/keijiro/01dc44ad440623a5d30c
layout(location = 0) in vec3 aPos;
layout(location = 0) out vec3 outWorldPos;
layout(set = 0, binding = 0) uniform CameraData
{
    mat4 Projection;
    mat4 ViewProjection;
    mat4 UnreversedProjectionMatrix;
    vec3 Position;
    float NearPlane;
    float FarPlane;
}CameraUBO;
const float PI = 3.1415926535897932384626433832795f;

layout(set = 0, binding = 2) uniform SkyBoxData
{
    vec3 TintColor;
    float Lod;
    float Exposure;
    float Roation;
}u_SkyBoxInfo;
vec3 Rotate(float angle, vec3 axis)
{
	float rad = radians(angle);
	mat3x3 rotationMatrix = {
		vec3(cos(rad), 0.0, sin(rad)),
		vec3(0.0, 1.0, 0.0),
		vec3(-sin(rad), 0.0, cos(rad))
	};
	
	return rotationMatrix * axis;
}
vec4 remapSkyPositionZ(in vec4 position)
{
    position.z = position.w;
    return position;
}
void main()
{
    outWorldPos = aPos;
    mat4 rotView = mat4(mat3(CameraUBO.ViewProjection));
    vec4 clipPos = CameraUBO.Projection* rotView *vec4(outWorldPos,1.0);
    gl_Position = clipPos.xyww;
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
    float Intensity;
    float Roation;
}u_SkyBoxInfo;

const float PI = 3.1415926535897932384626433832795f;
void main()
{

    vec3 envColor = textureLod(u_EnvironmentMap, WorldPos,u_SkyBoxInfo.Lod).rgb;
    envColor *= u_SkyBoxInfo.TintColor; 
    envColor *= u_SkyBoxInfo.Intensity;
    //// HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));
    outFragColor = vec4(envColor,  1.0);
}

