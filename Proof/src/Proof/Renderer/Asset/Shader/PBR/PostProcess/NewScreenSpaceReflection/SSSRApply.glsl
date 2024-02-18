
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/samples/sssr/shaders/sssr_apply_reflections.hlsl
//https://github.com/qiutang98/flower/blob/743891b28090008972bc7c7fbc49e6f839557c8c/source/shader/sssr/sssr_apply.glsl
#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>

layout( binding = 0) uniform texture2D u_BRDFLUT;
layout( binding = 1,rgba32f) uniform image2D o_OutputGeometryImage;
layout( binding = 2) uniform texture2D u_MetalnessRoughnessMap;
layout( binding = 3) uniform texture2D u_WorldTexture;
layout( binding = 4) uniform texture2D u_SSSRIntersectionMap;


// Important bits from the PBR shader
vec3 ComputeIBLContribution(float perceptualRoughness, vec3 specularColor, vec3 specularLight,  vec3 n, vec3 v)
{
    float NdotV = clamp(dot(n, v), 0.0, 1.0);

    // NOTE:
    // alphaRoughness = perceptualRoughness * perceptualRoughness
    // Use perceptualRoughness to lut and prefilter search to get better view.

    // Load precompute brdf texture value.
    vec2 brdfSamplePoint = clamp(vec2(NdotV, perceptualRoughness), vec2(0.0, 0.0), vec2(1.0, 1.0));

    // retrieve a scale and bias to F0. See [1], Figure 3
    vec2 brdf = texture(sampler2D(u_BRDFLUT, u_LinearClampEdgeSampler), brdfSamplePoint).rg;

    // Add env ibl specular light, also scale ssr radiance.
    specularLight = max(specularLight, vec3(0.0));

    vec3 specular = specularLight * (specularColor * brdf.x + brdf.y); 

    return specular;
}
vec3 UnpackWorldNormal(vec3 pack)
{
	return normalize(pack * 2.0 - vec3(1.0));
}
layout (local_size_x = 8, local_size_y = 8) in;
void main()
{
    uvec2 groupThreadId = Remap8x8(gl_LocalInvocationIndex);
    uvec2 dispatchId = groupThreadId + gl_WorkGroupID.xy * 8;
    ivec2 workPos = ivec2(dispatchId);

    ivec2 workSize = imageSize(o_OutputGeometryImage);
    if(workPos.x >= workSize.x || workPos.y >= workSize.y)
    {
        return;
    }

    const vec2 uv = (vec2(workPos) + vec2(0.5f)) / vec2(workSize);
    float deviceZ = texelFetch(u_DepthMap, workPos, 0).r;

    if(deviceZ <= 0.0f)
    {
        return;
    }
    const vec4 inGbufferSValue = texelFetch(u_MetalnessRoughnessMap, workPos, 0);
    const vec4 inGbufferAValue = texelFetch(u_WorldTexture, workPos, 0);

    float metallic = inGbufferSValue.r;
    const vec3 f0 = vec3(0.04);
    const vec3 baseColor = inGbufferAValue.rgb;
    vec3 specularColor = mix(f0, baseColor.rgb, metallic);

    float perceptualRoughness = texelFetch(u_MetalnessRoughnessMap, workPos, 0).g;
    vec3 n = UnpackWorldNormal(texelFetch(u_NormalMap, workPos, 0).xyz); 

    vec3 worldPos = GetWorldPos(uv, deviceZ);
    //vec3 worldPos = ScreenSpaceToWorldSpace(vec3(uv,deviceZ));
    vec3 v = normalize(u_Camera.Position - worldPos);

    //float ao = min(texture(sampler2D(inSSAO, u_LinearClampEdgeSampler), uv).r, inGbufferSValue.b);
    //vec3 multiBounceAO = AoMultiBounce(ao, baseColor);
    vec3 multiBounceAO = vec3(1.0);


    vec3 ssrResult = texelFetch(u_SSSRIntersectionMap, workPos, 0).xyz;
    ssrResult = ComputeIBLContribution(perceptualRoughness, specularColor, ssrResult, n, v) * multiBounceAO;

    vec4 resultColor;
    vec4 srcHdrSceneColor = imageLoad(o_OutputGeometryImage, workPos);

    resultColor.xyz = max(ssrResult, vec3(0.0)) + srcHdrSceneColor.xyz;
    resultColor.w = 1.0;

    imageStore(o_OutputGeometryImage, workPos, resultColor);
}