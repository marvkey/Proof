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
		m_BaseEntity = m_World->CreateEntity("Base Prefab");
	}
	Prefab::~Prefab()
	{
		m_World = nullptr;
		//m_Registry = {};
	}
	void Prefab::SetEntity(Entity srcEntity)
	{
		if (!srcEntity)return;
		if (m_World->HasEntity(m_BaseEntity.GetUUID()))
		{
			m_World->DeleteEntity(m_BaseEntity);
			m_World->DeleteEntitiesfromQeue();
		}
		m_BaseEntity = m_World->CreateEntity();

		srcEntity.GetCurrentWorld()->PrefabCopyEntity(this, srcEntity, m_BaseEntity);
	}
	void Prefab::ReCheckHierachy()
	{
		if (!m_World->HasEntity(m_BaseEntity.GetUUID()))
		{
			for (auto& [entityID, entity] : m_World->GetEntities())
				m_World->DeleteEntity(entity);

			m_World->DeleteEntitiesfromQeue();
			m_BaseEntity = m_World->CreateEntity("Base Prefab");
		}
		if (m_World->GetEntities().size() == 0)
		{
			m_BaseEntity = m_World->CreateEntity("Base Prefab");
		}
		for (auto& [entityID,entity] : m_World->GetEntities())
		{
			if (entity != m_BaseEntity)
			{
				if (!entity.IsDescendantOf(m_BaseEntity))
					m_World->ParentEntity(entity, m_BaseEntity);
			}
		}

	}
}
