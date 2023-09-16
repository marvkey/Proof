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

#include "Proof/Asset/AssetManager.h"
//(IMPORTANT)
/*
*WHEN PASSING A MONO TYPE MAKE SURE ITS A SRUCT BECAUSE WHEN ITS A CLASS IT GETS SOME UNDEFNIED BEHAVIOR
*/
namespace Proof
{
	struct Transform
	{
		glm::vec3 Location;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
	
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	namespace ScriptFuncUtils
	{
		
	
	}
	#define PF_ADD_INTERNAL_CALL(Name){\
		mono_add_internal_call("Proof.InternalCalls::" #Name, (void*)Name);\
		PF_ENGINE_TRACE("	C# registered function {}", #Name);\
	}

	static void ApplyCameraRotate(uint64_t entityID) 
	{
		return;
		World* world = ScriptEngine::GetWorldContext();
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
		Mouse::CaptureMouse(caputre);
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
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		world->Pause();
		
	}

	static void World_Play() {
		World* world = ScriptEngine::GetWorldContext();
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

		World* world = ScriptEngine::GetWorldContext();
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
		World* world = ScriptEngine::GetWorldContext();
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
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		world->EnableRestart = true;
		///World* world = ScriptEngine::GetWorldContext();
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
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		std::string tag = ScriptEngine::MonoToString(classFullName);
		Entity entity = world->TryGetEntityByTag(tag);
		return entity.GetUUID();
	}
	static void World_ForEachEntityWith(MonoString* classFullName, MonoArray** theArray)
	{
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");

		std::vector<uint64_t> objects;
		std::string className = ScriptEngine::MonoToString(classFullName);

		for (auto& [entityID, scripts] : ScriptEngine::EachEntityScript())
		{
			if (scripts.contains(className))
				objects.emplace_back(entityID);
		}
		if (objects.size() == 0)
			return;
		*theArray = mono_array_new(ScriptEngine::GetDomain(), mono_get_uint64_class(), objects.size());
		memcpy(mono_array_addr(*theArray, uint64_t, 0), objects.data(), objects.size() * sizeof(uint64_t));
	}
	static void World_DeleteEntity(uint64_t entityID, bool deleteChildren) 
	{
		World* world = ScriptEngine::GetWorldContext();
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
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		Entity entity = world->GetEntity(entityID);

		PF_CORE_ASSERT(entity, "Entity is null");

		std::vector<uint64_t> objects;
		entity.EachChild([&](Entity child) {
			objects.emplace_back(child.GetUUID());
		});
		if (objects.size() == 0)
			return;
		*theArray = mono_array_new(ScriptEngine::GetDomain(), mono_get_uint64_class(), objects.size());
		memcpy(mono_array_addr(*theArray, uint64_t, 0), objects.data(), objects.size() * sizeof(uint64_t));
	}
	static bool Entity_HasComponent(uint64_t entityID, MonoReflectionType* componentType) {
		if (entityID == 0)return false;
		World* world = ScriptEngine::GetWorldContext();
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
		if (!ScriptEngine::EntityHasScripts(ScriptEngine::GetWorldContext()->GetEntity(entityID)))
			return nullptr;
		return ScriptEngine::GetMonoManagedObject(entityID,ScriptEngine::MonoToString(classFullName));
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
		*tag = ScriptEngine::StringToMono(entity.GetComponent<TagComponent>().Tag);
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

		std::string newTag = ScriptEngine::MonoToString(*tag);
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
		* text = ScriptEngine::StringToMono(entity.GetComponent<TextComponent>().Text);
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
		entity.GetComponent<TextComponent>().Text = ScriptEngine::MonoToString(*text);
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
	static void RigidBody_AddForce(UUID entityID, Vector force, int forceMode, bool autoAwake) {
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.AddForce - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->AddForce(force, (ForceMode)forceMode, autoAwake);
	}
	static void RigidBody_AddTorque(UUID entityID, Vector force, int forceMode, bool autoAwake) {
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.AddTorque - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->AddTorque(force, (ForceMode)forceMode, autoAwake);
	}

	static void RigidBody_ClearForce(UUID entityID, int forceMode)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.ClearForce - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->ClearForce((ForceMode)forceMode);
	}

	static void RigidBody_ClearTorque(UUID entityID, int forceMode)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.ClearTorque - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->ClearTorque((ForceMode)forceMode);
	}
	static void RigidBody_GetLinearVelocity(UUID entityID, Vector* force) 
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.GetLinearVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		*force = actor->GetLinearVelocity();
	}

