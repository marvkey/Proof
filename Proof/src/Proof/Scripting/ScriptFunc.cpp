#include "Proofprch.h"
#include "ScriptFunc.h"
#include "Proof/Scene/Entity.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "ScriptEngine.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Input/InputManager.h"
#include <mono/metadata/appdomain.h>
#include "Proof/Scene/SceneSerializer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Proof/Input/Mouse.h"
#include "Proof/Math/MathInclude.h"
#include "Proof/Core/Application.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Physics/PhysicsShapes.h"
#include "Proof/Physics/PhysicsMaterial.h"

#include "Proof/Scene/Mesh.h"

#include "Proof/Asset/AssetManager.h"


#include "ScriptUtils.h"
//(IMPORTPF)
/*
*WHEN PASSING A MONO TYPE MAKE SURE ITS A SRUCT BECAUSE WHEN ITS A CLASS IT GETS SOME UNDEFNIED BEHAVIOR
* 
* basic types liek ints, floats,enume can be returned do not include vectors as return take as pointers
*/
namespace Proof
{
	struct Transform
	{
		glm::vec3 Location;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
	static inline Entity GetEntity(uint64_t entityID)
	{
		Count<World> scene = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(scene, "No active World!");
		return scene->TryGetEntityWithUUID(entityID);
	};

	static inline Count<PhysicsActor> GetPhysicsActor(Entity entity)
	{
		Count<World> scene = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(scene, "No active World!");
		Count<PhysicsWorld> physicsScene = scene->GetPhysicsWorld();
		PF_CORE_ASSERT(physicsScene, "No physics scene world!");
		return physicsScene->GetActor(entity);
	}
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	namespace ScriptFuncUtils
	{
		
	
	}
	#define PF_ADD_INTERNAL_CALL(Name){\
		mono_add_internal_call("Proof.InternalCalls::" #Name, (void*)Name);\
		PF_ENGINE_TRACE("	ScriptFuncs registered function {}", #Name);\
	}

	static void ApplyCameraRotate(uint64_t entityID) 
	{
		return;
		Count<World> world = ScriptEngine::GetWorldContext();
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);

		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("World.DeleteEntity  - entity is invalid");
			return;
		}
		#endif
		//glm::quat quaternionRotation = glm::quat(glm::radians(ProofToglmVec(world->GetWorldRotation(entity))));
		//float playerRotationZ = glm::degrees(glm::eulerAngles(quaternionRotation).y);
		//glm::mat4 cameraRotation = glm::rotate(glm::mat4(1.0f), glm::radians(playerRotationZ), glm::vec3(0.0f, 1.0f, 0.0f));
		//quaternionRotation = glm::quat_cast(cameraRotation);
		//
		//glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(quaternionRotation));
		//entity.GetCamera().GetComponent<TransformComponent>().Rotation.Z = entity.GetComponent<TransformComponent>().Rotation.Z;
		//entity.GetCamera().GetComponent<TransformComponent>().Rotation.Z += 90;
	}
#pragma region Log
	static void Log_Message(int logType, MonoString* message) {
		std::wstring ws = mono_string_chars(message);
		std::string str(ws.begin(), ws.end());
		Log::AppendString(logType, Proof::Log::GetClientLogger()->GetLogString(str));
	}
#pragma endregion 

	#pragma region Application 
	static void Application_Shutdown()
	{
		Application::Get()->ShutDown();
	}

	static float Application_GetFPS()
	{
		return Application::GetFPS();
	}
	#pragma endregion 
	#pragma region Mouse
	static void Mouse_CaptureMouse(bool caputre) 
	{
		//Mouse::CaptureMouse(caputre);
	}

	static void Mouse_GetPosition(Vector2* pos) {
		*pos = { Mouse::GetPosX(),Mouse::GetPosY() };
	}
	static bool Mouse_IsMouseCaptured() 
	{
		return Mouse::IsMouseCaptured();
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
	#pragma region World
	static void World_Pause() 
	{
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		world->Pause();
		
	}

	static void World_Play() {
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		world->Play();
	}
	//retutnrs entity ID
	static uint64_t  World_Instanciate(uint64_t prefabID, Transform transform)
	{
		if (!AssetManager::HasAsset(prefabID))
		{
			PF_EC_ERROR("World.Instanciate - prefabID is invalid {}",prefabID);
			return 0;
		}

		Count<World> world = ScriptEngine::GetWorldContext();
		AssetInfo info = AssetManager::GetAssetInfo(prefabID);
		Count<Prefab> prefab = AssetManager::GetAsset<Prefab>(prefabID);

		TransformComponent componet;
		componet.Location = transform.Location;
		componet.SetRotationEuler(glm::radians(transform.Rotation));
		componet.Scale = transform.Scale;
		Entity entity = world->CreateEntity(info.GetName(), prefab, componet,UUID());
		return entity.GetUUID().Get();
	}

	static bool World_OpenWorld(uint64_t worldID) 
	{
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		if (!AssetManager::HasAsset(worldID))
		{
			PF_EC_ERROR("World.OpenWorld  ID {} is invalid",worldID);
			return false;
		}
		
		//world->EndRuntime();
		
		//SceneSerializer sere(world);
		//if (sere.DeSerilizeText(AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(worldID).Path).string()) == true)
		//{
		//	AssetManager::LoadMultipleAsset(sere.GetAssetLoadID());
		//	//world->StartRuntime();
		//	return true;
		//}
		return false;
	}
	static void World_Restart()
	{
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		world->EnableRestart = true;
		///Count<World> world = ScriptEngine::GetWorldContext();
		///PF_CORE_ASSERT(world, "world is nullptr");
		///SceneSerializer scerelizer(world);
		///auto path = AssetManager::GetAssetInfo(world->GetID()).Path;
		///if (scerelizer.DeSerilizeText(AssetManager::GetAssetFileSystemPath(path).string()) == true)
		///{
		///	AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
		///}

	}
	static uint64_t World_TryFindEntityByTag(MonoString* classFullName)
	{ 
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		std::string tag = ScriptUtils::MonoStringToUTF8(classFullName);
		Entity entity = world->TryGetEntityByTag(tag);
		return entity.GetUUID();
	}

	static bool World_IsEntityValid(uint64_t ID)
	{
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		return world->HasEntity((UUID)ID);
	}
	static void World_ForEachEntityWith(MonoString* classFullName, MonoArray** theArray)
	{
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");

		std::vector<uint64_t> objects;
		std::string className = ScriptUtils::MonoStringToUTF8(classFullName);
		#if 0
		for (auto& [entityID, scripts] : ScriptEngine::EachEntityScript())
		{
			if (scripts.contains(className))
				objects.emplace_back(entityID);
		}
		if (objects.size() == 0)
			return;
		*theArray = mono_array_new(ScriptEngine::GetDomain(), mono_get_uint64_class(), objects.size());
		memcpy(mono_array_addr(*theArray, uint64_t, 0), objects.data(), objects.size() * sizeof(uint64_t));
		#endif
	}
	static void World_DeleteEntity(uint64_t entityID, bool deleteChildren) 
	{
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("World.DeleteEntity  - entity is invalid");
			return;
		}
		#endif
		world->DeleteEntity(entity, deleteChildren);
	}

	//static void World_OpenWorld(AssetID worldID, uint32_t playerCount ) {
	//
	//}
	//static void 
	static float World_GetTimeStep() {
		return FrameTime::GetWorldDeltaTime();
	}
	#pragma endregion
	
	#pragma region Entity
	static void Entity_GetChildren(uint64_t entityID,MonoArray** theArray)
	{
		Count <World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		Entity entity = world->GetEntity(entityID);

		PF_CORE_ASSERT(entity, "Entity is null");

		std::vector<uint64_t> objects;
		entity.EachChild([&](Entity child) {
			objects.emplace_back(child.GetUUID());
		});
		if (objects.size() == 0)
			return;
		#if 0
		*theArray = mono_array_new(ScriptEngine::GetDomain(), mono_get_uint64_class(), objects.size());
		memcpy(mono_array_addr(*theArray, uint64_t, 0), objects.data(), objects.size() * sizeof(uint64_t));
		#endif
	}
	static bool Entity_HasComponent(uint64_t entityID, MonoReflectionType* componentType) {
		if (entityID == 0)return false;
		Count<World> world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world,"world is nullptr");
		Entity entity = world->GetEntity(entityID);
		PF_CORE_ASSERT(entity,"Entity is null");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		PF_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(),"mangaed type does not exist");
		// second paremter is calling the funciton
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static MonoObject* GetScriptInstance(UUID entityID, MonoString* classFullName)
	{
	//	if (!ScriptEngine::EntityHasScripts(ScriptEngine::GetWorldContext()->GetEntity(entityID)))
			return nullptr;
		//return ScriptEngine::GetMonoManagedObject(entityID,ScriptUtils::MonoStringToUTF8(classFullName));
	}

	static void Entity_GetParent(uint64_t entityID, uint64_t* owenerId)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("Entity.GetOwner - entity is invalid");
			return;
		}
		#endif

		if (!entity.GetParent())
		{
			*owenerId = 0;
			return;
		}
		*owenerId = entity.GetParentUUID();
	}
#pragma endregion 
	
	#pragma region AssetID
	static bool AssetID_IsValid(AssetID* AssetID)
	{
		if (!AssetID)
			return false;

		return AssetManager::HasAsset(*AssetID);
	}

	#pragma endregion 
	
	#pragma region TagComponent
	static void TagComponent_GetTag(uint64_t entityID, MonoString** tag)
	{
		//https://stackoverflow.com/questions/22428411/add-mono-internal-call-where-a-string-is-passed-by-reference
		// pass mono by refernce with stirngs

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TagComponent.GetTag - entity is invalid");
			return;
		}

		#endif
		*tag = ScriptUtils::UTF8StringToMono(entity.GetComponent<TagComponent>().Tag);
	}
	static void TagComponent_SetTag(uint64_t entityID, MonoString** tag)
	{
		//https://stackoverflow.com/questions/22428411/add-mono-internal-call-where-a-string-is-passed-by-reference
		// pass mono by refernce with stirngs

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TagComponent.SetTag - entity is invalid");
			return;
		}
		#endif

		std::string newTag = ScriptUtils::MonoStringToUTF8(*tag);
		entity.GetComponent<TagComponent>().Tag  = newTag;
	}
