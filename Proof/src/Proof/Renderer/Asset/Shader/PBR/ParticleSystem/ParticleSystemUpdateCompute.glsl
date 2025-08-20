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

layout(std430, binding = 2) buffer EmitterSettings
{
    ParticleEmission Emission;
    ParticleShape Shape;
    ParticleVelocityOverLifetime VelocityOverLifetime;
    ParticleColorOverLifetime ColorOverLife;
    ParticleSizeOverLifetime SizeOverLifeTime;
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
vec3 randomUnitVector(uint gid, float saltA, float saltB) {
    float u = randRNG(gid, saltA);
    float v = randRNG(gid, saltB);
    float az = u * 6.2831853;               // 2π
    float cz = 2.0 * v - 1.0;               // cos(phi) in [-1,1]
    float sz = sqrt(max(0.0, 1.0 - cz*cz));
    return vec3(sz * cos(az), cz, sz * sin(az));
}

// Sample a random direction within a cone around +Y with half-angle `alphaRad`
vec3 sampleConeDirection(uint gid, float alphaRad) {
    float cosMin = cos(alphaRad);
    float t      = randRNG(gid, 17.31);              // pick band in [cosMin, 1]
    float cosTh  = mix(1.0, cosMin, t);
    float sinTh  = sqrt(max(0.0, 1.0 - cosTh*cosTh));
    float az     = randRNG(gid, 61.99) * 6.2831853;  // 2π
    return vec3(sinTh * cos(az), cosTh, sinTh * sin(az)); // +Y is axis
}

void RespawnParticleCone(inout Particle particle, uint gid,uint lid)
{
    vec3  center   = s_InitialState.EmitterPosition;
    float R        = s_EmitterSettings.Shape.ConeRadius;
    float angleRad = radians(s_EmitterSettings.Shape.ConeAngleDegrees);

    // --- Spawn position: on a disk in XZ at the emitter (Unity "Base" disk)
    // RandomizePosition: 0 = ring at R, 1 = uniform fill
    float a0   = randRNG(gid, 11.11) * 6.2831853; // azimuth
    float rU   = randRNG(gid, 22.22);             // for radius
    float r    = mix(R, R * sqrt(rU), clamp(s_EmitterSettings.Shape.RandomizePosition, 0.0, 1.0));
    vec3  spawnPos = center + vec3(r * cos(a0), 0.0, r * sin(a0));

    // --- Direction: inside cone around +Y
    vec3 dirCone = sampleConeDirection(gid, angleRad);

    // RandomizeDirection: blend with a totally random direction (wider spread)
    float rndAmt = clamp(s_EmitterSettings.Shape.RandomizeDirection, 0.0, 1.0);
    vec3  dirRnd = randomUnitVector(gid, 33.33, 44.44);
    vec3  dir    = normalize(mix(dirCone, dirRnd, rndAmt));
    //dir = u_EmitterRotation * dir;

    // SpherizeDirection: pull direction back toward the cone axis (+Y)
    float sphAmt = clamp(s_EmitterSettings.Shape.SpherizeDirection, 0.0, 1.0);
    dir = normalize(mix(dir, vec3(0.0, 1.0, 0.0), sphAmt));

    // --- Final write
    particle.Position = spawnPos;
    particle.Velocity = dir * s_InitialState.StartSpeed;
    particle.Size3D   = s_InitialState.StartSize;
    particle.Color    = s_InitialState.StartColor;
    particle.Life     = s_InitialState.StartLifetime;
}

void RespawnParticleSphere(inout Particle particle,uint gid, uint lid)
{
    // -------- Sample point on/inside sphere (Unity-like "radius thickness")
    float R   = s_EmitterSettings.Shape.SphereRadius;
    float u   = randRNG(gid, 11.13);     // dir azimuth
    float v   = randRNG(gid, 71.77);     // dir polar
    float u2  = randRNG(gid, 29.91);     // for radius/volume
    float twoPi = 6.2831853;

    // Unit direction on sphere
    float theta = u * twoPi;
    float z     = 2.0 * v - 1.0;                   // cos(phi) in [-1,1]
    float xy    = sqrt(max(0.0, 1.0 - z*z));
    vec3 unitOnSphere = vec3(xy * cos(theta), xy * sin(theta), z);

    // RandomizePosition acts like Unity's radius thickness (0 = surface, 1 = full volume)
    // Uniform volume radius uses cbrt(rand). Blend between surface and volume.
    float rSurface = R;
    float rVolume  = R * pow(u2, 1.0/3.0);
    float r        = mix(rSurface, rVolume, clamp(s_EmitterSettings.Shape.RandomizePosition, 0.0, 1.0));

    vec3 center   = s_InitialState.EmitterPosition;
    vec3 spawnPos = center + unitOnSphere * r;

    // -------- Direction (Unity-like: randomize, then spherize toward surface normal)
    // Start with the perfect radial direction (sphere normal)
    vec3 radialDir = unitOnSphere;

    // Make a truly random unit direction
    float u3 = randRNG(gid, 93.31);
    float v3 = randRNG(gid, 37.42);
    float th3 = u3 * twoPi;
    float z3  = 2.0 * v3 - 1.0;
    float xy3 = sqrt(max(0.0, 1.0 - z3*z3));
    vec3 randomDir = vec3(xy3 * cos(th3), xy3 * sin(th3), z3);

    // Apply RandomizeDirection: blend radial with random to add spread
    float rndDirAmt = clamp(s_EmitterSettings.Shape.RandomizeDirection, 0.0, 1.0);
    vec3 dir = normalize(mix(radialDir, randomDir, rndDirAmt));

    // Apply SpherizeDirection: pull direction back toward the surface normal
    float spherizeAmt = clamp(s_EmitterSettings.Shape.SpherizeDirection, 0.0, 1.0);
    dir = normalize(mix(dir, radialDir, spherizeAmt));

    // -------- Write particle state
    particle.Position = spawnPos;
    particle.Velocity = dir * s_InitialState.StartSpeed;
    particle.Size3D   = s_InitialState.StartSize;
    particle.Color    = s_InitialState.StartColor;
    particle.Life     = s_InitialState.StartLifetime;
}
void RespawnParticle(inout Particle particle,uint gid, uint lid)
{
    if (s_EmitterSettings.Shape.bEnabled == 0) 
    {
        // Fallback: simple upward spawn
        particle.Position = s_InitialState.EmitterPosition;
        particle.Velocity = vec3(0.0, s_InitialState.StartSpeed, 0.0);
        particle.Size3D   = s_InitialState.StartSize;
        particle.Color    = s_InitialState.StartColor;
        particle.Life     = s_InitialState.StartLifetime;
        return;
    }
    if(s_EmitterSettings.Shape.Shape == PARTICLE_SHAPE_CONE)
    {
        RespawnParticleCone(particle,gid,lid);
    }
    else if(s_EmitterSettings.Shape.Shape == PARTICLE_SHAPE_SPHERE)
    {
        RespawnParticleSphere(particle,gid,lid);
    }
}



void UpdateVelocityOverLifeTime(inout Particle particle,uint gid, uint lid,ParticleVelocityOverLifetime velocityOverLifeTime)
{
    if (velocityOverLifeTime.bEnabled == 0)
        return;

    float deltaTime = u_FrameData.DeltaTime;

    // Radial velocity from emitter center
    vec3 radialDir = normalize(particle.Position - s_InitialState.EmitterPosition);
    vec3 radialVelocity = radialDir * velocityOverLifeTime.Radial;

    // Orbital (perpendicular) velocity (around Y axis for simplicity)
    vec3 orbitalDir = vec3(-radialDir.z, 0.0, radialDir.x); // 90 deg rotate on XZ plane
    vec3 orbitalVelocity = orbitalDir * velocityOverLifeTime.Orbital;

    // Apply linear velocity
    particle.Velocity += velocityOverLifeTime.Linear * velocityOverLifeTime.SpeedModifier * deltaTime;

    // Apply orbital and radial components
    particle.Velocity += (radialVelocity + orbitalVelocity) * deltaTime;

    // Apply constant offset directly to position
    particle.Position += velocityOverLifeTime.Offset * deltaTime;
}

void UpdateColorOverLifetime(inout Particle particle,uint gid, uint lid,ParticleColorOverLifetime colorOverLifetime)
{
    if (colorOverLifetime.bEnabled != 0 && colorOverLifetime.bEnabled != 1)
    {
        particle.Color = vec4(1, 0, 1, 1); // bright magenta: shows invalid data
    }
    if (colorOverLifetime.bEnabled == 0)
        return;

    float lifePercent = (particle.Life / s_InitialState.StartLifetime);
    lifePercent = clamp(lifePercent, 0.0, 1.0);

    particle.Color = mix(colorOverLifetime.FinalColor, s_InitialState.StartColor, lifePercent);
}

void UpdateSizeOverLifetime(inout Particle particle,uint gid, uint lid,ParticleSizeOverLifetime sizeOverLifeTime)
{
    if (sizeOverLifeTime.bEnabled == 0)
            return;

    float lifePercent = (particle.Life / s_InitialState.StartLifetime);
    lifePercent = clamp(lifePercent, 0.0, 1.0);

    particle.Size3D = mix(sizeOverLifeTime.FinalSize, s_InitialState.StartSize , lifePercent);
}

void UpdateParticle(inout Particle particle,uint gid, uint lid)
{
    UpdateVelocityOverLifeTime(particle,gid,lid,s_EmitterSettings.VelocityOverLifetime);
    UpdateColorOverLifetime(particle,gid,lid,s_EmitterSettings.ColorOverLife);
    UpdateSizeOverLifetime(particle,gid,lid,s_EmitterSettings.SizeOverLifeTime);

    float deltaTime = u_FrameData.DeltaTime;

    // integrate motion
    particle.Position += particle.Velocity * deltaTime;

    // base delta (follow emitter)
    vec3 baseDelta = s_InitialState.EmitterPosition - s_InitialState.EmitterPrevPosition;
    particle.Position += baseDelta;


    // turbulence
    //float turbulenceX = sin(particle.Position.y * 8.0 + deltaTime) * 0.3;
    //float turbulenceZ = cos(particle.Position.y * 5.0 + deltaTime * 1.5) * 0.1;
    //
    //particle.Position.x += turbulenceX * deltaTime;
    //particle.Position.z += turbulenceZ * deltaTime;

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
    /*
    if(s_InitialState.Duration < s_TrackableData.TimeElapsed)
    {
        if(s_InitialState.bLooping == int(false))
        {
            // fade out if emitter is gone
           // float fadeOutSpeed = 1.0;
           // localParticles[lid].Color.a -= s_InitialState.FadeOutSpeed * deltaTime;
           // localParticles[lid].Color.a = max(localParticles[lid].Color.a, 0.0);
            //return;
        }
    }
    */
        
   // Particle currentParticle = localParticles[lid];
    localParticles[lid].Life -= u_FrameData.DeltaTime;

    // If the particle is dead, respawn it
    if (localParticles[lid].Life <= 0.0)
    {
        RespawnParticle(localParticles[lid],gid,lid);
    }
    else
    {
        UpdateParticle(localParticles[lid],gid,lid);
    }

    // Classify as alive or dead AFTER update
    if (localParticles[lid].Life > 0.0)
        atomicAdd(s_TrackableData.ActiveParticles, 1);
    else
        atomicAdd(s_TrackableData.DeadParticles, 1);

    barrier();
    Particles[gid] = localParticles[lid];
}