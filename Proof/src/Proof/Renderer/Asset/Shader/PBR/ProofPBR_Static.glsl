#Vertex Shader
#version 450

#include <Common.glslh>

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
// Transform buffer
//layout(location = 5) in vec4 a_MRow0;
//layout(location = 6) in vec4 a_MRow1;
//layout(location = 7) in vec4 a_MRow2;
struct VertexOutput
{
   vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;

    mat3 CameraView; 

    vec3 ShadowMapCoords[4];
    vec4 CascadeSplits;

    vec3 ViewPosition;
};
layout(location = 0) out VertexOutput Output;


layout(std140, set = 1, binding = 0) uniform ShadowMapProjections {

    mat4 ViewProjections[4];
} u_CascadePositions;
layout(std140, set = 1, binding = 1) uniform CascadeSplits {
    
    // each input is a cascade splti
    vec4 CascadSplit;
} u_CascadeSplits;
//https://computergraphics.stackexchange.com/questions/8979/strange-dark-spot-when-lighting-in-deferred-rendering-pbr-shaders
//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/739ff016ad2a23e3843517c4866dda09ce5d112f/Resources/Shaders/PBR_Static.glsl
void main() {
	//mat4 aTransform = mat4(
	//	vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
	//	vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
	//	vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
	//	vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	//);

    Output.Tangent = aTangent;
    Output.Bitangent = aBitangent;
    Output.TexCoords = aTexCoords;
    //Output.TexCoords = vec2(aTexCoords.x,1 - aTexCoords.y);
    vec4 worldPos = aTransform * vec4(aPosition, 1.0);
    Output.WorldPosition = worldPos.xyz;

    vec4 shadowCoords[4];
    shadowCoords[0] = u_CascadePositions.ViewProjections[0] * vec4(Output.WorldPosition, 1.0);
	shadowCoords[1] = u_CascadePositions.ViewProjections[1] * vec4(Output.WorldPosition, 1.0);
	shadowCoords[2] = u_CascadePositions.ViewProjections[2] * vec4(Output.WorldPosition, 1.0);
	shadowCoords[3] = u_CascadePositions.ViewProjections[3] * vec4(Output.WorldPosition, 1.0);

    Output.ShadowMapCoords[0] = vec3(shadowCoords[0].xyz / shadowCoords[0].w);
	Output.ShadowMapCoords[1] = vec3(shadowCoords[1].xyz / shadowCoords[1].w);
	Output.ShadowMapCoords[2] = vec3(shadowCoords[2].xyz / shadowCoords[2].w);
	Output.ShadowMapCoords[3] = vec3(shadowCoords[3].xyz / shadowCoords[3].w);

    Output.CascadeSplits[0] =  u_CascadeSplits.CascadSplit[0];
    Output.CascadeSplits[1] =  u_CascadeSplits.CascadSplit[1];
    Output.CascadeSplits[2] =  u_CascadeSplits.CascadSplit[2];
    Output.CascadeSplits[3] =  u_CascadeSplits.CascadSplit[3];

    mat3 normalMatrix = transpose(inverse(mat3(aTransform)));
    Output.Normal = normalMatrix * aNormal;
    Output.CameraPosition = u_Camera.Position;

    Output.ViewPosition = vec3(u_Camera.ViewMatrix * vec4(Output.WorldPosition, 1.0));
    Output.CameraView = mat3(u_Camera.ViewMatrix);

    gl_Position = u_Camera.ProjectionMatrix * u_Camera.ViewMatrix * vec4(Output.WorldPosition, 1.0);
}

#Fragment Shader
#version 450 core
#include <PBR/PBR.glslh>
#include <PBR/Shadow/ShadowMapping.glslh>

//https://github.com/Nadrin/PBR/blob/master/data/shaders/glsl/pbr_fs.glsl
//https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.2.2.ibl_specular_textured/2.2.2.pbr.fs
//https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh
//https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
//https://github.com/HighLo-Engine/HighLo-Engine/blob/b9bf9c3ed87e63ba358ff50ec180927a3801829c/HighLo/assets/shaders/HighLoPBR.glsl


//ouputs
layout(location = 0) out vec4 out_FragColor;

// pbr map
layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

//environmentMap
layout(set = 1, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 1, binding = 3) uniform samplerCube u_PrefilterMap;
layout(set = 1, binding = 4) uniform sampler2D u_BRDFLUT;

//shadow map
layout(set = 1, binding = 5) uniform sampler2DArray u_ShadowMap;


struct VertexOutput
{
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;

    mat3 CameraView; 

    vec3 ShadowMapCoords[4];
    vec4 CascadeSplits;

    vec3 ViewPosition;
};
layout(location = 0) in VertexOutput Input;

layout(push_constant) uniform Material
{
    vec3 Albedo;
    float Metalness;
    float Roughness;
    bool AlbedoTexToggle;
    bool NormalTexToggle;
    bool RoghnessTexToggle;
    bool MetallnesTexToggle;
    vec2 Tiling;
    vec2 Offset;
    //int padding[2]; // Padding to ensure proper alignment
}u_MaterialUniform;

