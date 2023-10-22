#pragma once
#include "Proof/Core/Core.h"
#include "ScriptGCManager.h"
#include <map>
#include "Proof/Asset/AssetTypes.h"
namespace Proof
{
	struct ScriptClassMetaData
	{
		std::string className;
		AssetID ScriptAssetID;
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
		AssetID ScriptAssetID;
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
	class ScriptWorld : RefCounted
	{
	public:
		ScriptWorld(Count<class World> world);
		~ScriptWorld();
		void InstantiateScriptEntity(Entity entity);
		void ScriptEntityPushScript(Entity entity, Count<class ScriptFile> script);
		bool IsEntityScriptInstantiated(Entity entity);
		void DestroyEntityScript(Entity entity);
		ScriptClassesContainerMetaData* GetEntityFields(Entity entity) const;
		void DuplicateScriptInstance(Entity srcEntity, Entity dstEntity);

		Count <class World> GetWorld()const { return m_World; }

		void BeginRuntime();
		void EndRuntime();
	private:

		//void EditorInstantiateScriptEntity(Entity entity);
		//void RuntimeInstantiateScriptEntity(Entity entity);

		void EditorScriptEntityPushScript(Entity entity, Count<class ScriptFile> script);
		void RuntimeScriptEntityPushScript(Entity entity, Count<class ScriptFile> script);

		bool EditorIsEntityScriptInstantiated(Entity entity);
		bool RuntimeIsEntityScriptInstantiated(Entity entity);

		void DestroyEntityScript(Entity entity, bool clear );
		void EditorDestroyEntityScript(Entity entity, bool clear);
		void RuntimeDestroyEntityScript(Entity entity, bool clear);	

	private:
		static Count<ScriptWorld>CopyScriptWorld(Count<ScriptWorld> scirptWorld, Count<World> newWorld, bool useSameMemmory = false);
	private:
		bool m_IsRuntime = false;
		Count<class World> m_World;
		mutable ScriptEntityClassStorage m_EntityClassesStorage;
		mutable ScriptInstanceMap m_RuntimeEntityClassStorage;
		UUID m_SpecificID;
		friend class ScriptEngine;
		friend class World;

		static const std::map<UUID, WeakCount<ScriptWorld>>& GetScriptWorlds();
	};
}