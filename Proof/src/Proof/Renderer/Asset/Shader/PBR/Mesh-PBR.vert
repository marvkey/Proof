
#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outCameraPostion;
layout(location = 4) out vec3 outTangent;
layout(location = 5) out vec3 outBitangent;
layout(set = 0, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}CameraUBO;
void main() {
    outTangent = aTangent;
    outBitangent = aBitangent;
    outTexCoords = aTexCoords;
    outWorldPos = vec3(aTransform * vec4(aPosition, 1.0));
    outNormal = mat3(aTransform) * aNormal;
    outCameraPostion = CameraUBO.Position;
    gl_Position = CameraUBO.ProjectionMatrix * CameraUBO.ViewMatrix * vec4(outWorldPos, 1.0);

}