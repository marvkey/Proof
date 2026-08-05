


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

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoords;
layout(location = 2) flat out uint v_Active;

void main()
{
    vec3 camRight = vec3(u_Camera.View[0][0], u_Camera.View[1][0], u_Camera.View[2][0]);
    vec3 camUp = vec3(u_Camera.View[0][1], u_Camera.View[1][1], u_Camera.View[2][1]);
    // vec3 camForward = normalize(-vec3(u_Camera.InverseView[2].xyz)); // +Z (if needed)

    uint index = gl_InstanceIndex;
    Particle particle = particles[index];

    v_Color = particle.Color;
    v_TexCoords = aTexCoords;
    v_Active = uint(particle.bActive);

    vec2 particleScale = particle.Size3D.xy;

    vec3 vertexPosition =
    particle.Position.xyz +
    camRight * aPosition.x * particleScale.x +
    camUp * aPosition.y * particleScale.y;

    gl_Position = u_Camera.Projection * u_Camera.View * vec4(vertexPosition, 1.0);
}

#Fragment Shader
#version 450 core

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoords;
layout(location = 2) flat in uint v_Active;

layout(set = 0, binding = 1) uniform sampler2D u_Texture;

void main()
{
    if (v_Active == 0)
    discard;

    vec4 texColor = texture(u_Texture, -v_TexCoords);

    if (texColor.a < 0.001)
    discard;

    if (v_Color.a < 0.001)
    discard;

    outColor = texColor * v_Color;
}