#pragma endregion 
	
	#pragma region TransformComponent
	static void TransformComponent_GetLocation(uint64_t entityID, glm::vec3* outLocation ) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
			if (!entity)
			{
				PF_EC_ERROR("TransformComponent.GetLocation - entity is invalid");
				return;
			}
		#endif
		*outLocation = entity.GetComponent<TransformComponent>().Location;
	};
	static void TransformComponent_SetLocation(UUID entityID, glm::vec3* location) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.SetLocation - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>().Location = *location;
	};

	static void TransformComponent_GetRotation(uint64_t entityID, glm::vec3* outRoation) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.Rotation - entity is invalid");
			return;
		}
		#endif
		* outRoation = glm::degrees( entity.GetComponent<TransformComponent>().GetRotationEuler());
	};

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.SetRotation - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(*rotation));
	};
	static void TransformComponent_GetScale(uint64_t entityID, glm::vec3* outScale) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.Scale - entity is invalid");
			return;
		}
		#endif
		* outScale = entity.GetComponent<TransformComponent>().Scale;
	};
	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.SetScale - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>().Scale = *scale;
	};
	static void TransformComponent_GetFowardVector(uint64_t entityID, glm::vec3* vec)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.GetFowardVector - entity is invalid");
			return;
		}
		#endif
		*vec = entity.GetComponent<TransformComponent>().GetFowardVector();
	}

	static void TransformComponent_GetTransform(uint64_t entityID, Transform* outTransform)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.GetTransform - entity is Invalid");
			*outTransform = Transform();
			return;
		}

		const auto& tc = entity.GetComponent<TransformComponent>();
		outTransform->Location = tc.Location;
		outTransform->Rotation = glm::degrees(tc.GetRotationEuler());
		outTransform->Scale = tc.Scale;
	}

	void TransformComponent_SetTransform(uint64_t entityID, Transform* inTransform)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.SetTransform - Invalid entity!");
			return;
		}

		if (inTransform == nullptr)
		{
			PF_EC_ERROR("TransformComponent.SetTransform - Attempting to set null transform!");
			return;
		}

		auto tc = entity.GetComponent<TransformComponent>();
		tc.Location = inTransform->Location;
		tc.SetRotationEuler(glm::radians(inTransform->Rotation));
		tc.Scale = inTransform->Scale;
	}

	void TransformComponent_GetWorldSpaceTransform(uint64_t entityID, Transform* outTransform)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.GetWorldSpaceTransform - Invalid entity!");
			*outTransform = Transform();
			return;
		}

		Count<World> scene = ScriptEngine::GetWorldContext();
		const auto& wt = scene->GetWorldSpaceTransformComponent(entity);
		outTransform->Location = wt.Location;
		outTransform->Rotation = glm::degrees(wt.GetRotationEuler());
		outTransform->Scale = wt.Scale;
	}
#pragma endregion 
	
	#pragma region TextComponent

	static void TextComponent_GetText(uint64_t entityID, MonoString** text) 
	{

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TextComponent.GetText - entity is invalid");
			return;
		}

		#endif
		if (!entity.HasComponent<TextComponent>())
		{
			PF_EC_ERROR("TextComponent.GetText - Does not have TextComponent");
			return;
		}
		* text = ScriptUtils::UTF8StringToMono(entity.GetComponent<TextComponent>().Text);
	}

	static void TextComponent_SetText(uint64_t entityID, MonoString** text)
	{

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TextComponent.SetText - entity is invalid");
			return;
		}
		#endif
		if (!entity.HasComponent<TextComponent>())
		{
			PF_EC_ERROR("TextComponent.SetText - Does not have TextComponent");
			return;
		}
		entity.GetComponent<TextComponent>().Text = ScriptUtils::MonoStringToUTF8(*text);
	}
#pragma endregion

	#pragma region RigidBody
	static bool RigidBody_GetGravity(UUID entityID) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.GetGravity - entity is invalid or Does not have rigidBody");
			return false;
		}
		#endif
		return entity.GetComponent<RigidBodyComponent>().Gravity;
	}

	static void RigidBody_SetGravity(UUID entityID, bool* gravity) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.SetGravity - entity is invalid or Does not have rigidBody");
			return ;
		}
		#endif
		entity.GetComponent<RigidBodyComponent>().Gravity = *gravity;
	}
	static void RigidBody_GetMass(UUID entityID, float* outMass) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.GetMass - entity is invalid or Does not have rigidBody");
			return;
		}
		#endif
		*outMass = entity.GetComponent<RigidBodyComponent>().Mass;
	}
	static void RigidBody_SetMass(UUID entityID, float* mass) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.SetMass - entity is invalid or Does not have rigidBody");
			return;
		}
		#endif
		entity.GetComponent<RigidBodyComponent>().Mass = *mass;
	}
	static void RigidBody_AddForce(UUID entityID, glm::vec3* force, int forceMode, bool autoAwake) 
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.AddForce - entity is not valid");
			return;
		}
		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.AddForce - physics actor not found");
			return;
		}
		actor->AddForce(*force, (ForceMode)forceMode, autoAwake);
	}
	static void RigidBody_AddTorque(UUID entityID, glm::vec3* force, int forceMode, bool autoAwake) 
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.AddTorque - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.AddTorque - physics actor not found");
			return;
		}
		actor->AddTorque(*force, (ForceMode)forceMode, autoAwake);
	}

	static void RigidBody_ClearForce(UUID entityID, int* forceMode)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.ClearForce - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.ClearForce - physics actor not found");
			return;
		}
		actor->ClearForce((ForceMode)*forceMode);
	}

	static void RigidBody_ClearTorque(UUID entityID, int* forceMode)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.ClearTorque - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.ClearTorque - physics actor not found");
			return;
		}
		actor->ClearTorque((ForceMode)*forceMode);
	}

	static RigidBodyType RigidBodyComponent_GetBodyType(UUID entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyComponentGetBodyType - entity is not valid");
			return (RigidBodyType)-1;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyComponentGetBodyType - physics actor not found");
			return (RigidBodyType)-1;
		}
		return actor->GetRigidBodyType();
		
	}

	static void RigidBodyComponent_SetBodyType(UUID entityID, RigidBodyType type)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyComponentSetBodyType - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyComponentSetBodyType - physics actor not found");
			return;
		}
		actor->SetRigidType(type);

	}

	static bool RigidBodyComponent_IsKinematic(UUID entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyComponentIsKinematic - entity is not valid");
			return false;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyComponentIsKinematic - physics actor not found");
			return false;
		}
		return actor->IsKinematic();

	}

	static void RigidBodyComponent_SetIsKinematic(UUID entityID, bool value)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyComponentSetIsKinematic - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyComponentSetIsKinematic - physics actor not found");
			return;
		}
		actor->SetKinematic(value);

	}

	static bool RigidBodyComponent_IsSleeping(UUID entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyComponentIsSleeping - entity is not valid");
			return false;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyComponentIsSleeping - physics actor not found");
			return false;
		}
		return actor->IsSleeping();

	}

	static void RigidBodyComponent_SetIsSleeping(UUID entityID, bool isSleeping)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyComponentIsSleeping - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyComponentIsSleeping - physics actor not found");
			return;
		}
		if (isSleeping)
			actor->PutToSleep();
		else
			actor->WakeUp();

	}

	static void RigidBodyComponent_GetLocation(UUID entityID, glm::vec3* location)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyGetLocation - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyGetLocation - physics actor not found");
			return;
		}
		*location = actor->GetLocation();
	}
	static void RigidBodyComponent_SetLocation(UUID entityID, glm::vec3* location)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodySetLocation - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodySetLocation - physics actor not found");
			return;
		}
		actor->SetLocation(*location);
	}

	static void RigidBodyComponent_GetRotation(UUID entityID, glm::vec3* rotationRadians)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodyGetRotation - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodyGetRotation - physics actor not found");
			return;
		}
		*rotationRadians = actor->GetRotationEuler();
	}
	static void RigidBodyComponent_SetRotation(UUID entityID, glm::vec3* rotationRadians)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.RigidBodySetRotation - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.RigidBodySetRotation - physics actor not found");
			return;
		}
		actor->SetRotation(glm::quat(*rotationRadians));
	}

	static void RigidBodyComponent_GetLinearVelocity(UUID entityID, glm::vec3* force)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.GetLinearVelocity - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.GetLinearVelocity - physics actor not found");
			return;
		}
		*force = actor->GetLinearVelocity();
	}

	static void RigidBodyComponent_SetLinearVelocity(UUID entityID, glm::vec3* force)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.SetLinearVelocity - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.SetLinearVelocity - physics actor not found");
			return;
		}

		actor->SetLinearVelocity(*force);
	}
	static void RigidBodyComponent_SetAngularVelocity(UUID entityID, glm::vec3* force)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.SetAngularVelocity - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.SetAngularVelocity - physics actor not found");
			return;
		}

		actor->SetAngularVelocity(*force);
	}

	static void RigidBodyComponent_GetAngularVelocity(UUID entityID, glm::vec3* force)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.GetAngularVelocity - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.GetAngularVelocity - physics actor not found");
			return;
		}

		*force = actor->GetAngularVelocity();
	}

	static float RigidBodyComponent_GetMaxLinearVelocity(UUID entityID)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.GetMaxLinearVelocity - entity is not valid");
			return 0;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.GetMaxLinearVelocity - physics actor not found");
			return 0;
		}

		return actor->GetMaxLinearVelocity();
	}

	static void RigidBodyComponent_SetMaxLinearVelocity(UUID entityID, float* linearVelocity)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.SetMaxLinearVelocity - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.SetMaxLinearVelocity - physics actor not found");
			return;
		}

		actor->SetMaxLinearVelocity(*linearVelocity);
	}

	static float RigidBodyComponent_GetMaxAngularVelocity(UUID entityID)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.GetMaxAngularVelocity - entity is not valid");
			return 0;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.GetMaxAngularVelocity - physics actor not found");
			return 0;
		}

		return actor->GetMaxAngularVelocity();
	}

	static void RigidBodyComponent_SetMaxAngularVelocity(UUID entityID, float* angularVeloctiy)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.SetMaxAngularVelocity - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.SetMaxAngularVelocity - physics actor not found");
			return;
		}

		actor->SetMaxAngularVelocity(*angularVeloctiy);
	}

	static float RigidBodyComponent_GetLinearDrag(UUID entityID)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.GetLinearDrag - entity is not valid");
			return 0;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.GetLinearDrag - physics actor not found");
			return 0;
		}

		return actor->GetLinearDrag();
	}

	static void RigidBodyComponent_SetLinearDrag(UUID entityID, float* linearDrag)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.SetLinearDrag - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.SetLinearDrag - physics actor not found");
			return;
		}

		actor->SetLinearDrag(*linearDrag);
	}

	static float RigidBodyComponent_GetAngularDrag(UUID entityID)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.GetAngularDrag - entity is not valid");
			return 0;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.GetAngularDrag - physics actor not found");
			return 0;
		}

		return actor->GetAngularDrag();
	}

	static void RigidBodyComponent_SetAngularDrag(UUID entityID, float* angularDrag)
	{

		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBody.SetAngularDrag - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBody.SetAngularDrag - physics actor not found");
			return;
		}

		actor->SetAngularDrag(*angularDrag);
	}
	static uint32_t RigidBodyComponent_GetLayer(uint64_t entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.GetLayer - entity is not valid");
			return 0;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.GetLayer - No actor found!");
			return 0;
		}

		return entity.GetComponent<RigidBodyComponent>().PhysicsLayerID;
	}

	static void RigidBodyComponent_SetLayer(uint64_t entityID, uint32_t* layerID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.SetLayer - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.SetLayer - No actor found!");
			return;
		}

		if (!PhysicsLayerManager::IsLayerValid(*layerID))
		{
			PF_ERROR("RigidBodyComponent.SetLayer called with an invalid layer ID '{0}'!", *layerID);
			return;
		}

		if (actor->SetSimulationData(*layerID))
		{
			auto& component = entity.GetComponent<RigidBodyComponent>();
			component.PhysicsLayerID = *layerID;
		}
		else
		{
			PF_ERROR("Cannot change the layer of an actor that has shared collider shapes!");
		}
	}

	static void RigidBodyComponent_GetLayerName(uint64_t entityID, MonoString** tag)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.GetLayerName - entity is not valid");
			*tag = ScriptUtils::UTF8StringToMono("");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.GetLayerName - No actor found!");
			return;
		}

		const auto& component = entity.GetComponent<RigidBodyComponent>();

		if (!PhysicsLayerManager::IsLayerValid(component.PhysicsLayerID))
		{
			PF_ERROR("RigidBodyComponent.GetLayerName can't find a layer with ID '{0}'!", component.PhysicsLayerID);
			return;
		}

		const auto& layer = PhysicsLayerManager::GetLayer(component.PhysicsLayerID);
		*tag = ScriptUtils::UTF8StringToMono(layer.Name);
	}

	void RigidBodyComponent_SetLayerByName(uint64_t entityID, MonoString** name)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.SetLayerByName - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.SetLayerByName - No actor found!");
			return;
		}

		if (name == nullptr)
		{
			PF_ERROR("RigidBodyComponent.SetLayerByName called with 'null' name!");
			return;
		}

		std::string layerName = ScriptUtils::MonoStringToUTF8(*name);

		if (!PhysicsLayerManager::IsLayerValid(layerName))
		{
			PF_ERROR("RigidBodyComponent.SetLayerByName called with an invalid layer name '{0}'!", layerName);
			return;
		}

		const auto& layer = PhysicsLayerManager::GetLayer(layerName);

		if (actor->SetSimulationData(layer.LayerID))
		{
			auto& component = entity.GetComponent<RigidBodyComponent>();
			component.PhysicsLayerID = layer.LayerID;
		}
		else
		{
			PF_ERROR("Cannot change the layer of an actor that has shared collider shapes!");
		}
	}

	void RigidBodyComponent_GetKinematicTarget(uint64_t entityID, glm::vec3* outTargetPosition, glm::vec3* outTargetRotation)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.GetKinematicTarget - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.GetKinematicTarget - No actor found!");
			return;
		}

		*outTargetPosition = actor->GetKinematicTargetPosition();
		*outTargetRotation = glm::degrees(actor->GetKinematicTargetRotationEuler());
	}

	void RigidBodyComponent_SetKinematicTarget(uint64_t entityID, glm::vec3* inTargetPosition, glm::vec3* inTargetRotation)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.SetKinematicTarget - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.SetKinematicTarget - No actor found!");
			return;
		}

		if (inTargetPosition == nullptr || inTargetRotation == nullptr)
		{
			PF_ERROR("RigidBodyComponent.SetKinematicTarget - targetPosition or targetRotation is null!");
			return;
		}

		actor->SetKinematicTarget(*inTargetPosition, glm::radians(*inTargetRotation));
	}


	void RigidBodyComponent_AddForceAtLocation(uint64_t entityID, glm::vec3* inForce, glm::vec3* inLocation, ForceMode forceMode)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("RigidBodyComponent.AddForce - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("RigidBodyComponent.AddForce - No actor found!");
			return;
		}

		if (inForce == nullptr)
		{
			PF_ERROR("RigidBodyComponent.AddForce - Cannot add null force to a RigidBodyComponent!");
			return;
		}

		actor->AddForceAtLocation(*inForce, *inLocation, forceMode);
	}
