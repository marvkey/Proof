//https://github.com/shff/opengl_sky/blob/master/main.c
#Compute Shader
#version 450 core

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_CubeMap;

layout(push_constant) uniform Uniforms
{
    vec4 SunDirection_Time; // xyz = sun direction, w = time
    vec2 CloudParams;       // x = cirrus, y = cumulus
} u_Uniforms;

const float Br = 0.0005;
const float Bm = 0.0003;
const float g  = 0.9800;
const vec3 nitrogen = vec3(0.650, 0.570, 0.475);
const vec3 Kr = Br / pow(nitrogen, vec3(4.0));
const vec3 Km = Bm / pow(nitrogen, vec3(0.84));

vec3 GetCubeMapTexCoord()
{
    ivec2 cubeSize = imageSize(o_CubeMap);
    vec2 st = (vec2(gl_GlobalInvocationID.xy) + 0.5) / vec2(cubeSize);
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

float hash(float n)
{
    return fract(sin(n) * 43758.5453123);
}

float noise(vec3 x)
{
    vec3 f = fract(x);
    float n = dot(floor(x), vec3(1.0, 157.0, 113.0));
    return mix(mix(mix(hash(n +   0.0), hash(n +   1.0), f.x),
    mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
    mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
    mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
}

const mat3 m = mat3(0.0, 1.60,  1.20, -1.6, 0.72, -0.96, -1.2, -0.96, 1.28);
float fbm(vec3 p)
{
    float f = 0.0;
    f += noise(p) / 2.0;  p = m * p * 1.1;
    f += noise(p) / 4.0;  p = m * p * 1.2;
    f += noise(p) / 6.0;  p = m * p * 1.3;
    f += noise(p) / 12.0; p = m * p * 1.4;
    f += noise(p) / 24.0;
    return f;
}

// direct port of the original fragment shader's main(), pos -> dir, fsun -> sunDir
vec3 proceduralSky(vec3 dir, vec3 sunDir, float time, float cirrus, float cumulus)
{
    // original: discard below horizon. compute has to fill the whole
    // sphere, so clamp the atmosphere math instead of discarding, and
    // fade to a ground tone at the very end.
    float y = max(dir.y, 0.001);

    float mu = dot(dir, sunDir);
    float rayleigh = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu);
    vec3 mie = (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm);

    vec3 day_extinction = exp(-exp(-((y + sunDir.y * 4.0) * (exp(-y * 16.0) + 0.1) / 80.0) / Br) * (exp(-y * 16.0) + 0.1) * Kr / Br)
    * exp(-y * exp(-y * 8.0) * 4.0) * exp(-y * 2.0) * 4.0;
    vec3 night_extinction = vec3(1.0 - exp(sunDir.y)) * 0.2;
    vec3 extinction = mix(day_extinction, night_extinction, -sunDir.y * 0.2 + 0.5);

    vec3 col = rayleigh * mie * extinction;

    float yClamped = max(dir.y, 0.0);

    // Cirrus
    float density = smoothstep(1.0 - cirrus, 1.0, fbm(dir.xyz / y * 2.0 + time * 0.05)) * 0.3;
    col = mix(col, extinction * 4.0, density * yClamped);

    // Cumulus
    for (int i = 0; i < 3; i++)
    {
        float d = smoothstep(1.0 - cumulus, 1.0, fbm((0.7 + float(i) * 0.01) * dir.xyz / y + time * 0.3));
        col = mix(col, extinction * d * 5.0, min(d, 1.0) * yClamped);
    }

    col += noise(dir * 1000.0) * 0.01;

    vec3 groundColor = vec3(0.02, 0.02, 0.025);
    float horizonFade = 1.0 - smoothstep(-0.15, 0.0, dir.y);
    col = mix(col, groundColor, horizonFade);

    return max(col, vec3(0.0));
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    ivec2 cubeSize = imageSize(o_CubeMap);
    if (gl_GlobalInvocationID.x >= uint(cubeSize.x) || gl_GlobalInvocationID.y >= uint(cubeSize.y))
    return;

    vec3 dir = GetCubeMapTexCoord();
    vec3 sunDir = normalize(u_Uniforms.SunDirection_Time.xyz);
    float time = u_Uniforms.SunDirection_Time.w;

    vec3 col = proceduralSky(dir, sunDir, time, u_Uniforms.CloudParams.x, u_Uniforms.CloudParams.y);
    col = pow(1.0 - exp(-1.3 * col), vec3(1.3));
    imageStore(o_CubeMap, ivec3(gl_GlobalInvocationID), vec4(col, 1.0));
}