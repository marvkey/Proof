


#Vertex Shader
#version 450


#include <PBR/ParticleSystem/ParticleSystem.glslh>
#include <Common.glslh>


layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;


layout(std430, binding = 0) readonly restrict buffer s_Particles 
{
    Particle particles[];
};

struct VertexOutput
{
    vec4 Color;
};
layout(location = 0) out VertexOutput Output;

void main()
{
vec3 camRight = normalize(vec3(u_Camera.InverseView[0].xyz)); // +X
vec3 camUp    = normalize(vec3(u_Camera.InverseView[1].xyz)); // +Y
// vec3 camForward = normalize(-vec3(u_Camera.InverseView[2].xyz)); // +Z (if needed)

    uint index = gl_InstanceIndex;

    Particle particle = particles[index];
    // override particle pos to (0,0,0)

    vec2 particlescale = vec2(3.0);

    vec3 vertexPosition =
        particle.Position.xyz + 
        camRight * aPosition.x * particlescale.x +
        camUp * aPosition.y * particlescale.y;

    Output.Color = vec4(1.0f);
    // transform with camera’s view-projection
    gl_Position = u_Camera.ViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

#Fragment Shader
#version 450 core
layout(location=0) out vec4 outColor;

struct VertexOutput
{
    vec4 Color;
};
layout(location = 0) in VertexOutput Input;

void main()
{
    outColor = Input.Color; // white points
}