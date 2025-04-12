
#include <PBR/PBR.glslh>
#include <PBR/Shadow/ShadowMapping.glslh>
//https://github.com/Nadrin/PBR/blob/master/data/shaders/glsl/pbr_fs.glsl
//https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.2.2.ibl_specular_textured/2.2.2.pbr.fs
//https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh
//https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
//https://github.com/HighLo-Engine/HighLo-Engine/blob/b9bf9c3ed87e63ba358ff50ec180927a3801829c/HighLo/assets/shaders/HighLoPBR.glsl

/*
struct PBRParameters
{
	vec3 AlbedoColor;
	float Metalness;
	float Roughness;

	vec3 Normal;
	vec3 View;
	float NdotV;
    vec3 m_PBRParams.F0;

    vec2 TexCoords;
} m_PBRParams;
*/

struct PBRVertexOutput
{
    vec3 WorldPosition;
    vec3 VertexPosition;
    vec3 Normal;
    mat3 WorldNormals;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;

    mat3 CameraView; 

    vec3 ShadowMapCoords[4];

    vec3 ViewPosition;

    vec4 NormalizePositionCur; // nomralize device position current
	vec4 NormalizePositionPrev;
    vec3 ViewSpaceValue; // for depth wieghted blending
};

struct PBRData
{
    vec3 Albedo;
    float Metalness;
    float Roughness;
    vec3 Normal;
    vec2 TexCoords;
    float Emission;// defalut 0
    vec3 EmissionColour;

    float Alpha; // float 1
    bool UseOnlyAlbedo;// false

    int OutFinalColor;
    int OutViewNormalsLuminance;
    int OutMetallnessRoughness;
    int OutVelocity;
    int OutDirectLighting;
};
layout(location = 0) in PBRVertexOutput PBR_Input;

layout(location = 0) out vec4 out_FragColor;
layout(location = 1) out vec4 out_ViewNormalsLuminance;
layout(location = 2) out vec4 out_MetalnessRoughness; //RGBA //R= metallnes, G = Roughness
layout(location = 3) out vec2 out_Velocity; //R16G16 float, velocity
layout(location = 4) out vec4 out_DirectLighting; //RGBA float Point Light,Directional Light,Spotlight,Area Light


//environmentMap
layout(set = 1, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 1, binding = 3) uniform samplerCube u_PrefilterMap;
layout(set = 1, binding = 4) uniform sampler2D u_BRDFLUT;

//shadow map
layout(set = 1, binding = 5) uniform sampler2DArray u_ShadowMap;


const vec3  Fidelectric = vec3(0.04);


void PBR_SetUpPbr(vec3 albedocolor, float metalness, float roughness, vec3 normal,vec2 texCoords)
{
    m_PBRParams.AlbedoColor = albedocolor;
    m_PBRParams.Metalness = metalness;
    m_PBRParams.Roughness = roughness;
    m_PBRParams.Normal = normal;
    m_PBRParams.TexCoords = texCoords;

    m_PBRParams.View = normalize(PBR_Input.CameraPosition - PBR_Input.WorldPosition);

    
    vec3 reflection = reflect(-m_PBRParams.View , m_PBRParams.Normal);
    m_PBRParams.F0 = mix(Fidelectric, m_PBRParams.AlbedoColor , m_PBRParams.Metalness);

    
    // Angle between surface normal and outgoing view Direction.
	m_PBRParams.NdotV = max(dot(m_PBRParams.Normal, m_PBRParams.View),0.0);

    out_MetalnessRoughness = vec4(m_PBRParams.Metalness, m_PBRParams.Roughness, 0.f, 1.f);
    // View normals
    out_ViewNormalsLuminance.xyz = PBR_Input.CameraView * normalize(PBR_Input.Normal);
}

void LightLateUpdate(inout vec3 directLighting, inout vec3 diffuseBRDF, inout vec3 specularBRDF, DirectionalLight currentLight);
   

vec3 PBR_DirectionalLighting()
{
    vec3 directLighting = vec3(0); 
    for (int i = 0; i < 1; i++)
    {
               //https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
        DirectionalLight currentLight= u_DirectionalLightData.Lights[i];
        vec3 Li = -normalize(currentLight.Direction);
        vec3 Lradiance = currentLight.Color * currentLight.Intensity;
        vec3 Lh = normalize(Li + m_PBRParams.View);

        // Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(m_PBRParams.Normal, Li));
		float cosLh = max(0.0, dot(m_PBRParams.Normal, Lh));

        vec3 F = FresnelSchlickRoughness(max(0.0, dot(Lh, m_PBRParams.View)),m_PBRParams.F0,m_PBRParams.Roughness);
		float D = NdfGGX(cosLh, m_PBRParams.Roughness);
		float G = GaSchlickGGX(cosLi, m_PBRParams.NdotV, m_PBRParams.Roughness);

        vec3 kd = (1.0 - F) * (1.0 - m_PBRParams.Metalness);
		vec3 diffuseBRDF = kd * m_PBRParams.AlbedoColor;

        // Cook-Torrance
		vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * cosLi * m_PBRParams.NdotV);
		specularBRDF = clamp(specularBRDF, vec3(0.0f), vec3(10.0f));

        specularBRDF = specularBRDF* Lradiance * cosLi;
        diffuseBRDF = diffuseBRDF * Lradiance * cosLi;
        LightLateUpdate(Li, diffuseBRDF, specularBRDF,currentLight);

		directLighting += (diffuseBRDF + specularBRDF);

    }

    return directLighting;
}

