#Vertex Shader

#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_multiview : enable

layout(location = 0) in vec3 A_POSITION;

layout(location = 0) out vec3 outPosition;

layout(set = 0, binding = 0) uniform ProjView{
    mat4 projection;
    mat4 view[6];
} pv;



void main() {
    outPosition = A_POSITION;
    gl_Position = pv.projection * pv.view[gl_ViewIndex] * vec4(A_POSITION, 1.0f);
}

#Fragment Shader
#version 450


#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D equirectangularMap;



const float PI = 3.1415926535897932384626433832795f;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = SampleSphericalMap(normalize(inPosition));
    vec3 color = texture(equirectangularMap, uv).rgb;

    outColor = vec4(color, 1.0f);
}
