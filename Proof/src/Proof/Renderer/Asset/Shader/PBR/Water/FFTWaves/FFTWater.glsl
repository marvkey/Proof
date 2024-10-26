#Vertex Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>
#include <PBR/Water/FFTWaves/FFTGlobal.glslh>
#include <PBR/Water/FFTWaves/OceanClipMap.glslh>


layout(std140, set=0, binding = 0) uniform FFTWaterInputBuffer
{
	 float UVWarpStrength;
    float CascadesFadesDist;
} u_FFTWaterInput;

struct VertexOutput
{
    vec4 PositionHCS;
    vec3 PositionWS;
    float ViewDepth;
    vec4 PositionNDC;
    vec2 WorldUV;
    float CascadesFadesDist;
};
#include <PBR/Water/FFTWaves/OceanSimulation.glslh>

layout(location = 23) out VertexOutput Output;

void Vertex(inout PBRVertexInput vertexInput)
{
Output.CascadesFadesDist =u_FFTWaterInput.CascadesFadesDist;
	Output.PositionWS = ClipMapVertex(vertexInput.VertexPosition,vertexInput.TexCoords);
	Output.WorldUV = Output.PositionWS.xz;

	vec3 viewVector = Output.PositionWS - u_Camera.Position;
	float viewDist = length(viewVector);
    float viewDistXzSquared = dot(viewVector.xz, viewVector.xz);

	float warpDistance = GlobalStruct.OceanLengthScales.x * 0.5;
    Output.WorldUV += sin(Output.WorldUV.yx / warpDistance)
        * min(1.0, viewDistXzSquared / (warpDistance * warpDistance * 100.0))
        * warpDistance * 0.4 * u_FFTWaterInput.UVWarpStrength;


	vec4 weights = LodWeights(viewDist, u_FFTWaterInput.CascadesFadesDist);
    Output.PositionWS += SampleDisplacement(Output.WorldUV, weights, vec4(1.0));

	vec3 positionOS = TransformWorldToObject(aTransform,Output.PositionWS);

	VertexPositionInputs positionInputs = GetVertexPositionInputs(aTransform,positionOS);
    Output.ViewDepth = -positionInputs.PositionVS.z;
    Output.PositionNDC = positionInputs.PositionNDC;
    Output.PositionHCS = positionInputs.PositionCS;
}

#Fragment Shader
#version 450 core

//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.FragmentTransperant.glsl>
#include <PBR/Water/FFTWaves/FFTGlobal.glslh>
#include <PBR/Water/FFTWaves/OceanFoam.glslh>
#include <PBR/Water/FFTWaves/OceanSurface.glslh>
//#include <PBR/Water/FFTWaves/OceanSimulation.glsl>



struct VertexOutput
{
     vec4 PositionHCS;
    vec3 PositionWS;
    float ViewDepth;
    vec4 PositionNDC;
    vec2 WorldUV;
    float CascadesFadesDist;
};
layout(location = 23) in VertexOutput Input;

void Fragment(inout PBRData pbrData)
{
    vec3 viewDir = u_Camera.Position - Input.PositionWS;
    float viewDist = length(viewDir);
    viewDir = viewDir / viewDist;

    vec4 lodWeights = LodWeights(viewDist, Input.CascadesFadesDist);

    vec4 shoreWeights = vec4(1);// ShoreModulation(i.shore.x);
    mat4 derivatives = SampleDerivatives(Input.WorldUV, lodWeights * shoreWeights);
    vec3 normal = NormalFromDerivatives(derivatives, vec4(1));

     FoamInput fi;
    fi.derivatives = derivatives;
    fi.worldUV = Input.WorldUV;
    fi.viewDist = viewDist;
    fi.lodWeights = lodWeights;
    fi.shoreWeights = shoreWeights;
    fi.positionNDC = Input.PositionNDC;
    fi.viewDepth = Input.ViewDepth;
    fi.time = u_FrameData.AppTimeSeconds;
    fi.viewDir = viewDir;
    fi.normal = normal;
    FoamData foamData = GetFoamData(fi);

    DirectionalLight dirLight = u_DirectionalLightData.Lights[0];

    float roughnessMap = texture(u_DistanceRoughnessMap,TransformTex(Input.WorldUV,GlobalStruct.DistanctRoughnessMap_ST)*0.001).r;

    roughnessMap *= Saturate(viewDist /GlobalStruct.OceanLengthScales.x * 0.05);

      LightingInput li;
                li.normal = normal;
                li.viewDir = viewDir;
                li.viewDist = viewDist;
                li.roughnessMap = roughnessMap;
                li.positionWS = Input.PositionWS;
                li.shore = vec4(0);
                li.positionNDC = Input.PositionNDC;
                li.viewDepth = Input.ViewDepth;
                li.cameraPos = u_Camera.Position;
                li.mainLight = dirLight;

    bool backface = dot(normal, viewDir) < 0;
    vec3 oceanColor;

    if (backface)
        li.normal = reflect(li.normal, li.viewDir);
        oceanColor = GetOceanColor(li, foamData);
   
   pbrData.Albedo = oceanColor;
pbrData.Metalness = 0;
pbrData.Roughness = 0.7;
pbrData.Normal = normal;
pbrData.Alpha = 1.0;
}