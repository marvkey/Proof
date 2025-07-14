//https://github.com/2Retr0/GodotOceanWaves/blob/4e1e219bc8f55c38092ed34e6fee568b900d95b5/assets/shaders/spatial/fft_water.gdshader

#Vertex Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>
layout(set = 0, binding = 1) uniform sampler2DArray u_Displacements;

#define MAX_CASCADES 8


layout(set = 0, binding = 0) uniform WaterUniforms 
{

    vec4 WaterColor;

    vec3 FoamColor;
    float Roughness;

    int NumCascades;
    float NormalStrength;
    float ClipMap_Scale;
    float ClipMap_LevelHalfSize;
    
    vec3  ClipMap_ViewerPosition;


} u_PC;

layout(set = 0, binding = 5) uniform ScalesWaterUniform
{

   vec4 MapScales[MAX_CASCADES];
};


struct VertexOutput
{
   vec2 UV;
   float WaveHeight;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) out VertexOutput Output;

float ModifiedManhattanDistance(vec3 a, vec3 b) {
    vec3 v = a - b;
    return max(abs(v.x + v.z) + abs(v.x - v.z), abs(v.y)) * 0.5;
}

// Snaps a vertex around the viewer to simulate infinite terrain tiling
vec3 ClipMap_VertexSnap(vec3 positionOS, vec2 uv) {
    float meshScale = u_PC.ClipMap_Scale;
    float step = meshScale * 4.0;

    vec2 snappedViewerXZ = floor(u_PC.ClipMap_ViewerPosition.xz / step) * step;
    vec3 worldPos = vec3(snappedViewerXZ + positionOS.xz * meshScale, 0.0);

    float morphStart = ((u_PC.ClipMap_LevelHalfSize + 1.0) * 0.5 + 8.0) * meshScale;
    float morphEnd   = (u_PC.ClipMap_LevelHalfSize - 2.0) * meshScale;

    float t = clamp((ModifiedManhattanDistance(worldPos, u_PC.ClipMap_ViewerPosition) - morphStart) / (morphEnd - morphStart), 0.0, 1.0);
    worldPos += vec3(uv, 0.0) * meshScale * t;

    return worldPos;
}
void Vertex(inout PBRVertexInput vertexInput)
{


    //vertexInput.VertexPosition = ClipMap_VertexSnap(vertexInput.VertexPosition, vertexInput.TexCoords);
    Output.UV = vertexInput.VertexPosition.xz;

    vec3 displacement = vec3(0.0);
    for (int i = 0; i < u_PC.NumCascades; ++i) 
    {
        vec4 scales = MapScales[i];

        // Reconstruct tileLength from 1.0 / scale
        vec2 tileLength = vec2(1.0 / scales.x, 1.0 / scales.y);

        // Wrap world position into tile space [0, tileLength]
        vec2 worldPos = vertexInput.VertexPosition.xz;
        vec2 wrapped = mod(mod(worldPos, tileLength) + tileLength, tileLength); // handles negative coords
        vec2 uv = wrapped / tileLength; // map to [0,1]

        displacement += texture(u_Displacements, vec3(uv, float(i))).xyz * scales.z;
    }

Output.WaveHeight = displacement.y;

float distanceFactor = min(exp(-(length(Output.UV - u_Camera.Position.xz) - 150.0) * 0.007), 1.0);

vertexInput.VertexPosition += displacement * distanceFactor;

}

#Fragment Shader
#version 450 core

//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>


layout(set = 0, binding = 2) uniform sampler2DArray u_Normals;

#define MAX_CASCADES 8

layout(set = 0, binding = 3) uniform WaterUniformsF 
{ 
vec4 WaterColor;

    vec3 FoamColor;
    float Roughness;

    int NumCascades;
    float NormalStrength;
	float MinMeshScale;
    int LevelhalfSize;

    vec3  ViewerPosition;

} u_PC;

layout(set = 0, binding = 7) uniform ScalesWaterUniformF
{

   vec4 MapScales[MAX_CASCADES];
};

struct VertexOutput
{
   vec2 UV;
   float WaveHeight;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) flat in VertexOutput Input;

/** Filter weights for a cubic B-spline. */
vec4 cubic_weights(float a) {
	float a2 = a*a;
	float a3 = a2*a;
	
	float w0 =-a3     + a2*3.0 - a*3.0 + 1.0;
	float w1 = a3*3.0 - a2*6.0         + 4.0;
	float w2 =-a3*3.0 + a2*3.0 + a*3.0 + 1.0;
	float w3 = a3;
	return vec4(w0, w1, w2, w3) / 6.0;
}

/** Performs bicubic B-spline filtering on the provided sampler. */
// Source: https://developer.nvidia.com/gpugems/gpugems2/part-iii-high-quality-rendering/chapter-20-fast-third-order-texture-filtering
vec4 texture_bicubic(in vec3 uvw) 
{
	vec2 dims = vec2(textureSize(u_Normals, 0).xy);
	vec2 dims_inv = 1.0 / dims;
	uvw.xy = uvw.xy*dims + 0.5;
	
	vec2 fuv = fract(uvw.xy);
	vec4 wx = cubic_weights(fuv.x);
	vec4 wy = cubic_weights(fuv.y);

	vec4 g = vec4(wx.xz + wx.yw, wy.xz + wy.yw);
	vec4 h = (vec4(wx.yw, wy.yw) / g + vec2(-1.5, 0.5).xyxy + floor(uvw.xy).xxyy)*dims_inv.xxyy;
	vec2 w = g.xz / (g.xz + g.yw);
    return mix(
		mix(texture(u_Normals, vec3(h.yw, uvw.z)), texture(u_Normals, vec3(h.xw, uvw.z)), w.x),
		mix(texture(u_Normals, vec3(h.yz, uvw.z)), texture(u_Normals, vec3(h.xz, uvw.z)), w.x), w.y); 
}

