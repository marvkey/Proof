//https://github.com/2Retr0/GodotOceanWaves/blob/4e1e219bc8f55c38092ed34e6fee568b900d95b5/assets/shaders/compute/fft_butterfly.glsl
#Compute Shader
#version 460
#include <PBR/ParticleSystem/ParticleSystem.glslh>
#include <Common.glslh>

layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer s_Particles { Particle Particles[]; };
layout(std430, binding=1) coherent restrict buffer particle_index_buffer {
  int[] indices;
} FreeList;

layout(std430, binding=2) coherent restrict buffer FreeListCount {
	int count;
} s_FreeListCount;


 layout(push_constant) uniform PushConstant
{
    uint ParticleToSpawn;
	uint PaticlesPerThread;
    uint NumParticles;
} pc_PushConstant;


float sphereRadius = 5.0f;
float maxLifetime = 10;
vec3 particleEmitterCurrentPos = vec3(0.0f);
vec3 prevParticleEmitterPos = vec3(0.0f);
bool emitterAlive = true;

// Shared memory for particles within a workgroup
shared Particle localParticles[512];

void main() 
{
	uint gid = gl_GlobalInvocationID.x;
    uint lid = gl_LocalInvocationID.x;

    if (gid >= pc_PushConstant.NumParticles)
        return;

    // Load particle into shared memory
    localParticles[lid] = Particles[gid];
    barrier();

    float deltaTime = u_FrameData.DeltaTime;
    if (emitterAlive)
    {
        // Update lifetime
        localParticles[lid].Life -= u_FrameData.DeltaTime;

        // If the particle is dead, respawn it
        if (localParticles[lid].Life <= 0.0)
        {
            // Generate random values
            float u = fract(sin(float(gid) * 12.9898) * 43758.5453);
            float v = fract(sin(float(gid) * 78.233) * 43758.5453);

            float theta = u * 2.0 * 3.14159;
            float phi   = acos(2.0 * v - 1.0);
            float r     = sphereRadius * pow(fract(sin(float(gid) * 34.567) * 43758.5453), 1.0 / 3.0);

            localParticles[lid].Position = vec3(
                particleEmitterCurrentPos.x + r * sin(phi) * cos(theta),
                particleEmitterCurrentPos.y + r * sin(phi) * sin(theta),
                particleEmitterCurrentPos.z + r * cos(phi)
            );

            localParticles[lid].Size3D = vec3(0.2);

            localParticles[lid].Color = vec4(
                0.9, 0.9, 0.9,
                0.5 + 0.5 * fract(sin(float(gid) * 12.7) * 43758.5453)
            );

            localParticles[lid].Velocity = vec3(
                mix(-0.2, 0.2, fract(sin(float(gid) * 32.8) * 43758.5453)),
                0.5 + 0.5 * fract(sin(float(gid) * 94.2) * 43758.5453),
                mix(-0.2, 0.2, fract(sin(float(gid) * 70.3) * 43758.5453))
            );

            localParticles[lid].Life = maxLifetime * fract(sin(float(gid) * 18.6) * 43758.5453);
        }
        else
        {
            // integrate motion
            localParticles[lid].Position += localParticles[lid].Velocity * deltaTime;

            // base delta (follow emitter)
            vec3 baseDelta = particleEmitterCurrentPos - prevParticleEmitterPos;
            localParticles[lid].Position += baseDelta;

            // wind
            localParticles[lid].Velocity.x += 0.05 * deltaTime *
                sin(localParticles[lid].Position.y * 0.5 + deltaTime * 0.2);

            // lifetime % for scaling + fade
            float lifePercent = 1.0 - (localParticles[lid].Life / maxLifetime);


            float minSize = 0.8;
            float maxSize = 2.0;
            float sizeScale = mix(0.8, 2.0, lifePercent);;
            localParticles[lid].Size3D = localParticles[lid].Size3D * sizeScale;

            localParticles[lid].Color.a  = (1.0 - lifePercent) * 0.7;

            // turbulence
            float turbulenceX = sin(localParticles[lid].Position.y * 8.0 + deltaTime) * 0.3;
            float turbulenceZ = cos(localParticles[lid].Position.y * 5.0 + deltaTime * 1.5) * 0.1;
            localParticles[lid].Position.x += turbulenceX * deltaTime;
            localParticles[lid].Position.z += turbulenceZ * deltaTime;

            // slow rise
            localParticles[lid].Velocity.y *= (1.0 - 0.1 * deltaTime);
        }
    }
    else
    {
        // fade out if emitter is gone
        float fadeOutSpeed = 1.0;
        localParticles[lid].Color.a -= fadeOutSpeed * deltaTime;
        localParticles[lid].Color.a = max(localParticles[lid].Color.a, 0.0);
    }

    barrier();
   Particles[gid] = localParticles[lid];

}