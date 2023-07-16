#include "Proofprch.h"
#include "Prefab.h"

namespace Proof {

	template<typename... Component>
	static void CopyComponentSinglePrefab(entt::registry64& dst, Entity srcEntity, const std::unordered_map<UUID, uint64_t>& enttMap)
	{
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
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry64& dst, Entity srcEntity, const std::unordered_map<UUID, uint64_t>& enttMap)
	{
		CopyComponentSinglePrefab<Component...>(dst, srcEntity, enttMap);
	}
	Prefab::Prefab(Entity entity)
	{
		if (!entity)
			return;
		SetEntity(entity);
	}
	Prefab::Prefab() {

	}
	Prefab::~Prefab()
	{
		//m_Registry = {};
	}
	void Prefab::SetEntity(Entity srcEntity)
	{
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

	}
}
