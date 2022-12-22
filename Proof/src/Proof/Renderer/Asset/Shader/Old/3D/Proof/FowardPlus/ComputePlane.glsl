#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos,1.0);
}


#Fragment Shader
#version 450 core

struct Plane{
    vec3 Normal;
    float DistanceToOrigin;
}Plane;
struct Frustum
{
    Plane planes[4];   // left, right, top, bottom frustum planes.
}Frustum;

// Four planes of a view frustum (in view space).
// The planes are:
//  * Left,
//  * Right,
//  * Top,
//  * Bottom.
// The back and/or front planes can be computed from depth values in the 
// light culling compute shader.
struct Frustum {
    Plane planes[4];   // left, right, top, bottom frustum planes.
}Frustum;
    
// Compute a plane from 3 noncollinear points that form a triangle.
// This equation assumes a right-handed (counter-clockwise winding order) 
// coordinate system to determine the direction of the plane normal.
Plane ComputePlane(vec3 p0,vec3 p1,vec3 p2);
void main() {

}

Plane ComputePlane(vec3 p0,vec3 p1,vec3 p2){
    Plane plane;

    vec3 v0 = p1 - p0;
    vec3 v2 = p2 - p0;

    plane.Normal = normalize(cross(v0,v2));

    // Compute the distance to the origin using p0.
    plane.DistanceToOrigin = dot(plane.Normal,p0);

    return plane;
}