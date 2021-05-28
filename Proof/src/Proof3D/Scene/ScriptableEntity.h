#pragma once
#include "Proof/Core/Core.h"
#include "Entity.h"
class Proof_API ScriptableEntity{
public:
	virtual ~ScriptableEntity() {};
protected:
	virtual void OnCreate() {}; // called when places or spawned in world
	virtual void OnUpdate(FrameTime DeltaTime) {}; // called every frame
	virtual void OnlyOnCreate() {};// called when only placed in world
	virtual void OnSpawn() {}; // called only when spawned into the world
	virtual void OnDestroy() {}; // called when destoryed

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
	template<class T>
	T* GetComponent() {
		if (OwnerEntity == nullptr)return nullptr;
		return OwnerEntity->GetComponent<T>();
	}
	template<class T>
	bool HasComponent() {
		if (OwnerEntity == nullptr) {
			PF_ENGINE_INFO("owner entity is nullptr");
			return false;
		}
		return OwnerEntity->HasComponent<T>();
	}
	Entity* GetOwner() {
		return OwnerEntity;
	}
private:
	friend class World;
	friend class Entity;
	friend struct NativeScriptComponent;
	Entity* OwnerEntity =nullptr;
};