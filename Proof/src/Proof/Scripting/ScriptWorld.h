#pragma once
#include "Proof/Core/Core.h"
#include "ScriptGCManager.h"
#include <map>
#include "Proof/Asset/AssetTypes.h"
#include "Proof/Scene/World.h"

namespace Proof
{
	struct ScriptClassMetaData
	{
		std::string className;
		//AssetID ScriptAssetID;
		ScriptGCHandle ScriptHandle = nullptr;
		bool IsExistOnlyRuntime = true;
		std::unordered_map<std::string, Count<class FieldStorageBase>> Fields;
	};

	struct ScriptClassesContainerMetaData
	{
		const std::unordered_map<std::string, ScriptClassMetaData>& GetClassesMetaData()const
		{
			return Classes;
		}
		ScriptClassMetaData* GetClassMetaData(const std::string& classname)const
		{
			if (!Classes.contains(classname))
				return nullptr;

			return &Classes.at(classname);
		}


		bool HasClassMetaData(const std::string& className)
		{
			return Classes.contains(className);
		}
	private:
		// class name,, class metaData
		mutable std::unordered_map<std::string, ScriptClassMetaData> Classes;
		friend class ScriptEngine;
		friend class ScriptWorld;
	};

	using ScriptEntityClassStorage = std::unordered_map < UUID, ScriptClassesContainerMetaData>;


	struct RuntimeScriptClassMetaData
	{
		std::string ClassName;
		ScriptGCHandle ScriptHandle = nullptr;
	};
	struct RuntimeScriptClassesContainerMetaData
	{
		const std::unordered_map<std::string, RuntimeScriptClassMetaData>& GetClassesMetaData()const
		{
			return Classes;
		}
		RuntimeScriptClassMetaData* GetClassMetaData(const std::string& classname)const
		{
			if (!Classes.contains(classname))
				return nullptr;

			return &Classes.at(classname);
		}


		bool HasClassMetaData(const std::string& className)
		{
			return Classes.contains(className);
		}
	private:
		mutable std::unordered_map<std::string, RuntimeScriptClassMetaData> Classes;
		friend class ScriptEngine;
		friend class ScriptWorld;
	};

	using ScriptInstanceMap = std::unordered_map<UUID, RuntimeScriptClassesContainerMetaData>;

	class Entity;

	struct EntityInvokeFuncs
	{
		UUID EntityID = 0;

		float Time =0.0f;
		ScriptGCHandle ScriptHandle = nullptr;
		std::string MeathodName = "";

		bool Repeat = false;
		float RepeatTime = Time;
	};
	class ScriptWorld : public RefCounted
	{
	public:
		ScriptWorld(class World* world);
		~ScriptWorld();

		void InstantiateScriptEntity(Entity entity);
		void ScriptEntityPushScript(Entity entity, const std::string& classFullName);
		void ScriptEntityDeleteScript(Entity entity, const std::string& classFullName);
		bool IsEntityScriptInstantiated(Entity entity);
		void DestroyEntityScript(Entity entity);

		ScriptClassesContainerMetaData* GetEntityClassesContainer(Entity entity, bool ifRuntimeGenerateFields = false) const;
		ScriptClassMetaData* GetEntityClass(Entity entity, const std::string& classFullName) const;
		Count<FieldStorageBase> GetEntityClassField(Entity entity, const std::string& classFullName, const std::string& fieldName);
		ScriptGCHandle GetScriptInstance(Entity entity, const std::string& classFullName);
		ScriptGCHandle GetScriptInstanceOfType(Entity entity, const std::string& classFullName); // retursn if its a sub class of that
		const std::unordered_map<std::string, RuntimeScriptClassMetaData>& GetRuntimeAllScriptInstances(Entity entity);
		const std::unordered_map<std::string, ScriptClassMetaData>& GetEditorAllScriptInstances(Entity entity);

		void DuplicateScriptInstance(Entity srcEntity, Entity dstEntity);
		// for things like prefabs or creating from another entity
		// if for example the base entity has a entity field
		// that points to a camera of a sub entity
		// we check it here and update the field accordinly
		// first entity swap is srcEntity, // second is dstEntity
		void PostDuplicateScriptInstance(Entity srcEntity, Entity dstEntity, std::unordered_map<UUID, UUID>& entitySwapID);

		Count <class World> GetWorld();

		void BeginRuntime();
		void OnUpdate(FrameTime frame);
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime);
		void OnPostUpdate(FrameTime frame);
		void EndRuntime();

		bool IsRuntime() { return m_IsRuntime; }

		void AddInvoke(EntityInvokeFuncs invoke);
		const std::unordered_map<std::string, std::unordered_set<UUID>>& GetScriptByType()const {return m_ScriptByType; };
		const std::unordered_map<std::string, std::unordered_set<UUID>>& GetScriptByType(){return m_ScriptByType; };
	private:


		void EditorScriptEntityPushScript(Entity entity, const std::string& classFullName);
		void RuntimeScriptEntityPushScript(Entity entity, const std::string& classFullName);

		void EditorScriptEntityDeleteScript(Entity entity, const std::string& classFullName);
		void RuntimeScriptEntityDeleteScript(Entity entity, const std::string& classFullName);

		void InternalPushScript(Entity entity, const std::string& classFullName);
		void InternalDeleteScript(Entity entity, const std::string& classFullName);

		bool EditorIsEntityScriptInstantiated(Entity entity);
		bool RuntimeIsEntityScriptInstantiated(Entity entity);

		void DestroyEntityScript(Entity entity, bool clear);
		void EditorDestroyEntityScript(Entity entity, bool clear);
		void RuntimeDestroyEntityScript(Entity entity, bool clear);


		const ScriptEntityClassStorage& GetEditorEntityClassesStorage()const { return m_EntityClassesStorage; };
		const ScriptInstanceMap& GetRuntimeEntityClassStorage()const { return m_RuntimeEntityClassStorage; };

	private:
		static Count<ScriptWorld>CopyScriptWorld(Count<ScriptWorld> scirptWorld, Count<World> newWorld, bool useSameMemmory = false);
	private:
		bool m_IsRuntime = false;
		// weak tr it is alive whenver world is alive not only in rumtime, so it keeps world alive and not delete

		WeakCount<class World> m_World;
		mutable ScriptEntityClassStorage m_EntityClassesStorage;
		mutable ScriptInstanceMap m_RuntimeEntityClassStorage;
		std::unordered_set<ScriptGCHandle> m_CallOnCreate;// dont wanna call excactly when created because other entity systems might still be not created yet
		UUID m_SpecificID;
		friend class ScriptEngine;
		friend class World;

		static const std::map<UUID, WeakCount<ScriptWorld>>& GetScriptWorlds();

		std::vector< EntityInvokeFuncs> m_EntityInvokes;
		std::unordered_map<std::string, std::unordered_set<UUID>> m_ScriptByType; // (class name, list of all entities with that class)

	};
}