#pragma endregion
#pragma region BoxColliderComponent
	void BoxColliderComponent_GetSize(uint64_t entityID, glm::vec3* outSize)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.GetSize - entity is not valid");
			return;
		}

		*outSize = entity.GetComponent<BoxColliderComponent>().Size;
	}

	void BoxColliderComponent_SetSize(uint64_t entityID, glm::vec3* inSize)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.SetHalfSize - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("BoxColliderComponent.SetHalfSize - Physics actor not found");
			return;
		}

		auto boxCollider = actor->GetCollider<BoxColliderShape>();
		if (!boxCollider)
		{
			PF_ERROR("BoxColliderComponent.SetHalfSize - No BoxColliderShape found?");
			return;
		}

		boxCollider->SetSize(*inSize);
		actor->WakeUp();
	}

	void BoxColliderComponent_GetCenter(uint64_t entityID, glm::vec3* outCenter)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.GetCenter - entity is not valid");
			return;
		}

		*outCenter = entity.GetComponent<BoxColliderComponent>().Center;
	}

	void BoxColliderComponent_SetCenter(uint64_t entityID, glm::vec3* inOffset)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.SetCenter - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("BoxColliderComponent.SetCenter - Physics actor not found");
			return;
		}

		auto boxCollider = actor->GetCollider<BoxColliderShape>();
		if (!boxCollider)
		{
			PF_ERROR("BoxColliderComponent.SetCenter - No BoxColliderShape found?");
			return;
		}

		boxCollider->SetCenter(*inOffset);
		actor->WakeUp();
	}

	bool BoxColliderComponent_IsTrigger(uint64_t entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.IsTrigger - entity is not valid");
			return false;
		}

		return entity.GetComponent<BoxColliderComponent>().IsTrigger;
	}

	void BoxColliderComponent_SetTrigger(uint64_t entityID, bool isTrigger)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.SetTrigger - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("BoxColliderComponent.SetTrigger - Physics actor not found");
			return;
		}

		auto boxCollider = actor->GetCollider<BoxColliderShape>();
		if (!boxCollider)
		{
			PF_ERROR("BoxColliderComponent.SetTrigger - No BoxColliderShape found?");
			return;
		}

		boxCollider->SetTrigger(isTrigger);
		actor->WakeUp();
	}

	bool BoxColliderComponent_GetPhysicsMaterialID(uint64_t entityID, AssetID* outID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("BoxColliderComponent.GetPhysicsMaterialID - entity is not valid");
			*outID = AssetID(0);
			return false;
		}
		if (entity.GetComponent<BoxColliderComponent>().HasPhysicsMaterial())
			*outID = entity.GetComponent<BoxColliderComponent>().GetPhysicsMaterial().As<Asset>()->GetID();
		else
			*outID = 0;
		return true;
	}
#pragma endregion

#pragma region SphereColliderComponent

	float SphereColliderComponent_GetRadius(uint64_t entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.GetRadius - entity is not valid");
			return 0.0f;
		}

		return entity.GetComponent<SphereColliderComponent>().Radius;
	}

	void SphereColliderComponent_SetRadius(uint64_t entityID, float radius)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.SetRadius - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("SphereColliderComponent.SetRadius - No PhysicsActor found!");
			return;
		}

		auto sphereCollider = actor->GetCollider<SphereColliderShape>();
		if (!sphereCollider)
		{
			PF_ERROR("SphereColliderComponent.SetRadius - No SphereColliderShape found?");
			return;
		}

		sphereCollider->SetRadius(radius);
		actor->WakeUp();
	}

	void SphereColliderComponent_GetCenter(uint64_t entityID, glm::vec3* outCenter)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.GetCenter - entity is not valid");
			return;
		}

		*outCenter = entity.GetComponent<SphereColliderComponent>().Center;
	}

	void SphereColliderComponent_SetCenter(uint64_t entityID, glm::vec3* inCenter)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.SetOffset - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("SphereColliderComponent.SetCenter - No PhysicsActor found!");
			return;
		}

		auto sphereCollider = actor->GetCollider<SphereColliderShape>();
		if (!sphereCollider)
		{
			PF_ERROR("SphereColliderComponent.SetCenter - No SphereColliderShape found?");
			return;
		}

		sphereCollider->SetCenter(*inCenter);
		actor->WakeUp();
	}

	bool SphereColliderComponent_IsTrigger(uint64_t entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.IsTrigger - entity is not valid");
			return false;
		}

		return entity.GetComponent<BoxColliderComponent>().IsTrigger;
	}

	void SphereColliderComponent_SetTrigger(uint64_t entityID, bool isTrigger)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.SetTrigger - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("SphereColliderComponent.SetTrigger - No PhysicsActor found!");
			return;
		}

		auto sphereCollider = actor->GetCollider<SphereColliderShape>();
		if (!sphereCollider)
		{
			PF_ERROR("SphereColliderComponent.SetTrigger - No SphereColliderShape found?");
			return;
		}

		sphereCollider->SetTrigger(isTrigger);
		actor->WakeUp();
	}
	bool SphereColliderComponent_GetPhysicsMaterialID(uint64_t entityID, AssetID* outID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("SphereColliderComponent.GetPhysicsMaterialID - entity is not valid");
			*outID = AssetID(0);
			return false;
		}
		if (entity.GetComponent<SphereColliderComponent>().HasPhysicsMaterial())
			*outID = entity.GetComponent<SphereColliderComponent>().GetPhysicsMaterial().As<Asset>()->GetID();
		else
			*outID = 0;
		return true;
	}

