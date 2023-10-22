#include "Proofprch.h"
#include "ScriptWorld.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Asset/AssetManager.h"
#include "ScriptEngine.h"
#include "ScriptFile.h"
#include "ScriptField.h"
namespace Proof
{
    static std::map<UUID, WeakCount<ScriptWorld>> s_ScriptWorldReferences;

	ScriptWorld::ScriptWorld(Count<class World> world)
		:m_World(world)
	{
		PF_CORE_ASSERT(m_World, "Needs an active world");

        m_SpecificID = UUID();

        while (s_ScriptWorldReferences.contains(m_SpecificID))
        {
            m_SpecificID = UUID();
        }
        s_ScriptWorldReferences[m_SpecificID] = this;
	}
	ScriptWorld::~ScriptWorld()
	{
        s_ScriptWorldReferences.erase(m_SpecificID);
	}
    const std::map<UUID, WeakCount<ScriptWorld>>& ScriptWorld::GetScriptWorlds()
    {
        return s_ScriptWorldReferences;
    }

    ScriptClassesContainerMetaData* ScriptWorld::GetEntityFields(Entity entity) const
    {
        if (m_IsRuntime)
        {
            if (!m_RuntimeEntityClassStorage.contains(entity.GetUUID()))
                return nullptr;

            if (!m_EntityClassesStorage.contains(entity.GetUUID()))
                m_EntityClassesStorage[entity.GetUUID()] = {};

            for (const auto& [className, classMetaData] : m_RuntimeEntityClassStorage[entity.GetUUID()].Classes)
            {
                // call a non const in const
                const_cast<ScriptWorld*>(this)->EditorScriptEntityPushScript(entity, AssetManager::GetAsset< ScriptFile>(classMetaData.ScriptAssetID));
                //EditorScriptEntityPushScript(entity, AssetManager::GetAsset< ScriptFile>(classMetaData.ScriptAssetID));
            }
            return &m_EntityClassesStorage[entity.GetUUID()];
        }
        if (!m_EntityClassesStorage.contains(entity.GetUUID()))
            return nullptr;

        return &m_EntityClassesStorage[entity.GetUUID()];
    }

	void ScriptWorld::InstantiateScriptEntity(Entity entity)
	{
        PF_PROFILE_FUNC();

        if (!entity.HasComponent<ScriptComponent>())
            return;
        ScriptComponent& script = entity.GetComponent<ScriptComponent>();

        bool hasScripts = false;
        for (int i = 0; i < script.ScriptMetadates.size(); i++)
        {
            if (AssetManager::HasAsset(script.ScriptMetadates[i].ScriptClassID))
            {
                hasScripts = true;
                if (m_IsRuntime)
                {
                    m_RuntimeEntityClassStorage[entity.GetUUID()] = {};
                }
                else
                {
                    m_EntityClassesStorage[entity.GetUUID()] = {};
                }
                break;
            }
        }

        if (!hasScripts)
        {
            PF_ENGINE_WARN("Entity {} contains no valid ScriptModule ", entity.GetName());
            script.ScriptMetadates.clear();
            return;
        }
        for (int i = 0; i < script.ScriptMetadates.size(); i++)
        {
            if (AssetManager::HasAsset(script.ScriptMetadates[i].ScriptClassID) && ScriptEngine::IsModuleValid(AssetManager::GetAsset<ScriptFile>(script.ScriptMetadates[i].ScriptClassID)))
            {
                ScriptEntityPushScript(entity, AssetManager::GetAsset<ScriptFile>(script.ScriptMetadates[i].ScriptClassID));
            }
            else
            {
                script.ScriptMetadates.erase(script.ScriptMetadates.begin() + i);
            }
        }
	}
	

	void ScriptWorld::ScriptEntityPushScript(Entity entity, Count<class ScriptFile> script)
	{
        if (m_IsRuntime)
            RuntimeScriptEntityPushScript(entity, script);
        else
            EditorScriptEntityPushScript(entity, script);
	}
   
	void ScriptWorld::EditorScriptEntityPushScript(Entity entity, Count<class ScriptFile> scriptFile)
	{
        PF_PROFILE_FUNC();

        PF_CORE_ASSERT(m_EntityClassesStorage.contains(entity.GetUUID()), fmt::format("Entity {} is not contained in Script Engine ", entity.GetName()));

        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!ScriptEngine::IsModuleValid(scriptFile))
        {
            PF_ENGINE_ERROR("Trying to add invalid script to Entity {}", entity.GetName());
            return;
        }

        if (m_EntityClassesStorage[entity.GetUUID()].HasClassMetaData(scriptFile->GetFullName()))
        {
            PF_ENGINE_ERROR("Trying to Script: {} to Entity:{} that already contains script", scriptFile->GetFullName(), entity.GetName());
            return;
        }
        ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(scriptFile->GetFullName());
        if (!managedClass)
            return;

        // in case we want to view in ditorc
        if (m_IsRuntime)
        {
            if(m_EntityClassesStorage[entity.GetUUID()].Classes.contains(scriptFile->GetFullName()))
                return;
        }
        auto& scriptEngineData = m_EntityClassesStorage[entity.GetUUID()].Classes[(scriptFile->GetFullName())];

