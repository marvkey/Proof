#Vertex Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

//#define MID

// === Uniforms ===
layout(set = 0, binding = 0) uniform sampler2D _Displacement_c0;
layout(set = 0, binding = 1) uniform sampler2D _Displacement_c1;
layout(set = 0, binding = 2) uniform sampler2D _Displacement_c2;

layout(set = 0, binding = 4) uniform OceanParams {
    float LengthScale0;
    float LengthScale1;
    float LengthScale2;
    float _LOD_scale;
    float _SSSBase;
    float _SSSScale;
};

struct VertexOutput
{
   vec2 vWorldUV;
   vec3 vViewVector;
   vec4 vLodScales;
};

layout(location = 23) out VertexOutput Output;

void Vertex(inout PBRVertexInput vertexInput)
{

// Transform local vertex position to world space using aTransform
    vec4 worldPos = aTransform * vec4(vertexInput.VertexPosition, 1.0);
    vec2 worldUV = worldPos.xz;
    //vec2 worldUV = vertexInput.VertexPosition.xz;
    Output.vWorldUV = worldUV;

    Output.vViewVector = u_Camera.Position - worldPos.xyz;
    float viewDist =max(1.0, length(Output.vViewVector));

    // LOD calculations
    float lod_c0 = min(_LOD_scale * LengthScale0 / viewDist, 1.0);
    float lod_c1 = min(_LOD_scale * LengthScale1 / viewDist, 1.0);
    float lod_c2 = min(_LOD_scale * LengthScale2 / viewDist, 1.0);

    // Sample displacement
    vec3 displacement = vec3(0.0);
    float largeWavesBias = 0.0;

    displacement += textureLod(_Displacement_c0, worldUV / LengthScale0, 0.0).xyz * lod_c0;
    largeWavesBias = displacement.y;

#if defined(MID) || defined(CLOSE)
    displacement += textureLod(_Displacement_c1, worldUV / LengthScale1, 0.0).xyz * lod_c1;
#endif
#if defined(CLOSE)
    displacement += textureLod(_Displacement_c2, worldUV / LengthScale2, 0.0).xyz * lod_c2;
#endif

    // Apply displacement in object space
    mat4 inverseTransform = inverse(aTransform);
    vec3 displacedLocal = (inverseTransform * vec4(displacement, 0.0)).xyz;
    vertexInput.VertexPosition += displacement;

    // Output LOD scales
    Output.vLodScales = vec4(
        lod_c0,
        lod_c1,
        lod_c2,
        max(displacement.y - largeWavesBias * 0.8 - _SSSBase, 0.0) / _SSSScale
    );
}


#Fragment Shader
#version 450 core

//#define MID
#include <Common.glslh>
//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
struct VertexOutput
{
   vec2 vWorldUV;
   vec3 vViewVector;
   vec4 vLodScales;
};

layout(location = 23) in VertexOutput Input;


// === Uniforms ===
layout(set = 0, binding = 5) uniform sampler2D f_Derivatives_c0;
layout(set = 0, binding = 6) uniform sampler2D f_Derivatives_c1;
layout(set = 0, binding = 7) uniform sampler2D f_Derivatives_c2;

layout(set = 0, binding = 8) uniform sampler2D f_Turbulence_c0;
layout(set = 0, binding = 9) uniform sampler2D f_Turbulence_c1;
layout(set = 0, binding = 10) uniform sampler2D f_Turbulence_c2;

layout(set = 0, binding = 11) uniform sampler2D f_FoamTexture;

layout(set = 0, binding = 13) uniform OceanSettings {
    vec4 _Color;
    vec4 _FoamColor;
    vec4 _SSSColor;

    float _SSSStrength;
    float _Roughness;
    float _RoughnessScale;
    float _MaxGloss;

    float _FoamBiasLOD0;
    float _FoamBiasLOD1;
    float _FoamBiasLOD2;
    float _FoamScale;

    float _ContactFoam;
};

layout(set = 0, binding = 14) uniform fOceanParams {
    float LengthScale0;
    float LengthScale1;
    float LengthScale2;
    float _LOD_scale;

    float _SSSBase;
    float _SSSScale;
};

// === Helper Functions ===
float pow5(float x) {
    float x2 = x * x;
    return x2 * x2 * x;
}

