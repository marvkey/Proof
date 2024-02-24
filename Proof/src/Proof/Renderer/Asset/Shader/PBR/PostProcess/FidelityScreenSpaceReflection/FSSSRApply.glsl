
#version 450
#Vertex Shader
#version 450 core
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/framework/cauldron/framework/inc/shaders/fullscreen.hlsl

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


layout(location = 0) out vec2 o_TexCoord;
void main()
{
    o_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}

#Fragment Shader
#version 450 core
layout(location = 0) in vec2 vs_TexCoord;
layout(location = 0) out vec4 o_FragColor;

#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>

layout(binding = 0) uniform sampler2D u_ReflectionTarget;
layout(binding = 1) uniform sampler2D u_BaseColorAlphaTexture;
layout(binding = 2) uniform sampler2D u_BrdfTexture;

// Important bits from the PBR shader
vec3 GetIBLContribution(float perceptualRoughness, vec3 f0, vec3 specularLight, vec3 n, vec3 v)
{
    float NdotV = clamp(dot(n, v), 0.0, 1.0);
    vec2 brdfSamplePoint = clamp(vec2(NdotV, 1.0 - perceptualRoughness), 0.0, 1.0);
    vec2 brdf = texture(u_BrdfTexture, brdfSamplePoint).rg;

    return specularLight * (f0 * brdf.x + brdf.y);
}

void main() 
{
    int showReflectionTarget  =0;
    int drawReflections =1;
    vec3 radiance = texture(u_ReflectionTarget, vs_TexCoord).xyz;
    vec4 aoRoughnessMetallic = LoadRoughnesMaterialTexture(ivec2(vs_TexCoord));
    float perceptualRoughness = sqrt(LoadRoughnessFromMaterialParametersInput(vs_TexCoord)); // aoRoughnessMetallic.g contains alphaRoughness
    vec3 baseColor = texture(u_BaseColorAlphaTexture, vs_TexCoord).rgb;
    vec3 f0 = mix(vec3(0.04, 0.04, 0.04), baseColor, vec3(aoRoughnessMetallic.b));
    //vec3 normal = 2.0 * texture(u_NormalMap, vs_TexCoord, 0.0f).xyz - 1.0;
    vec3 normal = LoadWorldSpaceNormal(ivec2(vs_TexCoord));
    vec3 view = u_Camera.ViewDirection;

    if (showReflectionTarget == 1) 
    {
        // Show just the reflection view
        o_FragColor = vec4(radiance, 0);
    } else if (drawReflections == 1) 
    {
        float reflectionsIntensity =1;
        // reflectionsIntensity should always be 1 for PBR correctness. We expose it in the UI to help visualize reflections better.
        radiance = GetIBLContribution(perceptualRoughness, f0, radiance, normal, view) * reflectionsIntensity;
        o_FragColor = vec4(radiance, 1); // Show the reflections applied to the scene
    } else  
    {
        // Show just the scene
        o_FragColor = vec4(0, 0, 0, 1);
    }
}