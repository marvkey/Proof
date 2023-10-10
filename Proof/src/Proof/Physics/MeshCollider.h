#pragma once
#include "Proof/Asset/Asset.h"
#include "glm/glm.hpp"
namespace Proof
{
	enum class ECollisionComplexity : uint8_t
	{
		Default = 0, // Use simple for collision and complex for scene queries
		UseComplexAsSimple = 1, // Use complex for collision AND scene queries.This means that if a complex query is requested, the engine will still query against simple shapes; basically ignoring the trimesh. This helps save memory since we don't need to bake the trimesh and can improve performance 
		//if the collision geometry is simpler.
		UseSimpleAsComplex = 2 // Use simple for collision AND scene queries.This means that if a simple query is requested, the engine will query against complex shapes; basically ignoring the simple collision. This allows us to use the trimesh for the physics simulation collision. Note that if you are using UseComplexAsSimple you cannot simulate the object, 
		//but you can use it to collide with other simulated (simple) objects.
	};

	class MeshCollider : public Asset
	{
	public:
		AssetID ColliderMesh = 0;
		ECollisionComplexity CollisionComplexity = ECollisionComplexity::Default;
		glm::vec3 ColliderScale = glm::vec3(1.0f);
		bool AlwaysShareShape = false;

		// collison 
		bool EnableVertexWelding = true;
		float VertexWeldTolerance = 0.1f;

		// triangle collision
		bool FlipNormals = false;
		//convex
		bool CheckZeroAreaTriangles = true;
		float AreaTestEpsilon = 0.06f;
		bool ShiftVerticesToOrigin = false;

		MeshCollider() {};
		MeshCollider(AssetID colliderMesh)
			: ColliderMesh(colliderMesh)
		{
		}
		ASSET_CLASS_TYPE(MeshCollider);
	};
}