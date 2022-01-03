#include "Collider.h"
#include "SphereCollider.h"
#include "CubeCollider.h"
namespace ProofPhysicsEngine {
	IntersectData Collider::Intersect(const Collider& other)const{
		if (m_Type == ColliderType::Sphere && other.m_Type == ColliderType::Sphere) // spehrer on sphere
			return ((SphereCollider*)this)->IntersectSphereCollider((SphereCollider&)other);
		
		if (m_Type == ColliderType::Cube && other.m_Type == ColliderType::Cube)// cube on cube
			return ((CubeCollider*)this)->IntersectCubeCollider((CubeCollider&)other);

		/*SPHERE ON CUBE COLLIDER*/
		/*---------------------------------------------------------------------------------*/
		if(m_Type == ColliderType::Cube && other.m_Type== ColliderType::Sphere )
			return ((CubeCollider*)this)->IntersectSphereCollider((SphereCollider&)other);

		if (m_Type == ColliderType::Sphere && other.m_Type == ColliderType::Cube)
			return ((SphereCollider*)this)->IntersectCubeCollider((CubeCollider&)other);
		/*---------------------------------------------------------------------------------*/

		return IntersectData(false, 0);
	}
}