#pragma endregion

#pragma region MeshColliderComponent

	bool MeshColliderComponent_IsMeshStatic(uint64_t entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("MeshColliderComponent.IsMeshStatic - entity is not valid");
			return false;
		}

		const auto& component = entity.GetComponent<MeshColliderComponent>();

		if (!AssetManager::HasAsset(component.ColliderID))
		{
			PF_ERROR("MeshColliderComponent.IsMeshStatic - Invalid collider asset!");
			return false;
		}
		Count<MeshCollider> collider = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		if (!AssetManager::HasAsset(collider->ColliderMesh))
		{
			PF_ERROR("MeshColliderComponent.IsMeshStatic - Invalid mesh!");
			return false;
		}
		AssetInfo colliderMetadata = AssetManager::GetAssetInfo(collider->ColliderMesh);
		return colliderMetadata.Type == AssetType::Mesh;
	}

	bool MeshColliderComponent_IsColliderMeshValid(uint64_t entityID, AssetID* meshHandle)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("MeshColliderComponent.IsColliderMeshValid - entity is not valid");
			return false;
		}

		const auto& component = entity.GetComponent<MeshColliderComponent>();
		if (!AssetManager::HasAsset(component.ColliderID))
		{
			PF_ERROR("MeshColliderComponent.IsColliderMeshValid - Invalid collider asset!");
			return false;
		}

		Count<MeshCollider> collider = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		return *meshHandle == collider->ColliderMesh;
	}

	bool MeshColliderComponent_GetColliderMesh(uint64_t entityID, AssetID* outHandle)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("MeshColliderComponent.GetColliderMesh - entity is not valid");
			*outHandle = AssetID(0);
			return false;
		}
		const auto& component = entity.GetComponent<MeshColliderComponent>();

		if (!AssetManager::HasAsset(component.ColliderID))
		{
			PF_ERROR("MeshColliderComponent.GetColliderMesh - Invalid collider asset!");
			return false;
		}
		Count<MeshCollider> collider = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		if (!AssetManager::HasAsset(collider->ColliderMesh))
		{
			PF_ERROR("MeshColliderComponent.GetColliderMesh - This component doesn't have a valid collider mesh!");
			*outHandle = AssetID(0);
			return false;
		}
		*outHandle = component.ColliderID;
		return true;
	}

	void MeshColliderComponent_SetColliderMesh(uint64_t entityID, AssetID* meshHandle)
	{

	}

	bool MeshColliderComponent_IsTrigger(uint64_t entityID)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("MeshColliderComponent.IsTrigger - entity is not valid");
			return false;
		}

		const auto& component = entity.GetComponent<MeshColliderComponent>();
		return component.IsTrigger;
	}

	void MeshColliderComponent_SetTrigger(uint64_t entityID, bool isTrigger)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("MeshColliderComponent.SetTrigger - entity is not valid");
			return;
		}

		auto actor = GetPhysicsActor(entity);
		if (!actor)
		{
			PF_ERROR("MeshColliderComponent.SetTrigger - No PhysicsActor found!");
			return;
		}

		const auto& component = entity.GetComponent<MeshColliderComponent>();
		if (!AssetManager::HasAsset(component.ColliderID))
		{
			PF_ERROR("MeshColliderComponent.SetTrigger - Invalid collider asset!");
			return;
		}

		Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		if (!AssetManager::HasAsset(colliderAsset->ColliderMesh))
		{
			PF_ERROR("MeshColliderComponent.SetTrigger - This component doesn't have a valid collider mesh!");
			return;
		}
		
		auto convexMeshCollider = actor->GetCollider<ConvexMeshShape>();
		if (!convexMeshCollider)
		{
			PF_ERROR("MeshColliderComponent.SetTrigger - Failed to find Simple Shape! Only simple shapes can be trigger shapes.");
			return;
		}

		convexMeshCollider->SetTrigger(isTrigger);
	}

	bool MeshColliderComponent_GetPhysicsMaterialID(uint64_t entityID, AssetID* outHandle)
	{
		auto entity = GetEntity(entityID);
		if (!entity)
		{
			PF_ERROR("MeshColliderComponent.GetPhysicsMaterialID - entity is not valid");
			*outHandle = AssetID(0);
			return false;
		}

		const auto& component = entity.GetComponent<MeshColliderComponent>();

		if (!AssetManager::HasAsset(component.ColliderID))
		{
			PF_ERROR("MeshColliderComponent.GetPhysicsMaterialID - Invalid collider asset!");
			*outHandle = AssetID(0);
			return false;
		}

		Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		*outHandle = colliderAsset->PhysicsMaterial;
		if (component.HasPhysicsMaterial())
			*outHandle = component.GetPhysicsMaterial()->GetID();

		return true;
	}

#pragma endregion
#pragma region MeshBase

	bool MeshBase_GetMaterialByIndex(AssetID* meshHandle, uint32_t index, AssetID* outHandle)
	{
		if (!AssetManager::HasAsset(*meshHandle))
		{
			PF_ERROR("MeshBase.GetMaterialByIndex called on an invalid Mesh instance!");
			*outHandle = AssetID(0);
			return false;
		}

		Count<MeshBase> mesh = AssetManager::GetAsset<MeshBase>(*meshHandle);

		Count<MaterialTable> materialTable = mesh->GetMaterialTable();
		if (materialTable == nullptr)
		{
			PF_ERROR("Mesh.GetMaterialByIndex - Mesh has no materials!");
			*outHandle = AssetID(0);
			return false;
		}

		if (materialTable->GetMaterialCount() == 0)
		{
			*outHandle = AssetID(0);
			return false;
		}

		if ((uint32_t)index >= materialTable->GetMaterialCount())
		{
			PF_ERROR("Mesh.GetMaterialByIndex - Material index out of range. Index: {0}, MaxIndex: {1}", index, materialTable->GetMaterialCount() - 1);
			*outHandle = AssetID(0);
			return false;
		}

		*outHandle = materialTable->GetMaterial(index).As<Asset>()->GetID();
		return true;
	}

	uint32_t MeshBase_GetMaterialCount(AssetID* meshHandle)
	{
		if (!AssetManager::HasAsset(*meshHandle))
		{
			PF_ERROR("MeshBase.GetMaterialCount called on an invalid Mesh instance!");
			return 0;
		}

		Count<Mesh> mesh = AssetManager::GetAsset<Mesh>(*meshHandle);

		Count<MaterialTable> materialTable = mesh->GetMaterialTable();
		if (materialTable == nullptr)
			return 0;

		return materialTable->GetMaterialCount();
	}

	bool MeshBase_IsStaticMesh(AssetID* meshHandle)
	{
		if (!AssetManager::HasAsset(*meshHandle))
		{
			PF_ERROR("MeshBase.IsStaticMesh called on an invalid Mesh instance!");
			return false;
		}

		auto assetInfo = AssetManager::GetAssetInfo(*meshHandle);
		return assetInfo.Type == AssetType::Mesh;
	}