vec3 getWorldNormal(vec2 uv) 
{
    vec4 d = texture(f_Derivatives_c0, uv / LengthScale0);
#if defined(MID) || defined(CLOSE)
    d += texture(f_Derivatives_c1, uv / LengthScale1) * Input.vLodScales.y;
#endif
#if defined(CLOSE)
    d += texture(f_Derivatives_c2, uv / LengthScale2) * Input.vLodScales.z;
#endif
    vec2 slope = vec2(d.x / (1.0 + d.z), d.y / (1.0 + d.w));
    return normalize(vec3(-slope.x, 1.0, -slope.y));
}

//r,g,b color, a inteisty
vec4 DecomposeEmission(vec3 emissiondecoom) 
{
    float intensity = max(max(emissiondecoom.r, emissiondecoom.g), emissiondecoom.b);
    vec3 color = (intensity > 0.0) ? emissiondecoom / intensity : vec3(0.0);
    return vec4(color, intensity);
}

vec3 emission;
vec3 sssColor;
float fresnel;
void Fragment(inout PBRData pbrData)
{
    vec2 uv = Input.vWorldUV;
    vec3 viewDir = normalize(Input.vViewVector);
    vec3 worldNormal = getWorldNormal(uv);

    // --- Jacobian Foam ---
    float jacobian = 0.0;
#if defined(CLOSE)
    jacobian = texture(f_Turbulence_c0, uv / LengthScale0).r
             + texture(f_Turbulence_c1, uv / LengthScale1).r
             + texture(f_Turbulence_c2, uv / LengthScale2).r;
    jacobian = clamp((-jacobian + _FoamBiasLOD2) * _FoamScale, 0.0, 1.0);
#elif defined(MID)
    jacobian = texture(f_Turbulence_c0, uv / LengthScale0).r
             + texture(f_Turbulence_c1, uv / LengthScale1).r;
    jacobian = clamp((-jacobian + _FoamBiasLOD1) * _FoamScale, 0.0, 1.0);
#else
    jacobian = texture(f_Turbulence_c0, uv / LengthScale0).r;
    jacobian = clamp((-jacobian + _FoamBiasLOD0) * _FoamScale, 0.0, 1.0);
#endif

    // --- Contact Foam Approximation ---
    //float foamNoise = texture(f_FoamTexture, uv * 0.5).r;
    float foamNoise = 0.2;
    jacobian += _ContactFoam * clamp(foamNoise * 0.9, 0.0, 1.0);

    emission = mix(sssColor * (1.0 - fresnel), vec3(0.0), jacobian);

    // --- Gloss & Roughness ---
    float viewLen = length(Input.vViewVector);
    float gloss = mix(1.0 - _Roughness, _MaxGloss, 1.0 / (1.0 + viewLen * _RoughnessScale));
    float smoothness = mix(gloss, 0.0, jacobian);
  
    vec3 foamColor = mix(vec3(0.0), _FoamColor.rgb, jacobian);

    // --- Assign PBR Outputs ---
    pbrData.Albedo = vec3(0,0,1);
    //pbrData.Albedo = foamColor;
    //pbrData.Roughness = 1.0 - smoothness;
    //pbrData.Metalness = 0.0;
    //pbrData.Normal = worldNormal;
   // pbrData.EmissionColour = DecomposeEmission(emission).rgb;
    //pbrData.Emission = DecomposeEmission(emission).a;
}

void LightLateUpdate(inout vec3 lightDirection, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{
    vec3 worldNormal = getWorldNormal(Input.vWorldUV);
    vec3 viewDir = normalize(Input.vViewVector);
    // --- Subsurface Lighting ---
    vec3 lightDir = normalize(lightDirection);
    vec3 halfVec = normalize(-worldNormal + lightDir);
    float ViewDotH = pow5(clamp(dot(viewDir, -halfVec), 0.0, 1.0)) * 30.0 * _SSSStrength;

    sssColor = mix(_Color.rgb,
                        clamp(_Color.rgb + _SSSColor.rgb * ViewDotH * Input.vLodScales.w, 0.0, 1.0),
                        Input.vLodScales.z);

      // --- Fresnel Emission ---
    fresnel = clamp(1.0 - dot(worldNormal, viewDir), 0.0, 1.0);
    fresnel = pow5(fresnel);


}
void PreEndFragment()
{
}