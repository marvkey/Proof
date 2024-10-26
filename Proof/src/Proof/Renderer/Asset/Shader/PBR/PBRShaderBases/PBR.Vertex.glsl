layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;

#extension GL_ARB_explicit_attrib_location : enable
#include <Common.glslh>
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


void ApplyPbrVertex(PBRVertexInput pbrvertex)
{
    vec3 modifiedPosition = pbrvertex.VertexPosition;

    PBR_Output.Tangent = pbrvertex.Tangent;
    PBR_Output.Bitangent = pbrvertex.Bitangent;
    PBR_Output.TexCoords = pbrvertex.TexCoords;


    vec4 worldPos = aTransform * vec4(modifiedPosition , 1.0);
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
    PBR_Output.Normal = mat3(aTransform) * pbrvertex.Normal;
    PBR_Output.WorldNormals = mat3(aTransform) * mat3(pbrvertex.Tangent, pbrvertex.Bitangent, pbrvertex.Normal);

   // PBR_Output.Normal = normalMatrix * pbrvertex.Normal; 
    //PBR_Output.Normal = mat3(aTransform) * pbrvertex.Normal;
    //PBR_Output.WorldNormals = mat3(aTransform) * mat3(aTangent, aBitangent,aNormal);
  //  PBR_Output.WorldNormals = mat3(aTransform) * mat3(aTangent, aBitangent,aNormal );
        
   // PBR_Output.Tangent = mat3(aTransform) * pbrvertex.Tangent;
    //PBR_Output.Normal = normalMatrix * pbrvertex.Normal;
   // PBR_Output.Bitangent = normalize(cross(PBR_Output.Normal,PBR_Output.Tangent));
   // PBR_Output.WorldNormals = transpose(mat3(PBR_Output.Tangent, PBR_Output.Bitangent, PBR_Output.Normal));
    
    //PBR_Output.Normal = pbrvertex.Normal;
   // PBR_Output.WorldNormals = mat3(aTransform) * transpose(mat3(pbrvertex.Tangent, pbrvertex.Bitangent, pbrvertex.Normal));

    PBR_Output.CameraPosition = u_Camera.Position;

    PBR_Output.ViewPosition = vec3(u_Camera.View * vec4(PBR_Output.WorldPosition, 1.0));
    PBR_Output.CameraView = mat3(u_Camera.View);

    vec4 posProjPrev = (u_Camera.PrevViewProjectionMatrix * aPrevTransform * vec4(modifiedPosition, 1.0));
	vec4 posProjCur = (u_Camera.ViewProjectionMatrix * aTransform * vec4(modifiedPosition,1.0));
	
	PBR_Output.NormalizePositionCur = posProjCur;
	PBR_Output.NormalizePositionPrev = posProjPrev;

   gl_Position =  u_Camera.Projection * u_Camera.View * vec4(PBR_Output.WorldPosition, 1.0);
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