#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

//void main() 
//{
//   ApplyPbrVertex();
//}

void Vertex(inout PBRVertexInput vertexinput)
{
    
}

#Fragment Shader
#version 450 core
//#include <PBR/PBR.glslh>
#include <PBR/PBRShaderBases/PBR.FragmentTransperant.glsl>
//#include <PBR/Shadow/ShadowMapping.glslh>

// pbr map
layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

layout(location = 5) out vec4 out_Accum; 
layout(location = 6) out float out_Reveal; 



layout(push_constant) uniform Material
{
    vec4 Albedo;

    float Metalness;
    float Roughness;
    float Emission;
    bool NormalTexToggle;

    vec2 TextureTiling;
    vec2 TextureOffset;

    vec3 EmissionOverrideColor; // if EmissionOverrideColorToggle is equal to true then we will override the emission color
    bool EmissionOverrideColorToggle;
} u_MaterialUniform;

void Fragment(inout PBRData pbrData)
{
    vec2 texCoords = PBR_Input.TexCoords * u_MaterialUniform.TextureTiling + u_MaterialUniform.TextureOffset;

    pbrData.Albedo = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo.xyz;
    pbrData.Metalness = texture(u_MetallicMap, texCoords).r * u_MaterialUniform.Metalness;
    pbrData.Roughness = texture(u_RoughnessMap, texCoords).r * max(u_MaterialUniform.Roughness,0.00);

    pbrData.Normal = normalize(PBR_Input.Normal);
    if (u_MaterialUniform.NormalTexToggle)
	{
		pbrData.Normal = normalize(texture(u_NormalMap, texCoords).rgb * 2.0f - 1.0f);
		pbrData.Normal = normalize(PBR_Input.WorldNormals * pbrData.Normal);
	}

    pbrData.TexCoords = texCoords;
    pbrData.Emission = u_MaterialUniform.Emission;
    
    if(u_MaterialUniform.EmissionOverrideColorToggle == false)
    {
        pbrData.EmissionColour = pbrData.Albedo;//emision
    }
    else
    {
        pbrData.EmissionColour = u_MaterialUniform.EmissionOverrideColor ;//emision
    }
    pbrData.Alpha = u_MaterialUniform.Albedo.a;
    pbrData.OutFinalColor = 0;
}


void PreEndFragment()
{
	float weight = clamp(pow(min(1.0, finalEndingCOlor.a * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);

	// store pixel color accumulation
	out_Accum = vec4(finalEndingCOlor.rgb * finalEndingCOlor.a, finalEndingCOlor.a) * weight;
	
	// store pixel revealage threshold
	out_Reveal = finalEndingCOlor.a;

    /*
    vec4 color = finalEndingCOlor;
    color.rgb *= color.a;

    const float depthZ = -PBR_Input.ViewSpaceValue.z * 10.0f;

    const float distWeight = clamp(0.03 / (1e-5 + pow(depthZ / 200, 4.0)), 1e-2, 3e3);

    float alphaWeight = min(1.0, max(max(color.r, color.g), max(color.b, color.a)) * 40.0 + 0.01);
    alphaWeight *= alphaWeight;

    const float weight = alphaWeight * distWeight;

    // GL Blend function: GL_ONE, GL_ONE
    out_Accum = color ;
    //out_Accum = vec4(0.5);

    // GL blend function: GL_ZERO, GL_ONE_MINUS_SRC_ALPHA
    out_Reveal = color.a;
    */

}