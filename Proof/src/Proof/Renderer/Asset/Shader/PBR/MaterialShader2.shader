//https://github.com/SaferGo/CroissantVulkanRenderer/blob/main/shaders/scene.frag // sourcw
//https://www.youtube.com/watch?v=RRE-F57fbXw soruce 
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh


#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable
#include "PBR/PBRFunctions.inc"

layout(location = 0) in vec3 in_WorldPos;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in vec3 in_Bitangent;
layout(location = 5) in vec3 in_CameraPosition;


layout(set = 0, binding = 4) uniform samplerCube u_IrradianceMap;
layout(set = 0, binding = 5) uniform samplerCube u_PrefilterMap;
layout(set = 0, binding = 6) uniform sampler2D u_BRDFLUT;


layout(location = 0) out vec4 out_FragColor;
layout(set = 1, binding = 0) uniform sampler2D u_AlbedoMap;
//layout(set = 1, binding = 1) uniform sampler2D u_NormalMap;
//layout(set = 1, binding = 2) uniform sampler2D u_MetallicMap;
//layout(set = 1, binding = 3) uniform sampler2D u_RoughnessMap;

layout(std140, set = 0, binding = 1) uniform DirectionalLightBuffer
{
   DirectionalLight Light;
}DirectionalLightData;


void main()
{
    vec3 normal = CalculateNormal(in_Normal, in_Tangent, in_Bitangent);
   // vec3 normal = in_Normal;
	vec3 view = normalize(in_CameraPosition - in_WorldPos);

    // should clamp rougness to like (0.01)
	float roughness = 0.5;
	float metallic = 0.1;

    vec3 meshColor = texture(u_AlbedoMap, in_TexCoord).xyz;
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, meshColor, metallic);
    
    vec3 outGoingLight = vec3(0);
    outGoingLight += CalulateDirectionalLight(DirectionalLightData.Light, view, normal, F0, meshColor,roughness, metallic);

    vec3 ambient = AmbientLighting(roughness, metallic,view,normal,F0,meshColor, u_IrradianceMap, u_PrefilterMap, u_BRDFLUT);
    vec3 finalColor = ambient + outGoingLight ;

    //finalColor = color / (color + vec3(1.0));
    //// gamma correct
    //finalColor = pow(color, vec3(1.0 / 2.2));normal

    out_FragColor = vec4(finalColor, 1.0);

}
