//https://github.com/unrealsid/OpenGL-GPU-Particles-2/blob/main/OpenGL_Particles/OpenGL_Particles/shaders/compute.glsl
#Compute Shader
#version 460
#include <PBR/ParticleSystem/ParticleSystem.glslh>



layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer s_Particles { Particle Particles[]; };

layout(std430, binding=1) coherent restrict buffer particle_index_buffer {
  int[] indices;
} FreeList;

layout(std430, binding=2) coherent restrict buffer FreeListCount {
	int count;
} s_FreeListCount;


layout (std140, binding = 3) uniform EmitterSettings 
{
	vec4 minColor, maxColor;
	vec3 minOffset, maxOffset;
	vec3 minVelocity, maxVelocity;
	vec3 minAccel, maxAccel;
	mat4 modelMatrix;
	mat4 rotationMatrix;
	float minLife, maxLife;
} u_Emitter;

 layout(push_constant) uniform PushConstant
{
    uint ParticleToSpawn;
	uint PaticlesPerThread;
    uint NumParticles;
} pc_PushConstant;

uint rng_state; // global variable in shader
uint rand_xorshift()
{
    rng_state ^= (rng_state << 13);
    rng_state ^= (rng_state >> 17);
    rng_state ^= (rng_state << 5);
    return rng_state;
}

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float random()  // 0 -1
{
	return float(wang_hash(rand_xorshift())) * (1.0 / 4294967296.0);
}

void MakeParticle(out Particle particle) 
{
	particle.Life = max(0.1, mix(u_Emitter.minLife, u_Emitter.maxLife, random()));
	//particle.initialLife = particle.life;

	particle.Position = u_Emitter.modelMatrix[3].xyz;
	vec4 offset = vec4(
						vec3(
							mix(u_Emitter.minOffset.x, u_Emitter.maxOffset.x, random()),
							mix(u_Emitter.minOffset.y, u_Emitter.maxOffset.y, random()), 
							mix(u_Emitter.minOffset.z, u_Emitter.maxOffset.z, random())
						),
						1
				  );
	particle.Position += vec3(u_Emitter.rotationMatrix * offset);
	//particle.scale.xy = vec2(0.1);

	particle.Velocity.x = mix(u_Emitter.minVelocity.x, u_Emitter.maxVelocity.x, random());
	particle.Velocity.y = mix(u_Emitter.minVelocity.y, u_Emitter.maxVelocity.y, random());
	particle.Velocity.z = mix(u_Emitter.minVelocity.z, u_Emitter.maxVelocity.z, random());
	particle.Velocity = vec3(u_Emitter.rotationMatrix * vec4(particle.Velocity, 1));
	
	//particle.accel.x = mix(u_Emitter.minAccel.x, u_Emitter.maxAccel.x, random());
	//particle.accel.y = mix(u_Emitter.minAccel.y, u_Emitter.maxAccel.y, random());
	//particle.accel.z = mix(u_Emitter.minAccel.z, u_Emitter.maxAccel.z, random());
	//particle.accel = vec3(u_Emitter.rotationMatrix * vec4(particle.accel, 1));

	particle.Color = u_Emitter.minColor;
}

void SpawnParticle(uint index) 
{
	if (index >= pc_PushConstant.ParticleToSpawn)
		return;
	// undo decrement and return if nothing in freelist
	int freeListIndex = atomicAdd(s_FreeListCount.count, -1) - 1;
	if (freeListIndex < 0) 
	{
		atomicAdd(s_FreeListCount.count, 1);
		return;
	}

	int particleIndex = FreeList.indices[freeListIndex];

	rng_state = particleIndex;

	//if (u_useShape && shape.verticesCount > 0) {
	//	MakeParticleShape(particles[particleIndex], shape.vertices[particleIndex % shape.verticesCount].xyz);
	//}
	//else {
		MakeParticle(Particles[particleIndex]);
	//}
}


void main() 
{
	uint index = gl_GlobalInvocationID.x * pc_PushConstant.PaticlesPerThread;

	for (int i = 0; i < pc_PushConstant.PaticlesPerThread; i++) 
	{
		SpawnParticle(index + i);
	}
}