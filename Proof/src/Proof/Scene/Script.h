#pragma once
#include "Proof/Core/Core.h"
#include "Entity.h"
#include "Proof/Core/FrameTime.h"
namespace Proof{
	class Proof_API Script{
	public:
		virtual ~Script() {};
	protected:
		virtual void OnCreate(); // called when placed or spawned in world
		virtual void OnUpdate(FrameTime DeltaTime) {}; // called every frame
		virtual void OnlyOnCreate() {};// called when only placed in world
		virtual void OnSpawn() {}; // called only when spawned into the world
		virtual void OnDestroy() {}; // called when destoryed
		/*
		template<class T,typename... Args>
		void AddComponent() {
			if (OwnerEntity == nullptr) {
				PF_ENGINE_INFO("cannot add Entity, pointer to owner entity is null");
			};
			if (OwnerEntity->HasComponent<T>() == true) {
				return;
			}
			OwnerEntity->AddComponent<T>(Args&&...args);
		}
		*/
		template<class T>
		T* GetComponent() {
			return OwnerEntity.GetComponent<T>();
		}
		template<class T>
		bool HasComponent() {
			return OwnerEntity.HasComponent<T>();
		}

		template<class T> 
		T* GetComponent(uint32_t IndexSlot){
			OwnerEntity.GetComponent<T>(IndexSlot);
		}
		bool b_CallPerframe = true;
	private:
		friend class World;
		friend class Entity;
		friend struct NativeScriptComponent;
		Entity OwnerEntity;
	};
}
