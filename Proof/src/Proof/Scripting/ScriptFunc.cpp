#include "Proofprch.h"
#include "ScriptFunc.h"
#include "Proof/Scene/Entity.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "ScriptEngine.h"
namespace Proof
{

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	#define PF_ADD_INTERNAL_CALL(Name){\
		mono_add_internal_call("Proof.InternalCalls::" #Name, Name);\
		PF_ENGINE_TRACE("	C# registered function {}", #Name);\
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType) {
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world,"world is nullptr");
		Entity entity =world->GetEntity( entityID);
		PF_CORE_ASSERT(entity,"world is null");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		PF_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(),"mangaed type does not exist");
		// second paremter is calling the funciton
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}
#pragma region TransformComponent
	static void TransformComponent_GetLocation(UUID entityID, glm::vec3* outLocation) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		*outLocation = entity.GetComponent<TransformComponent>()->Location;
	};
	static void TransformComponent_SetLocation(UUID entityID, glm::vec3* location) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		entity.GetComponent<TransformComponent>()->Location = *location;
	};
#pragma endregion 

#pragma region Random
	static float Random_Float(float min, float max) {
		return Random::Real<float>(min, max);
	}
#pragma endregion 

	template<typename... Component>
	static void RegisterComponent() {
		//iterate over templates list
		PF_ENGINE_INFO("C# Register Components");
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Proof.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType) {
					PF_ENGINE_ERROR("	C# Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
				PF_ENGINE_TRACE("	C# Component Registered {}", managedTypename);
			}(), ...); //... keep expanding templates
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>) {
		RegisterComponent<Component...>();
	}

	
	void ScriptFunc::RegisterAllComponents() {
		RegisterComponent(AllComponents{});

	}
	void ScriptFunc::RegisterFunctions() {
		PF_ENGINE_INFO("C# Register Functions");
		PF_ADD_INTERNAL_CALL(Entity_HasComponent);

		//Transform Component
		{
			PF_ADD_INTERNAL_CALL(TransformComponent_GetLocation);
			PF_ADD_INTERNAL_CALL(TransformComponent_SetLocation);
		}
		//Random
		{
			PF_ADD_INTERNAL_CALL(Random_Float);
		}
	}
}
