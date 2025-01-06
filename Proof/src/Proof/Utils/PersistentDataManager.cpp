#include "Proofprch.h"
#include "PersistentDataManager.h"

namespace Proof
{
    ScopeBuffer& PersistentDataManager::LoadData(const std::string& name)
    {
        PF_CORE_ASSERT(HasData(name),"Does not contain data storage");
        return m_Buffers.at(name);
    }
    void PersistentDataManager::ClearData(const std::string& name)
    {
        if (HasData(name))
            m_Buffers.erase(name);
    }
}
