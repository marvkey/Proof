#pragma once
#include <unordered_map>
#include <type_traits>
#include<vector>
#include "Proof3D/Scene/Component.h"
namespace Proof
{
	class Proof_API ECS {
	public:
		ECS() = default;
		ECS(const ECS&) = default;

		uint32_t Create();
		void Delete(uint32_t ID);
		template<typename T,typename... Args>
		T* AddComponent(uint32_t ID) {
			if (HasEntity(ID) == true) {
				Component* Temp = new T();
				auto& a = EntityHolder.find(ID);
				a->second->emplace_back(Temp);
				return dynamic_cast<T*>(Temp);
			}
			PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
			return nullptr;
		}

		template<typename T>
		T* GetComponent(uint32_t ID) {
			if (HasEntity(ID) == true) {
				auto& Temp = EntityHolder.at(ID);
				for (int i = 0; i < Temp->size(); i++) {
					if (dynamic_cast<T*>(Temp->at(i)) != nullptr)
						return dynamic_cast<T*>(Temp->at(i));
					if (Temp->size() == 1)break;
				}
				return nullptr;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
		template<typename T>
		T* GetComponent(uint32_t ID,const std::string& CompName) {
			if (HasEntity(ID) == true) {
				auto& Temp = EntityHolder.at(ID);
				for (int i = 0; i < Temp->size(); i++) {
					T* tempComp = dynamic_cast<T*>(Temp->at(i));
					if (tempComp != nullptr) {
						return tempComp ? tempComp->GetName() ==CompName : continue;
					}

					if (Temp->size() == 1)break;
				}
				return nullptr;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

		Component* GetComponent(uint32_t ID,uint32_t Index){
			if (HasEntity(ID) == true) {
				if (EntityHolder.size() >= Index) {
					auto& TempVec = EntityHolder.at(ID);
					Component* TempComp = TempVec->at(Index);
					return TempComp;
				}
				PF_ENGINE_WARN("ID Is Not Valid");
				return nullptr;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
		template<typename T>
		bool HasComponent(uint32_t ID) {
			if (HasEntity(ID) == true) {
				auto& Temp = EntityHolder.at(ID);
				for (int i = 0; i < Temp->size(); i++) {
					if (dynamic_cast<T*>(Temp->at(i)) != nullptr) {
						return true;
					}
					if (Temp->size() == 1)break;
				}
				return false;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

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

		void RemoveComponent(uint32_t ID,Component* Comp) {
			if (HasEntity(ID) == true) {
				auto& TempVec = EntityHolder.at(ID);
				auto it = std::find(TempVec->begin(),TempVec->end(),Comp);
				if (it != TempVec->end()) {
					TempVec->erase(it);
					delete Comp;
					return;
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or it is already deleted");
				return;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

		void RemoveComponent(uint32_t ID,uint32_t Index){
			if (HasEntity(ID) == true) {
				if(EntityHolder.size() >= Index){
					auto& TempVec = EntityHolder.at(ID);
					Component* TempComp = TempVec->at(Index);
					TempVec->erase(TempVec->begin()+Index);
				}
				PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid");
				return;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
		const std::unordered_map<uint32_t,std::vector<class Component*>*>& GetEntities() {
			return EntityHolder;
		}
		const std::vector<uint32_t>& GetAllID() { // Temporary
			return AllEntityID;
		}


	private:
		std::unordered_map<uint32_t,std::vector<class Component*>*>EntityHolder;
		std::vector<uint32_t> AllEntityID; // Temporary
		bool HasEntity(uint32_t ID) {
			return EntityHolder.find(ID) != EntityHolder.end();
		}

		friend class World;
	};

}

