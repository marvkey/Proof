#include "Proofprch.h"
#include "ScriptImportSettings.h"
#include "ScriptRegistry.h"
#include "Proof/Core/Buffer.h"
#include "ScriptField.h"
#include "ScriptTypes.h"
namespace Proof
{
    std::unordered_map<std::string, ScriptImportClassMetaData> ScriptImportSettings::s_Classes;
    const ScriptImportClassMetaData& ScriptImportSettings::GetClassMetaData(std::string className)
    {
        PF_CORE_ASSERT(HasClass(className));

        return s_Classes.at(className);
    }
    struct ScriptReloadClassReloadMetadata
    {
        std::string ClassName;
        std::unordered_map<std::string, std::pair<ScriptFieldType,Buffer>> Fields;
    };
    void ScriptImportSettings::LoadAssembly()
    {
        // script name, fields
        std::unordered_map<std::string, ScriptReloadClassReloadMetadata> oldFieldValues;

        for (auto& [scriptName,scriptMetaData] : s_Classes)
        {
            ScriptReloadClassReloadMetadata& scriptReloadData = oldFieldValues[scriptName];
            scriptReloadData.ClassName = scriptName;

            for (auto& [fieldName,field]: scriptMetaData.Fields)
            {
                scriptReloadData.Fields[fieldName].second.Copy(field->GetValueBuffer());
                scriptReloadData.Fields[fieldName].first = field->GetFieldInfo()->Type;
            }
        }

        s_Classes.clear();

        for (auto& [scriptName, script] : ScriptRegistry::GetEntityScripts())
        {
            s_Classes[scriptName].className = scriptName;

            ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(scriptName);

            PF_ENGINE_INFO("ScriptImportSettings New Script: {}", scriptName);
            for (auto& fieldName : managedClass->Fields)
            {
                ScriptField* field = ScriptRegistry::GetFieldByName(fieldName);

                if (!field->IsWritable())
                    continue;

                ScriptFieldType nativeType = field->Type;
                if (!IsScriptFieldAssetType(nativeType))
                    continue;
                if(field->IsArray())
                {
                    s_Classes[scriptName].Fields[fieldName] = Count<ArrayFieldStorage>::Create(field);
                }
                else
                {
                    s_Classes[scriptName].Fields[fieldName] = Count<FieldStorage>::Create(field);
                }
                PF_ENGINE_TRACE("    Added Field: {}", fieldName);
            }
        }

        for (auto& [oldClassName, reloadClassMetaData] : oldFieldValues)
        {
            if (!s_Classes.contains(oldClassName))
                continue;
            for (auto& [fieldName,fieldData] : reloadClassMetaData.Fields)
            {
                if (!s_Classes[oldClassName].Fields.contains(fieldName))
                    continue;

                if (s_Classes[oldClassName].Fields[fieldName]->GetFieldInfo()->Type == fieldData.first)
                {
                    s_Classes[oldClassName].Fields[fieldName]->SetValueBuffer(fieldData.second);
                }
            }
        }
    }
    void ScriptImportSettings::Init()
    {
    }
    void ScriptImportSettings::ShutDown()
    {
        s_Classes.clear();
    }
}