#pragma endregion
#pragma region Physics
	struct ScriptRaycastHit
	{
		uint64_t HitEntity = 0;
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f);
		float Distance = 0.0f;
		MonoObject* HitCollider = nullptr;
	};
	struct ScriptRaycastData
	{
		glm::vec3 Origin;
		glm::vec3 Direction;
		float MaxDistance;
		MonoArray* RequiredComponentTypes;
		MonoArray* ExcludeEntities;
	};

	struct ScriptSphereCastData
	{
		glm::vec3 Origin;
		glm::vec3 Direction;
		float Radius;
		float MaxDistance;
		MonoArray* RequiredComponentTypes;
		MonoArray* ExcludeEntities;
	};
	bool Physics_Raycast(ScriptRaycastData* inRaycastData, ScriptRaycastHit* outHit)
	{
		Count<World> scene = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(scene, "Physics.Raycast No active World!");

		auto physicsWorld = scene->GetPhysicsWorld();
		if (!physicsWorld)
		{
			PF_CORE_ASSERT("Physics.Raycast can only be called in Play mode!");
			return false;
		}

		PF_CORE_ASSERT(physicsWorld);

		RaycastHit tempHit;
		bool success = false;

		if (inRaycastData->ExcludeEntities)
		{
			PF_CORE_ASSERT(false,"Does not support exlude entities yet");
			/*
			size_t excludeEntitiesCount = mono_array_length(inRaycastData->ExcludeEntities);
			std::unordered_set<UUID> entityIDs(excludeEntitiesCount);
			for (size_t i = 0; i < excludeEntitiesCount; i++)
			{
				uint64_t entityID = mono_array_get(inRaycastData->ExcludeEntities, uint64_t, i);
				entityIDs.insert(entityID);
			}
			success = scene->GetPhysicsScene()->RaycastExcludeEntities(inRaycastData->Origin, inRaycastData->Direction, inRaycastData->MaxDistance, &tempHit, entityIDs);
			*/
		}
		else
		{
			success = physicsWorld->Raycast(inRaycastData->Origin, inRaycastData->Direction, inRaycastData->MaxDistance, &tempHit);
			
		}

		if (success && inRaycastData->RequiredComponentTypes != nullptr)
		{
			Entity entity = scene->TryGetEntityWithUUID(tempHit.HitEntity);
			size_t requiredComponentsCount = mono_array_length(inRaycastData->RequiredComponentTypes);

			for (size_t i = 0; i < requiredComponentsCount; i++)
			{
				void* reflectionType = mono_array_get(inRaycastData->RequiredComponentTypes, void*, i);
				if (reflectionType == nullptr)
				{
					PF_ERROR("Physics.Raycast - Why did you feel the need to pass a \"null\" as a required component?");
					success = false;
					break;
				}

				MonoType* componentType = mono_reflection_type_get_type((MonoReflectionType*)reflectionType);

#ifdef PF_DEBUG
				MonoClass* typeClass = mono_type_get_class(componentType);
				MonoClass* parentClass = mono_class_get_parent(typeClass);

				bool validComponentFilter = parentClass != nullptr;
				if (validComponentFilter)
				{
					const char* parentClassName = mono_class_get_name(parentClass);
					const char* parentNameSpace = mono_class_get_namespace(parentClass);
					validComponentFilter = strstr(parentClassName, "Component") != nullptr && strstr(parentNameSpace, "PF") != nullptr;
				}

				if (!validComponentFilter)
				{
					PF_CONSOLE_LOG_ERROR("Physics.Raycast - {0} does not inherit from PF.Component!", mono_class_get_name(typeClass));
					success = false;
					break;
				}
#endif

				if (!s_EntityHasComponentFuncs[componentType](entity))
				{
					success = false;
					break;
				}
			}
		}

		if (success)
		{
			outHit->HitEntity = tempHit.HitEntity;
			outHit->Position = tempHit.Position;
			outHit->Normal = tempHit.Normal;
			outHit->Distance = tempHit.Distance;

			if (tempHit.HitCollider)
			{
				switch (tempHit.HitCollider->GetType())
				{
					case ColliderType::Box:
					{
						Count<BoxColliderShape> shape = tempHit.HitCollider.As<BoxColliderShape>();
						outHit->HitCollider = ScriptEngine::CreateManagedObject("Proof.BoxCollider", outHit->HitEntity, shape->IsTrigger(), shape->GetSize(), shape->GetCenter());
						break;
					}
					case ColliderType::Sphere:
					{
						Count<SphereColliderShape> shape = tempHit.HitCollider.As<SphereColliderShape>();
						outHit->HitCollider = ScriptEngine::CreateManagedObject("Proof.SphereCollider", outHit->HitEntity, shape->IsTrigger(), shape->GetRadius());
						break;
					}
					//case ColliderType::Capsule:
					//{
					//	Count<CapsuleColliderShape> shape = tempHit.HitCollider.As<CapsuleColliderShape>();
					//	outHit->HitCollider = ScriptEngine::CreateManagedObject("PF.CapsuleCollider", outHit->HitEntity, shape->IsTrigger(), shape->GetRadius(), shape->GetHeight());
					//	break;
					//}
					case ColliderType::ConvexMesh:
					{
						Count<ConvexMeshShape> shape = tempHit.HitCollider.As<ConvexMeshShape>();
						Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(shape->GetColliderHandle());
						outHit->HitCollider = ScriptEngine::CreateManagedObject("Proof.MeshCollider", outHit->HitEntity, shape->IsTrigger(), colliderAsset->ColliderMesh);
						break;
					}
					case ColliderType::TriangleMesh:
					{
						Count<TriangleMeshShape> shape = tempHit.HitCollider.As<TriangleMeshShape>();
						Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(shape->GetColliderHandle());
						outHit->HitCollider = ScriptEngine::CreateManagedObject("Proof.MeshCollider", outHit->HitEntity, shape->IsTrigger(), colliderAsset->ColliderMesh);
						break;
					}
					default:
						PF_CORE_ASSERT(false);
				}
			}
		}
		else
		{
			*outHit = ScriptRaycastHit();
		}

		return success;
	}

	bool Physics_SphereCast(ScriptSphereCastData* inSphereCastData, ScriptRaycastHit* outHit)
	{
		Count<World> scene = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(scene, "Physics.SphereCast No active World!");

		auto physicsWorld = scene->GetPhysicsWorld();
		if (!physicsWorld)
		{
			PF_CORE_ASSERT("Physics.SphereCast can only be called in Play mode!");
			return false;
		}

		PF_CORE_ASSERT(physicsWorld);

		RaycastHit tempHit;
		bool success = false;

		if (inSphereCastData->ExcludeEntities)
		{
			PF_CORE_ASSERT(false, "Not support exlude entitites")
			size_t excludeEntitiesCount = mono_array_length(inSphereCastData->ExcludeEntities);
			std::unordered_set<UUID> entityIDs(excludeEntitiesCount);
			for (size_t i = 0; i < excludeEntitiesCount; i++)
			{
				uint64_t entityID = mono_array_get(inSphereCastData->ExcludeEntities, uint64_t, i);
				entityIDs.insert(entityID);
			}
			//PF_CORE_VERIFY(false);
			//success = scene->GetPhysicsScene()->RaycastExcludeEntities(inSphereCastData->Origin, inSphereCastData->Direction, inSphereCastData->MaxDistance, &tempHit, entityIDs);
		}
		else
		{
			success = scene->GetPhysicsWorld()->SphereCast(inSphereCastData->Origin, inSphereCastData->Direction, inSphereCastData->Radius, inSphereCastData->MaxDistance, &tempHit);
		}

		if (success && inSphereCastData->RequiredComponentTypes != nullptr)
		{
			Entity entity = scene->TryGetEntityWithUUID(tempHit.HitEntity);
			size_t requiredComponentsCount = mono_array_length(inSphereCastData->RequiredComponentTypes);

			for (size_t i = 0; i < requiredComponentsCount; i++)
			{
				void* reflectionType = mono_array_get(inSphereCastData->RequiredComponentTypes, void*, i);
				if (reflectionType == nullptr)
				{
					PF_ERROR("Physics.SphereRaycast - Why did you feel the need to pass a \"null\" as a required component?");
					success = false;
					break;
				}

				MonoType* componentType = mono_reflection_type_get_type((MonoReflectionType*)reflectionType);

#ifdef PF_DEBUG
				MonoClass* typeClass = mono_type_get_class(componentType);
				MonoClass* parentClass = mono_class_get_parent(typeClass);

				bool validComponentFilter = parentClass != nullptr;
				if (validComponentFilter)
				{
					const char* parentClassName = mono_class_get_name(parentClass);
					const char* parentNameSpace = mono_class_get_namespace(parentClass);
					validComponentFilter = strstr(parentClassName, "Component") != nullptr && strstr(parentNameSpace, "PF") != nullptr;
				}

				if (!validComponentFilter)
				{
					PF_CONSOLE_LOG_ERROR("Physics.Raycast - {0} does not inherit from PF.Component!", mono_class_get_name(typeClass));
					success = false;
					break;
				}
#endif

				if (!s_EntityHasComponentFuncs[componentType](entity))
				{
					success = false;
					break;
				}
			}
		}

		if (success)
		{
			outHit->HitEntity = tempHit.HitEntity;
			outHit->Position = tempHit.Position;
			outHit->Normal = tempHit.Normal;
			outHit->Distance = tempHit.Distance;

			if (tempHit.HitCollider)
			{
				switch (tempHit.HitCollider->GetType())
				{
					case ColliderType::Box:
					{
						Count<BoxColliderShape> shape = tempHit.HitCollider.As<BoxColliderShape>();
						outHit->HitCollider = ScriptEngine::CreateManagedObject("PF.BoxCollider", outHit->HitEntity, shape->IsTrigger(), shape->GetSize(), shape->GetCenter());
						break;
					}
					case ColliderType::Sphere:
					{
						Count<SphereColliderShape> shape = tempHit.HitCollider.As<SphereColliderShape>();
						outHit->HitCollider = ScriptEngine::CreateManagedObject("PF.SphereCollider", outHit->HitEntity, shape->IsTrigger(), shape->GetRadius());
						break;
					}
					//case ColliderType::Capsule:
					//{
					//	Ref<CapsuleColliderShape> shape = tempHit.HitCollider.As<CapsuleColliderShape>();
					//	outHit->HitCollider = ScriptEngine::CreateManagedObject("PF.CapsuleCollider", outHit->HitEntity, shape->IsTrigger(), shape->GetRadius(), shape->GetHeight());
					//	break;
					//}
					case ColliderType::ConvexMesh:
					{
						Count<ConvexMeshShape> shape = tempHit.HitCollider.As<ConvexMeshShape>();
						Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(shape->GetColliderHandle());
						outHit->HitCollider = ScriptEngine::CreateManagedObject("PF.MeshCollider", outHit->HitEntity, shape->IsTrigger(), colliderAsset->ColliderMesh);
						break;
					}
					case ColliderType::TriangleMesh:
					{
						Count<TriangleMeshShape> shape = tempHit.HitCollider.As<TriangleMeshShape>();
						Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(shape->GetColliderHandle());
						outHit->HitCollider = ScriptEngine::CreateManagedObject("PF.MeshCollider", outHit->HitEntity, shape->IsTrigger(), colliderAsset->ColliderMesh);
						break;
					}
					default:
						PF_CORE_ASSERT(false);
				}
			}
		}
		else
		{
			*outHit = ScriptRaycastHit();
		}

		return success;
	}

	void Physics_AddRadialImpulse(glm::vec3* inOrigin, float radius, float strength, EFalloffMode falloff, bool velocityChange)
	{
		Count<World> scene = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(scene, "Physics.AddRadialImpulse No active World!");
		scene->GetPhysicsWorld()->AddRadialImpulse(*inOrigin, radius, strength, falloff, velocityChange);
	}
