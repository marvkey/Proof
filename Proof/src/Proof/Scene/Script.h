#pragma once
#include "Proof/Core/Core.h"
#include "Entity.h"
#include "Proof/Core/FrameTime.h"
#include <memory>
#include <unordered_map>
#include <iostream>
#include "CollisionInfo.h"
namespace Proof
{
	
	class Proof_API Script {
	public:
		virtual ~Script() {};
		virtual void OnCreate() {} // called when placed or spawned in world
		virtual void OnUpdate(FrameTime DeltaTime) {}; // called every frame
		virtual void OnPlaced() {};// called when only placed in world
		virtual void OnSpawn() {}; // called only when spawned into the world
		virtual void OnDestroy() {}; // called when destoryed

		virtual void OnTriggerEnter(ColliderTypes colliderSetOff, TriggerInfo& info) {};
		virtual void OnTriggerStay(ColliderTypes colliderSetOff, TriggerInfo& info) {};
		virtual void OnTriggerLeave(ColliderTypes colliderSetOff, TriggerInfo& info) {};
		
		virtual void OnCollisionEnter(ColliderTypes colliderSetOff, CollisionInfo& info) {};
		virtual void OnCollisionStay(ColliderTypes colliderSetOff, CollisionInfo& info) {};
		virtual void OnCollisionLeave(ColliderTypes colliderSetOff, CollisionInfo& info) {};
		template<class T>
		T* GetComponent() {
			return m_Owner.GetComponent<T>();
		}
		template<class T>
		bool HasComponent() {
			return m_Owner.HasComponent<T>();
		}
		World* GetWorld() { return m_World; };
		bool b_CallPerframe = true;
	private:
		friend class World;
		friend class Entity;
		friend struct NativeScriptComponent;
		Entity m_Owner = {};
		World* m_World =nullptr;
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
			return Random::Int<uint32_t>(1);
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