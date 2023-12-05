#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Entity.h"
namespace Proof 
{
	class Entity;
	enum class PhysicsControllerType { None = 0, Actor, Controller };
	class PhysicsActorBase : RefCounted
	{
	public:
		Entity GetEntity() const { return m_Entity; }

		virtual bool SetSimulationData(uint32_t layerId) = 0;
		virtual glm::vec3 GetLocation() const = 0;
		virtual void SetLocation(const glm::vec3& translation, const bool autowake = true) = 0;

		virtual bool IsGravityEnabled() const = 0;
		virtual void SetGravityEnabled(const bool enableGravity) = 0;
		virtual bool IsSleeping() = 0;
		PhysicsControllerType GetType() const { return m_Type; }
	private:
		virtual void SyncTransform() = 0;
	protected:
		PhysicsActorBase(Count<class PhysicsWorld> world, PhysicsControllerType type, Entity entity)
			: m_Type(type), m_Entity(entity),m_PhysicsWorld(world) {}
	protected:
		Entity m_Entity;
		Count<class PhysicsWorld> m_PhysicsWorld;
	private:
		PhysicsControllerType m_Type = PhysicsControllerType::None;

		friend class PhysicsWorld;
	};
}