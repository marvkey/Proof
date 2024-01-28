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
    mat3 WorldNormals;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;

    mat3 CameraView; 
    vec3 ShadowMapCoords[4];

    vec3 ViewPosition;
};
layout(location = 0) out VertexOutput Output;


layout(std140, set = 1, binding = 0) uniform ShadowMapProjections {

    mat4 ViewProjections[4];
} u_CascadePositions;

//https://computergraphics.stackexchange.com/questions/8979/strange-dark-spot-when-lighting-in-deferred-rendering-pbr-shaders
//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/739ff016ad2a23e3843517c4866dda09ce5d112f/Resources/Shaders/PBR_Static.glsl
void main() {

    Output.Tangent = aTangent;
    Output.Bitangent = aBitangent;
    //Output.TexCoords = aTexCoords;
    Output.TexCoords = vec2(aTexCoords.x,1 - aTexCoords.y);
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

    mat3 normalMatrix = transpose(inverse(mat3(aTransform)));
    //Output.Normal = normalMatrix * aNormal;
    Output.Normal = mat3(aTransform) * aNormal;
    Output.WorldNormals = mat3(aTransform) * mat3(aTangent, aBitangent, aNormal);

    Output.CameraPosition = u_Camera.Position;

    Output.ViewPosition = vec3(u_Camera.View * vec4(Output.WorldPosition, 1.0));
    Output.CameraView = mat3(u_Camera.View);

    gl_Position =  u_Camera.Projection * u_Camera.View * vec4(Output.WorldPosition, 1.0);
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
// `out_ViewNormalsLuminance` is an output variable used to store view-space normals
// and potentially luminance information during rendering. It's a part of a more
// complex rendering pipeline, likely involving lighting calculations, screen-space
// effects, or post-processing. View-space normals are necessary for lighting, while
// luminance could represent brightness or intensity used for various purposes
// such as tone mapping or color grading. The exact role of this variable depends
// on the specific rendering pipeline and shader's purpose within it.
layout(location = 1) out vec4 out_ViewNormalsLuminance;
layout(location = 2) out vec4 out_MetalnessRoughness; //RGBA //R= metallnes, G = Roughness
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
    mat3 WorldNormals;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;

    mat3 CameraView; 

    vec3 ShadowMapCoords[4];

    vec3 ViewPosition;
};
layout(location = 0) in VertexOutput Input;

layout(push_constant) uniform Material
{
    vec3 Albedo;
    float Metalness;

    float Roughness;
    float Emission;
    bool EmissionOverrideColorToggle;
    bool NormalTexToggle;

    vec2 TextureTiling;
    vec2 TextureOffset;

    vec3 EmissionOverrideColor; // if EmissionOverrideColorToggle is equal to true then we will override the emission color
} u_MaterialUniform;

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

