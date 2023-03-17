#include "Proofprch.h"
#include "ScriptFunc.h"
#include "Proof/Scene/Entity.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "ScriptEngine.h"
#include "Proof/Scene/Physics/PhysicsWorld.h"
//(IMPORTANT)
/*
*WHEN PASSING A MONO TYPE MAKE SURE ITS A SRUCT BECAUSE WHEN ITS A CLASS IT GETS SOME UNDEFNIED BEHAVIOR
*/
namespace Proof
{

	
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	namespace ScriptFuncUtils
	{
		
	
	}
	#define PF_ADD_INTERNAL_CALL(Name){\
		mono_add_internal_call("Proof.InternalCalls::" #Name, (void*)Name);\
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
	static bool Entity_HasComponent(EntityID entityID, MonoReflectionType* componentType) {
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world,"world is nullptr");
		Entity entity =world->TryGetEntity( entityID);
		PF_CORE_ASSERT(entity,"world is null");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		PF_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(),"mangaed type does not exist");
		// second paremter is calling the funciton
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}
#pragma endregion 

#pragma region TransformComponent
	static void TransformComponent_GetLocation(uint64_t entityID,Vector* outLocation ) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
			if (!entity)
			{
				PF_ERROR("TransformComponent.GetLocation - entity is invalid");
				return;
			}
		#endif
		*outLocation = entity.GetComponent<TransformComponent>()->Location;
	};
	static void TransformComponent_SetLocation(EntityID entityID, Vector* location) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("TransformComponent.SetLocation - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>()->Location = *location;
	};
#pragma endregion 

#pragma region RigidBody
	static void RigidBody_GetMass(EntityID entityID, float* outMass) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.GetMass - entity is invalid or Does not have rigidBody");
			return;
		}
		#endif
		*outMass = entity.GetComponent<RigidBodyComponent>()->Mass;
	}
	static void RigidBody_SetMass(EntityID entityID, float* mass) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.SetMass - entity is invalid or Does not have rigidBody");
			return;
		}
		#endif
		entity.GetComponent<RigidBodyComponent>()->Mass = *mass;
	}
	static void RigidBody_AddForce(EntityID entityID, Vector force, int forceMode, bool autoAwake) {
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.AddForce - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->AddForce(force, (ForceMode)forceMode, autoAwake);
	}
	static void RigidBody_AddTorque(EntityID entityID, Vector force, int forceMode, bool autoAwake) {
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.AddTorque - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->AddTorque(force, (ForceMode)forceMode, autoAwake);
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
		s_EntityHasComponentFuncs.clear();
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
