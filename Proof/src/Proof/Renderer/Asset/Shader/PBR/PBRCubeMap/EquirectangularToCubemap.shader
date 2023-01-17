#Vertex Shader

#version 450
layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 outWorldPos;
layout(set = 0, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}CameraUBO;

void main()
{
    outWorldPos = aPos;
    gl_Position = CameraUBO.ProjectionMatrix * CameraUBO.ViewMatrix * vec4(outWorldPos, 1.0);
}

#Fragment Shader
#version 450


//layout(set = 0, binding = 1) uniform  sampler  equirectangularMap;
layout(set = 0, binding = 1) uniform  sampler2D  equirectangularMap;
layout(location = 0) in vec3 worldPos;
layout(location = 0) out vec4 outFragColor;
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
void main()
{
    vec2 uv = SampleSphericalMap(normalize(worldPos)); // make sure to normalize localPos

    //vec3 color = textureLod(equirectangularMap, vec3(uv,0.0),0.0).rgb;
    vec3 color =  texture(equirectangularMap, uv).xyz;
    outFragColor = vec4(color, 1.0);
}