vec3 GetGradient(float value)
{
	vec3 zero = vec3(0.0, 0.0, 0.0);
	vec3 white = vec3(0.0, 0.1, 0.9);
	vec3 red = vec3(0.2, 0.9, 0.4);
	vec3 blue = vec3(0.8, 0.8, 0.3);
	vec3 green = vec3(0.9, 0.2, 0.3);

	float step0 = 0.0f;
	float step1 = 2.0f;
	float step2 = 4.0f;
	float step3 = 8.0f;
	float step4 = 16.0f;

    /*
    When value is in the range [0.0, 2.0), the color transitions from zero to white.
    When value is in the range [2.0, 4.0), the color transitions from white to red.
    When value is in the range [4.0, 8.0), the color transitions from red to blue.
    When value is in the range [8.0, 16.0), the color transitions from blue to green.
    */
	vec3 color = mix(zero, white, smoothstep(step0, step1, value));
	color = mix(color, white, smoothstep(step1, step2, value));
	color = mix(color, red, smoothstep(step1, step2, value));
	color = mix(color, blue, smoothstep(step2, step3, value));
	color = mix(color, green, smoothstep(step3, step4, value));

	return color;
}
// Constant normal incidence Fresnel factor for all dielectrics.
const vec3  Fidelectric = vec3(0.04);
void main()
{
    vec2 texCoords = Input.TexCoords * u_MaterialUniform.TextureTiling + u_MaterialUniform.TextureOffset;

    m_Params.AlbedoColor = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo;
    m_Params.Metalness = texture(u_MetallicMap, texCoords).r * u_MaterialUniform.Metalness ;
    m_Params.Roughness = texture(u_RoughnessMap, texCoords).r * max(u_MaterialUniform.Roughness,0.00); // max to keep specular
   // m_Params.Normal = CalculateNormal();

   	m_Params.Normal = normalize(Input.Normal);
	if (u_MaterialUniform.NormalTexToggle)
	{
		m_Params.Normal = normalize(texture(u_NormalMap, texCoords).rgb * 2.0f - 1.0f);
		m_Params.Normal = normalize(Input.WorldNormals * m_Params.Normal);
	}
    out_MetalnessRoughness = vec4(m_Params.Metalness, m_Params.Roughness, 0.f, 1.f);

    m_Params.View = normalize(Input.CameraPosition - Input.WorldPosition);
    vec3 reflection = reflect(-m_Params.View , m_Params.Normal);
    const vec3 F0 = mix(Fidelectric, m_Params.AlbedoColor , m_Params.Metalness);
    
    // Angle between surface normal and outgoing view Direction.
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View),0.0);

    // View normals
    out_ViewNormalsLuminance.xyz = Input.CameraView * normalize(Input.Normal);
    vec3 directLighting = vec3(0);
    for (int i = 0; i < 1; i++)
    {
        //https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
        DirectionalLight currentLight= u_DirectionalLightData.Lights[i];
        vec3 Li = -normalize(currentLight.Direction);
        vec3 Lradiance = currentLight.Color * currentLight.Intensity;
        vec3 Lh = normalize(Li + m_Params.View);

        // Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(m_Params.Normal, Li));
		float cosLh = max(0.0, dot(m_Params.Normal, Lh));

        vec3 F = FresnelSchlickRoughness(max(0.0, dot(Lh, m_Params.View)),F0,m_Params.Roughness);
		float D = NdfGGX(cosLh, m_Params.Roughness);
		float G = GaSchlickGGX(cosLi, m_Params.NdotV, m_Params.Roughness);

        vec3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
		vec3 diffuseBRDF = kd * m_Params.AlbedoColor;

        // Cook-Torrance
		vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * cosLi * m_Params.NdotV);
		specularBRDF = clamp(specularBRDF, vec3(0.0f), vec3(10.0f));
		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
        /*
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
        */
    }
    
    vec3 iblEfeect = vec3(0);
    //IBL
    {
        vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View ;
        //https://github.com/kidrigger/Blaze/blob/canon/Blaze/shaders/forward/fPBR.frag
        //https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh

        // fresnel reflectance
        vec3 F = FresnelSchlickRoughness(m_Params.NdotV, F0, m_Params.Roughness);
	    vec3 R = 2.0 * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;

        vec3 irradiance = texture(u_IrradianceMap, m_Params.Normal).rgb ;
        vec3 kd = (1 - F) * (1.0 - m_Params.Metalness);

        vec3 diffuseIBL = m_Params.AlbedoColor * irradiance;

        int specularTextureLevels = textureQueryLevels(u_PrefilterMap);
		vec3 specularIrradiance = textureLod(u_PrefilterMap, Lr,specularTextureLevels * m_Params.Roughness).rgb;

        vec2 specularBRDF  = texture(u_BRDFLUT, vec2(m_Params.NdotV,1.0 - m_Params.Roughness)).rg;

        //TODO Fix why brdf is causing shader bug
        //of a grey dot
		vec3 specularIBL = specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y) ; 
		//vec3 specularIBL = specularIrradiance * (F0 ) ; 

        iblEfeect += kd * diffuseIBL + specularIBL;
        iblEfeect = iblEfeect * (u_SkyBoxInfo.Intensity) * (u_SkyBoxInfo.TintColor);  
    }

    DirectionalLight currentLight = u_DirectionalLightData.Lights[0];

    vec3 lightDirection = currentLight.Direction;
    const int SHADOW_MAP_CASCADE_COUNT = 4;
    uint cascadeIndex = 0;
	for (uint i = 0; i < SHADOW_MAP_CASCADE_COUNT - 1; i++)
	{
		if (Input.ViewPosition.z < u_RendererData.CascadeSplits[i])
			cascadeIndex = i + 1;
	}

	float shadowDistance = u_RendererData.MaxShadowDistance; 
	float transitionDistance = u_RendererData.ShadowFade; // expos
	float distance = length(Input.ViewPosition);
	float shadowFade = distance - (shadowDistance - transitionDistance);
	shadowFade /= transitionDistance;
	shadowFade = clamp(1.0 - shadowFade, 0.0, 1.0);

	float shadowScale;
    bool castSoftShadow = false;
    if(u_RendererData.SoftShadows == true && currentLight.CastSoftShadow == true)
    {
        castSoftShadow = true;
    }
    bool fadeCascades = u_RendererData.CascadeFading;
	if (fadeCascades)
	{
		float cascadeTransitionFade = u_RendererData.CascadeTransitionFade;

		float c0 = smoothstep(u_RendererData.CascadeSplits[0] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[0] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		float c1 = smoothstep(u_RendererData.CascadeSplits[1] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[1] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		float c2 = smoothstep(u_RendererData.CascadeSplits[2] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[2] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		if (c0 > 0.0 && c0 < 1.0)
		{
			// Sample 0 & 1
			vec3 shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, 0);
			float shadowAmount0 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 0, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 0, shadowMapCoords,lightDirection);
			shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, 1);
			float shadowAmount1 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 1, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 1, shadowMapCoords,lightDirection);

			shadowScale = mix(shadowAmount0, shadowAmount1, c0);
		}
		else if (c1 > 0.0 && c1 < 1.0)
		{
			// Sample 1 & 2
			vec3 shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, 1); 
			float shadowAmount1 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 1, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 1, shadowMapCoords,lightDirection);
			shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, 2);
			float shadowAmount2 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 2, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 2, shadowMapCoords,lightDirection);

			shadowScale = mix(shadowAmount1, shadowAmount2, c1);
		}
		else if (c2 > 0.0 && c2 < 1.0)
		{
			// Sample 2 & 3
			vec3 shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, 2);
			float shadowAmount2 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 2, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 2, shadowMapCoords,lightDirection);
			shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, 3);
			float shadowAmount3 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 3, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 3, shadowMapCoords,lightDirection);

			shadowScale = mix(shadowAmount2, shadowAmount3, c2);
		}
		else
		{
			vec3 shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, cascadeIndex);
			shadowScale = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords,lightDirection);
		}
	}
	else
	{
		vec3 shadowMapCoords = GetShadowMapCoords(Input.ShadowMapCoords, cascadeIndex);
		//shadowScale = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords,lightDirection);
		shadowScale = castSoftShadow ? PCF_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords,lightDirection);
	}

	shadowScale = 1.0 - clamp(currentLight.ShadowStrength - shadowScale, 0.0f, 1.0f);
    // Shadow mask with respect to bright surfaces.
    // Calculate the alpha component of out_ViewNormalsLuminance, likely for shading or rendering effects.
    // 1. shadowScale represents the shadow intensity at the current fragment.
    // 2. color.rgb is the RGB color of the fragment, indicating its final color.
    // 3. vec3(0.2125f, 0.7154f, 0.0721f) are weights for converting RGB to grayscale, 
    //    reflecting human perception of brightness for red, green, and blue channels.
    // 4. dot(color.rgb, vec3(0.2125f, 0.7154f, 0.0721f)) calculates the luminance (brightness) of the color.
    // 5. shadowScale + dot(color.rgb, vec3(0.2125f, 0.7154f, 0.0721f)) combines shadow intensity and luminance.
    // 6. clamp(...) ensures the result remains within the valid alpha value range [0.0, 1.0].


    vec3 finalColor = directLighting * shadowScale ;
    finalColor += CalculatePointLights(F0, Input.WorldPosition);
    finalColor += CalculateSpotLights(F0, Input.WorldPosition); //* sahdow

    if(u_MaterialUniform.EmissionOverrideColorToggle == false)
    {
        finalColor += m_Params.AlbedoColor * u_MaterialUniform.Emission;//emision
    }
    else
    {
        finalColor += u_MaterialUniform.EmissionOverrideColor * u_MaterialUniform.Emission;//emision
    }

    if(u_Scene.ShowLightGrid)
    {
        int pointLightCount = GetPointLightCountForPixel();
		int spotLightCount = GetSpotLightCountForPixel();

		float value = float(pointLightCount + spotLightCount);
		finalColor.rgb = (finalColor.rgb * 0.2) + GetGradient(value);
    }

    out_FragColor = vec4(finalColor + iblEfeect ,1.0);

    out_ViewNormalsLuminance.a = clamp(shadowScale + dot(out_FragColor.rgb, vec3(0.2125f, 0.7154f, 0.0721f)), 0.0f, 1.0f);

    if(u_RendererData.ShowCascades)
    {
        switch(cascadeIndex) {
			    case 0 : 
				    out_FragColor.rgb *= vec3(1.0f, 0.25f, 0.25f);
				    break;
			    case 1 : 
				    out_FragColor.rgb *= vec3(0.25f, 1.0f, 0.25f);
				    break;
			    case 2 : 
				    out_FragColor.rgb *= vec3(0.25f, 0.25f, 1.0f);
				    break;
			    case 3 : 
				    out_FragColor.rgb *= vec3(1.0f, 1.0f, 0.25f);
				    break;
		    }
        }
}   
