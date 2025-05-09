#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsUtils.h"
#include "MeshCollider.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Math/AABB.h"
namespace Proof
{
	class Entity;
	class ColliderShape : public RefCounted
	{
	protected:
		ColliderShape(ColliderType type, Entity entity, bool isShared = false);

	public:
		virtual ~ColliderShape()
		{
			Release();
		}
		void SetMaterial(Count<class PhysicsMaterial> material);

		ColliderType GetType() const { return m_Type; }

		virtual const glm::vec3& GetCenter() const = 0;
		virtual void SetCenter(const glm::vec3& offset) = 0;

		virtual bool IsTrigger() const = 0;
		virtual void SetTrigger(bool isTrigger) = 0;

		virtual void SetFilterData(const physx::PxFilterData& filterData) = 0;

		virtual void DetachFromActor(physx::PxRigidActor* actor) = 0;

		virtual const char* GetShapeName() const = 0;

		bool IsShared() const { return m_IsShared; }

		virtual bool IsValid() const { return m_Material != nullptr; }

		Count<PhysicsMaterial> GetMaterial() { return m_Material; }

		//shapes and counts
		virtual std::pair< physx::PxShape*, size_t> GetShapes() = 0;
		bool IsPointInsideCollider(glm::vec3 point);
		bool UseRayIsPointInsideCollider(glm::vec3 point,float rayLength);

		virtual AABB GetBoundingBox();

		glm::mat4 GetInitalShapeLocalTransform(); 
		glm::mat4 GetInitalShapeWorldTransform(); 
	protected:
		ColliderType m_Type;
		bool m_IsShared = false;
		Count<class PhysicsMaterial> m_Material;
		class Entity m_Entity;
	private:
		void Release();
	};

	class PhysicsActor;
	class BoxColliderShape : public ColliderShape
	{
	public:
		BoxColliderShape(const struct BoxColliderComponent& component, const PhysicsActor& actor, class Entity entity);
		~BoxColliderShape();

		const glm::vec3& GetSize();
		void SetSize(const glm::vec3& Size);

		const glm::vec3& GetCenter() const override;
		void SetCenter(const glm::vec3& offset) override;

		virtual bool IsTrigger() const override;
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;
		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "BoxCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		static ColliderType GetStaticType() { return ColliderType::Box; }

		virtual AABB GetBoundingBox();

		
	private:
		physx::PxShape* m_Shape = nullptr;
	private:
		virtual std::pair< physx::PxShape*, size_t> GetShapes()
		{
			if (m_Shape)
				return { m_Shape,1 };
			return { nullptr, 0 };
		}
	};

	class SphereColliderShape : public ColliderShape
	{
	public:
		SphereColliderShape(const struct SphereColliderComponent& component, const PhysicsActor& actor, class Entity entity);
		~SphereColliderShape();
		virtual AABB GetBoundingBox();

		float GetRadius() const;
		void SetRadius(float radius);

		const glm::vec3& GetCenter() const override;
		void SetCenter(const glm::vec3& offset) override;

		virtual bool IsTrigger() const override;
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;
		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "SphereCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		static ColliderType GetStaticType() { return ColliderType::Sphere; }

	private:
		physx::PxShape* m_Shape = nullptr;
	private:
		virtual std::pair< physx::PxShape*, size_t> GetShapes()
		{
			if (m_Shape)
				return { m_Shape,1 };
			return { nullptr, 0 };
		}
	};
	struct CapusleData
	{
		glm::vec3 offsetRotation;
		float radiusScale;
		float scaleDirection;
	};
	enum class CapsuleDirection;

	CapusleData GetCapsuleData(CapsuleDirection direction, const TransformComponent& worldTransform);

	class CapsuleColliderShape : public ColliderShape
	{
	public:
		CapsuleColliderShape(const struct CapsuleColliderComponent& component, const PhysicsActor& actor, class Entity entity);
		~CapsuleColliderShape();

		float GetRadius() const;
		void SetRadius(float radius);
		virtual AABB GetBoundingBox();


