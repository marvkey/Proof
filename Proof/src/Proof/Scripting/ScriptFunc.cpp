#include "Proofprch.h"
#include "ScriptFunc.h"
#include "Proof/Scene/Entity.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "ScriptEngine.h"

namespace Proof
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	namespace ScriptFuncUtils
	{
		bool CheckMonoError(MonoError& error) {
			bool hasError = !mono_error_ok(&error);
			if (hasError) {
				uint32_t errorCode = mono_error_get_error_code(&error);
				const char* errorMessage = mono_error_get_message(&error);
				PF_ERROR("Code: {} Message: {}", errorCode, errorMessage);
				mono_error_cleanup(&error);
			}
			return hasError;
		}
		std::string MonoToString(MonoString* monoString) {
			if (monoString == nullptr || mono_string_length(monoString) == 0)
				return "";

			MonoError error;
			char* utf8 = mono_string_to_utf8_checked(monoString, &error);
			if (CheckMonoError(error))
				return "";
			std::string result(utf8);
			mono_free(utf8);
			return result;

		}
	}
	#define PF_ADD_INTERNAL_CALL(Name){\
		mono_add_internal_call("Proof.InternalCalls::" #Name, Name);\
		PF_ENGINE_TRACE("	C# registered function {}", #Name);\
	}
#pragma region Log
	static void Log_Message(int logType, MonoString* message) {
		std::wstring ws = mono_string_chars(message);
		std::string str(ws.begin(), ws.end());
		Log::AppendString(logType, Proof::Log::GetClientLogger()->GetLogString(str));
	}
#pragma endregion 

#pragma region Input
	static bool Input_IsKeyClicked(int keycode) {
		return Input::IsKeyClicked((KeyBoardKey) keycode);
	}
	static bool Input_IsKeyPressed(int keycode) {
		return Input::IsKeyPressed((KeyBoardKey)keycode);
	}
	static bool Input_IsKeyReleased(int keycode) {
		return Input::IsKeyReleased((KeyBoardKey)keycode);
	}
	static bool Input_IsKeyHold(int keycode) {
		return Input::IsKeyHold((KeyBoardKey)keycode);
	}
	static bool Input_IsKeyDoubleClick(int keycode) {
		return Input::IsKeyDoubleClick((KeyBoardKey)keycode);
	}
	static bool Input_IsMouseButtonClicked(int mouseCode) {
		return Input::IsMouseButtonClicked((MouseButton)mouseCode);
	}
	static bool Input_IsMouseButtonPressed(int mouseCode) {
		return Input::IsMouseButtonPressed((MouseButton)mouseCode);
	}
	static bool Input_IsMouseButtonReleased(int mouseCode) {
		return Input::IsMouseButtonReleased((MouseButton)mouseCode);
	}
	static bool Input_IsMouseButtonDoubleClicked(int mouseCode) {
		return Input::IsMouseButtonDoubleClicked((MouseButton)mouseCode);
	}
#pragma endregion 

#pragma region Entity
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
#pragma endregion 

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

#pragma region RigidBody
	static void RigidBody_GetMass(UUID entityID, float* outMass) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		*outMass = entity.GetComponent<RigidBodyComponent>()->Mass;
	}
	static void RigidBody_SetMass(UUID entityID, float* mass) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		entity.GetComponent<RigidBodyComponent>()->Mass = *mass;
	}
	static void RigidBody_AddForce(UUID entityID, glm::vec3 force, int forceMode, bool autoAwake) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		entity.GetComponent<RigidBodyComponent>()->AddForce(force, (ForceMode)forceMode, autoAwake);
	}
	static void RigidBody_AddTorque(UUID entityID, glm::vec3 force, int forceMode, bool autoAwake) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		entity.GetComponent<RigidBodyComponent>()->AddTorque(force, (ForceMode)forceMode, autoAwake);
	}
#pragma endregion
#pragma region ScriptFunc

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
#pragma endregion 

	void ScriptFunc::RegisterFunctions() {
		PF_ENGINE_INFO("C# Register Functions");
		//LOG
		{
			PF_ADD_INTERNAL_CALL(Log_Message);
		}
		//Input
		{
			PF_ADD_INTERNAL_CALL(Input_IsKeyClicked);
			PF_ADD_INTERNAL_CALL(Input_IsKeyPressed); 
			PF_ADD_INTERNAL_CALL(Input_IsKeyReleased);
			PF_ADD_INTERNAL_CALL(Input_IsKeyHold);
			PF_ADD_INTERNAL_CALL(Input_IsKeyDoubleClick);

			PF_ADD_INTERNAL_CALL(Input_IsMouseButtonClicked);
			PF_ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
			PF_ADD_INTERNAL_CALL(Input_IsMouseButtonReleased);
			PF_ADD_INTERNAL_CALL(Input_IsMouseButtonDoubleClicked);
		}
		//Entity 
		{
			PF_ADD_INTERNAL_CALL(Entity_HasComponent);
		}
		//Transform Component
		{
			PF_ADD_INTERNAL_CALL(TransformComponent_GetLocation);
			PF_ADD_INTERNAL_CALL(TransformComponent_SetLocation);
		}

		//Rigid Body Component
		{

			PF_ADD_INTERNAL_CALL(RigidBody_GetMass);
			PF_ADD_INTERNAL_CALL(RigidBody_SetMass);
			PF_ADD_INTERNAL_CALL(RigidBody_AddForce);
			PF_ADD_INTERNAL_CALL(RigidBody_AddTorque);
		}
		
	}
}
