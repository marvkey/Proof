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
    vec4 FoamColor;

    vec4 MapScales[MAX_CASCADES];

    float Roughness;
    int NumCascades;
    float NormalStrength;
} u_PC;


struct VertexOutput
{
   vec2 UV;
   float WaveHeight;
};

layout(location = 23) out VertexOutput Output;
void Vertex(inout PBRVertexInput vertexInput)
{


    Output.UV = vertexInput.VertexPosition.xz;

    float distanceFactor = min(exp(-(length(Output.UV - u_Camera.Position.xz) - 150.0) * 0.007), 1.0); // Displacement amonut falls off after 150m.

    // Read displacements from displacement maps.
    vec3 displacement = vec3(0.0);
    for (int i = 0; i < u_PC.NumCascades; ++i) {
        vec4 scales = u_PC.MapScales[i];
        displacement += texture(u_Displacements, vec3(Output.UV * scales.xy, float(i))).xyz * scales.z;
    }

    vertexInput.VertexPosition +=  displacement * distanceFactor ;
    Output.WaveHeight = displacement.y;

}

#Fragment Shader
#version 450 core

//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.FragmentTransperant.glsl>
#define MAX_CASCADES 8


layout(set = 0, binding = 2) uniform sampler2DArray u_Normals;

layout(set = 0, binding = 3) uniform WaterUniformsF 
{
    vec4 WaterColor;
    vec4 FoamColor;

    vec4 MapScales[MAX_CASCADES];

    float Roughness;
    int NumCascades;
    float NormalStrength;
} u_PC;

struct VertexOutput
{
   vec2 UV;
   float WaveHeight;
};

layout(location = 23) flat in VertexOutput Input;

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

void Fragment(inout PBRData pbrData)
{

    float map_size = float(textureSize(u_Normals, 0).x);
	float dist = length(PBR_Input.VertexPosition.xz);

    vec3 gradient = vec3(0);
	for (uint i = 0U; i < u_PC.NumCascades; ++i) 
    {
		vec4 scales = u_PC.MapScales[i];
		vec3 coords = vec3(Input.UV*scales.xy, float(i));
		float ppm = map_size * min(scales.x, scales.y); // Pixels per meter
		// Mix between bicubic and bilinear filtering depending on the world space pixels per meter.
		// This is dependent on the tile size as well as displacement/normal map resolution.
		gradient += mix(texture_bicubic(coords), texture(u_Normals, coords), min(1.0, ppm*0.1)).xyw * vec3(scales.ww, 1.0);
	}
	

    pbrData.Albedo = vec3(0,0,1);
    pbrData.Metalness = 0.3;
    pbrData.Roughness = 0.7;

   // gradient *= mix(0.015, u_PC.NormalStrength, exp(-dist*0.0175)); // Blend normal with terrain normal as distance increases.
   // gradient *= mix(0.015, u_PC.NormalStrength, exp(-dist*0.0175)); // Blend normal with terrain normal as distance increases.

   // pbrData.Normal =  (u_Camera.View * vec4(normalize(vec3(-gradient.x, 1.0, -gradient.y)), 0.0)).xyz;
    pbrData.Alpha = 1.0;


}