		CapsuleDirection GetDirection()const;
		void SetDirection(CapsuleDirection direction);

		float GetHeight() const;
		void SetHeight(float height);

		const glm::vec3& GetCenter() const override;
		void SetCenter(const glm::vec3& offset) override;

		virtual bool IsTrigger() const override;
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;

		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "CapsuleCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		static ColliderType GetStaticType() { return ColliderType::Capsule; }

	private:
		physx::PxShape* m_Shape = nullptr;

	private:
		virtual std::pair< physx::PxShape*, size_t> GetShapes()
		{
			if (m_Shape)
				return { m_Shape,1 };
			return { nullptr, 0 };
		}
	};

	class ConvexMeshShape : public ColliderShape
	{
	public:
		ConvexMeshShape(struct MeshColliderComponent& component, const PhysicsActor& actor, class Entity entity);
		virtual ~ConvexMeshShape();

		AssetID GetColliderHandle() const;

		virtual const glm::vec3& GetOffset() const
		{
			static glm::vec3 defaultOffset = glm::vec3(0.0f);
			return defaultOffset;
		}
		virtual void SetOffset(const glm::vec3& offset) {}

		virtual bool IsTrigger() const override;
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;

		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "ConvexMeshCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && !m_Shapes.empty(); }

		static ColliderType GetStaticType() { return ColliderType::ConvexMesh; }

		const glm::vec3& GetCenter() const override { return glm::vec3{ 0 }; }
		void SetCenter(const glm::vec3& offset) override {};
		void SetMaterial(Count<class PhysicsMaterial> material);

	private:
		bool m_BlockSetMaterial = false;
		std::vector<physx::PxShape*> m_Shapes;
	private:
		virtual std::pair< physx::PxShape*, size_t> GetShapes()
		{
			if (m_Shapes.size())
				return { m_Shapes[0],m_Shapes.size()};
			return { nullptr,0 };
		}
	};

	class TriangleMeshShape : public ColliderShape
	{
	public:
		TriangleMeshShape(struct MeshColliderComponent& component, const PhysicsActor& actor, class Entity entity);
		~TriangleMeshShape();

		AssetID GetColliderHandle() const;

		const glm::vec3& GetCenter() const override { return glm::vec3{ 0 }; }
		void SetCenter(const glm::vec3& offset) override {};

		virtual bool IsTrigger() const override;
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;

		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "TriangleMeshCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && !m_Shapes.empty(); }

		static ColliderType GetStaticType() { return ColliderType::TriangleMesh; }
		void SetMaterial(Count<class PhysicsMaterial> material);

	private:
		bool m_BlockSetMaterial = false;
		std::vector<physx::PxShape*> m_Shapes;
	private:
		virtual std::pair< physx::PxShape*, size_t> GetShapes()
		{
			if (m_Shapes.size())
				return { m_Shapes[0],m_Shapes.size() };
			return { nullptr,0 };
		}
	};

	class SharedShapeManager
	{
	public:
		struct SharedShapeData
		{
			ECollisionComplexity Usage = ECollisionComplexity::Default; // collider complexity
			std::unordered_map<uint32_t, physx::PxShape*> Shapes; // every submesh shape of the mesh collider
		};

		using SharedShapeMap = std::unordered_map<ColliderType, std::unordered_map<AssetID, std::vector<SharedShapeData*>>>;// collider type, collider handle, 

	public:
		static SharedShapeData* CreateSharedShapeData(ColliderType colliderType, AssetID colliderHandle);
		static SharedShapeData* FindSuitableSharedShape(ColliderType colliderType, const Count<class MeshCollider>& collider, const bool checkSubmeshIndex = false, const uint32_t submeshIndex = 0);
		static void RemoveSharedShapeData(ColliderType colliderType, const Count<class MeshCollider>& collider, const bool checkSubmeshIndex = false, const uint32_t submeshIndex = 0);
		static void ClearSharedShapes();

	private:
		static SharedShapeMap s_SharedShapes;
	};
}