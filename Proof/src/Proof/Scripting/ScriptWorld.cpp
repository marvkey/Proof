#include "Proofprch.h"
#include "ScriptWorld.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
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

    ScriptClassesContainerMetaData* ScriptWorld::GetEntityClassesContainer(Entity entity) const
    {
        if (m_IsRuntime)
        {
            if (!m_RuntimeEntityClassStorage.contains(entity.GetUUID()))
                return nullptr;

            if (!m_EntityClassesStorage.contains(entity.GetUUID()))
                m_EntityClassesStorage[entity.GetUUID()] = {};

            for (const auto& [className, classMetaData] : m_RuntimeEntityClassStorage[entity.GetUUID()].Classes)
            {
                if (m_EntityClassesStorage[entity.GetUUID()].Classes.contains(className))
                    continue;
                // call a non const in const
                const_cast<ScriptWorld*>(this)->EditorScriptEntityPushScript(entity, classMetaData.ClassName);
                //EditorScriptEntityPushScript(entity, AssetManager::GetAsset< ScriptFile>(classMetaData.ScriptAssetID));
            }
            return &m_EntityClassesStorage[entity.GetUUID()];
        }
        if (!m_EntityClassesStorage.contains(entity.GetUUID()))
            return nullptr;

        return &m_EntityClassesStorage[entity.GetUUID()];
    }

    ScriptClassMetaData* ScriptWorld::GetEntityClass(Entity entity, const std::string& classFullName) const
    {

        ScriptClassesContainerMetaData* classContainer = GetEntityClassesContainer(entity);
        if (!classContainer)return nullptr;

        if (classContainer->HasClassMetaData(classFullName))
            return &classContainer->Classes.at(classFullName);

        return nullptr;
    }


    Count<FieldStorageBase> ScriptWorld::GetEntityClassField(Entity entity, const std::string& classFullName, const std::string& fieldName)
    {
        ScriptClassMetaData* classMetaData = GetEntityClass(entity, classFullName);
        if (!classMetaData)return nullptr;
        if (!classMetaData->Fields.contains(classFullName))return nullptr;
        return classMetaData->Fields.at(fieldName);
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
            if (ScriptEngine::IsModuleValid((script.ScriptMetadates[i].ClassName)))
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
            if (ScriptEngine::IsModuleValid(script.ScriptMetadates[i].ClassName))
            {
                ScriptEntityPushScript(entity, script.ScriptMetadates[i].ClassName);
            }
            else
            {
                script.ScriptMetadates.erase(script.ScriptMetadates.begin() + i);
            }
        }
	}

	void ScriptWorld::ScriptEntityPushScript(Entity entity, const std::string& classFullName)
	{
        if (m_IsRuntime)
            RuntimeScriptEntityPushScript(entity, classFullName);
        else
            EditorScriptEntityPushScript(entity, classFullName);
	}
   
	void ScriptWorld::EditorScriptEntityPushScript(Entity entity, const std::string& classFullName)
	{
        PF_PROFILE_FUNC();

        PF_CORE_ASSERT(m_EntityClassesStorage.contains(entity.GetUUID()), fmt::format("Entity {} is not contained in Script Engine ", entity.GetName()));

        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!ScriptEngine::IsModuleValid(classFullName))
        {
            PF_ENGINE_ERROR("Trying to add invalid script to Entity {}", entity.GetName());
            return;
        }

        if (m_EntityClassesStorage[entity.GetUUID()].HasClassMetaData(classFullName))
        {
            PF_ENGINE_ERROR("Trying to add Script: {} to Entity:{} that already contains script", classFullName, entity.GetName());
            return;
        }
        ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(classFullName);
        if (!managedClass)
            return;

        // in case we want to view in ditorc
        if (m_IsRuntime)
        {
            if(m_EntityClassesStorage[entity.GetUUID()].Classes.contains(classFullName))
                return;
        }
        auto& scriptEngineData = m_EntityClassesStorage[entity.GetUUID()].Classes[(classFullName)];

        scriptEngineData.className = classFullName;
       
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
                scriptEngineData.Fields[fieldName]->SetRuntimeInstance(m_RuntimeEntityClassStorage[entity.GetUUID()].Classes[classFullName].ScriptHandle);
            }
        }

        if (m_IsRuntime)
            return;

        if (!scriptComponent.HasScript(classFullName))
            scriptComponent.ScriptMetadates.emplace_back(classFullName);
	}
	void ScriptWorld::RuntimeScriptEntityPushScript(Entity entity, const std::string& classFullName)
	{
        PF_PROFILE_FUNC();

        PF_CORE_ASSERT(m_RuntimeEntityClassStorage.contains(entity.GetUUID()), fmt::format("Entity {} is not contained in Script Engine ", entity.GetName()));

        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!ScriptEngine::IsModuleValid(classFullName))
        {
            PF_ENGINE_ERROR("Runtime Trying to add invalid script to Entity {}", entity.GetName());
            return;
        }

        if (m_RuntimeEntityClassStorage[entity.GetUUID()].HasClassMetaData(classFullName))
        {
            PF_ENGINE_ERROR("Runtime Trying to add Script: {} to Entity:{} that already contains script", classFullName, entity.GetName());
            return;
        }
        ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(classFullName);
        if (!managedClass)
            return;

        auto& scriptEngineData = m_RuntimeEntityClassStorage[entity.GetUUID()].Classes[(classFullName)];

        MonoObject* runtimeInstance = ScriptEngine::CreateManagedObject(classFullName, entity.GetUUID());
        ScriptGCHandle instanceHandle = ScriptGCManager::CreateObjectReference(runtimeInstance, false);

        scriptEngineData.ClassName = classFullName;
        scriptEngineData.ScriptHandle = instanceHandle;

        if (m_EntityClassesStorage.contains(entity.GetUUID()))
        {
            ScriptClassMetaData* classMetaData =  m_EntityClassesStorage.at(entity.GetUUID()).GetClassMetaData(classFullName);
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
        if (!scriptComponent.HasScript(classFullName))
            scriptComponent.ScriptMetadates.emplace_back(classFullName);

        ScriptEngine::CallMethod(instanceHandle, "OnCreate");

	}
    void ScriptWorld::ScriptEntityDeleteScript(Entity entity, const std::string& classFullName)
    {
        if (m_IsRuntime)
            RuntimeScriptEntityDeleteScript(entity, classFullName);
        else
            EditorScriptEntityDeleteScript(entity, classFullName);
    }
    
    void ScriptWorld::EditorScriptEntityDeleteScript(Entity entity, const std::string& classFullName)
    {
        if (!entity.HasComponent<ScriptComponent>())
            return;
        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!EditorIsEntityScriptInstantiated(entity))
            return;

        ScriptClassesContainerMetaData* entityClassContainer = GetEntityClassesContainer(entity);

        if (!entityClassContainer)
            return;

        if(!entityClassContainer->HasClassMetaData(classFullName))
            return;


        if (m_IsRuntime)
        {
            if (entityClassContainer->Classes.at(classFullName).IsExistOnlyRuntime == false)
                return;
        }

        entityClassContainer->Classes.erase(classFullName);

        int index = scriptComponent.GetScriptIndex(classFullName);
        if (index != -1)
            scriptComponent.ScriptMetadates.erase(scriptComponent.ScriptMetadates.begin() + index);
    }

    void ScriptWorld::RuntimeScriptEntityDeleteScript(Entity entity, const std::string& classFullName)
    {
        if (!entity.HasComponent<ScriptComponent>())
            return;
        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!RuntimeIsEntityScriptInstantiated(entity))
            return;

        RuntimeScriptClassesContainerMetaData* entityClassContainer = &m_RuntimeEntityClassStorage.at(entity.GetUUID());
        if (!entityClassContainer)
            return;
        if (!entityClassContainer->HasClassMetaData(classFullName))
            return;

        ScriptEngine::CallMethod(entityClassContainer->Classes.at(classFullName).ScriptHandle, "OnDestroy");
        entityClassContainer->Classes.erase(classFullName);

        int index = scriptComponent.GetScriptIndex(classFullName);
        if (index != -1)
            scriptComponent.ScriptMetadates.erase(scriptComponent.ScriptMetadates.begin() + index);

        if (m_EntityClassesStorage.contains(entity.GetUUID()))
        {
            if(m_EntityClassesStorage.at(entity.GetUUID()).Classes.contains(classFullName))
                EditorScriptEntityDeleteScript(entity, classFullName);
        }
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
            PF_ENGINE_ERROR("Cannot Delete Script Entity: {} Does not exist", entity.GetName());
            return;
        }

        if (clear)
        {
            auto& sc = entity.GetComponent<ScriptComponent>();
            for (auto& metadata : sc.ScriptMetadates)
                ScriptEntityDeleteScript(entity, metadata.ClassName);
            sc.ScriptMetadates.clear();
        }
        m_EntityClassesStorage.erase(entity.GetUUID());
    }
    void ScriptWorld::RuntimeDestroyEntityScript(Entity entity, bool clear)
    {
        PF_PROFILE_FUNC();

        if (!entity.HasComponent<ScriptComponent>())
            return;

        if (!RuntimeIsEntityScriptInstantiated(entity))
        {
            PF_ENGINE_ERROR("Runtime Cannot Delete Script Entity: {} Does not exist", entity.GetName());
            return;
        }
        if (clear)
        {
            auto& sc = entity.GetComponent<ScriptComponent>();
            for (auto& [className,metadata] : m_RuntimeEntityClassStorage.at(entity.GetUUID()).Classes)
                ScriptEntityDeleteScript(entity, metadata.ClassName);
            sc.ScriptMetadates.clear();
        }
        m_RuntimeEntityClassStorage.erase(entity.GetUUID());


        #if 0
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
        #endif

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
            newScirptWorld->m_EntityClassesStorage[entityID] = {};

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
        ScriptClassesContainerMetaData* srcClassesMetaData =  srcScriptWorld->GetEntityClassesContainer(srcEntity);
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

        // we make a copy when we are going to run the scen
        m_World->ForEachEnitityWith<ScriptComponent>([&](Entity entity)
        {
            RuntimeDestroyEntityScript(entity,true);
        });
        m_IsRuntime = false;
        ScriptEngine::EndRuntime();
    }

}

