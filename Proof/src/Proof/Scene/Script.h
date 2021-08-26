#pragma once
#include "Proof/Core/Core.h"
#include "Entity.h"
#include "Proof/Core/FrameTime.h"
#include <memory>
#include <unordered_map>
#include <iostream>
namespace Proof
{
#define CreateNew() new this;
	class Proof_API Script {
	public:
		virtual ~Script() {};
		virtual void OnCreate() {} // called when placed or spawned in world
		virtual void OnUpdate(FrameTime DeltaTime) {}; // called every frame
		virtual void OnlyOnCreate() {};// called when only placed in world
		virtual void OnSpawn() {}; // called only when spawned into the world
		virtual void OnDestroy() {}; // called when destoryed

		/*
		template<class T,typename... Args>
		void AddComponent() {
			if (OwnerEntity == nullptrdd Entity, pointer to owner entity is null");
			};
			if (OwnerEntity->HasComponent<T>() == true) {
			) {
				PF_ENGINE_INFO("cannot a	return;
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
		T* GetComponent(uint32_t IndexSlot) {
			OwnerEntity.GetComponent<T>(IndexSlot);
		}
		bool b_CallPerframe = true;
	private:
		friend class World;
		friend class Entity;
		friend struct NativeScriptComponent;
		Entity OwnerEntity;

	};
	namespace ScriptDetail{	
		template<class T>
		Script* CreateScript() {
			return new T;
		}
		
		static std::unordered_map <uint32_t,std::pair<std::string,Script*>>& GetScriptRegisry() {
			static std::unordered_map <uint32_t,std::pair<std::string,Script*>>ScriptsRegistry;
			return ScriptsRegistry;
		}
		
		
		static uint32_t GenerateRandomIDScript() {
			return Math::RandUINT(1,1000000);
		}
		static bool ScriptIDExist(uint32_t ID) {
			return GetScriptRegisry().find(ID) != GetScriptRegisry().end();
		}
		
		static bool RegisterScript(const char* Name,Script* script) {
			a:
			uint32_t  ID = GenerateRandomIDScript();
			if (ID == 0 || ScriptIDExist(ID)) goto a;
			GetScriptRegisry().insert({ID,{Name,script}});
			return true;
		}
	}


}
#define REGISTER_SCRIPT(TYPE)\
	class TYPE;\
	namespace {\
		bool Reg##TYPE{\
			Proof::ScriptDetail::RegisterScript(#TYPE,Proof::ScriptDetail::CreateScript<TYPE>())};\
	}