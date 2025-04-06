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
/*
#include <Common.glslh>

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;
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

    vec4 NormalizePositionCur; // nomralize device position current
	vec4 NormalizePositionPrev;
};
layout(location = 0) out VertexOutput Output;


layout(std140, set = 1, binding = 0) uniform ShadowMapProjections {

    mat4 ViewProjections[4];
} u_CascadePositions;

//https://computergraphics.stackexchange.com/questions/8979/strange-dark-spot-when-lighting-in-deferred-rendering-pbr-shaders
//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/739ff016ad2a23e3843517c4866dda09ce5d112f/Resources/Shaders/PBR_Static.glsl
void main() 
{

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

    vec4 posProjPrev = (u_Camera.PrevViewProjectionMatrix * aPrevTransform * vec4(aPosition, 1.0));
	vec4 posProjCur = (u_Camera.ViewProjectionMatrix * aTransform * vec4(aPosition,1.0));
	
	Output.NormalizePositionCur = posProjCur;
	Output.NormalizePositionPrev = posProjPrev;

   gl_Position =  u_Camera.Projection * u_Camera.View * vec4(Output.WorldPosition, 1.0);
}
*/

#Fragment Shader
#version 450 core
//#include <PBR/PBR.glslh>
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
//#include <PBR/Shadow/ShadowMapping.glslh>



//ouputs
//layout(location = 0) out vec4 out_FragColor;
// `out_ViewNormalsLuminance` is an output variable used to store view-space normals
// and potentially luminance information during rendering. It's a part of a more
// complex rendering pipeline, likely involving lighting calculations, screen-space
// effects, or post-processing. View-space normals are necessary for lighting, while
// luminance could represent brightness or intensity used for various purposes
// such as tone mapping or color grading. The exact role of this variable depends
// on the specific rendering pipeline and shader's purpose within it.
//layout(location = 1) out vec4 out_ViewNormalsLuminance;
//layout(location = 2) out vec4 out_MetalnessRoughness; //RGBA //R= metallnes, G = Roughness
//layout(location = 3) out vec2 out_Velocity; //R16G16 float, velocity
//layout(location = 4) out vec4 out_DirectLighting; //RGBA float Point Light,Directional Light,Spotlight,Area Light

// pbr map
layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

//environmentMap
//layout(set = 1, binding = 2) uniform samplerCube u_IrradianceMap;
//layout(set = 1, binding = 3) uniform samplerCube u_PrefilterMap;
//layout(set = 1, binding = 4) uniform sampler2D u_BRDFLUT;

//shadow map
//layout(set = 1, binding = 5) uniform sampler2DArray u_ShadowMap;


//struct VertexOutput
//{
//    vec3 WorldPosition;
//    vec3 Normal;
//    mat3 WorldNormals;
//    vec2 TexCoords;
//    vec3 Tangent;
//    vec3 Bitangent;
//    vec3 CameraPosition;
//
//    mat3 CameraView; 
//
//    vec3 ShadowMapCoords[4];
//
//    vec3 ViewPosition;
//
//    vec4 NormalizePositionCur; // nomralize device position current
//	vec4 NormalizePositionPrev;
//};
//layout(location = 0) in VertexOutput Input;

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
/*
void main()
{
    vec2 texCoords = PBR_Input.TexCoords * u_MaterialUniform.TextureTiling + u_MaterialUniform.TextureOffset;
 
   

    pbrDatas pbrData;
    pbrData.Albedo = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo;
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

    ApplyPBR(pbrData);
     
}
*/

void Fragment(inout PBRData pbrData)
{
    vec2 texCoords = PBR_Input.TexCoords * u_MaterialUniform.TextureTiling + u_MaterialUniform.TextureOffset;

    pbrData.Albedo = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo;
    pbrData.Albedo = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo;
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
        pbrData.EmissionColour = u_MaterialUniform.Albedo;//emision
    }
    else
    {
        pbrData.EmissionColour = u_MaterialUniform.EmissionOverrideColor ;//emision
    }
    pbrData.OutFinalColor = 1;

}


void PreEndFragment()
{
}