#pragma region PhysicsMaterial

	float PhysicsMaterial_GetStaticFriction(AssetID* handle)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.GetStaticFriction called on an invalid PhysicsMaterial instance!");
			return std::numeric_limits<float>::max();
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.GetStaticFriction called with an AssetID that doesn't represent a PhysicsMaterial!");
			return std::numeric_limits<float>::max();
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.GetStaticFriction called on an invalid PhysicsMaterial!");
			return std::numeric_limits<float>::max();
		}

		return material->GetStaticFriction();
	}

	// TODO(Peter): Make these set functions actually take effect...

	void PhysicsMaterial_SetStaticFriction(AssetID* handle, float value)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.SetStaticFriction called on an invalid PhysicsMaterial instance!");
			return;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.SetStaticFriction called with an AssetID that doesn't represent a PhysicsMaterial!");
			return;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.SetStaticFriction called on an invalid PhysicsMaterial!");
			return;
		}

		material->SetStaticFriction(value);
	}

	float PhysicsMaterial_GetDynamicFriction(AssetID* handle)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.GetDynamicFriction called on an invalid PhysicsMaterial instance!");
			return std::numeric_limits<float>::max();
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.GetDynamicFriction called with an AssetID that doesn't represent a PhysicsMaterial!");
			return std::numeric_limits<float>::max();
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.GetDynamicFriction called on an invalid PhysicsMaterial!");
			return std::numeric_limits<float>::max();
		}

		return material->GetDynamicFriction();
	}

	void PhysicsMaterial_SetDynamicFriction(AssetID* handle, float value)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.SetDynamicFriction called on an invalid PhysicsMaterial instance!");
			return;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.SetDynamicFriction called with an AssetID that doesn't represent a PhysicsMaterial!");
			return;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.SetDynamicFriction called on an invalid PhysicsMaterial!");
			return;
		}

		material->SetDynamicFriction(value);
	}

	float PhysicsMaterial_GetBounciness(AssetID* handle)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.GetBounciness called on an invalid PhysicsMaterial instance!");
			return std::numeric_limits<float>::max();
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.GetBounciness called with an AssetID that doesn't represent a PhysicsMaterial!");
			return std::numeric_limits<float>::max();
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.GetBounciness called on an invalid PhysicsMaterial!");
			return std::numeric_limits<float>::max();
		}

		return material->GetBounciness();
	}

	void PhysicsMaterial_SetBounciness(AssetID* handle, float value)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.SetBounciness called on an invalid PhysicsMaterial instance!");
			return;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.SetBounciness called with an AssetID that doesn't represent a PhysicsMaterial!");
			return;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.SetBounciness called on an invalid PhysicsMaterial!");
			return;
		}

		material->SetBounciness(value);
	}

	CombineMode PhysicsMaterial_GetFrictionCombineMode(AssetID* handle)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.GetFrictionCombineMode called on an invalid PhysicsMaterial instance!");
			return (CombineMode) - 1;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.GetFrictionCombineMode called with an AssetID that doesn't represent a PhysicsMaterial!");
			return (CombineMode) - 1;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.GetFrictionCombineMode called on an invalid PhysicsMaterial!");
			return (CombineMode) - 1;
		}

		return material->GetFrictionCombineMode();
	}

	void PhysicsMaterial_SetFrictionCombineMode(AssetID* handle, CombineMode value)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.SetFrictionCombineMode called on an invalid PhysicsMaterial instance!");
			return;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.SetFrictionCombineMode with an AssetID that doesn't represent a PhysicsMaterial!");
			return;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.SetFrictionCombineMode called on an invalid PhysicsMaterial!");
			return;
		}

		material->SetFrictionCombineMode(value);
	}

	CombineMode PhysicsMaterial_GetBouncinessCombineMode(AssetID* handle)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.GetBouncinessCombineMode called on an invalid PhysicsMaterial instance!");
			return (CombineMode)-1;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.GetBouncinessCombineMode called with an AssetID that doesn't represent a PhysicsMaterial!");
			return (CombineMode)-1;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.GetBouncinessCombineMode called on an invalid PhysicsMaterial!");
			return (CombineMode)-1;
		}

		return material->GetBouncinessCombineMode();
	}

	void PhysicsMaterial_SetBouncinessCombineMode(AssetID* handle, CombineMode value)
	{
		if (!AssetManager::HasAsset(*handle))
		{
			PF_ERROR("PhysicsMaterial.SetBouncinessCombineMode called on an invalid PhysicsMaterial instance!");
			return;
		}

		const auto& metadata = AssetManager::GetAssetInfo(*handle);

		if (metadata.Type != AssetType::PhysicsMaterial)
		{
			PF_ERROR("PhysicsMaterial.SetBouncinessCombineMode with an AssetID that doesn't represent a PhysicsMaterial!");
			return;
		}

		Count<PhysicsMaterial> material = AssetManager::GetAsset<PhysicsMaterial>(*handle);
		if (!material)
		{
			PF_ERROR("PhysicsMaterial.SetBouncinessCombineMode called on an invalid PhysicsMaterial!");
			return;
		}

		material->SetBouncinessCombineMode(value);
	}
#pragma endregion

#pragma endregion

#pragma region Random
	static bool Random_RandomBool() {
		return Random::Bool();
	}

	static int32_t Random_RandomInt32(int32_t min, int32_t max){
		return Random::Int<int32_t>(min, max);
	}

	static float Random_RandomFloat(float min, float max)
	{
		return Random::Real<float>(min, max);
	}

	static double Random_RandomDouble(double min, double max)
	{
		return Random::Real<double>(min, max);
	}
	#pragma endregion

	#pragma region ChildComponent 

	static void ChildComponent_AddChild(uint64_t entityID, uint64_t childId) 
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		Entity childEntity = ScriptEngine::GetWorldContext()->GetEntity(childId);
		#if PF_ENABLE_DEBUG
		if (!entity || childEntity)
		{
			PF_ERROR("ChildComponent.AddChild - entity is invalid ");
			return;
		}
		#endif
		entity.AddChild(childEntity);
	}

	static void ChildComponent_RemoveChild(uint64_t entityID, uint64_t childId)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		Entity childEntity = ScriptEngine::GetWorldContext()->GetEntity(childId); 
		#if PF_ENABLE_DEBUG
		if (!entity || childEntity)
		{
			PF_ERROR("ChildComponent.RemoveChild - entity is invalid ");
			return;
		}
		#endif
		entity.RemoveChild(childEntity);
	}
	#pragma endregion 

	#pragma region MeshComponent
	static bool MeshComponent_GetVisible(uint64_t entityID)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("MeshComponent.GetVisible - entity is invalid ");
			return false;
		}
		#endif

		if (entity.HasComponent<MeshComponent>())
		{
			return entity.GetComponent<MeshComponent>().Visible;
		}
		PF_ERROR("MeshComponent.GetVisible entity tag: {} ID: {}  does not conatin mesh Compoonent", entity.GetName(), entity.GetUUID());
	}
	static void MeshComponent_SetVisible(uint64_t entityID,bool visible)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("MeshComponent.GetVisible - entity is invalid ");
			return;
		}
		#endif

		if (entity.HasComponent<MeshComponent>())
		{
			entity.GetComponent<MeshComponent>().Visible = visible;
			return;
		}
		PF_ERROR("MeshComponent.SetVisible entity tag: {} ID: {}  does not conatin mesh Compoonent", entity.GetName(), entity.GetUUID());
	}
	#pragma endregion
#pragma region MeshCollider

	bool MeshCollider_IsStaticMesh(AssetID* meshHandle)
	{
		if (!AssetManager::HasAsset(*meshHandle))
			return false;

		const auto& metadata = AssetManager::GetAssetInfo(*meshHandle);

		if (metadata.Type != AssetType::Mesh && metadata.Type != AssetType::DynamicMesh)
		{
			PF_ERROR("MeshCollider recieved AssetID to a non-mesh asset?");
			return false;
		}

		return metadata.Type == AssetType::Mesh;
	}

