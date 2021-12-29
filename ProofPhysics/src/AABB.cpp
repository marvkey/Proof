#include "AABB.h"
#include <math.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
namespace ProofPhysicsEngine {
    IntersectData AABB::IntersectAABB(const AABB& other) const
    {
        // depeind on teh orientation we do it 
        // one of this distnces will give us a negative number while the other gives a positive
        // so we would take the positve one 
        glm::vec3 distances1 = other.m_MinExtents - m_MaxExtents;
        glm::vec3 distances2 = m_MinExtents - other.m_MaxExtents;
        glm::vec3 distance = glm::max(distances1, distances2);

        float maxXorY = fmaxf(distance.x, distance.y);
        float maxDistance = fmaxf(maxXorY, distance.z   );
        // maxDistance<0  if the max distance is greater than 0 there some 
        // amount of distnac eon some axis
        return IntersectData(maxDistance <0, maxDistance);
    }

}