vec3 CalculateNormal()
{
    if (u_MaterialUniform.NormalTexToggle == false)
        return normalize(Input.Normal);
    vec3 tangentNormal = texture(u_NormalMap, Input.TexCoords).xyz*2.0 -1.0f;
    vec3 Q1 = dFdx(Input.WorldPosition);
    vec3 Q2 = dFdy(Input.WorldPosition);
    vec2 st1 = dFdx(Input.TexCoords);
    vec2 st2 = dFdy(Input.TexCoords);

    vec3 N = normalize(Input.Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// Constant normal incidence Fresnel factor for all dielectrics.
const vec3  Fidelectric = vec3(0.04);
void main()
{
    m_Params.AlbedoColor = u_MaterialUniform.AlbedoTexToggle == true ? texture(u_AlbedoMap, Input.TexCoords).rgb * u_MaterialUniform.Albedo : u_MaterialUniform.Albedo;
    m_Params.Metalness = u_MaterialUniform.MetallnesTexToggle == true ? texture(u_MetallicMap, Input.TexCoords).r * u_MaterialUniform.Metalness : u_MaterialUniform.Metalness;
    m_Params.Roughness = u_MaterialUniform.RoghnessTexToggle == true ? texture(u_RoughnessMap, Input.TexCoords).r * max(u_MaterialUniform.Roughness,0.00) : max(u_MaterialUniform.Roughness,0.00); // max to keep specular
    m_Params.Normal = CalculateNormal();

    m_Params.View = normalize(Input.CameraPosition - Input.WorldPosition);
    vec3 reflection = reflect(-m_Params.View , m_Params.Normal);
    const vec3 F0 = mix(Fidelectric, m_Params.AlbedoColor , m_Params.Metalness);
    
    // Angle between surface normal and outgoing view Direction.
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View),0.0);

    vec3 directLighting = vec3(0);
    for (int i = 0; i < 1; i++)
    {
        //https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
        DirectionalLight currentLight= u_DirectionalLightData.Lights[i];
        vec3 lightDirection = -normalize(currentLight.Direction);
        vec3 halfway = normalize(m_Params.View  + lightDirection);
        float nDotL = max(dot(m_Params.Normal, lightDirection), 0.0);
        vec3 radiance = currentLight.Color * currentLight.Intensity;
        
        float NDF = DistributionGGX(m_Params.Normal, halfway, m_Params.Roughness);
        float G   = GeometrySmith(m_Params.NdotV, nDotL, m_Params.Roughness);
        vec3  F   = FresnelSchlick(max(dot(halfway,m_Params.View ), 0.0), F0);

        vec3 kD = 1.0 - F;
        kD *= 1.0 - m_Params.Metalness;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * m_Params.NdotV * nDotL;
        vec3 specular = numerator / max (denominator, 0.0001);

        vec3 lightEffect = (kD * m_Params.AlbedoColor / PI + specular )  * radiance * nDotL;
        directLighting +=lightEffect;
    }
    
    vec3 iblEfeect = vec3(0);
    //IBL
    {
        vec3 Lr = 2.0 *dot(m_Params.View ,m_Params.Normal) * m_Params.Normal - m_Params.View ;
        //https://github.com/kidrigger/Blaze/blob/canon/Blaze/shaders/forward/fPBR.frag
        //https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh

        // fresnel reflectance
        vec3 F= FresnelSchlickRoughness(m_Params.NdotV, F0, m_Params.Roughness);
        vec3 irradiance = texture(u_IrradianceMap, m_Params.Normal).rgb ;
        vec3 kd = (1 - F) * (1.0 - m_Params.Metalness);

        vec3 diffuseIBL = kd * m_Params.AlbedoColor * irradiance;

        int specularTextureLevels = textureQueryLevels(u_PrefilterMap);
		vec3 specularIrradiance = textureLod(u_PrefilterMap, Lr,specularTextureLevels * m_Params.Roughness).rgb;

        vec2 specularBRDF  = texture(u_BRDFLUT, vec2(m_Params.NdotV,m_Params.Roughness)).rg;

        //TODO Fix why brdf is causing shader bug
        //of a grey dot
		vec3 specularIBL = specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y) ; 
		//vec3 specularIBL = specularIrradiance * (F0) ; 

        iblEfeect += (specularIBL + diffuseIBL) ;
        //iblEfeect = iblEfeect * (u_SkyBoxInfo.Intensity);  
    }
    const int SHADOW_MAP_CASCADE_COUNT = 4;
    uint cascadeIndex = 0;
	for (uint i = 0; i < SHADOW_MAP_CASCADE_COUNT - 1; i++)
	{
		if (Input.ViewPosition.z < Input.CascadeSplits[i])
			cascadeIndex = i + 1;
	}

	float shadowDistance = 4500; // controll
	float transitionDistance = 50.0f; // expos
	float distance = length(Input.ViewPosition);
	float shadowFade = distance - (shadowDistance - transitionDistance);
	shadowFade /= transitionDistance;
	shadowFade = clamp(1.0 - shadowFade, 0.0, 1.0);

	float shadowScale;
    
    vec3 shadowMapCoords = Input.ShadowMapCoords[cascadeIndex];

   shadowScale  = HardShadows_DirectionalLight(u_ShadowMap, 1, cascadeIndex, shadowMapCoords);
   shadowScale = 1.0 - clamp(0.5 - shadowScale, 0.0f, 1.0f);

    vec3 finalColor =  iblEfeect + (directLighting * shadowScale);
    
    //finalColor = vec3(1) * shadowScale * 0.2f;
    // HDR tonemapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    // gamma correct
    finalColor = pow(finalColor, vec3(1.0/2.2)); 
    out_FragColor = vec4(finalColor ,1.0);
    //switch(cascadeIndex) {
	//		case 0 : 
	//			out_FragColor.rgb *= vec3(1.0f, 0.25f, 0.25f);
	//			break;
	//		case 1 : 
	//			out_FragColor.rgb *= vec3(0.25f, 1.0f, 0.25f);
	//			break;
	//		case 2 : 
	//			out_FragColor.rgb *= vec3(0.25f, 0.25f, 1.0f);
	//			break;
	//		case 3 : 
	//			out_FragColor.rgb *= vec3(1.0f, 1.0f, 0.25f);
	//			break;
	//	}
}   