#pragma endregion
	#pragma region PlayerInputComponent
	static void PlayerInputComponent_SetAction(uint64_t entityID, MonoString* className,MonoString* ActionName, uint32_t inputState, MonoString* meathodName)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerInputComponent.SetAction - entity is invalid or Does not have rigidBody");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerInputComponent>())
			return;
		#if 0
		PlayerInputComponent& playerInput = entity.GetComponent <PlayerInputComponent>();
		
		auto entityScripts = ScriptEngine::GetScriptInstnace(entity);

		std::string classAsString = ScriptUtils::MonoStringToUTF8(className);
		if(!entityScripts.contains(classAsString))return;

		Count<ScriptInstance> script = entityScripts.at(classAsString);

		std::string meathodNameStr = ScriptUtils::MonoStringToUTF8(meathodName);
		MonoMethod* meathod = mono_class_get_method_from_name(script->GetScriptClass()->GetMonoClass(), meathodNameStr.c_str(), 0);
		auto call = [script = script, meathod = meathod]() {
			ScriptMeathod::CallMeathod(script, meathod, nullptr);
		};
		InputManagerMeathods::BindAction(ScriptUtils::MonoStringToUTF8(ActionName), (uint32_t)playerInput.InputPlayer, (InputEvent) inputState,call);

		#endif
	}

	static void PlayerInputComponent_SetMotion(uint64_t entityID, MonoString* className, MonoString* motionName, MonoString* meathodName)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerInputComponent.SetMotion - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerInputComponent>())
			return;

		PlayerInputComponent& playerInput = entity.GetComponent <PlayerInputComponent>();
		#if 0
		auto entityScripts = ScriptEngine::GetScriptInstnace(entity);

		std::string classAsString = ScriptUtils::MonoStringToUTF8(className);
		if (!entityScripts.contains(classAsString))return;

		Count<ScriptInstance> script = entityScripts.at(classAsString);
		std::string meathodNameStr = ScriptUtils::MonoStringToUTF8(meathodName);
		MonoMethod* meathod = mono_class_get_method_from_name(script->GetScriptClass()->GetMonoClass(), meathodNameStr.c_str(), 1);
		auto call = [script = script, meathod = meathod](float motionValue) {
			void* param = &motionValue;
			ScriptMeathod::CallMeathod(script, meathod, &param);
		};
		InputManagerMeathods::BindMotion(ScriptUtils::MonoStringToUTF8(motionName), (uint32_t)playerInput.InputPlayer, call);
		#endif
	}
	
	static void PlayerInputComponent_SetInputState(uint64_t entityID,int inputState)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerInputComponent.SetInputState - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerInputComponent>())
			return;

		PlayerInputComponent& playerInput = entity.GetComponent <PlayerInputComponent>();
		InputManagerMeathods::SetPlayerInput((uint32_t)playerInput.InputPlayer, (PlayerInputState)inputState);
	}

	#pragma endregion 

	#pragma region ParticleSystemComponent
	static bool ParticleSystemComponent_HasParticleIndex(uint64_t entityID, uint32_t tableIndex)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.HasParticleIndex - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.HasParticleIndex entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return false;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable!= nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex))
		{
			return true;
		}
		return false;
	}
	static bool ParticleSystemComponent_ParticleIndexHasParticle(uint64_t entityID, uint32_t tableIndex)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.ParticleIndexHasParticle - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.ParticleIndexHasParticle entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return false;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex)!=nullptr)
		{
			return true;
		}
		return false;
	}
	static bool ParticleSystemComponent_GetVisible(uint64_t entityID, uint32_t tableIndex) 
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.GetVisible - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.GetVisible entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return {};
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			return comp.ParticleHandlerTable->GetHandler(tableIndex)->Visible;
		}
		PF_ERROR("PlayerHUDComponent.GetVisible entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
		return false;
	}
	static void ParticleSystemComponent_SetVisible(uint64_t entityID, uint32_t tableIndex, bool* visible)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.SetVisible - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.SetVisible entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			comp.ParticleHandlerTable->GetHandler(tableIndex)->Visible = *visible;
			return;
		}
		PF_ERROR("ParticleSystemComponent.SetVisible entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}

	static void ParticleSystemComponent_Play(uint64_t entityID, uint32_t tableIndex)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.Play - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.Play() entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			comp.ParticleHandlerTable->GetHandler(tableIndex)->Play();
			return;
		}
		PF_ERROR("ParticleSystemComponent.Play() entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}
	static void ParticleSystemComponent_Pause(uint64_t entityID, uint32_t tableIndex)
	{

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.Pause() - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.Pause() entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			comp.ParticleHandlerTable->GetHandler(tableIndex)->Pause();
			return;
		}
		PF_ERROR("ParticleSystemComponent.Pause() entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}
	static void ParticleSystemComponent_End(uint64_t entityID, uint32_t tableIndex)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.End() - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.End() entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			comp.ParticleHandlerTable->GetHandler(tableIndex)->End();
			return;
		}
		PF_ERROR("ParticleSystemComponent.End() entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}
	static void ParticleSystemComponent_Restart(uint64_t entityID, uint32_t tableIndex)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.Restart() - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.Restart() entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			comp.ParticleHandlerTable->GetHandler(tableIndex)->Restart();
			return;
		}
		PF_ERROR("ParticleSystemComponent.Restart() entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}
	static int ParticleSystemComponent_GetState(uint64_t entityID, uint32_t tableIndex)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.GetState - entity is invalid ");
			return 0 ;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.GetState entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return 0;
		}
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();
		if (comp.ParticleHandlerTable != nullptr && comp.ParticleHandlerTable->HasHandler(tableIndex) && comp.ParticleHandlerTable->GetHandler(tableIndex) != nullptr)
		{
			return (int)comp.ParticleHandlerTable->GetHandler(tableIndex)->GetState();
		}
		PF_ERROR("ParticleSystemComponent.GetState entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}

	static void ParticleSystemComponent_GetParticles(uint64_t entityID, MonoArray** theArray)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("ParticleSystemComponent.GetParticles - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<ParticleSystemComponent>())
		{
			PF_ERROR("ParticleSystemComponent.GetParticles entity tag: {} ID: {}  does not conatin ParticleSystem Component", entity.GetName(), entity.GetUUID());
			return;
		}
		std::vector<uint32_t> objects;
		ParticleSystemComponent& comp = entity.GetComponent<ParticleSystemComponent>();

		for (auto& part : comp.ParticleHandlerTable->GetHandlers())
		{
			objects.emplace_back(part.first);
		}
		#if 0
		if (objects.size() == 0)
			return;
		*theArray = mono_array_new(ScriptEngine::GetDomain(), mono_get_uint32_class(), objects.size());
		memcpy(mono_array_addr(*theArray, uint64_t, 0), objects.data(), objects.size() * sizeof(uint64_t));
		#endif
	}
	#pragma endregion

	#pragma region PlayerHUDComponent
	
	static uint64_t PlayerHUDComponent_GetHUDAssetID(uint64_t entityID, uint32_t index) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.GetHUDAssetID - entity is invalid ");
			return 0;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetHUDAssetID entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return 0;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(index)&& comp.HudTable->GetPanel(index) != nullptr)
		{
			return comp.HudTable->GetPanel(index)->GetID();
		}
		else
		{
			return 0;
		}
	}
	struct UIBaseData {
		glm::vec2 Position;
		glm::vec2 Rotation;
		glm::vec2 Size;
		bool Visible = true;
		glm::vec4 Color;
	};

	struct UIImageButtonData {
		UIBaseData Base;
		uint64_t AssetID;
	};

	struct UITextData {
		UIBaseData Base;
		float Kerning;
		float LineSpacing;
	};
	static bool PlayerHUDComponent_IndexHasHUD(uint64_t entityID, uint32_t tableIndex) 
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.IndexHasHUD - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.IndexHasHUD entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return {};
		}
		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			return true;
		}
		return false;
	}

	static bool PlayerHUDComponent_GetVisible(uint64_t entityID, uint32_t tableIndex) {

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.GetVisible - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetVisible entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return {};
		}
		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			return comp.HudTable->GetPanel(tableIndex)->Visible;
		}
		PF_ERROR("PlayerHUDComponent.GetVisible entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
		return false;
	}
	static void PlayerHUDComponent_SetVisible(uint64_t entityID, uint32_t tableIndex, bool* visible) {

		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetVisible - entity is invalid ");
			return ;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetVisible entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return ;
		}
		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			comp.HudTable->GetPanel(tableIndex)->Visible = *visible;
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetVisible entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}

	static bool PlayerHUDComponent_HasButton(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName) 
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.HasButton - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.HasButton entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return false;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(buttonName);
			if (panel->ButtonHas(buttonNamestr))
			{
				return true;
			}
		}
		return false;
	}
	static void PlayerHUDComponent_SetButtonData(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName, UIBaseData* data)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetButtonData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(buttonName);
			if (!panel->ButtonHas(buttonNamestr))
			{
				PF_ERROR("PlayerHUDComponent.SetButtonData index {} does not contain button {}", tableIndex, buttonNamestr);
				return;
			}
			UIButton& button = panel->ButtonGet(buttonNamestr);

			button.TintColour = data->Color;
			button.Postion = data->Position;
			button.Rotation = data->Rotation;
			button.Visible = data->Visible;
			button.Size = data->Size;
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);

	}
	static UIBaseData PlayerHUDComponent_GetButtonData(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent_GetButtonData - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return {};
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		UIBaseData data;
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(buttonName);
			if (!panel->ButtonHas(buttonNamestr))
			{
				PF_ERROR("PlayerHUDComponent.GetButtonData index {} does not contain button {}", tableIndex, buttonNamestr);
				return {};
			}
			UIButton& button = panel->ButtonGet(buttonNamestr);

			data.Color = button.TintColour;
			data.Position = button.Postion;
			data.Rotation = button.Rotation;
			data.Visible = button.Visible;
			data.Size = button.Size;
			return data;
			
		}
		PF_ERROR("PlayerHUDComponent.GetButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
		return {};
	}

	static bool PlayerHUDComponent_HasImageButton(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.HasImageButton - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.HasImageButton entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return false;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(buttonName);
			if (panel->ImageButtonHas(buttonNamestr))
			{
				return true;
			}
		}
		return false;
	}
	static UIImageButtonData PlayerHUDComponent_GetImageButtonData(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent_GetImageButtonData - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetImageButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return {};
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		UIImageButtonData data;
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(buttonName);
			if (!panel->ImageButtonHas(buttonNamestr))
			{
				PF_ERROR("PlayerHUDComponent.GetImageButtonData index {} does not contain button {}", tableIndex, buttonNamestr);
				return {};
			}
			UIButtonImage& button = panel->GetImageButton(buttonNamestr);

			data.Base.Color = button.TintColour;
			data.Base.Position = button.Postion;
			data.Base.Rotation = button.Rotation;
			data.Base.Size = button.Size;
			data.Base.Visible = button.Visible;
			data.AssetID = (button.Texture != nullptr ) ? button.Texture->GetID() : AssetID(0);
			return data;

		}
		PF_ERROR("PlayerHUDComponent.GetImageButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
		return {};
	}
	static void PlayerHUDComponent_SetImageButtonData(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName, UIImageButtonData* data)
	{
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetImageButtonData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetImageButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(buttonName);
			if (!panel->ImageButtonHas(buttonNamestr))
			{
				PF_ERROR("PlayerHUDComponent.SetImageButtonData index {} does not contain button {}", tableIndex, buttonNamestr);
				return;
			}
			UIButtonImage& button = panel->GetImageButton(buttonNamestr);

			button.TintColour = data->Base.Color;
			button.Postion = data->Base.Position;
			button.Visible = data->Base.Visible;
			button.Rotation = data->Base.Rotation;
			button.Size = data->Base.Size;
			if (data->AssetID != 0)
			{
				if (button.Texture == nullptr)
				{
					if (AssetManager::HasAsset(data->AssetID))
					{
						button.Texture = AssetManager::GetAsset<Texture2D>(data->AssetID);
						return;
					}
				}

				if (button.Texture->GetID() == data->AssetID)
					return;
				button.Texture = AssetManager::GetAsset<Texture2D>(data->AssetID);
			}

			if (data->AssetID == 0 && button.Texture != nullptr)
				button.Texture = nullptr;
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetImageButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}

	static bool PlayerHUDComponent_HasText(uint64_t entityID, uint32_t tableIndex, MonoString* textName) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.HasText - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.HasText entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return false;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(textName);
			if (panel->TextHas(buttonNamestr))
			{
				return true;
			}
		}
		return false;
	}
	static void PlayerHUDComponent_GetTextData(uint64_t entityID, uint32_t tableIndex, MonoString* textName, UITextData* data,MonoString** textData) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.GetTextData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetTextData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(textName);
			if (!panel->TextHas(buttonNamestr))
			{
				PF_ERROR("PlayerHUDComponent.GetTextData index {} does not contain button {}", tableIndex, buttonNamestr);
				return;
			}
			UIText& text = panel->TextGet(buttonNamestr);

			data->Kerning = text.Param.Kerning;
			data->LineSpacing = text.Param.LineSpacing;
			data->Base.Color = text.Param.Color;
			data->Base.Position = text.Postion;
			data->Base.Rotation = text.Rotation;
			data->Base.Size = text.Size;
			data->Base.Visible = text.Visible;
			*textData = ScriptUtils::UTF8StringToMono(text.Text);
			return;
		}
		PF_ERROR("PlayerHUDComponent.GetTextData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}
	static void PlayerHUDComponent_SetTextData(uint64_t entityID, uint32_t tableIndex, MonoString* textName, UITextData* data, MonoString** textData) {
		Entity entity = ScriptEngine::GetWorldContext()->GetEntity(entityID);
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetTextData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetTextData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetUUID());
			return;
		}

		PlayerHUDComponent& comp = entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			auto panel = comp.HudTable->GetPanel(tableIndex);
			std::string buttonNamestr = ScriptUtils::MonoStringToUTF8(textName);
			if (!panel->TextHas(buttonNamestr))
			{
				PF_ERROR("PlayerHUDComponent.SetTextData index {} does not contain button {}", tableIndex, buttonNamestr);
				return;
			}
			UIText& text = panel->TextGet(buttonNamestr);

			text.Param.Kerning = data->Kerning;
			text.Param.LineSpacing = data->LineSpacing ;
			text.Param.Color = data->Base.Color ;
			text.Postion= data->Base.Position ;
			text.Rotation = data->Base.Rotation ;
			text.Size = data->Base.Size;
			text.Visible =data->Base.Visible;
			text.Text = ScriptUtils::MonoStringToUTF8(*textData);
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetTextData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
	}
	#pragma endregion