float Fresnel;
float FoamFactor =.0f;
vec3 Normal = vec3(0);
#define REFLECTANCE  0.02 // Reflectance from air to water (eta=1.33).

void Fragment(inout PBRData pbrData)
{
    float map_size = float(textureSize(u_Normals, 0).x);
	float dist = length(PBR_Input.VertexPosition.xz);

    vec3 gradient = vec3(0);
	for (uint i = 0U; i < u_PC.NumCascades; ++i) 
    {
		vec4 scales = MapScales[i];
		vec2 tileLength = vec2(1.0 / scales.x, 1.0 / scales.y);
        vec2 worldPos = Input.UV;
        vec2 wrapped = mod(mod(worldPos, tileLength) + tileLength, tileLength);
        vec2 uv = wrapped / tileLength;

        vec3 coords = vec3(uv, float(i));
        float ppm = map_size * min(scales.x, scales.y);
       gradient  += mix(texture_bicubic(coords), texture(u_Normals, coords), min(1.0, ppm * 0.1)).xyw * vec3(scales.ww, 1.0);

	}
	
	FoamFactor = smoothstep(0.0, 1.0, gradient.z*0.75) * exp(-dist*0.0075);

    pbrData.Albedo = mix(u_PC.WaterColor.xyz, u_PC.FoamColor, FoamFactor);
    pbrData.Metalness = 0.3;

    gradient *= mix(0.015, u_PC.NormalStrength, exp(-dist*0.0175)); // Blend normal with terrain normal as distance increases.
    pbrData.Normal =  (u_Camera.View * vec4(normalize(vec3(-gradient.x, 1.0, -gradient.y)), 0.0)).xyz;

    Fresnel = mix(pow(1.0 - max(0.0,dot(m_PBRParams.View, pbrData.Normal)), 5.0* exp(-2.69 * u_PC.Roughness)) / (1.0 + 22.7 * pow(u_PC.Roughness,1.5)),1.0,REFLECTANCE);
    pbrData.Roughness = (1.0 - Fresnel) * FoamFactor + 0.4;// Roughness is proportional to foam/fog amount and fresnel.

    pbrData.Alpha = u_PC.WaterColor.w;

	Normal =pbrData.Normal;

}
float smith_masking_shadowing(in float cos_theta, in float alpha) {
	float a = cos_theta / (alpha * sqrt(1.0 - cos_theta*cos_theta)); // Approximate: 1.0 / (alpha * tan(acos(cos_theta)))
	float a_sq = a*a;
	return a < 1.6 ? (1.0 - 1.259*a + 0.396*a_sq) / (3.535*a + 2.181*a_sq) : 0.0;
}

// Source: https://github.com/godotengine/godot/blob/7b56111c297f24304eb911fe75082d8cdc3d4141/drivers/gles3/shaders/scene.glsl#L995
float ggx_distribution(in float cos_theta, in float alpha) {
	float a_sq = alpha*alpha;
	float d = 1.0 + (a_sq - 1.0) * cos_theta * cos_theta;
	return a_sq / (PI * d*d);
}


void LightLateUpdate(inout vec3 lightDirection, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{
    vec3 halfway= normalize(m_PBRParams.View + lightDirection);

    float dot_nl = max(2e-5, dot(Normal, lightDirection));
    float dot_nv = max(2e-5, dot(Normal, m_PBRParams.View));

	const float ATTENUATION = 1.0f; //Todo figure out this attenuation factor. 

    // specular
    float light_mask = smith_masking_shadowing(u_PC.Roughness, dot_nv);
	float view_mask = smith_masking_shadowing(u_PC.Roughness, dot_nl);
	float microfacet_distribution = ggx_distribution(dot(Normal, halfway), u_PC.Roughness);
	float geometric_attenuation = 1.0 / (1.0 + light_mask + view_mask);
	specularBRDF = vec3 (Fresnel * microfacet_distribution * geometric_attenuation / (4.0 * dot_nv + 0.1) * ATTENUATION);

	// Diffuse

	const vec3 sss_modifier = vec3(0.9,1.15,0.85); // Subsurface scattering produces a 'greener' color.
	float sss_height = 1.0*max(0.0, Input.WaveHeight + 2.5) * pow(max(dot(lightDirection, -m_PBRParams.View), 0.0), 4.0) * pow(0.5 - 0.5 * dot(lightDirection, Normal), 3.0);
	float sss_near = 0.5*pow(dot_nv, 2.0);
	float lambertian = 0.5*dot_nl;
	diffuseBRDF = mix((sss_height + sss_near) * sss_modifier / (1.0 + light_mask) + lambertian, u_PC.FoamColor.rgb, FoamFactor) * (1.0 - Fresnel) * ATTENUATION * currentLight.Color;
}

void PreEndFragment()
{
}