vec3 PBR_CalculateIBL()
{
        vec3 iblEfeect = vec3(0);

    
        vec3 Lr = 2.0 * m_PBRParams.NdotV * m_PBRParams.Normal - m_PBRParams.View ;
        //https://github.com/kidrigger/Blaze/blob/canon/Blaze/shaders/forward/fPBR.frag
        //https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh

        // fresnel reflectance
        vec3 F = FresnelSchlickRoughness(m_PBRParams.NdotV, m_PBRParams.F0, m_PBRParams.Roughness);
	    vec3 R = 2.0 * dot(m_PBRParams.View, m_PBRParams.Normal) * m_PBRParams.Normal - m_PBRParams.View;

        vec3 irradiance = texture(u_IrradianceMap, m_PBRParams.Normal).rgb ;
        vec3 kd = (1 - F) * (1.0 - m_PBRParams.Metalness);

        vec3 diffuseIBL = m_PBRParams.AlbedoColor * irradiance;

        int specularTextureLevels = textureQueryLevels(u_PrefilterMap);
		vec3 specularIrradiance = textureLod(u_PrefilterMap, Lr,specularTextureLevels * m_PBRParams.Roughness).rgb;

        vec2 specularBRDF  = texture(u_BRDFLUT, vec2(m_PBRParams.NdotV,1.0 - m_PBRParams.Roughness)).rg;

        //TODO Fix why brdf is causing shader bug
        //of a grey dot
		vec3 specularIBL = specularIrradiance * (F * specularBRDF.x + specularBRDF.y) ; 
		//vec3 specularIBL = specularIrradiance * (m_PBRParams.F0 ) ; 

        iblEfeect += kd * diffuseIBL + specularIBL;
        iblEfeect = iblEfeect * (u_SkyBoxInfo.Intensity) * (u_SkyBoxInfo.TintColor); 

        return iblEfeect;
}

// retutrns shadowScale
float PBR_CastShadow(inout uint cascadeIndex)
{

    DirectionalLight currentLight = u_DirectionalLightData.Lights[0];

    if(currentLight.CastShadows == false)
        return 1;

    vec3 lightDirection = currentLight.Direction;
    const int SHADOW_MAP_CASCADE_COUNT = 4;
    cascadeIndex = 0;
	for (uint i = 0; i < SHADOW_MAP_CASCADE_COUNT - 1; i++)
	{
		if (PBR_Input.ViewPosition.z < u_RendererData.CascadeSplits[i])
			cascadeIndex = i + 1;
	}

	float shadowDistance = u_RendererData.MaxShadowDistance; 
	float transitionDistance = u_RendererData.ShadowFade; // expos
	float distance = length(PBR_Input.ViewPosition);
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

		float c0 = smoothstep(u_RendererData.CascadeSplits[0] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[0] - cascadeTransitionFade * 0.5f, PBR_Input.ViewPosition.z);
		float c1 = smoothstep(u_RendererData.CascadeSplits[1] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[1] - cascadeTransitionFade * 0.5f, PBR_Input.ViewPosition.z);
		float c2 = smoothstep(u_RendererData.CascadeSplits[2] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[2] - cascadeTransitionFade * 0.5f, PBR_Input.ViewPosition.z);
		if (c0 > 0.0 && c0 < 1.0)
		{
			// Sample 0 & 1
			vec3 shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, 0);
			float shadowAmount0 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 0, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 0, shadowMapCoords,lightDirection);
			shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, 1);
			float shadowAmount1 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 1, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 1, shadowMapCoords,lightDirection);

			shadowScale = mix(shadowAmount0, shadowAmount1, c0);
		}
		else if (c1 > 0.0 && c1 < 1.0)
		{
			// Sample 1 & 2
			vec3 shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, 1); 
			float shadowAmount1 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 1, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 1, shadowMapCoords,lightDirection);
			shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, 2);
			float shadowAmount2 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 2, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 2, shadowMapCoords,lightDirection);

			shadowScale = mix(shadowAmount1, shadowAmount2, c1);
		}
		else if (c2 > 0.0 && c2 < 1.0)
		{
			// Sample 2 & 3
			vec3 shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, 2);
			float shadowAmount2 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 2, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 2, shadowMapCoords,lightDirection);
			shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, 3);
			float shadowAmount3 = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, 3, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, 3, shadowMapCoords,lightDirection);

			shadowScale = mix(shadowAmount2, shadowAmount3, c2);
		}
		else
		{
			vec3 shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, cascadeIndex);
			shadowScale = castSoftShadow ? PCSS_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords, currentLight.ShadowSoftness,lightDirection) : HardShadows_DirectionalLight(u_ShadowMap, cascadeIndex, shadowMapCoords,lightDirection);
		}
	}
	else
	{
		vec3 shadowMapCoords = GetShadowMapCoords(PBR_Input.ShadowMapCoords, cascadeIndex);
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

    return shadowScale;
}

