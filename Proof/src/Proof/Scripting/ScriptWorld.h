#pragma once
#include "Proof/Core/Core.h"
#include "ScriptGCManager.h"
#include <map>
namespace Proof
{
	struct ScriptClassMetaData
	{
		std::string className;
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

	class Entity;
	class ScriptWorld : RefCounted
	{
	public:
		ScriptWorld(Count<class World> world);
		~ScriptWorld();
		void InstantiateScriptEntity(Entity entity);
		void ScriptEntityPushScript(Entity entity, Count<class ScriptFile> script);
		bool IsEntityScriptInstantiated(Entity entity);
		ScriptClassesContainerMetaData* GetEntityFields(Entity entity) const;

	private:

		void EditorInstantiateScriptEntity(Entity entity);
		void RuntimeInstantiateScriptEntity(Entity entity);

		void EditorScriptEntityPushScript(Entity entity, Count<class ScriptFile> script);
		void RuntimeScriptEntityPushScript(Entity entity, Count<class ScriptFile> script);

		bool EditorIsEntityScriptInstantiated(Entity entity);
		bool RuntimeIsEntityScriptInstantiated(Entity entity);
	private:
		bool m_IsRuntime = false;
		Count<class World> m_World;
		mutable ScriptEntityClassStorage m_EntityClassesStorage;
		UUID m_SpecificID;
		friend class ScriptEngine;

		static const std::map<UUID, WeakCount<ScriptWorld>>& GetScriptWorlds();
	};
}