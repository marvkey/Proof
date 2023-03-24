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

			UUID id = srcEntity.GetComponent<IDComponent>()->GetID();
			uint64_t dstEntity = enttMap.at(id);
			auto& srcComponent = *srcEntity.GetComponent<Component>();
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			
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
	void Prefab::SetEntity(Entity entity)
	{
		if (!entity)return;
		m_Registry.clear();


		std::unordered_map<UUID, uint64_t> enttMap;


		m_Registry.entities.emplace_back(entity.GetEntityID());
		uint64_t id = entity.GetEntityID();
		enttMap.insert({ id,id });
		CopyComponent(AllComponents{}, m_Registry, entity, enttMap);
		m_BaseEntityID = id;


		// for easier user interface reasons
		m_Registry.get<TransformComponent>(id).Location = { 0,0,0 };
		if (!entity.HasChildren())
			return;

		entity.EachChild([&](Entity childEntity) {
			m_Registry.entities.emplace_back(childEntity.GetEntityID());
			uint64_t newId = childEntity.GetEntityID();
			enttMap.insert({ newId,newId });
			CopyComponent(AllComponents{}, m_Registry, childEntity, enttMap);
		});


	}
}
