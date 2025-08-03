
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;

#ifdef PBR_ANIMATED

layout(location = 13) in uvec4 aBoneIndices;
layout(location = 14) in vec4 aBoneWeights;
 layout(push_constant) uniform PushConstantVertex
    {
        int BaseIndex;
    } pc_BoneTransformIndex;


    layout(set = 1, binding = 2) buffer BoneTransforms
    {
        mat4 BoneTransforms[];
    } s_AnimationBonesTransforms;

#endif

invariant gl_Position;

#extension GL_ARB_explicit_attrib_location : enable
#include <Common.glslh>

#define PBR_SHADER_VERTEX_BASE
struct PBRVertexOutput
{
    vec3 WorldPosition;
    vec3 VertexPosition;
    vec3 Normal;
    mat3 WorldNormals;
    vec3 VecWorldNormal;
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
layout(location = 0) out PBRVertexOutput PBR_Output;

layout(std140, set = 1, binding = 0) uniform ShadowMapProjections 
{

    mat4 ViewProjections[4];
} u_CascadePositions;

struct PBRVertexInput
{
    vec3 VertexPosition;
    vec2 TexCoords;
    vec3 Tangent; 
    vec3 Bitangent;
    vec3 Normal;
    vec3 WorldPositionOffset;
};

vec3 GetWorldNormal(vec3 objectNormal, mat4 modelViewMatrix, mat4 inverseViewMatrix)
{
    // Transform to view space first (don't affect original normal)
    vec3 viewNormal = normalize((modelViewMatrix * vec4(objectNormal, 0.0)).xyz);

    // Transform back to world space
    vec3 worldNormal = normalize((inverseViewMatrix * vec4(viewNormal, 0.0)).xyz);

    return worldNormal;
}
void ApplyPbrVertex(PBRVertexInput pbrvertex)
{
    vec3 modifiedPosition = pbrvertex.VertexPosition;

    PBR_Output.Tangent = pbrvertex.Tangent;
    PBR_Output.Bitangent = pbrvertex.Bitangent;
    PBR_Output.TexCoords = pbrvertex.TexCoords;

    mat4 boneTransform = mat4(1.0f);
#ifdef PBR_ANIMATED
    boneTransform =      s_AnimationBonesTransforms.BoneTransforms[(pc_BoneTransformIndex.BaseIndex + gl_InstanceIndex) * MAX_BONES + aBoneIndices[0]] * aBoneWeights[0];
	boneTransform     += s_AnimationBonesTransforms.BoneTransforms[(pc_BoneTransformIndex.BaseIndex + gl_InstanceIndex) * MAX_BONES + aBoneIndices[1]] * aBoneWeights[1];
	boneTransform     += s_AnimationBonesTransforms.BoneTransforms[(pc_BoneTransformIndex.BaseIndex + gl_InstanceIndex) * MAX_BONES + aBoneIndices[2]] * aBoneWeights[2];
	boneTransform     += s_AnimationBonesTransforms.BoneTransforms[(pc_BoneTransformIndex.BaseIndex + gl_InstanceIndex) * MAX_BONES + aBoneIndices[3]] * aBoneWeights[3];
#endif

    vec4 worldPos = aTransform * boneTransform * vec4(modifiedPosition , 1.0);
    //worldPos = worldPos + vec4(pbrvertex.WorldPositionOffset,0);
    PBR_Output.WorldPosition = worldPos.xyz+ pbrvertex.WorldPositionOffset ;

    pbrvertex.VertexPosition = modifiedPosition; 
    vec4 shadowCoords[4];
    shadowCoords[0] = u_CascadePositions.ViewProjections[0] * vec4(PBR_Output.WorldPosition, 1.0);
	shadowCoords[1] = u_CascadePositions.ViewProjections[1] * vec4(PBR_Output.WorldPosition, 1.0);
	shadowCoords[2] = u_CascadePositions.ViewProjections[2] * vec4(PBR_Output.WorldPosition, 1.0);
	shadowCoords[3] = u_CascadePositions.ViewProjections[3] * vec4(PBR_Output.WorldPosition, 1.0);

    PBR_Output.ShadowMapCoords[0] = vec3(shadowCoords[0].xyz / shadowCoords[0].w);
	PBR_Output.ShadowMapCoords[1] = vec3(shadowCoords[1].xyz / shadowCoords[1].w);
	PBR_Output.ShadowMapCoords[2] = vec3(shadowCoords[2].xyz / shadowCoords[2].w);
	PBR_Output.ShadowMapCoords[3] = vec3(shadowCoords[3].xyz / shadowCoords[3].w);

    mat3 normalMatrix = transpose(inverse(mat3(aTransform)));
    PBR_Output.Normal = mat3(aTransform) * mat3(boneTransform) * pbrvertex.Normal;
    PBR_Output.WorldNormals = mat3(aTransform) * mat3(boneTransform) * mat3(pbrvertex.Tangent, pbrvertex.Bitangent, pbrvertex.Normal);
    PBR_Output.VecWorldNormal = GetWorldNormal(PBR_Output.Normal, aTransform, u_Camera.InverseView);



    PBR_Output.CameraPosition = u_Camera.Position;

    PBR_Output.ViewPosition = vec3(u_Camera.View * vec4(PBR_Output.WorldPosition, 1.0));
    PBR_Output.CameraView = mat3(u_Camera.View);

    vec4 posProjPrev = (u_Camera.PrevViewProjectionMatrix * aPrevTransform * vec4(modifiedPosition, 1.0));
	vec4 posProjCur = (u_Camera.ViewProjectionMatrix * aTransform * vec4(modifiedPosition,1.0));
	
	PBR_Output.NormalizePositionCur = posProjCur;
	PBR_Output.NormalizePositionPrev = posProjPrev;

    vec4 worldPosVec4= vec4(PBR_Output.WorldPosition, 1.0);
   gl_Position =  u_Camera.Projection * u_Camera.View * worldPosVec4 ;
   PBR_Output.ViewSpaceValue =  (u_Camera.View * vec4(PBR_Output.WorldPosition, 1.0)).xyz;

}


void Vertex(inout PBRVertexInput vertexinput);
void main()
{
    
    PBRVertexInput vertexInput;
    vertexInput.VertexPosition = aPosition;
    vertexInput.TexCoords = vec2(aTexCoords.x,1 - aTexCoords.y);
    vertexInput.Tangent = aTangent;
    vertexInput.Bitangent = aBitangent;
    vertexInput.Normal = aNormal;
    vertexInput.WorldPositionOffset = vec3(0);

    Vertex(vertexInput);
    ApplyPbrVertex(vertexInput);
}