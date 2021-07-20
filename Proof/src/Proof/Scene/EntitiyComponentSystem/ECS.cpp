#include "Proofprch.h"
#include "ECS.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Resources/Math/Math.h"
#include "Proof/Scene/Component.h"
namespace Proof{
    uint32_t ECS::Create() {
        uint32_t ID = Math::RandUINT(1,1000000);
        while(HasEntity(ID) == true){
            uint32_t ID = Math::RandUINT(1,10000000); // between 1 and 10 million
            PF_ENGINE_INFO("Conflict creating ID");
        }
        std::vector<Component*>* New = new std::vector<Component*>;
        EntityHolder.insert({ID,New});
        AllEntityID.emplace_back(ID);
        return ID;
    }

	uint32_t ECS::Create(uint32_t ID) {
        if(ID ==0){
            PF_CORE_ASSERT(false,"Id cannot be 0");
        }
		if(HasEntity(ID) == true){
			PF_ENGINE_INFO("Cannot pass entity ID by argument that already exist");
			return 0;
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

}
