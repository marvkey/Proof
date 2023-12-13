//https://github.com/diharaw/sky-models/blob/c74ce88ccec91aeb9502fb24a7e51e0fb3bfc51a/src/shader/sky_fs.glsl
//https://github.com/diharaw/sky-models/blob/c74ce88ccec91aeb9502fb24a7e51e0fb3bfc51a/src/shader/sky_models/preetham/atmosphere.glsl
//https://github.com/diharaw/sky-models/blob/c74ce88ccec91aeb9502fb24a7e51e0fb3bfc51a/src/preetham_sky_model.cpp
#Compute Shader
#version 450 core

const float PI = 3.141592;

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_CubeMap;

const float normalizeSunY = 1.15f;

layout (push_constant) uniform Uniforms
{
    vec4 Params;
} u_Uniforms;

vec3 Direction = u_Uniforms.Params.xyz;
float Turbidity = u_Uniforms.Params.w;

vec3 p_A, p_B, p_C, p_D, p_E, p_Z;

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

float zenith_chromacity(vec4 c0, vec4 c1, vec4 c2, float sunTheta, float turbidity)
{
    vec4 thetav = vec4(sunTheta * sunTheta * sunTheta, sunTheta * sunTheta, sunTheta, 1.0);
    return dot(vec3(turbidity * turbidity, turbidity, 1.0), vec3(dot(thetav, c0), dot(thetav, c1), dot(thetav, c2)));
}

float zenith_luminance(float sunTheta, float turbidity)
{
    float chi = (4.0 / 9.0 - turbidity / 120.0) * (PI - 2.0 * sunTheta);
    return (4.0453 * turbidity - 4.9710) * tan(chi) - 0.2155 * turbidity + 2.4192;
}

float perez(float theta, float gamma, float A, float B, float C, float D, float E)
{
    return (1.0 + A * exp(B / (cos(theta) + 0.01))) * (1.0 + C * exp(D * gamma) + E * cos(gamma) * cos(gamma));
}

void calculatePerezDistribution(in float turbidity, out vec3 A, out vec3 B, out vec3 C, out vec3 D, out vec3 E, out vec3 Z)
{
    // Constants
    const float sunTheta = acos(clamp(Direction.y, 0.0, 1.0));

    // A.2 Skylight Distribution Coefficients and Zenith Values: compute Perez distribution coefficients
    A = vec3(-0.0193, -0.0167, 0.1787) * turbidity + vec3(-0.2592, -0.2608, -1.4630);
    B = vec3(-0.0665, -0.0950, -0.3554) * turbidity + vec3(0.0008, 0.0092, 0.4275);
    C = vec3(-0.0004, -0.0079, -0.0227) * turbidity + vec3(0.2125, 0.2102, 5.3251);
    D = vec3(-0.0641, -0.0441, 0.1206) * turbidity + vec3(-0.8989, -1.6537, -2.5771);
    E = vec3(-0.0033, -0.0109, -0.0670) * turbidity + vec3(0.0452, 0.0529, 0.3703);

    // A.2 Skylight Distribution Coefficients and Zenith Values: compute zenith color
    Z.x = zenith_chromacity(vec4(0.00166, -0.00375, 0.00209, 0.0), vec4(-0.02903, 0.06377, -0.03202, 0.00394), vec4(0.11693, -0.21196, 0.06052, 0.25886), sunTheta, turbidity);
    Z.y = zenith_chromacity(vec4(0.00275, -0.00610, 0.00317, 0.0), vec4(-0.04214, 0.08970, -0.04153, 0.00516), vec4(0.15346, -0.26756, 0.06670, 0.26688), sunTheta, turbidity);
    Z.z = zenith_luminance(sunTheta, turbidity);
    Z.z *= 1000.0; // conversion from kcd/m^2 to cd/m^2

    // 3.2 Skylight Model: pre-divide zenith color by distribution denominator
    Z.x /= perez(0.0, sunTheta, A.x, B.x, C.x, D.x, E.x);
    Z.y /= perez(0.0, sunTheta, A.y, B.y, C.y, D.y, E.y);
    Z.z /= perez(0.0, sunTheta, A.z, B.z, C.z, D.z, E.z);

    // For low dynamic range simulation, normalize luminance to have a fixed value for sun
    if (normalizeSunY > 0.0) Z.z = normalizeSunY / perez(sunTheta, 0.0, A.z, B.z, C.z, D.z, E.z);
}

vec3 perez(float cos_theta, float gamma, float cos_gamma, vec3 A, vec3 B, vec3 C, vec3 D, vec3 E)
{
    return (1.0 + A * exp(B / (cos_theta + 0.01))) * (1.0 + C * exp(D * gamma) + E * cos_gamma * cos_gamma);
}

vec3 preetham_sky_rgb(vec3 v, vec3 sun_dir)
{
    float cos_theta = clamp(v.y, 0, 1);
    float cos_gamma = dot(v, sun_dir);
    float gamma = acos(cos_gamma);
    
    vec3 R_xyY = p_Z * perez(cos_theta, gamma, cos_gamma, p_A, p_B, p_C, p_D, p_E);
    
    vec3 R_XYZ = vec3(R_xyY.x, R_xyY.y, 1 - R_xyY.x - R_xyY.y) * R_xyY.z / R_xyY.y;
    
    // Radiance
    float r = dot(vec3( 3.240479, -1.537150, -0.498535), R_XYZ);
    float g = dot(vec3(-0.969256,  1.875992,  0.041556), R_XYZ);
    float b = dot(vec3( 0.055648, -0.204043,  1.057311), R_XYZ);

    return vec3(r, g, b);
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    vec3 cubeTC = GetCubeMapTexCoord();
    calculatePerezDistribution(Turbidity, p_A, p_B, p_C, p_D, p_E, p_Z);

    vec3 col = preetham_sky_rgb(cubeTC, Direction);
    imageStore(o_CubeMap, ivec3(gl_GlobalInvocationID), vec4(col, 1.0));
}