#pragma region ScriptFunc

	template<typename... Component>
	static void RegisterComponent() {
		//iterate over templates list
		PF_ENGINE_INFO("ScriptFuncs Register Components");
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Proof.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage);
				if (!managedType) {
					PF_ENGINE_ERROR("	ScriptFuncs Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
				PF_ENGINE_TRACE("	ScriptFuncs Component Registered {}", managedTypename);
			}(), ...); //... keep expanding templates
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>) {
		RegisterComponent<Component...>();
	}

	void ScriptFunc::RegisterAllComponents() 
	{
		ScopeTimer timer(__FUNCTION__);

		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});

	}
#pragma endregion 

	void ScriptFunc::RegisterFunctions() {
		ScopeTimer scopeTimer(__FUNCTION__);

		PF_ENGINE_INFO("ScriptFuncs Register Functions");

		{
			PF_ADD_INTERNAL_CALL(ApplyCameraRotate);
		}
		//Applicaiton 
		{
			PF_ADD_INTERNAL_CALL(Application_Shutdown);
			PF_ADD_INTERNAL_CALL(Application_GetFPS);
		}
		//LOG
		{
			PF_ADD_INTERNAL_CALL(Log_Message);
		}
		// Mouse
		{
			PF_ADD_INTERNAL_CALL(Mouse_CaptureMouse);
			PF_ADD_INTERNAL_CALL(Mouse_IsMouseCaptured);
			PF_ADD_INTERNAL_CALL(Mouse_GetPosition);
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
		//World
		{
			PF_ADD_INTERNAL_CALL(World_Instanciate);
			PF_ADD_INTERNAL_CALL(World_IsEntityValid);
			PF_ADD_INTERNAL_CALL(World_TryFindEntityByTag);
			PF_ADD_INTERNAL_CALL(World_DeleteEntity);
			PF_ADD_INTERNAL_CALL(World_GetTimeStep);
			PF_ADD_INTERNAL_CALL(World_ForEachEntityWith);
			PF_ADD_INTERNAL_CALL(World_Restart);
			PF_ADD_INTERNAL_CALL(World_OpenWorld);
			PF_ADD_INTERNAL_CALL(World_Play);
			PF_ADD_INTERNAL_CALL(World_Pause);
		}
		//Entity 
		{
			PF_ADD_INTERNAL_CALL(Entity_HasComponent);
			PF_ADD_INTERNAL_CALL(GetScriptInstance);
			PF_ADD_INTERNAL_CALL(Entity_GetParent);
			PF_ADD_INTERNAL_CALL(Entity_GetChildren);
		}
		//AssetID
		{
			PF_ADD_INTERNAL_CALL(AssetID_IsValid);
		}
		// Tag Componnent
		{
			PF_ADD_INTERNAL_CALL(TagComponent_GetTag);
			PF_ADD_INTERNAL_CALL(TagComponent_SetTag);
		}
		//Transform Component
		{
			PF_ADD_INTERNAL_CALL(TransformComponent_GetLocation);
			PF_ADD_INTERNAL_CALL(TransformComponent_SetLocation);
			PF_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
			PF_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
			PF_ADD_INTERNAL_CALL(TransformComponent_GetScale);
			PF_ADD_INTERNAL_CALL(TransformComponent_SetScale);
			PF_ADD_INTERNAL_CALL(TransformComponent_GetFowardVector);
			PF_ADD_INTERNAL_CALL(TransformComponent_GetTransform);
			PF_ADD_INTERNAL_CALL(TransformComponent_SetTransform);
			PF_ADD_INTERNAL_CALL(TransformComponent_GetWorldSpaceTransform);

		}
		//physics
		{
			PF_ADD_INTERNAL_CALL(Physics_Raycast);
			PF_ADD_INTERNAL_CALL(Physics_SphereCast);
			//PF_ADD_INTERNAL_CALL(Physics_Raycast2D);
			//PF_ADD_INTERNAL_CALL(Physics_OverlapBox);
			//PF_ADD_INTERNAL_CALL(Physics_OverlapCapsule);
			//PF_ADD_INTERNAL_CALL(Physics_OverlapSphere);
			//PF_ADD_INTERNAL_CALL(Physics_OverlapBoxNonAlloc);
			//PF_ADD_INTERNAL_CALL(Physics_OverlapCapsuleNonAlloc);
			//PF_ADD_INTERNAL_CALL(Physics_OverlapSphereNonAlloc);
			//PF_ADD_INTERNAL_CALL(Physics_GetGravity);
			//PF_ADD_INTERNAL_CALL(Physics_SetGravity);
			PF_ADD_INTERNAL_CALL(Physics_AddRadialImpulse);
		}

		//Rigid Body Component
		{
			PF_ADD_INTERNAL_CALL(RigidBody_GetGravity);
			PF_ADD_INTERNAL_CALL(RigidBody_SetGravity);
			PF_ADD_INTERNAL_CALL(RigidBody_GetMass);
			PF_ADD_INTERNAL_CALL(RigidBody_SetMass);
			PF_ADD_INTERNAL_CALL(RigidBody_AddForce);
			PF_ADD_INTERNAL_CALL(RigidBody_AddTorque);

			PF_ADD_INTERNAL_CALL(RigidBody_ClearForce);
			PF_ADD_INTERNAL_CALL(RigidBody_ClearTorque);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetAngularVelocity);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetAngularVelocity);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetLinearVelocity);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetLinearVelocity);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetBodyType);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetBodyType);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_IsKinematic);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetIsKinematic);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetLocation);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetLocation);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetRotation);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetRotation);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetMaxLinearVelocity);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetMaxLinearVelocity);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetMaxAngularVelocity);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetMaxAngularVelocity);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetLinearDrag);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetLinearDrag);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetAngularDrag);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetAngularDrag);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetLayer);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetLayer);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetLayerByName);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetLayerName);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_IsSleeping);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetIsSleeping);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_GetKinematicTarget);
			PF_ADD_INTERNAL_CALL(RigidBodyComponent_SetKinematicTarget);

			PF_ADD_INTERNAL_CALL(RigidBodyComponent_AddForceAtLocation);
		}
		//BoxColliderComponent
		{
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_GetSize);
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_SetSize);
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_GetCenter);
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_SetCenter);
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_IsTrigger);
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_SetTrigger);
			PF_ADD_INTERNAL_CALL(BoxColliderComponent_GetPhysicsMaterialID);
		}
		//Sphere Collider
		{
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_GetRadius);
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_SetRadius);
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_GetCenter);
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_SetCenter);
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_IsTrigger);
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_SetTrigger);
			PF_ADD_INTERNAL_CALL(SphereColliderComponent_GetPhysicsMaterialID);
		}

		// Mesh Collider
		{
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_IsMeshStatic);
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_IsColliderMeshValid);
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_GetColliderMesh);
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_SetColliderMesh);
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_IsTrigger);
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_SetTrigger);
			PF_ADD_INTERNAL_CALL(MeshColliderComponent_GetPhysicsMaterialID);
		}
		//PhysicsMaterial
		{
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_GetStaticFriction);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_SetStaticFriction);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_GetDynamicFriction);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_SetDynamicFriction);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_GetBounciness);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_SetBounciness);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_GetFrictionCombineMode);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_SetFrictionCombineMode);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_GetBouncinessCombineMode);
			PF_ADD_INTERNAL_CALL(PhysicsMaterial_SetBouncinessCombineMode);
		}

		//Mesh Collider
		{
			PF_ADD_INTERNAL_CALL(MeshCollider_IsStaticMesh);

		}
		// MeshComponent 
		{
			PF_ADD_INTERNAL_CALL(MeshComponent_GetVisible);
			PF_ADD_INTERNAL_CALL(MeshComponent_SetVisible);
		}

		//Mesh Base
		{
			PF_ADD_INTERNAL_CALL(MeshBase_GetMaterialByIndex);
			PF_ADD_INTERNAL_CALL(MeshBase_GetMaterialCount);
			PF_ADD_INTERNAL_CALL(MeshBase_IsStaticMesh);
		}
		//ChildCOmponet 
		{
			PF_ADD_INTERNAL_CALL(ChildComponent_AddChild);
			PF_ADD_INTERNAL_CALL(ChildComponent_RemoveChild);
		}
		//TextComponent
		{
			PF_ADD_INTERNAL_CALL(TextComponent_GetText);
			PF_ADD_INTERNAL_CALL(TextComponent_SetText);
		}
		//random
		{
			PF_ADD_INTERNAL_CALL(Random_RandomBool);
			PF_ADD_INTERNAL_CALL(Random_RandomInt32);
			PF_ADD_INTERNAL_CALL(Random_RandomFloat);
			PF_ADD_INTERNAL_CALL(Random_RandomDouble);
		}

		//Player InputComponent
		{
			PF_ADD_INTERNAL_CALL(PlayerInputComponent_SetAction);
			PF_ADD_INTERNAL_CALL(PlayerInputComponent_SetMotion);
			PF_ADD_INTERNAL_CALL(PlayerInputComponent_SetInputState);
		}
		
		//playerHud COmponent
		{
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_IndexHasHUD);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_GetVisible);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_SetVisible);

			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_HasButton);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_GetButtonData);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_SetButtonData);
			
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_HasImageButton);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_GetImageButtonData);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_SetImageButtonData);


			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_HasText);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_GetTextData);
			PF_ADD_INTERNAL_CALL(PlayerHUDComponent_SetTextData);
		}
		//particleSystem component
		{
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_HasParticleIndex);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_ParticleIndexHasParticle);

			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_GetVisible);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_SetVisible);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_Play);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_Pause);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_End);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_Restart);
			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_GetState);

			PF_ADD_INTERNAL_CALL(ParticleSystemComponent_GetParticles);
		}
	}
}
