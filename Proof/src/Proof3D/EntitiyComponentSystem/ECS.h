#pragma once
#include <unordered_map>
#include <type_traits>
#include<vector>
#include "Proof3D/Scene/Component.h"
namespace Proof{
	class ECS {
	public:
		uint32_t Create();
		void Delete(uint32_t ID);
		template<typename T, typename... Args>
		T* AddComponent(uint32_t ID){
			if(HasEntity(ID) ==true){
				Component* Temp = new T();
				auto& a =EntityHolder.find(ID);
				a->second->emplace_back(Temp);
				return dynamic_cast<T*>(Temp);
			}
			PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
			return nullptr;
		}

		template<typename T>
		T* GetComponent(uint32_t ID){
			if(HasEntity(ID) == true){
				auto& Temp  = EntityHolder.at(ID);
				for(int i=0; i< Temp->size(); i++){
					if (dynamic_cast<T*>(Temp->at(i)) != nullptr)
						return dynamic_cast<T*>(Temp->at(i));
					if(Temp->size()==1)break;
				}
				return nullptr;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

		template<typename T>
		bool HasComponent(uint32_t ID){
			if(HasEntity(ID) ==true){
				auto& Temp = EntityHolder.at(ID);
				for(int i=0; i<Temp->size();i++){
					if(dynamic_cast<T*>(Temp->at(i)) != nullptr){
						return true;
					}
					if(Temp->size() ==1)break;
				}
				return false;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

		template<typename T> 
		void RemoveComponent(uint32_t ID){
			if (HasEntity(ID) == true){
				auto* tempComp =GetComponent<T>(ID);
				auto TempVec = EntityHolder.at(ID);
				auto it = std::find(TempVec->begin(),TempVec->end(),tempComp);
				TempVec->erase(it);
				delete tempComp;
				return;
			}
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
	private:
		std::unordered_map<uint32_t,std::vector<Component*>*>EntityHolder;
		//std::unordered_map<uint32_t, std::vector<void*>*>EntityComponent; 
		bool HasEntity(uint32_t ID){
			return EntityHolder.find(ID) != EntityHolder.end();
		}
	};
	
}

