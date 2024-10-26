#include "Proofprch.h"
#include "Prefab.h"
#include "World.h"
#include "Entity.h"

namespace Proof {

	template<typename... Component>
	static void CopyComponentSinglePrefab(entt::registry& dst, Entity srcEntity, const std::unordered_map<UUID, uint64_t>& enttMap)
	{
		#if 0
		([&]()
		{
			if (typeid(Component) == typeid(IDComponent))
			{
				return;
			}

			if (!srcEntity.HasComponent<Component>())
				return;

			UUID id = srcEntity.GetComponent<IDComponent>().GetID();
			uint64_t dstComponent = enttMap.at(id);
			auto& srcComponent = srcEntity.GetComponent<Component>();
			dst.emplace_or_replace<Component>(dstComponent, srcComponent);
			
		}(), ...);
		#endif
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, Entity srcEntity, const std::unordered_map<UUID, uint64_t>& enttMap)
	{
		CopyComponentSinglePrefab<Component...>(dst, srcEntity, enttMap);
	}
	Prefab::Prefab() {
		m_World = Count<class World>::Create(fmt::format("Prefab {} world", GetID()));
		m_BaseEntityUUID = m_World->CreateEntity("Base Prefab").GetUUID();
	}
	Prefab::~Prefab()
	{
		m_World = nullptr;
		//m_Registry = {};
	}
	void Prefab::SetEntity(Entity srcEntity)
	{
		if (!srcEntity)return;
		if (m_World->HasEntity(GetBaseEntity().GetUUID()))
		{
			m_World->DeleteEntity(GetBaseEntity());
			m_World->DeleteEntitiesfromQeue();
		}
		m_BaseEntityUUID = m_World->CreateEntity().GetUUID();

		srcEntity.GetCurrentWorld()->PrefabCopyEntity(this, srcEntity, GetBaseEntity());
	}
	Entity Prefab::GetBaseEntity()
	{
		return m_World->GetEntity(m_BaseEntityUUID);
	}
	void Prefab::ReCheckHierachy()
	{
		if (!m_World->HasEntity(GetBaseEntity().GetUUID()))
		{
			for (auto& [entityID, entity] : m_World->GetEntities())
				m_World->DeleteEntity(entity);

			m_World->DeleteEntitiesfromQeue();
			m_BaseEntityUUID = m_World->CreateEntity("Base Prefab").GetUUID();
		}
		if (m_World->GetEntities().size() == 0)
		{
			m_BaseEntityUUID = m_World->CreateEntity("Base Prefab").GetUUID();
		}
		for (auto& [entityID,entity] : m_World->GetEntities())
		{
			if (entity != GetBaseEntity())
			{
				if (!entity.IsDescendantOf(GetBaseEntity()))
					m_World->ParentEntity(entity, GetBaseEntity());
			}
		}

	}
}
