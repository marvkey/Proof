#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsUtils.h"
#include "Proof/Scene/Entity.h"

namespace Proof
{
	enum class ColliderType
	{
		Box, Sphere, Capsule, Plane, Heightfield, ConvexMesh, TriangleMesh
	};

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


}