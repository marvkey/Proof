#include "SphereCollider.h"
#include "CubeCollider.h"
namespace ProofPhysicsEngine {
	
	IntersectData SphereCollider::IntersectSphereCollider(const SphereCollider& other){
		float radiusDistance = Radius + other.Radius; // how fare away are they from touching
		float CenterDistance = (other.Center - Center).GetLength();
		float distance = CenterDistance - radiusDistance;
		return IntersectData( CenterDistance < radiusDistance, distance );
		//CenterDistance < radiusDistance // if it is equal the spheres are touched  
	}
	IntersectData SphereCollider::IntersectCubeCollider(const CubeCollider& other){
		return other.IntersectSphereCollider(*this);
	}
	uint32_t SphereCollider::SphereAndCube(const SphereCollider& sphere, const CubeCollider& box, CollisionData& data) {
		// Early-out check to see if we can exclude the contact.
		//For each test axis we simply check if the half-width of the box plus the radius of
		//the sphere is greater than one component of the relative position of the sphere center
		//(i.e., the transformed position we found earlier).See the next block of code.
		Proof::Vector <float> sphereCenter = sphere.Center;
		Proof::Vector <float> boxCener = box.Center*box.GetOrientation(); // need to add rotation
		if (abs(boxCener.X) - sphere.Radius > boxCener.X/2 ||
			abs(boxCener.Y) - sphere.Radius > boxCener.Y/2 ||
			abs(boxCener.Z) - sphere.Radius > boxCener.Z/2) {
			return 0;
		}

		Proof::Vector<> closestPt = box.ClosestPoint(sphere.Center);

		float  dist;
		dist = (closestPt - boxCener).GetLengthSquared();
		if (dist > sphere.Radius * sphere.Radius) return 0;

		// Compile the contact.
		Proof::Vector<> closestPtWorld = box.ClosestPoint(closestPt);
		ContactData* contact = data.Contacts;
		contact->ContactNormal = (sphereCenter - closestPtWorld);
		contact->ContactNormal.Normalize();
		contact->ContactPoint = closestPtWorld;
		contact->penetration = sphere.Radius - sqrtf(dist);
		// Write the appropriate data.
		contact->body[0] = box.GetBody();
		contact->body[1] = sphere.GetBody();
		contact->Restitution = data.Restitution;
		contact->Friction = data.Friction;
	}
	uint32_t SphereCollider::SphereAndSphere(const SphereCollider& one, const SphereCollider& two, CollisionData& data) {
		// chcking if we have collsion left
		if (data.ContactsLeft <= 0)return 0;
		// findig the vector betwee objects
		Proof::Vector<float> midline = one.Center - two.Center;
		// if it is large enogu
		float size = midline.GetLength();
		if (size <= 0 || size >= one.Radius + two.Radius)
			return 0;
		// we can creata normal since we have size at hand
		// size to hand.
		Proof::Vector<float> normal = midline * (((float)1.0) / size);
		ContactData* contact = data.Contacts;
		contact->ContactNormal = normal;
		contact->ContactNormal = one.Center + midline * (float)0.5;
		contact->penetration = (one.Radius + two.Radius - size);
		// Write the appropriate data.
		contact->body[0] = one.GetBody();
		contact->body[1] = two.GetBody();
		contact->Restitution = data.Restitution;
		contact->Friction = data.Friction;

		return 1;
	}
} 

