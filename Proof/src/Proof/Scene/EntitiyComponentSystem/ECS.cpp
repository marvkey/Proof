#include "Proofprch.h"
#include "ECS.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
/*
namespace Proof{
    UUID ECS::Create() {
        UUID id =UUID();
        while(HasEntity(id) == true|| id==0){
            id = UUID();
            PF_ENGINE_INFO("Conflict creating ID");
        }
        std::vector<Component*>* New = new std::vector<Component*>;
        EntityHolder.insert({id,New});
        AllEntityID.emplace_back(id);
        return id;
    }

    UUID ECS::Create(UUID uuid) {
        while(uuid ==0){
            PF_CORE_ASSERT(false,"Id cannot be 0");
            uuid = UUID();
        }
		if(HasEntity(uuid) == true){
			PF_ENGINE_INFO("Cannot pass entity ID by argument that already exist");
			return 0;
		}
		std::vector<Component*>* New = new std::vector<Component*>;
		EntityHolder.insert({uuid,New});
		AllEntityID.emplace_back(uuid);
		return uuid;
	}
    
    void ECS::Delete(UUID ID) {
        auto it = EntityHolder.find(ID);
        if (it != EntityHolder.end()) {
            auto itVec = std::find(AllEntityID.begin(),AllEntityID.end(),ID);
            if(it->second){
                for(int i=0; i< it->second->size(); i++){
                    delete it->second->at(i);
                }
            }
            delete it->second;
            EntityHolder.erase(ID);
            if(itVec != AllEntityID.end())
                AllEntityID.erase(itVec);
            return;
        }
        PF_ENGINE_ERROR("Entity does not exist with ID No Delete");
    }

}
*/