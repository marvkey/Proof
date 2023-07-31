
#Compute Shader
#version 450 core
layout(local_size_x=32, local_size_y =32, local_size_z=1) in;
layout(set=0, binding=0, rgba16f) uniform  imageCube inputTexture;
layout(set=0, binding=1, rgba16f) restrict writeonly uniform imageCube outputTexture;

const float PI = 3.1415926535897932384626433832795;
const float TWO_PI = 2* PI;
const float HALF_PI = 0.5* PI;

const float sampleDelta = 0.025;
const float totalSamples = (TWO_PI / sampleDelta) * (HALF_PI / sampleDelta); 
const float invTotalSamples = 1.0 / totalSamples;

vec3 GetSamplingVector()
{
    vec2 st = gl_GlobalInvocationID.xy/vec2(imageSize(outputTexture));
    vec2 uv = 2.0 * vec2(st.x, 1.0-st.y) - vec2(1.0);

    vec3 ret;
    if(gl_GlobalInvocationID.z == 0)      ret = vec3(1.0,  uv.y, -uv.x);
    else if(gl_GlobalInvocationID.z == 1) ret = vec3(-1.0, uv.y,  uv.x);
    else if(gl_GlobalInvocationID.z == 2) ret = vec3(uv.x, 1.0, -uv.y);
    else if(gl_GlobalInvocationID.z == 3) ret = vec3(uv.x, -1.0, uv.y);
    else if(gl_GlobalInvocationID.z == 4) ret = vec3(uv.x, uv.y, 1.0);
    else if(gl_GlobalInvocationID.z == 5) ret = vec3(-uv.x, uv.y, -1.0);
    return normalize(ret);
}
float max3(vec3 v) 
{
  return max(max(v.x, v.y), v.z);
}
ivec3 texCoordToCube(vec3 texCoord, vec2 cubemapSize)
{
    vec3 abst = abs(texCoord);
    texCoord /= max3(abst);

    float cubeFace;
    vec2 uvCoord;
    if (abst.x > abst.y && abst.x > abst.z) 
    {
        // x major
        float negx = step(texCoord.x, 0.0);
        uvCoord = mix(-texCoord.zy, vec2(texCoord.z, -texCoord.y), negx);
        cubeFace = negx;
    } 
    else if (abst.y > abst.z) 
    {
        // y major
        float negy = step(texCoord.y, 0.0);
        uvCoord = mix(texCoord.xz, vec2(texCoord.x, -texCoord.z), negy);
        cubeFace = 2.0 + negy;
    } 
    else 
    {
        // z major
        float negz = step(texCoord.z, 0.0);
        uvCoord = mix(vec2(texCoord.x, -texCoord.y), -texCoord.xy, negz);
        cubeFace = 4.0 + negz;
    }

    uvCoord = (uvCoord + 1.0) * 0.5; // 0..1
    uvCoord = uvCoord * cubemapSize;
    uvCoord = clamp(uvCoord, vec2(0.0), cubemapSize - vec2(1.0));
    return ivec3(ivec2(uvCoord), int(cubeFace));
}
void main()
{
    const vec2 cubemapSize = vec2(512.0, 512.0);
	ivec2 outputSize = imageSize(outputTexture);
    //dont write past the texture 
    if(gl_GlobalInvocationID.x >= outputSize.x || gl_GlobalInvocationID.y >= outputSize.y)
    {
        return;
    }

    vec3 normal = normalize(GetSamplingVector());


    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up = cross(normal, right);

    vec3 irradiance = vec3(0.0);
    for(float phi = 0.0; phi < TWO_PI; phi += sampleDelta)
    {
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);
        for(float theta = 0.0; theta < HALF_PI; theta += sampleDelta)
        {
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            // spherical to cartesian, in tangent space
            vec3 sphereCoord = vec3(sinTheta * cosPhi,  sinTheta * sinPhi, cosTheta);
            vec3 sampleVec = sphereCoord.x * right + sphereCoord.y * up + sphereCoord.z * normal; 
            // world to cube coord
            ivec3 sampleCoord = texCoordToCube(sampleVec, cubemapSize);
            irradiance += imageLoad(inputTexture, sampleCoord).rgb * cosTheta * sinTheta;
        }
    }
    irradiance *= PI * invTotalSamples;

     imageStore(outputTexture, ivec3(gl_GlobalInvocationID), vec4(irradiance, 1.0));
}