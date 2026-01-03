#Compute Shader
#version 450

// match thread group size
layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

struct Boid {
    vec3 position;

    vec3 direction;

    vec3 flockHeading;

    vec3 flockCentre;

    vec3 avoidanceHeading;

    int numFlockmates;
};

// Structured buffer of boids
layout(std430, binding = 0) buffer BoidBuffer {
    Boid boids[];
};

// Uniforms
layout(push_constant) uniform BoidParams {
    int numBoids;
    float viewRadius;
    float avoidRadius;
} u_Params;

void main()
{
    uint id = gl_GlobalInvocationID.x;

    if (id >= uint(u_Params.numBoids))
        return;

    Boid boidA = boids[id];

    
    for (int indexB = 0; indexB < u_Params.numBoids; indexB++)
    {
        if (int(id) == indexB)
            continue;

        Boid boidB = boids[indexB];
        vec3 offset = boidB.position - boidA.position;
        float sqrDst = dot(offset, offset);

        if (sqrDst < u_Params.viewRadius * u_Params.viewRadius)
        {
            boidA.numFlockmates += 1;
            boidA.flockHeading += boidB.direction;
            boidA.flockCentre  += boidB.position;

            if (sqrDst < u_Params.avoidRadius * u_Params.avoidRadius)
            {
                boidA.avoidanceHeading -= offset / sqrDst;
            }
        }
    }

    // Write back to buffer
    boids[id] = boidA;
}


