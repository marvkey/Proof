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
    ScriptClassesContainerMetaData* ScriptWorld::GetEntityFields(Entity entity) const
    {
        if (!m_EntityClassesStorage.contains(entity.GetUUID()))
            return nullptr;

        return &m_EntityClassesStorage[entity.GetUUID()];
    }

	void ScriptWorld::InstantiateScriptEntity(Entity entity)
	{
		EditorInstantiateScriptEntity(entity);
	}
	void ScriptWorld::EditorInstantiateScriptEntity(Entity entity)
	{
        if (!entity.HasComponent<ScriptComponent>())
            return;
        ScriptComponent& script = entity.GetComponent<ScriptComponent>();

        bool hasScripts = false;
        for (int i = 0; i < script.ScriptMetadates.size(); i++)
        {
            if (AssetManager::HasAsset(script.ScriptMetadates[i].ScriptClassID))
            {
                hasScripts = true;
                m_EntityClassesStorage[entity.GetUUID()] = {};
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

	void ScriptWorld::RuntimeInstantiateScriptEntity(Entity entity)
	{
	}

	void ScriptWorld::ScriptEntityPushScript(Entity entity, Count<class ScriptFile> script)
	{
        EditorScriptEntityPushScript(entity, script);
	}
   
	void ScriptWorld::EditorScriptEntityPushScript(Entity entity, Count<class ScriptFile> scriptFile)
	{
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
        }

        if (!scriptComponent.HasScript(scriptFile->GetID()))
            scriptComponent.ScriptMetadates.emplace_back(scriptFile->GetID());
	}
	void ScriptWorld::RuntimeScriptEntityPushScript(Entity entity, Count<class ScriptFile> script)
	{
	}

    bool ScriptWorld::IsEntityScriptInstantiated(Entity entity)
    {
        return EditorIsEntityScriptInstantiated(entity);
    }

   
   
    bool ScriptWorld::EditorIsEntityScriptInstantiated(Entity entity)
    {
        return m_EntityClassesStorage.contains(entity.GetUUID());
    }
    bool ScriptWorld::RuntimeIsEntityScriptInstantiated(Entity entity)
    {
        return false;
    }
    void ScriptWorld::DestroyEntityScript(Entity entity, bool clear )
    {
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
    }
    Count<ScriptWorld> ScriptWorld::CopyScriptWorld(Count<ScriptWorld> world, Count<World> newWorld, bool useSameMemmory)
    {
        Count<ScriptWorld> newScirptWorld = Count<ScriptWorld>::Create(newWorld);
        if (useSameMemmory)
        {
            newScirptWorld->m_EntityClassesStorage = world->m_EntityClassesStorage;
            return newScirptWorld;
        }

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
                        fieldStorage->CopyFrom(fieldStorage);
                    }
                }
            }
        }
        return newScirptWorld;
    }
    const std::map<UUID, WeakCount<ScriptWorld>>& ScriptWorld::GetScriptWorlds()
    {
        return s_ScriptWorldReferences;
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

}