vec3 PBR_GetGradient(float value)
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
vec4 finalEndingCOlor;
void PBR_FinalOutput(vec3 directLighting, float shadowScale, vec3 IblEffect, vec3 emissionColor, float emission,uint shadowCascadeIndex,float alpha,int outputColor,PBRData pbrData)
{
    vec3 finalColor = vec3(0);
    
    if(pbrData.UseOnlyAlbedo)
    {
       finalColor =  pbrData.Albedo * shadowScale;
    }
    else
    {
        finalColor = directLighting * shadowScale ;
    }
    finalColor += CalculatePointLights(m_PBRParams.F0, PBR_Input.WorldPosition);
    finalColor += CalculateSpotLights(m_PBRParams.F0, PBR_Input.WorldPosition); //* sahdow

    finalColor += emissionColor * emission;//emision


    if(u_Scene.ShowLightGrid)
    {
        int pointLightCount = GetPointLightCountForPixel();
		int spotLightCount = GetSpotLightCountForPixel();

		float value = float(pointLightCount + spotLightCount);
		finalColor.rgb = (finalColor.rgb * 0.2) + PBR_GetGradient(value);
    }
    //sky light is indirect lighitn
    out_DirectLighting = vec4(finalColor,1.0);
    finalEndingCOlor =vec4(finalColor + IblEffect ,alpha);
     DirectionalLight currentLight = u_DirectionalLightData.Lights[0];
    float shadowLimunance = shadowScale;
    if(currentLight.CastShadows == false || currentLight.Intensity <= 0)
    {
        shadowLimunance = 0.0f;
    }
    out_ViewNormalsLuminance.a = clamp(shadowLimunance + dot(finalEndingCOlor.rgb, vec3(0.2125f, 0.7154f, 0.0721f)), 0.0f, 1.0f);

    if(u_RendererData.ShowCascades)
    {
        switch(shadowCascadeIndex) 
        {
			    case 0 : 
				    finalEndingCOlor.rgb *= vec3(1.0f, 0.25f, 0.25f);
				    break;
			    case 1 : 
				    finalEndingCOlor.rgb *= vec3(0.25f, 1.0f, 0.25f);
				    break;
			    case 2 : 
				    finalEndingCOlor.rgb *= vec3(0.25f, 0.25f, 1.0f);
				    break;
			    case 3 : 
				    finalEndingCOlor.rgb *= vec3(1.0f, 1.0f, 0.25f);
				    break;
		    }
    }
    vec4 ndcPre = PBR_Input.NormalizePositionPrev;
	ndcPre = ndcPre/ndcPre.w;

	vec4 ndcCur = PBR_Input.NormalizePositionCur;
	ndcCur = ndcCur/ndcCur.w;

	vec2 screenPosCur = (ndcCur.xy * vec2(0.5f, 0.5f) ) +vec2(0.5);
	vec2 screenPosPrev = (ndcPre.xy* vec2(0.5f, 0.5f)) + vec2(0.5);

	out_Velocity = screenPosPrev - screenPosCur;

    out_FragColor = finalEndingCOlor;
}


void ApplyPBR(PBRData pbrData)
{
    PBR_SetUpPbr(pbrData.Albedo,pbrData.Metalness,pbrData.Roughness,pbrData.Normal,pbrData.TexCoords);

    
    vec3 directLighting = PBR_DirectionalLighting();
    vec3 iblEffect = PBR_CalculateIBL();

    uint cascadeIndex = 0;
    float shadowFactor = PBR_CastShadow(cascadeIndex); // edits the value of the cascadeIndex and gives shadowFacotr

    PBR_FinalOutput(directLighting,shadowFactor,iblEffect,pbrData.EmissionColour,pbrData.Emission,cascadeIndex,pbrData.Alpha,pbrData.OutFinalColor,pbrData);
}

void Fragment(inout PBRData pbrData);

void PreEndFragment();

void main()
{

    PBRData pbrData;
    pbrData.Albedo = vec3(1);
    pbrData.Metalness = 0.0f;
    pbrData.Roughness = 0.4;

    pbrData.TexCoords = normalize(PBR_Input.TexCoords);
    pbrData.Normal = normalize(PBR_Input.Normal);
    
    pbrData.Emission = 0;
    pbrData.EmissionColour = pbrData.Albedo;//emision
    pbrData.Alpha = 1;
    pbrData.UseOnlyAlbedo = false;

    pbrData.OutFinalColor = 1;
    pbrData.OutViewNormalsLuminance = 1;
    pbrData.OutMetallnessRoughness = 1;
    pbrData.OutVelocity = 1;
    pbrData.OutDirectLighting = 1;

    Fragment(pbrData);

    ApplyPBR(pbrData);
    PreEndFragment();

    

}

