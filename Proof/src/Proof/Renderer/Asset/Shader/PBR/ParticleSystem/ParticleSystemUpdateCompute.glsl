//https://github.com/2Retr0/GodotOceanWaves/blob/4e1e219bc8f55c38092ed34e6fee568b900d95b5/assets/shaders/compute/fft_butterfly.glsl
#Compute Shader
#version 460
#include <PBR/ParticleSystem/ParticleSystem.glslh>
#include <Common.glslh>

layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer s_Particles { Particle Particles[]; };

layout(std430, binding=1) coherent restrict buffer ParticleInitialState 
{
	float Duration;
    int bLooping;
    float StartLifetime;
    float StartSpeed;

    vec3 StartSize;
    float GravityModifier;

    vec4 StartColor;

    vec3 EmitterPosition;
    float FadeOutSpeed; // particle duration over how fast remaingin particles fade out

    vec3 EmitterPrevPosition;

} s_InitialState;

layout(std430, binding = 2) coherent restrict buffer EmitterSettings
{
	ParticleEmitterSettings Settings;
} s_EmitterSettings;

layout(std430, binding = 3) coherent restrict buffer TrackableData
{
    float TimeElapsed;
	int ActiveParticles;
	int DeadParticles;
    int MaxParticles; // max particles not edited
}s_TrackableData;

// Shared memory for particles within a workgroup
shared Particle localParticles[512];

// cheap deterministic RNG in [0,1)
float randRNG(uint seed, float salt) 
{
    return fract(sin(float(seed) * salt) * 43758.5453);
}

void RespawnParticleSphere(inout Particle particle,uint gid, uint lid)
{
    // Random values for spherical coords
    float u = randRNG(gid, 12.9898);
    float v = randRNG(gid, 78.233);

    float sphereRadius = 5;

    float theta = u * 6.2831853;          // 2π
    float phi   = acos(2.0 * v - 1.0);
    float r     = sphereRadius * pow(randRNG(gid, 34.567), 1.0/3.0);

    particle.Position = vec3(
        s_InitialState.EmitterPosition.x + r * sin(phi) * cos(theta),
        s_InitialState.EmitterPosition.y + r * sin(phi) * sin(theta),
        s_InitialState.EmitterPosition.z + r * cos(phi)
    );

    particle.Size3D = s_InitialState.StartSize;
    particle.Color = vec4(1.0f);
    //particle.Color = s_InitialState.StartColor;

    particle.Velocity = vec3(
        mix(-0.2, 0.2, randRNG(gid, 32.8)),
        0.5 + 0.5 * randRNG(gid, 94.2),
        mix(-0.2, 0.2, randRNG(gid, 70.3))
    );

    particle.Velocity *= s_InitialState.StartSpeed;
    particle.Life = s_InitialState.StartLifetime;
}
void RespawnParticle(inout Particle particle,uint gid, uint lid)
{
    RespawnParticleSphere(particle,gid,lid);
}

void UpdateParticle(inout Particle particle,uint gid, uint lid)
{
    float deltaTime = u_FrameData.DeltaTime;

 // integrate motion
    particle.Position += particle.Velocity * deltaTime;

    // base delta (follow emitter)
    vec3 baseDelta = s_InitialState.EmitterPosition - s_InitialState.EmitterPrevPosition;
    particle.Position += baseDelta;

    // wind
    //p.Velocity.x += 0.05 * deltaTime *
    //    sin(p.Position.y * 0.5 + deltaTime * 0.2);

    // lifetime % for scaling + fade
    float lifePercent = 1.0 - (particle.Life / s_InitialState.StartLifetime);

    //float minSize = 0.8;
    //float maxSize = 2.0;
    //float sizeScale = mix(minSize, maxSize, lifePercent);
    //p.Size3D = p.Size3D * sizeScale;

   // particle.Color.a = (1.0 - lifePercent) * 0.7;

    // turbulence
    float turbulenceX = sin(particle.Position.y * 8.0 + deltaTime) * 0.3;
    float turbulenceZ = cos(particle.Position.y * 5.0 + deltaTime * 1.5) * 0.1;
    particle.Position.x += turbulenceX * deltaTime;
    particle.Position.z += turbulenceZ * deltaTime;

    // slow rise
    //p.Velocity.y *= (1.0 - 0.1 * deltaTime);
}
void main() 
{
	uint gid = gl_GlobalInvocationID.x;
    uint lid = gl_LocalInvocationID.x;

    // initial run
    if (gl_GlobalInvocationID.x == 0 && gl_GlobalInvocationID.y == 0 && gl_GlobalInvocationID.z == 0) 
    {
         s_TrackableData.ActiveParticles = 0;
         s_TrackableData.DeadParticles = 0;
        // Update elapsed time ONCE
        s_TrackableData.TimeElapsed += u_FrameData.DeltaTime;
    }

    if (gid >= s_TrackableData.MaxParticles)
        return;

    // Load particle into shared memory
    localParticles[lid] = Particles[gid];

    barrier();

    float deltaTime = u_FrameData.DeltaTime;

    if(s_InitialState.Duration < s_TrackableData.TimeElapsed)
    {
        if(s_InitialState.bLooping == int(false))
        {
            // fade out if emitter is gone
           // float fadeOutSpeed = 1.0;
           // localParticles[lid].Color.a -= s_InitialState.FadeOutSpeed * deltaTime;
           // localParticles[lid].Color.a = max(localParticles[lid].Color.a, 0.0);
            return;
        }
    }
        
    Particle currentParticle = localParticles[lid];
    currentParticle.Life -= u_FrameData.DeltaTime;

    // If the particle is dead, respawn it
    if (localParticles[lid].Life <= 0.0)
    {
        RespawnParticle(currentParticle,gid,lid);
    }
    else
    {
        UpdateParticle(currentParticle,gid,lid);
    }

    // Classify as alive or dead AFTER update
    if (currentParticle.Life > 0.0)
        atomicAdd(s_TrackableData.ActiveParticles, 1);
    else
        atomicAdd(s_TrackableData.DeadParticles, 1);

    barrier();
    Particles[gid] = localParticles[lid];
}