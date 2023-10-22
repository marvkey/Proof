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
		#if 0
		if (!srcEntity)return;	
		//m_Registry.clear();
		std::unordered_map<UUID, uint64_t> enttMap;
		{
			UUID newID = UUID();

			m_Registry.entities.emplace_back(newID);
			uint64_t copyID = srcEntity.GetEntityID();

			enttMap[copyID] = newID;
			CopyComponent(AllComponents{}, m_Registry, srcEntity, enttMap);
			m_BaseEntityID = newID;
			// for easier user interface reasons
			m_Registry.get<TransformComponent>(newID).Location = { 0,0,0 };
		}
		std::function<void(UUID)> createEntity = [&](UUID srcID)
		{
			World* entityWorld = srcEntity.GetCurrentWorld();
			Entity srcChildEntity = srcEntity.GetCurrentWorld()->GetEntity(srcID);

			if (srcChildEntity.HasOwner())
			{
				Entity owner = srcChildEntity.GetOwner();

				if (!enttMap.contains(owner.GetEntityID()))
				{
					createEntity(owner.GetEntityID());
				}
				UUID newChildID = UUID();
				m_Registry.entities.emplace_back(newChildID);
				enttMap.insert({ srcID,newChildID });

				CopyComponent(AllComponents{}, m_Registry, srcChildEntity, enttMap);

				UUID ownerPrefabId = enttMap[owner.GetEntityID()];

				m_Registry.get<ChildComponent>(ownerPrefabId).RemoveChild(srcID);
				m_Registry.get<ChildComponent>(ownerPrefabId).AddChild(newChildID);

				m_Registry.get<ChildComponent>(newChildID).SetOwner(ownerPrefabId);
			}
		};

		if (!srcEntity.HasChildren())
			return;

		srcEntity.EachChild([&](Entity childEntity) {
			if (!enttMap.contains(childEntity.GetEntityID()))
				createEntity(childEntity.GetEntityID());
		});
		#endif
	}
}
