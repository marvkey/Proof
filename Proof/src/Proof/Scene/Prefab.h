#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include "Proof/Asset/Asset.h"
namespace Proof
{
	class Entity;
	class Prefab : public Asset 
	{
	public: 
		Prefab();

		~Prefab();
		void SetEntity(class Entity entity);
		ASSET_CLASS_TYPE(Prefab);
		
		bool KeepLinkedOnSpawn = true;

		Count<class World> GetWorld() { return m_World;}
		class Entity GetBaseEntity();
		void ReCheckHierachy();
	private:
		UUID m_BaseEntityUUID;
		Count<class World> m_World;
		friend class PrefabAssetSerilizer;
		friend class World;
	};
}
