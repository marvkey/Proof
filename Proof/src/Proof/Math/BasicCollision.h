#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace Proof
{
	struct AABB;
	struct Ray;
	class Camera;
	struct OrientedBoundingBox;
	struct Plane
	{
		// unit vector
		glm::vec3 Normal = { 0.f, 1.f, 0.f };

		// distance from origin to the nearest point in the plane
		float Distance = 0.f;


		float GetSignedDistanceToPlane(const glm::vec3& point) const;
	};

	struct Frustum
	{
		Plane TopFace;
		Plane BottomFace;

		Plane RightFace;
		Plane LeftFace;

		Plane FarFace;
		Plane NearFace;

		 
		static Frustum CreateFrustrum(const class Camera* camera);
	};


	class BasicCollision
	{
	public:
		static bool IsPointInsideOBB(const glm::vec3& point, const OrientedBoundingBox& obb);
		static bool PointInAABB(const AABB& aabb, const glm::vec3& point);
		static bool AABBInAABB(const AABB& aabb0, const AABB& aabb1);
		static bool RayInAABB(const Ray& Ray,const AABB& aabb0, float& tmin, float& tmax);
		static bool RayInAABB(const Ray& Ray, const AABB& aabb, float& t);
		static bool RayInAABB(const Ray& Ray, const AABB& aabb, float tmin, float tmax, float& t);
		//https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
		//https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
		static bool AAABIsOnOrForwardPlane(const AABB& aabb, const Plane& plane);
		//https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
		static bool AABBIsOnFrustum(const AABB& aabb,const Frustum& camFrustum);

	};
}