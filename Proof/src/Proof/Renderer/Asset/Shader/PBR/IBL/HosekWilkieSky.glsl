//https://github.com/TKscoot/Ivy/blob/3b0a09d719e28c260c8eb5d7fbeb52be876e2af8/projects/Sandbox/shaders/HosekWilkieSkyModel.comp#L16
#Compute Shader
#version 450 core

const float PI = 3.141592;

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_CubeMap;

layout (push_constant) uniform Uniforms
{
    vec4 Direction;
    vec4 A, B, C, D, E, F, G, H, I, Z;
} u_Uniforms;

vec3 GetCubeMapTexCoord()
{
    vec2 st = gl_GlobalInvocationID.xy / vec2(imageSize(o_CubeMap));
    vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);

    vec3 ret;
    if (gl_GlobalInvocationID.z == 0)      ret = vec3(  1.0, uv.y, -uv.x);
    else if (gl_GlobalInvocationID.z == 1) ret = vec3( -1.0, uv.y,  uv.x);
    else if (gl_GlobalInvocationID.z == 2) ret = vec3( uv.x,  1.0, -uv.y);
    else if (gl_GlobalInvocationID.z == 3) ret = vec3( uv.x, -1.0,  uv.y);
    else if (gl_GlobalInvocationID.z == 4) ret = vec3( uv.x, uv.y,   1.0);
    else if (gl_GlobalInvocationID.z == 5) ret = vec3(-uv.x, uv.y,  -1.0);
    return normalize(ret);
}
 
 vec3 hosek_wilkie(float cos_theta, float gamma, float cos_gamma)
{
	vec3 chi = (1 + cos_gamma * cos_gamma) / pow(1 + u_Uniforms.H.xyz * u_Uniforms.H.xyz - 2 * cos_gamma * u_Uniforms.H.xyz, vec3(1.5));
    return (1 + u_Uniforms.A.xyz * exp(u_Uniforms.B.xyz / (cos_theta + 0.01))) * (u_Uniforms.C.xyz + u_Uniforms.D.xyz * exp(u_Uniforms.E.xyz * gamma) + u_Uniforms.F.xyz * (cos_gamma * cos_gamma) +u_Uniforms.G.xyz * chi + u_Uniforms.I.xyz * sqrt(cos_theta));
}

// ------------------------------------------------------------------

vec3 hosek_wilkie_sky_rgb(vec3 v, vec3 sun_dir)
{
    float cos_theta = clamp(v.y, 0, 1);
	float cos_gamma = clamp(dot(v, sun_dir), 0, 1);
	float gamma_ = acos(cos_gamma);

	vec3 R = u_Uniforms.Z.xyz * hosek_wilkie(cos_theta, gamma_, cos_gamma);
    return R;
}
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    vec3 cubeTC = GetCubeMapTexCoord();
	vec3 dir = normalize(cubeTC);

    vec3 color = hosek_wilkie_sky_rgb(dir,u_Uniforms.Direction.xyz);
    imageStore(o_CubeMap, ivec3(gl_GlobalInvocationID), vec4(color, 1.0));
}