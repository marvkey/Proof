//https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.2.1.ibl_specular/2.2.1.pbr.fs
//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/shaders/forward/fPBR.frag
#ifndef PBR_FUNCTIONS
#define PBR_FUNCTIONS
#include "Global.shader"
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a		 = roughness*roughness;
	float a2	 = a*a;
	float NdotH  = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;
	
	float num	= a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom		= PI * denom * denom;
	
	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f;

	float num   = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2  = GeometrySchlickGGX(NdotV, roughness);
	float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 CalulateDirectionalLight(DirectionalLight light, vec3 V, vec3 N,vec3 F0,vec3 meshColor,float roughness, float metallic )
{
	vec3 Lo = vec3(0);
	vec3 L	= normalize(-light.Direction);
	vec3 H	= normalize(V + L);
	float cosine = max(dot(L, N), 0.0f);

	vec3 radiance = light.Color * light.Intensity;


	float NDF = DistributionGGX(N, H, roughness);
	float G	= GeometrySmith(N, V, L, roughness);
	vec3 F	= FresnelSchlickRoughness(max(dot(H, V), 0.0f), F0, roughness);

	vec3 numerator	  = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular	  = numerator / max(denominator, 0.001);

	vec3 ks = F;
	vec3 kd = vec3(1.0f) - ks;
	kd *= 1.0f - metallic;

	float NdotL = max(dot(N, L), 0.0);  

	Lo = (kd * meshColor / PI + specular) * radiance * NdotL;
	return Lo;
}


vec3 AmbientLighting(float roughness, float metallic, vec3 V, vec3 N,vec3 F0,vec3 meshColor,samplerCube irradianceMap,samplerCube prefilteredMap, sampler2D brdfLUT)
{
	vec3 ambient = vec3(0);
	vec3 R = reflect(-V, N);

	const float MAX_REFLECTION_LOD = 4.0f;
	vec3 prefilteredColor = textureLod(prefilteredMap, R,  roughness * MAX_REFLECTION_LOD).rgb;

	vec3 F		  = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
	
	vec3 ks = F;
	vec3 kd = vec3(1.0f) - ks;
	kd *= 1.0f - metallic;

	vec3 diffuse = texture(irradianceMap, N).rgb * meshColor;

	float ao =1.0f;
	ambient = (kd * diffuse + specular) * ao;
	return ambient;
}
#endif
