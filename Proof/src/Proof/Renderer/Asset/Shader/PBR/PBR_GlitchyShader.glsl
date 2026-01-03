
#Vertex Shader

#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

layout(push_constant) uniform Material
{
    float GlitchRate;  // e.g. 0.3
    float GlitchScale; // e.g. 50
    vec3 Axis;         // direction of glitch (e.g. vec3(0, 1, 0))
    float Emission;
    float FlickerRate;
    float FlickerDensity; // determines on vs off when closer to 0.9 rare usally off, closer to 0.1 usually on
} u_MaterialUniform;

layout(set = 0, binding = 5) uniform sampler2D u_NoiseTexVer;

struct GlitShaderVertexOuput
{
    float GlitchRate;
    float GlitchScale;
    float FlickerRate;
    float FlickerDensity;
    vec3 Axis;
    float Emission;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) out GlitShaderVertexOuput OutPutData;
float rand(float n){ return fract(sin(n) * 43758.5453123); }



void Vertex(inout PBRVertexInput vertexinput)
{
    // Use a high-frequency time for the "jitter" snap seen in the video
    float time = u_FrameData.AppTimeSeconds * u_MaterialUniform.GlitchRate;

    // Sample noise based on Y-axis to create the "horizontal bands"
    vec2 noiseUV = vec2(0.5, vertexinput.VertexPosition.y * 0.1 + time);
    float noise = texture(u_NoiseTexVer, noiseUV).r;

    // Flip direction instantly to mimic digital "tearing"
    float direction = (fract(sin(floor(time)) * 43758.5453) > 0.5) ? 1.0 : -1.0;

    // The "Bleed" Logic: Only push vertices where the noise is high
    if (noise > 0.5) {
        float strength = (noise - 0.5) * u_MaterialUniform.GlitchScale;
        vertexinput.VertexPosition += u_MaterialUniform.Axis * strength * direction;
    }
    OutPutData.GlitchRate = u_MaterialUniform.GlitchRate;
    OutPutData.GlitchScale = u_MaterialUniform.GlitchScale;
    OutPutData.Axis = u_MaterialUniform.Axis;
    OutPutData.FlickerDensity = u_MaterialUniform.FlickerDensity;
    OutPutData.FlickerRate = u_MaterialUniform.FlickerRate;
    OutPutData.Emission = u_MaterialUniform.Emission;
}


#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>


#define PBR_DRAW_DEPTH_NONE      0   // Do not write depth (transparent or UI objects)
#define PBR_DRAW_DEPTH_PREPASS   1   // Write depth in the depth pre-pass
#define PBR_DRAW_DEPTH_OVERRIDE  2   // Write depth during main pass (used when displacement can't be replicated in pre-pass)


#define PBR_DRAW_DEPTH PBR_DRAW_DEPTH_NONE  // Options: NONE, PREPASS, OVERRIDE

// Function to convert a value to a rainbow spectrum
vec3 spectrum(float offset) {
    return cos(vec3(0, 2, 4) + offset) * 0.5 + 0.5;
}
layout(set = 0, binding = 6) uniform sampler2D u_NoiseTexFrag;
layout(set = 0, binding = 7) uniform sampler2D u_GlitchColorFrag;


struct GlitShaderVertexOuput
{
    float GlitchRate;
    float GlitchScale;
    float FlickerRate;
    float FlickerDensity;
    vec3 Axis;
    float Emission;
};
layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) in GlitShaderVertexOuput Input;

void Fragment(inout PBRData pbrData)
{
    float time = u_FrameData.AppTimeSeconds;
    vec2 uv = PBR_Input.TexCoords;

    // 1. Noise Mask (Crucial for the "jagged/broken" look)
    float noise = texture(u_NoiseTexFrag, uv + (time * Input.GlitchRate)).r;

    // 2. Fragment Discard: If noise is low, "cut a hole" in the mesh
    // This makes the glitch look like broken data rather than a solid object.
    if (noise < 0.2) discard;

    // 3. Generate Rainbow based on Y position and Time
    vec3 rainbow = spectrum(uv.y * 15.0 + time * 5.0);

    // 4. Flicker Effect
    float flickerSpeed = time * Input.FlickerRate;
    float flicker = step(Input.FlickerDensity, fract(sin(flickerSpeed) * 43758.5453));

    // 5. THE FIX FOR WHITE GLOW:
    // Keep Albedo dark so it doesn't wash out the color.
    pbrData.Albedo = rainbow * 0.1;

    // Assign the rainbow to the COLOR of the emission
    pbrData.EmissionColour = rainbow;

    // Use the float to drive the "hotness" of the glow. 
    // Start at 4.0. If it turns white, lower this value!
    pbrData.Emission = 2.0* flicker;

    // Remove realistic shading to keep the digital look
    pbrData.Alpha = flicker;
}

void PreEndFragment()
{
}

        
void LightLateUpdate(inout vec3 lightDir, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{

}