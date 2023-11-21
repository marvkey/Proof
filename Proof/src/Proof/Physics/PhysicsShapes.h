#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsUtils.h"
#include "Proof/Scene/Entity.h"
#include "MeshCollider.h"
namespace Proof
{
	

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

	protected:
		ColliderType m_Type;
		bool m_IsShared = false;
		Count<class PhysicsMaterial> m_Material;
		Entity m_Entity;
	private:
		void Release();
	};

	class PhysicsActor;
	struct BoxColliderComponent;
	class BoxColliderShape : public ColliderShape
	{
	public:
		BoxColliderShape(const BoxColliderComponent& component, const PhysicsActor& actor,Entity entity);
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

	private:
		physx::PxShape* m_Shape;
	};

	class SphereColliderShape : public ColliderShape
	{
	public:
		SphereColliderShape(const SphereColliderComponent& component, const PhysicsActor& actor, Entity entity);
		~SphereColliderShape();

		float GetRadius() const { return m_Entity.GetComponent<SphereColliderComponent>().Radius; }
		void SetRadius(float radius);

		const glm::vec3& GetCenter() const override { return m_Entity.GetComponent<SphereColliderComponent>().Center; }
		void SetCenter(const glm::vec3& offset) override;

		virtual bool IsTrigger() const override { return m_Entity.GetComponent<SphereColliderComponent>().IsTrigger; }
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;
		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "SphereCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		static ColliderType GetStaticType() { return ColliderType::Sphere; }

	private:
		physx::PxShape* m_Shape;
	};
	struct CapusleData
	{
		glm::vec3 offsetRotation;
		float radiusScale;
		float scaleDirection;
	};
	static CapusleData GetCapsuleData(CapsuleDirection direction, const TransformComponent& worldTransform)
	{
		glm::vec3 scaleabs = glm::abs(worldTransform.Scale);

		float radiusScale = 0;

		float scaleDirection;
		glm::vec3 offsetRotation;
		switch (direction)
		{
		case CapsuleDirection::X:
		{
			scaleDirection = scaleabs.x;
			offsetRotation = glm::vec3{ 0,0,0 };
			radiusScale = glm::max(scaleabs.y, scaleabs.z);
		}
		break;
		case CapsuleDirection::Y:
		{
			offsetRotation = glm::vec3{ 0,0,physx::PxHalfPi };
			scaleDirection = scaleabs.y;
			radiusScale = glm::max(scaleabs.x, scaleabs.z);
		}
		break;
		case CapsuleDirection::Z:
		{
			offsetRotation = glm::vec3{ 0,physx::PxHalfPi,0 };
			scaleDirection = scaleabs.z;
			radiusScale = glm::max(scaleabs.y, scaleabs.x);
		}
		break;
		default:
			break;
		}

		return { offsetRotation,radiusScale,scaleDirection };
	}
	class CapsuleColliderShape : public ColliderShape
	{
	public:
		CapsuleColliderShape(const CapsuleColliderComponent& component, const PhysicsActor& actor, Entity entity);
		~CapsuleColliderShape();

		float GetRadius() const { return m_Entity.GetComponent<CapsuleColliderComponent>().Radius; }
		void SetRadius(float radius);


		CapsuleDirection GetDirection()const { return m_Entity.GetComponent<CapsuleColliderComponent>().Direction; }
		void SetDirection(CapsuleDirection direction);

		float GetHeight() const { return m_Entity.GetComponent<CapsuleColliderComponent>().Height; }
		void SetHeight(float height);

		const glm::vec3& GetCenter() const override { return m_Entity.GetComponent<CapsuleColliderComponent>().Center; }
		void SetCenter(const glm::vec3& offset) override;

		virtual bool IsTrigger() const override { return m_Entity.GetComponent<CapsuleColliderComponent>().IsTrigger; }
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;

		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "CapsuleCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		static ColliderType GetStaticType() { return ColliderType::Capsule; }

	private:
		physx::PxShape* m_Shape;
	};

	class ConvexMeshShape : public ColliderShape
	{
	public:
		ConvexMeshShape(MeshColliderComponent& component, const PhysicsActor& actor, Entity entity);
		virtual ~ConvexMeshShape();

		AssetID GetColliderHandle() const { return m_Entity.GetComponent<MeshColliderComponent>().ColliderID; }

		virtual const glm::vec3& GetOffset() const
		{
			static glm::vec3 defaultOffset = glm::vec3(0.0f);
			return defaultOffset;
		}
		virtual void SetOffset(const glm::vec3& offset) {}

		virtual bool IsTrigger() const override { return m_Entity.GetComponent<MeshColliderComponent>().IsTrigger; }
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;

		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "ConvexMeshCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && !m_Shapes.empty(); }

		static ColliderType GetStaticType() { return ColliderType::ConvexMesh; }

		const glm::vec3& GetCenter() const override { return glm::vec3{ 0 }; }
		void SetCenter(const glm::vec3& offset) override {};
	private:
		std::vector<physx::PxShape*> m_Shapes;
	};

	class TriangleMeshShape : public ColliderShape
	{
	public:
		TriangleMeshShape(MeshColliderComponent& component, const PhysicsActor& actor, Entity entity);
		~TriangleMeshShape();

		AssetID GetColliderHandle() const { return m_Entity.GetComponent<MeshColliderComponent>().ColliderID; }

		const glm::vec3& GetCenter() const override { return glm::vec3{ 0 }; }
		void SetCenter(const glm::vec3& offset) override {};

		virtual bool IsTrigger() const override { return m_Entity.GetComponent<MeshColliderComponent>().IsTrigger; }
		virtual void SetTrigger(bool isTrigger) override;

		virtual void SetFilterData(const physx::PxFilterData& filterData) override;

		virtual void DetachFromActor(physx::PxRigidActor* actor) override;

		virtual const char* GetShapeName() const override { return "TriangleMeshCollider"; }
		virtual bool IsValid() const override { return ColliderShape::IsValid() && !m_Shapes.empty(); }

		static ColliderType GetStaticType() { return ColliderType::TriangleMesh; }

	private:
		std::vector<physx::PxShape*> m_Shapes;
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