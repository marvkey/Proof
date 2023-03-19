#ifndef GLOBAL_INC
#define GLOBAL_INC
const float PI = 3.1415926535897932384626433832795f;

struct DirectionalLight {
    vec3 Color;
    float Intensity;
    vec3 Direction;
};
vec3 CalculateNormal(vec3 normal, vec3 tangent, vec3 bitangent) 
{
    mat3 TBN = mat3(tangent, bitangent, normal);
    return normal;
}
vec3 CalculateNormal(vec3 normal, vec3 tangent, vec3 bitangent, vec2 texCoord,sampler2D normalSampler)
{
    mat3 TBN = mat3(tangent, bitangent, normal);

    return normalize(
            TBN * (texture(normalSampler, texCoord).rgb * 2.0 - 1.0)
        );
}
#endif
