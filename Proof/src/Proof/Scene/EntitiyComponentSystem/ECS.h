#pragma once
#include <unordered_map>
#include <vector>
#include "Proof/Scene/Component.h"
#include "Proof/Core/Core.h"
#include "Proof/Core/Log.h"
namespace Proof{
	class MeshComponent;
	class Proof_API ECS {
	public:
		ECS() = default;
		ECS(const ECS&) = default;
		using EntityID = uint64_t;

		EntityID Create();
		EntityID Create(EntityID ID);
		void Delete(EntityID ID);

		template<typename T>
		T* AddComponent(EntityID ID) {
			auto it= EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				T* Temp = new T();
				it->second->emplace_back(Temp);
				return Temp;
			}
			PF_ENGINE_ERROR("Entity Id Was Not FOund");
			return nullptr;
		}
		template<>
		MeshComponent* AddComponent(EntityID ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				MeshComponent* Temp = new MeshComponent();
				it->second->emplace_back(Temp);
				Temp->StartIndexSlot = SceneMeshComponents.size();
				SceneMeshComponents.emplace_back(Temp);
				return Temp;
			}
			PF_ENGINE_ERROR("Entity Id Was Not FOund");
			return nullptr;
		};

		template<>
		SpriteComponent* AddComponent(EntityID ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				SpriteComponent* Temp = new SpriteComponent();
				it->second->emplace_back(Temp);
				Temp->StartIndexSlot = SpriteComponents.size();
				SpriteComponents.emplace_back(Temp);
				return Temp;
			}
			PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
			return nullptr;
		}

		template<>
		LightComponent* AddComponent(EntityID ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				LightComponent* Temp = new LightComponent();
				it->second->emplace_back(Temp);
				Temp->StartIndexSlot = LightComponents.size();
				LightComponents.emplace_back(Temp);
				return Temp;
			}
			PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
			return nullptr;
		}

		
		template<>
		NativeScriptComponent* AddComponent(EntityID ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				NativeScriptComponent* Temp = new NativeScriptComponent();
				it->second->emplace_back(Temp);
				Temp->StartIndexSlot = NativeScripts.size();
				NativeScripts.emplace_back(Temp);
				return Temp;
			}
			PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
			return nullptr;

		};
		template<>
		CameraComponent* AddComponent(EntityID ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				CameraComponent* Temp = new CameraComponent();
				it->second->emplace_back(Temp);
				Temp->StartIndexSlot = m_CameraComponent.size();
				m_CameraComponent.emplace_back(Temp);
				return Temp;
			}
			PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
			return nullptr;
		};
		

		template<typename T>
		T* GetComponent(EntityID ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto Temp = it->second;
				for (int i = 0; i < Temp->size(); i++) {
					if (dynamic_cast<T*>(Temp->at(i)) != nullptr)
						return dynamic_cast<T*>(Temp->at(i));
					if (Temp->size() == 1)break;
				}
				return nullptr;
			}
			PF_ENGINE_ERROR(false,"Entity ID Was Not FOund");
			return nullptr;
		}
		template<typename T>
		T* GetComponent(EntityID ID,const std::string& CompName) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto Temp = it->second;
				for (int i = 0; i < Temp->size(); i++) {
					T* tempComp = dynamic_cast<T*>(Temp->at(i));
					if (tempComp != nullptr) {
						return tempComp ? tempComp->GetName() ==CompName : nullptr;
					}

					if (Temp->size() == 1)break;
				}
				return nullptr;
			}
			PF_ENGINE_ERROR("Entity ID Was Not FOund");
		}

		class Component* GetComponent(EntityID ID,uint32_t Index){
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				if (EntityHolder.size() >= Index) {
					auto TempVec = it->second;
					class Component* TempComp = TempVec->at(Index);
					return TempComp;
				}
				PF_ENGINE_WARN("ID Is Not Valid");
				return nullptr;
			}
			PF_ENGINE_ERROR(false,"Entity ID Was Not FOund");
		}
		template<typename T>
		bool HasComponent(uint32_t ID) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto Temp = it->second;
				for (int i = 0; i < Temp->size(); i++) {
					if (dynamic_cast<T*>(Temp->at(i)) != nullptr) {
						return true;
					}
					if (Temp->size() == 1)break;
				}
				return false;
			}
			PF_ENGINE_ERROR("Entity ID Was Not FOund");
			return false;
		}
		/*
		template<typename T>
		void RemoveComponent(uint32_t ID) {
			if (HasEntity(ID) == true) {
				auto* tempComp = GetComponent<T>(ID);
				auto& TempVec = EntityHolder.at(ID);
				auto it = std::find(TempVec->begin(),TempVec->end(),tempComp);
				if (it != TempVec->end()) {
					TempVec->erase(it);
					delete tempComp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or it is already deleted");
				return;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
		*/
		template<typename T>
		inline void RemoveComponent(EntityID ID,uint32_t Index){
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto TempVec= it->second;
				if(TempVec->size() >= Index){
					TempVec->erase(TempVec->begin()+Index);
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
				return;
			}
			PF_ENGINE_ERROR("Entity ID Was Not FOund");
		}

		template<>
		inline void RemoveComponent<MeshComponent>(EntityID ID,uint32_t Index) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto TempVec = it->second;
				if (TempVec->size() >= Index) {
					MeshComponent* TempComp = static_cast<MeshComponent*>(TempVec->at(Index));
					if (SceneMeshComponents.size() == 1) {
						SceneMeshComponents.erase(SceneMeshComponents.begin());
					}
					else if (SceneMeshComponents.size() == TempComp->StartIndexSlot) {
						SceneMeshComponents.erase(SceneMeshComponents.end());
					}
					else if (SceneMeshComponents.size() > TempComp->StartIndexSlot) {
						int TempLocation = SceneMeshComponents.size() - TempComp->StartIndexSlot;
						SceneMeshComponents.erase(SceneMeshComponents.end() - TempLocation);
					}
					else if (NativeScripts.size() < TempComp->StartIndexSlot) {
						int TempLocation = SceneMeshComponents.size() - TempComp->StartIndexSlot;
						SceneMeshComponents.erase(SceneMeshComponents.end() - TempLocation);
					}
					TempVec->erase(TempVec->begin() + Index);
					delete TempComp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
				return;
			}
		}


		template<>
		inline void RemoveComponent<SpriteComponent>(EntityID ID,uint32_t Index) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto TempVec = it->second;
				if (TempVec->size() >= Index) {
					SpriteComponent* TempComp = static_cast<SpriteComponent*>(TempVec->at(Index));
					if (SpriteComponents.size() == 1) {
						SpriteComponents.erase(SpriteComponents.begin());
					}
					else if (SpriteComponents.size() == TempComp->StartIndexSlot) {
						SpriteComponents.erase(SpriteComponents.end());
					}
					else if (SpriteComponents.size() > TempComp->StartIndexSlot) {
						int TempLocation = SpriteComponents.size() - TempComp->StartIndexSlot;
						SpriteComponents.erase(SpriteComponents.end() - TempLocation);
					}
					else if (SpriteComponents.size() < TempComp->StartIndexSlot) {
						int TempLocation = SpriteComponents.size() - TempComp->StartIndexSlot;
						SpriteComponents.erase(SpriteComponents.end() - TempLocation);
					}
					TempVec->erase(TempVec->begin() + Index);
					delete TempComp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
				return;
			}
		}
		template<>
		inline void RemoveComponent<NativeScriptComponent>(EntityID ID,uint32_t Index) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto TempVec = it->second;
				if (TempVec->size() >= Index) {
					NativeScriptComponent* TempComp = static_cast<NativeScriptComponent*>(TempVec->at(Index));
					if (NativeScripts.size() == 1) {
						NativeScripts.erase(NativeScripts.begin());
					}
					else if (NativeScripts.size() == TempComp->StartIndexSlot) {
						NativeScripts.erase(NativeScripts.end());
					}
					else if (NativeScripts.size() > TempComp->StartIndexSlot) {
						int TempLocation = NativeScripts.size() - TempComp->StartIndexSlot;
						NativeScripts.erase(NativeScripts.end() - TempLocation);
					}
					else if (NativeScripts.size() < TempComp->StartIndexSlot) {
						int TempLocation = NativeScripts.size() - TempComp->StartIndexSlot;
						NativeScripts.erase(NativeScripts.end() - TempLocation);
					}
					TempVec->erase(TempVec->begin() + Index);
					delete TempComp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
			}
			return;
		}
		
		template<>
		inline void RemoveComponent<LightComponent>(EntityID ID,uint32_t Index) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto TempVec = it->second;
				if (TempVec->size() >= Index) {
					LightComponent* TempComp = static_cast<LightComponent*>(TempVec->at(Index));
					if (LightComponents.size() == 1) {
						LightComponents.erase(LightComponents.begin());
					}
					else if (LightComponents.size() == TempComp->StartIndexSlot) {
						LightComponents.erase(LightComponents.end());
					}
					else if (LightComponents.size() > TempComp->StartIndexSlot) {
						int TempLocation = LightComponents.size() - TempComp->StartIndexSlot;
						LightComponents.erase(LightComponents.end() - TempLocation);
					}
					else if (LightComponents.size() < TempComp->StartIndexSlot) {
						int TempLocation = LightComponents.size() - TempComp->StartIndexSlot;
						LightComponents.erase(LightComponents.end() - TempLocation);
					}
					TempVec->erase(TempVec->begin() + Index);
					delete TempComp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
			}
			return;
		}

		template<>
		inline void RemoveComponent<CameraComponent>(EntityID ID,uint32_t Index) {
			auto it = EntityHolder.find(ID);
			if (it != EntityHolder.end()) {
				auto TempVec = it->second;
				if (TempVec->size() >= Index) {
					CameraComponent* TempComp = static_cast<CameraComponent*>(TempVec->at(Index));
					if (m_CameraComponent.size() == 1) {
						m_CameraComponent.erase(m_CameraComponent.begin());
					}
					else if (m_CameraComponent.size() == TempComp->StartIndexSlot) {
						m_CameraComponent.erase(m_CameraComponent.end());
					}
					else if (m_CameraComponent.size() > TempComp->StartIndexSlot) {
						int TempLocation = m_CameraComponent.size() - TempComp->StartIndexSlot;
						m_CameraComponent.erase(m_CameraComponent.end() - TempLocation);
					}
					else if (m_CameraComponent.size() < TempComp->StartIndexSlot) {
						int TempLocation = m_CameraComponent.size() - TempComp->StartIndexSlot;
						m_CameraComponent.erase(m_CameraComponent.end() - TempLocation);
					}
					TempVec->erase(TempVec->begin() + Index);
					delete TempComp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
			}
			return;
		}
		const std::unordered_map<EntityID,std::vector<class Component*>*>& GetEntities() {
			return EntityHolder;
		}
		const std::vector<EntityID>& GetAllID() { // Temporary
			return AllEntityID;
		}

		std::vector<class SpriteComponent*> SpriteComponents;
		std::vector<class MeshComponent*> SceneMeshComponents;
		std::vector<class NativeScriptComponent*> NativeScripts;
		std::vector<class LightComponent*> LightComponents;
		std::vector<class CameraComponent*> m_CameraComponent;

	private:
		std::unordered_map<EntityID,std::vector<class Component*>*>EntityHolder;
		std::vector<EntityID> AllEntityID; // Temporary

		bool HasEntity(EntityID ID) {
			return EntityHolder.find(ID) != EntityHolder.end();
		}
		friend class World;
	};

}

