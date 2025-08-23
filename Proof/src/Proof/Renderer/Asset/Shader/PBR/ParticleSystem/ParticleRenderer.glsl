


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
    vec2 TexCoords;
    vec2 dontShowColor;
};
layout(location = 0) out VertexOutput Output;

void main()
{

  vec3 camRight = vec3(u_Camera.View[0][0], u_Camera.View[1][0], u_Camera.View[2][0]);
    vec3 camUp = vec3(u_Camera.View[0][1], u_Camera.View[1][1], u_Camera.View[2][1]);
// vec3 camForward = normalize(-vec3(u_Camera.InverseView[2].xyz)); // +Z (if needed)

    uint index = gl_InstanceIndex;

    Particle particle = particles[index]; 

    vec2 particlescale = particle.Size3D.xy;

    vec3 vertexPosition =
        particle.Position.xyz + 
        camRight * aPosition.x * particlescale.x +
        camUp * aPosition.y * particlescale.y;

    Output.Color = particle.Color;
    Output.TexCoords = aTexCoords;

    if(particle.bActive == 0)
    {
        Output.dontShowColor = vec2(1.0);
        Output.Color.a = 0.0;
    }else
    {
        Output.dontShowColor = vec2(0.0);

    }
    // transform with camera’s view-projection
    gl_Position = u_Camera.Projection * u_Camera.View * vec4(vertexPosition, 1.0);
}

#Fragment Shader
#version 450 core
layout(location=0) out vec4 outColor;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoords;
    vec2 dontShowColor;
};
layout(location = 0) in VertexOutput Input;
layout(set = 0, binding = 1) uniform sampler2D u_Texture;

void main()
{
vec4 texColor = texture(u_Texture, -Input.TexCoords);
    if (texColor.a < 0.1)
        discard;

    if(Input.Color.a < 0.1)
        discard;
    if(Input.dontShowColor == vec2(1.0))
        discard;

    outColor = texColor * Input.Color;
}