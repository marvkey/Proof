#include "Proofprch.h"
#include "ScriptField.h"
#include "ScriptUtils.h"
#include "ScriptTypes.h"
namespace Proof
{

    bool FieldStorage::GetValueRuntime(Buffer& outBuffer) const
    {
        if (m_RuntimeInstance == nullptr)
            return false;

        MonoObject* runtimeObject = ScriptGCManager::GetReferencedObject(m_RuntimeInstance);
        if (runtimeObject == nullptr)
            return false;

        outBuffer = ScriptUtils::GetFieldValue(runtimeObject, m_FieldInfo->Name, m_FieldInfo->Type, m_FieldInfo->IsProperty);
        return true;
    }
    void FieldStorage::SetValueRuntime(const void* data)
    {
        if (m_RuntimeInstance == nullptr)
            return;

        MonoObject* runtimeObject = ScriptGCManager::GetReferencedObject(m_RuntimeInstance);
        ScriptUtils::SetFieldValue(runtimeObject, m_FieldInfo, data);
    }
    ManageEnumClass* EnumFieldStorage::ManageEnum() const
    {
        
        //m_FieldInfo->
        return nullptr;
    }
    bool EnumFieldStorage::GetValueRuntime(Buffer& outBuffer) const
    {
        if (m_RuntimeInstance == nullptr)
            return false;

        MonoObject* runtimeObject = ScriptGCManager::GetReferencedObject(m_RuntimeInstance);
        if (runtimeObject == nullptr)
            return false;

        outBuffer = ScriptUtils::GetFieldValue(runtimeObject, m_FieldInfo->Name, m_FieldInfo->Type, m_FieldInfo->IsProperty);
        return true;
    }
    void EnumFieldStorage::SetValueRuntime(const void* data)
    {
        if (m_RuntimeInstance == nullptr)
            return;

        MonoObject* runtimeObject = ScriptGCManager::GetReferencedObject(m_RuntimeInstance);
        ScriptUtils::SetFieldValue(runtimeObject, m_FieldInfo, data);
    }
}

