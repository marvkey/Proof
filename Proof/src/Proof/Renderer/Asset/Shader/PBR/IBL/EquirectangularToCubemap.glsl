#Compute Shader

#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_multiview : enable
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(set = 0, binding = 0) uniform sampler2D u_EquirectangularMap;
layout(set = 0, binding = 1,rgba16f) uniform writeonly imageCube u_CubeMap;

const float PI = 3.1415926535897932384626433832795f;

layout(push_constant) uniform CubeMapSize
{
    ivec2 equirectangularSize;
    uvec2 cubeMapSize;
} pc;

vec3 GetCubeMapTexCoord()
{
    vec2 st = gl_GlobalInvocationID.xy/ vec2(imageSize(u_CubeMap));
    vec2 uv = 2.0 * vec2(st.x,1.0-st.y) - vec2(1.0);


   vec3 ret;
    switch (gl_GlobalInvocationID.z)
    {
        case 0:
            ret = vec3(1.0, uv.y, -uv.x);
            break;
        case 1:
            ret = vec3(-1.0, uv.y, uv.x);
            break;
        case 2:
            ret = vec3(uv.x, 1.0, -uv.y);
            break;
        case 3:
            ret = vec3(uv.x, -1.0, uv.y);
            break;
        case 4:
            ret = vec3(uv.x, uv.y, 1.0);
            break;
        case 5:
            ret = vec3(-uv.x, uv.y, -1.0);
            break;
        default:
            ret = vec3(0.0);
            break;
    }
    
    return normalize(ret);
}
void main()
{
    vec3 cubeTC =GetCubeMapTexCoord(); 
    float phi = atan(cubeTC.z,cubeTC.x);
    float theta = acos(cubeTC.y);
    vec2 uv = vec2(phi/(2.0 * PI)+0.5,theta/PI);

    //vec3 color = imageLoad(u_EquirectangularMap, ivec2(uv * vec2(pc.equirectangularSize))).rgb;
    vec4 color = texture(u_EquirectangularMap, uv);
    color = min(color,vec4(500.0f));//https://www.youtube.com/watch?v=A-5F7-mA3pE&ab_channel=TheCherno min 20:10

    imageStore(u_CubeMap, ivec3(gl_GlobalInvocationID), color);
}