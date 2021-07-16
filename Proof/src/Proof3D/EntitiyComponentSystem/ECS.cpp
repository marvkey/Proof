#include "Proofprch.h"
#include "ECS.h"
#include "Proof3D/Scene/Entity.h"
#include "Proof3D/Math/Math.h"
#include "Proof3D/Scene/Component.h"
namespace Proof{
    uint32_t ECS::Create() {
        uint32_t ID = Math::RandUINT(1,1000000);
        while(HasEntity(ID) == true){
            uint32_t ID = Math::RandUINT(1,1000000);
            PF_ENGINE_INFO("Conflict creating ID");
        }
        std::vector<Component*>* New = new std::vector<Component*>;
        EntityHolder.insert({ID,New});
        AllEntityID.emplace_back(ID);
        return ID;
    }
    
    void ECS::Delete(uint32_t ID) {
        if(HasEntity(ID) == true){
            auto* a = EntityHolder.find(ID)->second;
            auto it = std::find(AllEntityID.begin(),AllEntityID.end(),ID);
            for(int i=0; i<a->size(); i++){
                delete a->at(i);
                if(a->size() ==1)break;
            }
            EntityHolder.erase(ID);
            AllEntityID.erase(it);
            return;
        }
        PF_ENGINE_ERROR("Entity does not exist with ID No Delete");
    }
	
	template<>
	MeshComponent* ECS::AddComponent(uint32_t ID) {
		if (HasEntity(ID) == true) {
			MeshComponent* Temp = new MeshComponent();
			auto& a = EntityHolder.find(ID);
			a->second->emplace_back(Temp);
			Temp->StartIndexSlot = SceneMeshComponents.size();
			SceneMeshComponents.emplace_back(Temp);
			return Temp;
		}
		PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
		return nullptr;
	};

	template<>
	SpriteComponent* ECS::AddComponent(uint32_t ID) {
		if (HasEntity(ID) == true) {
			SpriteComponent* Temp = new SpriteComponent();
			auto& a = EntityHolder.find(ID);
			a->second->emplace_back(Temp);
			Temp->StartIndexSlot = SpriteComponents.size();
			SpriteComponents.emplace_back(Temp);
			return Temp;
		}
		PF_CORE_ASSERT(false,"Entity Id Was Not FOund");
		return nullptr;
	}
	
	template<>
	void ECS::RemoveComponent<NativeScriptComponent>(uint32_t ID,uint32_t Index) {
		if (HasEntity(ID) == true) {
			auto& TempVec = EntityHolder.at(ID);
			if (TempVec->size() >= Index) {
				NativeScriptComponent* TempComp = static_cast<NativeScriptComponent*>(TempVec->at(Index));
				if (NativeScripts.size() == TempComp->StartIndexSlot) {
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
				return;
			}
			PF_ENGINE_WARN("Remove component Entitiy does not have component or ID Is Not Valid Size of Holder is %i",TempVec->size());
			return;
		}
	}
	template<>
	void ECS::RemoveComponent<MeshComponent>(uint32_t ID,uint32_t Index) {
		if (HasEntity(ID) == true) {
			auto& TempVec = EntityHolder.at(ID);
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
	void ECS::RemoveComponent<SpriteComponent>(uint32_t ID,uint32_t Index) {
		if (HasEntity(ID) == true) {
			auto& TempVec = EntityHolder.at(ID);
			if (TempVec->size() >= Index) {
				SpriteComponent* TempComp = static_cast<SpriteComponent*>(TempVec->at(Index));
				if(SpriteComponents.size() ==1){
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
}
