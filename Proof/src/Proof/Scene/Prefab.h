#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include "Entity.h"
namespace Proof{
	class Entity;
	class Prefab : public Asset {
	public: 
		Prefab();

		~Prefab();
		void SetEntity(Entity entity);
		ASSET_CLASS_TYPE(Prefab);
		
		bool KeepLinkedOnSpawn = true;

		Count<class World> GetWorld() { return m_World;}
		Entity GetBaseEntity() { return m_BaseEntity; }
	private:
		Entity m_BaseEntity;
		Count<class World> m_World;
		friend class PrefabAssetSerilizer;
		friend class World;
	};
}
