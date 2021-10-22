#include "Proofprch.h"
#include "ECS.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Resources/Math/Math.h"
#include "Proof/Scene/Component.h"
namespace Proof{
    EntityID ECS::Create() {
        EntityID ID = Math::RandUINT(1,18000000000000000000);
        while(HasEntity(ID) == true){
            ID = Math::RandUINT(1,18000000000000000000);
            PF_ENGINE_INFO("Conflict creating ID");
        }
        std::vector<Component*>* New = new std::vector<Component*>;
        EntityHolder.insert({ID,New});
        AllEntityID.emplace_back(ID);
        return ID;
    }

    EntityID ECS::Create(EntityID ID) {
        if(ID ==0){
            PF_CORE_ASSERT(false,"Id cannot be 0");
            ID = Math::RandUINT(1,18000000000000000000);
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
    
    void ECS::Delete(EntityID ID) {
        auto it = EntityHolder.find(ID);
        if (it != EntityHolder.end()) {
            auto* entityVector = it->second;
            auto it = std::find(AllEntityID.begin(),AllEntityID.end(),ID);
            for(int i=0; i< entityVector->size(); i++){
                delete entityVector->at(i);
            }
            EntityHolder.erase(ID);
            AllEntityID.erase(it);
            delete entityVector;
            return;
        }
        PF_ENGINE_ERROR("Entity does not exist with ID No Delete");
    }

}
