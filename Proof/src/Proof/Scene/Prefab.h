#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include "Entity.h"

#include "World.h"
namespace Proof{

	class Prefab : public Asset {
	public: 
		Prefab(Entity entity);

		Prefab();


		void SetEntity(Entity entity);
		ASSET_CLASS_TYPE(Prefab);

		const entt::registry64& GetRegistry() {
			return m_Registry;
		}

		UUID GetBaseEntity() {
			return m_BaseEntityID;
		}
	private:
		entt::registry64 m_Registry;
		UUID m_BaseEntityID= 0;
		friend class PrefabAssetSerilizer;
	};
}