        scriptEngineData.className = scriptFile->GetFullName();
        scriptEngineData.ScriptAssetID = scriptFile->GetID();

        for (auto fieldName : managedClass->Fields)
        {
            ScriptField* scriptField = ScriptRegistry::GetFieldByName(fieldName);

            if (!scriptField->HasFlag(FieldFlag::Public))
                continue;

            if (scriptField->IsArray())
            {
            }
            else if (scriptField->IsEnum())
            {
                scriptEngineData.Fields[fieldName] = Count<EnumFieldStorage>::Create(scriptField);
               
            }
            else
            {
                scriptEngineData.Fields[fieldName] = Count<FieldStorage>::Create(scriptField);
               
            }

            // in case we want to view in editor
            if (m_IsRuntime)
            {
                scriptEngineData.Fields[fieldName]->SetRuntimeInstance(m_RuntimeEntityClassStorage[entity.GetUUID()].Classes[scriptFile->GetFullName()].ScriptHandle);
            }
        }

        if (m_IsRuntime)
            return;

        if (!scriptComponent.HasScript(scriptFile->GetID()))
            scriptComponent.ScriptMetadates.emplace_back(scriptFile->GetID());
	}
	void ScriptWorld::RuntimeScriptEntityPushScript(Entity entity, Count<class ScriptFile> scriptFile)
	{
        PF_PROFILE_FUNC();

        PF_CORE_ASSERT(m_RuntimeEntityClassStorage.contains(entity.GetUUID()), fmt::format("Entity {} is not contained in Script Engine ", entity.GetName()));

        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!ScriptEngine::IsModuleValid(scriptFile))
        {
            PF_ENGINE_ERROR("Trying to add invalid script to Entity {}", entity.GetName());
            return;
        }

        if (m_RuntimeEntityClassStorage[entity.GetUUID()].HasClassMetaData(scriptFile->GetFullName()))
        {
            PF_ENGINE_ERROR("Trying to Script: {} to Entity:{} that already contains script", scriptFile->GetFullName(), entity.GetName());
            return;
        }
        ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(scriptFile->GetFullName());
        if (!managedClass)
            return;

        auto& scriptEngineData = m_RuntimeEntityClassStorage[entity.GetUUID()].Classes[(scriptFile->GetFullName())];

        MonoObject* runtimeInstance = ScriptEngine::CreateManagedObject(scriptFile->GetFullName(), entity.GetUUID());
        ScriptGCHandle instanceHandle = ScriptGCManager::CreateObjectReference(runtimeInstance, false);

        scriptEngineData.ClassName = scriptFile->GetFullName();
        scriptEngineData.ScriptAssetID = scriptFile->GetID();
        scriptEngineData.ScriptHandle = instanceHandle;

        if (m_EntityClassesStorage.contains(entity.GetUUID()))
        {
            ScriptClassMetaData* classMetaData =  m_EntityClassesStorage.at(entity.GetUUID()).GetClassMetaData(scriptFile->GetFullName());
            if (classMetaData)
            {
                classMetaData->ScriptHandle = instanceHandle;

                for (auto& [fieldName, fieldStorage] : classMetaData->Fields)
                {
                    if (fieldStorage)
                        fieldStorage->SetRuntimeInstance(instanceHandle);
                }
            }
        }
        if (!scriptComponent.HasScript(scriptFile->GetID()))
            scriptComponent.ScriptMetadates.emplace_back(scriptFile->GetID());

        ScriptEngine::CallMethod(instanceHandle, "OnCreate");

	}

    bool ScriptWorld::IsEntityScriptInstantiated(Entity entity)
    {
        if (m_IsRuntime)
            return RuntimeIsEntityScriptInstantiated(entity);

        return EditorIsEntityScriptInstantiated(entity);
    }

    bool ScriptWorld::EditorIsEntityScriptInstantiated(Entity entity)
    {
        return m_EntityClassesStorage.contains(entity.GetUUID());
    }
    bool ScriptWorld::RuntimeIsEntityScriptInstantiated(Entity entity)
    {
        return m_RuntimeEntityClassStorage.contains(entity.GetUUID());
    }
    void ScriptWorld::DestroyEntityScript(Entity entity, bool clear )
    {
        if (m_IsRuntime)
        {
            RuntimeDestroyEntityScript(entity, clear);
            return;
        }

        EditorDestroyEntityScript(entity,clear);
    }
    void ScriptWorld::DestroyEntityScript(Entity entity)
    {
        DestroyEntityScript(entity, true);
    }
    void ScriptWorld::EditorDestroyEntityScript(Entity entity, bool clear)
    {
        PF_PROFILE_FUNC()
        if (!entity.HasComponent<ScriptComponent>())
            return;

        if (!m_EntityClassesStorage.contains(entity.GetUUID()))
        {
            PF_ENGINE_ERROR("Cannot Deleta Script Entity: {} Does not exist", entity.GetName());
            return;
        }

        if(clear)
            entity.GetComponent<ScriptComponent>().ScriptMetadates.clear();
        m_EntityClassesStorage.erase(entity.GetUUID());
    }
    void ScriptWorld::RuntimeDestroyEntityScript(Entity entity, bool clear)
    {
        PF_PROFILE_FUNC();

        if (!entity.HasComponent<ScriptComponent>())
            return;

        if (RuntimeIsEntityScriptInstantiated(entity))
        {
            PF_ENGINE_ERROR("Cannot Deleta Script Entity: {} Does not exist", entity.GetName());
            return;
        }

        if(clear)
            entity.GetComponent<ScriptComponent>().ScriptMetadates.clear();

        if (EditorIsEntityScriptInstantiated(entity))
        {
            #if 0
            for (auto it = m_EntityClassesStorage.at(entity.GetUUID()).Classes.begin(); it != m_EntityClassesStorage.at(entity.GetUUID()).Classes.end(); /* No increment here */)
            {
                const auto& [className, classMetadata] = *it;

                if (classMetadata.IsExistOnlyRuntime)
                {
                    // Erase the element and update the iterator.
                    it = m_EntityClassesStorage.at(entity.GetUUID()).Classes.erase(it);
                }
                else
                {
                 // Move to the next element.
                    ++it;
                }
            }

            if (m_EntityClassesStorage.at(entity.GetUUID()).Classes.size() == 0)
            {
                m_EntityClassesStorage.erase(entity.GetUUID());
            }
            #endif
            // make a copy of this world when we want to play the world
            m_EntityClassesStorage.erase(entity.GetUUID());

        }
        m_RuntimeEntityClassStorage.erase(entity.GetUUID());
    }
    Count<ScriptWorld> ScriptWorld::CopyScriptWorld(Count<ScriptWorld> world, Count<World> newWorld, bool useSameMemmory)
    {
        Count<ScriptWorld> newScirptWorld = Count<ScriptWorld>::Create(newWorld);
        if (useSameMemmory)
        {
            newScirptWorld->m_EntityClassesStorage = world->m_EntityClassesStorage;

            for (auto& [entityUUID, classes] : newScirptWorld->m_EntityClassesStorage)
            {
                for (auto& [className, classMetaData] : classes.Classes)
                {
                    for (auto& [fieldName, fieldStorage] : classMetaData.Fields)
                    {
                        if (fieldStorage)
                        {
                            fieldStorage->SetRuntimeInstance(nullptr);
                        }
                    }
                }
            }
            return newScirptWorld;
        }

        for (auto& [entityID, classData] : world->m_EntityClassesStorage)
        {
            Entity srcentity = world->GetWorld()->TryGetEntityWithUUID(entityID);
            Entity dstentity = newWorld->TryGetEntityWithUUID(entityID);
            newScirptWorld->DuplicateScriptInstance(srcentity,dstentity);
        }
        return newScirptWorld;
    }
   

    void ScriptWorld::DuplicateScriptInstance(Entity srcEntity, Entity dstEntity)
    {
        PF_PROFILE_FUNC();
        if (!srcEntity.HasComponent<ScriptComponent>() || !dstEntity.HasComponent<ScriptComponent>())
            return;
        Count<ScriptWorld> srcScriptWorld = srcEntity.GetCurrentWorld()->GetScriptWorld();

        if (!srcScriptWorld->IsEntityScriptInstantiated(srcEntity))
            return;

        const auto& srcScriptComp = srcEntity.GetComponent<ScriptComponent>();
        auto& dstScriptComp = dstEntity.GetComponent<ScriptComponent>();


        DestroyEntityScript(dstEntity);
        dstScriptComp = srcScriptComp;

        InstantiateScriptEntity(dstEntity);
        if (!IsEntityScriptInstantiated(dstEntity))return;
        ScriptClassesContainerMetaData* srcClassesMetaData =  srcScriptWorld->GetEntityFields(srcEntity);
        if (!srcClassesMetaData)
            return;

        for (auto& [className, classMetaData] : srcClassesMetaData->Classes)
        {
            for (auto& [fieldName, fieldStorage] : classMetaData.Fields)
            {
                if(fieldStorage)
                    m_EntityClassesStorage[dstEntity.GetUUID()].Classes[className].Fields[fieldName]->CopyFrom(fieldStorage);
            }
        }
    }

    void ScriptWorld::BeginRuntime()
    {
        ScriptEngine::BeginRuntime(m_World);

        for (auto& [enityID, classes] : m_EntityClassesStorage)
        {
            for (auto& [className, classMetaData] : classes.Classes)
            {
                classMetaData.IsExistOnlyRuntime = false;
            }
        }
        m_IsRuntime = true;

        auto view = m_World->GetAllEntitiesWith<ScriptComponent>();
        m_World->ForEachEnitityWith<ScriptComponent>([&](Entity entity)
        {
            InstantiateScriptEntity(entity);
        });
    }

    void ScriptWorld::EndRuntime()
    {
        m_IsRuntime = false;

        // we make a copy when we are going to run the scen
        m_EntityClassesStorage.clear();
        m_RuntimeEntityClassStorage.clear();
        ScriptEngine::EndRuntime();
    }

}