	static void RigidBody_SetLinearVelocity(UUID entityID, Vector* force, bool wakeUP)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.SetLinearVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		 actor->SetLinearVelocity(*force, wakeUP);
	}
	static void RigidBody_SetAngularVelocity(UUID entityID, Vector* force, bool wakeUP)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.SetAngularVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->SetAngularVelocity(*force, wakeUP);
	}

	static void RigidBody_GetAngularVelocity(UUID entityID, Vector* force)
	{

		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.GetAngularVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		*force = actor->GetAngularVelocity();
	}
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

		PlayerInputComponent& playerInput = entity.GetComponent <PlayerInputComponent>();
		
		auto entityScripts = ScriptEngine::GetScriptInstnace(entity);

		std::string classAsString = ScriptEngine::MonoToString(className);
		if(!entityScripts.contains(classAsString))return;

		Count<ScriptInstance> script = entityScripts.at(classAsString);

		std::string meathodNameStr = ScriptEngine::MonoToString(meathodName);
		MonoMethod* meathod = mono_class_get_method_from_name(script->GetScriptClass()->GetMonoClass(), meathodNameStr.c_str(), 0);
		auto call = [script = script, meathod = meathod]() {
			ScriptMeathod::CallMeathod(script, meathod, nullptr);
		};
		InputManagerMeathods::BindAction(ScriptEngine::MonoToString(ActionName), (uint32_t)playerInput.InputPlayer, (InputEvent) inputState,call);
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

		auto entityScripts = ScriptEngine::GetScriptInstnace(entity);

		std::string classAsString = ScriptEngine::MonoToString(className);
		if (!entityScripts.contains(classAsString))return;

		Count<ScriptInstance> script = entityScripts.at(classAsString);
		std::string meathodNameStr = ScriptEngine::MonoToString(meathodName);
		MonoMethod* meathod = mono_class_get_method_from_name(script->GetScriptClass()->GetMonoClass(), meathodNameStr.c_str(), 1);
		auto call = [script = script, meathod = meathod](float motionValue) {
			void* param = &motionValue;
			ScriptMeathod::CallMeathod(script, meathod, &param);
		};
		InputManagerMeathods::BindMotion(ScriptEngine::MonoToString(motionName), (uint32_t)playerInput.InputPlayer, call);
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

		if (objects.size() == 0)
			return;
		*theArray = mono_array_new(ScriptEngine::GetDomain(), mono_get_uint32_class(), objects.size());
		memcpy(mono_array_addr(*theArray, uint64_t, 0), objects.data(), objects.size() * sizeof(uint64_t));
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
			std::string buttonNamestr = ScriptEngine::MonoToString(buttonName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(buttonName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(buttonName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(buttonName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(buttonName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(buttonName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(textName);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(textName);
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
			*textData =ScriptEngine::StringToMono( text.Text);
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
			std::string buttonNamestr = ScriptEngine::MonoToString(textName);
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
			text.Text = ScriptEngine::MonoToString(*textData);
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetTextData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetUUID(), tableIndex);
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

			PF_ADD_INTERNAL_CALL(RigidBody_GetAngularVelocity);
			PF_ADD_INTERNAL_CALL(RigidBody_SetAngularVelocity);

			PF_ADD_INTERNAL_CALL(RigidBody_GetLinearVelocity);
			PF_ADD_INTERNAL_CALL(RigidBody_SetLinearVelocity);
		}
		// MeshComponent 
		{
			PF_ADD_INTERNAL_CALL(MeshComponent_GetVisible);
			PF_ADD_INTERNAL_CALL(MeshComponent_SetVisible);
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
