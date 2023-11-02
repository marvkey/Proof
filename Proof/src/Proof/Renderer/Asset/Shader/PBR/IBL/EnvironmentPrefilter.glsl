#Compute Shader
//https://developer.nvidia.com/gpugems/gpugems3/part-iii-rendering/chapter-20-gpu-based-importance-sampling
//http://alinloghin.com/articles/compute_ibl.html
//https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
//https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
#version 450

//https://github.com/Nadrin/PBR/blob/master/data/shaders/glsl/spmap_cs.glsl
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;


layout(set = 0, binding = 0) uniform samplerCube u_EnvironmentMap;

const int NumMipLevels = 11-1;
// thera are more than 1 sent to the shader but due to vulkan not lettign set an array wth just [], 
//we set it to one but we set it to be 11-1 because the image (1024,1024) has 11 mips
// but we are only going ot write to mip 1-10 not writing not to mip 0
//but we going to set it to be 32 in the shader in case in the future we change the iamge size to be bigger t
layout(set = 0, binding = 1, rgba16f) uniform imageCube u_PrefilterMap[NumMipLevels];

layout(push_constant) uniform CubeMapData
{
    int Level;
    float Roughness;
}Input;
#define PARAM_LEVEL     Input.Level
#define PARAM_ROUGHNESS Input.Roughness
const float PI = 3.1415926535897932384626433832795;
const float TWO_PI = 2* PI;
const float NumSamples = 1024; 
const float InvNumSamples = 1.0/float(NumSamples);
const float Epsilon = 0.00001;
float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 SampleHammersley(uint index, float inverseTotalSamples)
{
    return vec2(index * inverseTotalSamples, RadicalInverse_VdC(index));
}

vec3 SampleGGX(float u1, float u2, float roughness)
{
    float alpha = roughness * roughness;

    // GGX distribution sampling
    float cosTheta = sqrt((1.0 - u2) / (1.0 + (alpha * alpha - 1.0) * u2));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    
    float phi = TWO_PI * u1;
    return vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float NdfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}
vec3 GetCubeMapTexCoord()
{
    vec2 st = gl_GlobalInvocationID.xy/ vec2(imageSize(u_PrefilterMap[PARAM_LEVEL]));
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

// Function to compute tangent (S) and bitangent (T) vectors based on the input normal (N).
void ComputeBasisVectors(const vec3 normal, out vec3 tangent, out vec3 bitangent)
{
    // Step 1: Compute the initial tangent vector (T) by crossing the input normal (N) with the up vector (0, 1, 0).
    bitangent= cross(normal, vec3(0.0, 1.0, 0.0));
    
    // Step 2: If the initial tangent vector (T) is close to zero (nearly perpendicular to the normal),
    // replace it with the cross product of the normal (N) and the right vector (1, 0, 0).
    bitangent = mix(cross(normal, vec3(1.0, 0.0, 0.0)), bitangent, step(Epsilon, dot(bitangent, bitangent)));

    // Step 3: Normalize the tangent (S) and bitangent (T) vectors to ensure they are unit length.
    bitangent = normalize(bitangent);
    tangent = normalize(cross(normal, bitangent));
}

vec3 TangentToWorld(const vec3 tangentSpaceVector, const vec3 worldNormal, const vec3 worldTangent, const vec3 worldBitangent)
{
    // Transform the vector from tangent space to world space using the provided basis vectors.
    return worldTangent * tangentSpaceVector.x + worldBitangent * tangentSpaceVector.y + worldNormal * tangentSpaceVector.z;
}

void main()
{
     ivec2 outputSize = imageSize(u_PrefilterMap[PARAM_LEVEL]);
    //dont write past the texture 
    if(gl_GlobalInvocationID.x >= outputSize.x || gl_GlobalInvocationID.y >= outputSize.y)
    {
        return;
    }

    vec2 inputSize = vec2(textureSize(u_EnvironmentMap,0));
    float wt  = 4.0 * PI/(6 * inputSize.x * inputSize.y);

    vec3 N = GetCubeMapTexCoord();
    vec3 lo = N;

    vec3 S,T;//(tangent, bitangent)
    ComputeBasisVectors(N,S,T);

    vec3 prefillterColor = vec3(0);
    float weight = 0;

    for(uint i =0; i<NumSamples; i++)
    {
        vec2 u = SampleHammersley(i,InvNumSamples);
        vec3 lh = TangentToWorld(SampleGGX(u.x,u.y,PARAM_ROUGHNESS),N,S,T);

        vec3 li = 2.0 *dot(lo,lh) * lh-lo;
        float NdotL = dot(N, li);
        if(NdotL> 0.0)
        {
            //usign filtered importance sampling to get better convergence
            //https://developer.nvidia.com/gpugems/gpugems3/part-iii-rendering/chapter-20-gpu-based-importance-sampling (section 20.4)
            float cosLh = max(dot(N,lh),0.0);

            float pdf = NdfGGX(cosLh, PARAM_ROUGHNESS) * 0.25;

            // solid angle 
            float ws = 1.0 / (NumSamples * pdf);
            
            // getting mip level
            float mipLevel = max(0.5 * log2(ws/wt)+1.0,0.0);

            prefillterColor += textureLod(u_EnvironmentMap,li,mipLevel).rgb * NdotL;
            weight +=NdotL;
        
        }
    }
    prefillterColor /=weight;
    imageStore(u_PrefilterMap[PARAM_LEVEL], ivec3(gl_GlobalInvocationID), vec4(prefillterColor, 1.0));
}