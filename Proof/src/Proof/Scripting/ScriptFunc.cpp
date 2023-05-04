#include "Proofprch.h"
#include "ScriptFunc.h"
#include "Proof/Scene/Entity.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "ScriptEngine.h"
#include "Proof/Scene/Physics/PhysicsWorld.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Input/InputManager.h"
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
	
	#pragma region World

	//retutnrs entity ID
	static uint64_t  World_Instanciate(uint64_t prefabID, TransformComponent transform)
	{
		if (!AssetManager::HasAsset(prefabID))
		{
			PF_EC_ERROR("World.Instanciate - pregabID is invalid {}",prefabID);
			return 0;
		}

		World* world = ScriptEngine::GetWorldContext();
		AssetInfo info = AssetManager::GetAssetInfo(prefabID);
		Count<Prefab> prefab = AssetManager::GetAsset<Prefab>(prefabID);
		Entity entity = world->CreateEntity(info.GetName(), prefab, transform);

		return entity.GetEntityID().Get();
	}

	static uint64_t World_TryFindEntityByTag(MonoString* classFullName)
	{ 
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");
		std::string tag = ScriptEngine::MonoToString(classFullName);
		Entity entity = world->FindEntityByTag(tag);
		return entity.GetEntityID();
	}

	static void World_DeleteEntity(uint64_t entityID, bool deleteChildren) 
	{
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world, "world is nullptr");

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("World.DeleteEntity  - entity is invalid");
			return;
		}
		#endif
		world->DeleteEntity(entity, deleteChildren);
	}
	static float World_GetTimeStep() {
		return FrameTime::GetWorldDeltaTime();
	}
	#pragma endregion
	
	#pragma region Entity
	static bool Entity_HasComponent(uint64_t entityID, MonoReflectionType* componentType) {
		World* world = ScriptEngine::GetWorldContext();
		PF_CORE_ASSERT(world,"world is nullptr");
		Entity entity = { entityID,world };
		PF_CORE_ASSERT(entity,"Entity is null");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		PF_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end(),"mangaed type does not exist");
		// second paremter is calling the funciton
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static MonoObject* GetScriptInstance(UUID entityID, MonoString* classFullName)
	{
		if (!ScriptEngine::EntityHasScripts({ entityID, ScriptEngine::GetWorldContext() }))
			return nullptr;
		return ScriptEngine::GetMonoManagedObject(entityID,ScriptEngine::MonoToString(classFullName));
	}

	static void Entity_GetOwner(uint64_t entityID, uint64_t* owenerId)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("Entity.GetOwner - entity is invalid");
			return;
		}
		#endif

		if (!entity.HasOwner())
		{
			*owenerId = 0;
			return;
		}
		*owenerId = entity.GetOwnerUUID();
	}
#pragma endregion 
	
	#pragma region TagComponent
	static void TagComponent_GetTag(uint64_t entityID, MonoString** tag)
	{
		//https://stackoverflow.com/questions/22428411/add-mono-internal-call-where-a-string-is-passed-by-reference
		// pass mono by refernce with stirngs

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TagComponent.GetTag - entity is invalid");
			return;
		}

		#endif
		*tag = ScriptEngine::StringToMono(entity.GetComponent<TagComponent>()->Tag);
	}
	static void TagComponent_SetTag(uint64_t entityID, MonoString** tag)
	{
		//https://stackoverflow.com/questions/22428411/add-mono-internal-call-where-a-string-is-passed-by-reference
		// pass mono by refernce with stirngs

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TagComponent.SetTag - entity is invalid");
			return;
		}
		#endif

		std::string newTag = ScriptEngine::MonoToString(*tag);
		entity.GetComponent<TagComponent>()->Tag  = newTag;
	}
#pragma endregion 
	
	#pragma region TransformComponent
	static void TransformComponent_GetLocation(uint64_t entityID,Vector* outLocation ) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
			if (!entity)
			{
				PF_EC_ERROR("TransformComponent.GetLocation - entity is invalid");
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
			PF_EC_ERROR("TransformComponent.SetLocation - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>()->Location = *location;
	};

	static void TransformComponent_GetRotation(uint64_t entityID, Vector* outRoation) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.Rotation - entity is invalid");
			return;
		}
		#endif
		* outRoation = entity.GetComponent<TransformComponent>()->Rotation;
	};

	static void TransformComponent_SetRotation(EntityID entityID, Vector* rotation) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.SetRotation - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>()->Rotation = *rotation;
	};
	static void TransformComponent_GetScale(uint64_t entityID, Vector* outScale) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.Scale - entity is invalid");
			return;
		}
		#endif
		* outScale = entity.GetComponent<TransformComponent>()->Scale;
	};
	static void TransformComponent_SetScale(EntityID entityID, Vector* scale) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.SetScale - entity is invalid");
			return;
		}
		#endif
		entity.GetComponent<TransformComponent>()->Scale = *scale;
	};
	static void TransformComponent_GetFowardVector(uint64_t entityID,Vector* vec)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_EC_ERROR("TransformComponent.GetFowardVector - entity is invalid");
			return;
		}
		#endif
		*vec = entity.GetComponent<TransformComponent>()->GetFowardVector();
	}
#pragma endregion 
	
	#pragma region TextComponent

	static void TextComponent_GetText(uint64_t entityID, MonoString** text) 
	{

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
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
		* text = ScriptEngine::StringToMono(entity.GetComponent<TextComponent>()->Text);
	}

	static void TextComponent_SetText(uint64_t entityID, MonoString** text)
	{

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
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
		entity.GetComponent<TextComponent>()->Text = ScriptEngine::MonoToString(*text);
	}
#pragma endregion

	#pragma region RigidBody
	static bool RigidBody_GetGravity(EntityID entityID) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.GetGravity - entity is invalid or Does not have rigidBody");
			return false;
		}
		#endif
		return entity.GetComponent<RigidBodyComponent>()->Gravity;
	}

	static void RigidBody_SetGravity(EntityID entityID, bool* gravity) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("RigidBody.SetGravity - entity is invalid or Does not have rigidBody");
			return ;
		}
		#endif
		entity.GetComponent<RigidBodyComponent>()->Gravity = *gravity;
	}
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

	static void RigidBody_ClearForce(EntityID entityID, int forceMode)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.ClearForce - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->ClearForce((ForceMode)forceMode);
	}

	static void RigidBody_ClearTorque(EntityID entityID, int forceMode)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.ClearTorque - entity is invalid  or does not have rigid body");
			return;
		}

		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->ClearTorque((ForceMode)forceMode);
	}
	static void RigidBody_GetLinearVelocity(EntityID entityID, Vector* force) 
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.GetLinearVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		*force = actor->GetLinearVelocity();
	}

	static void RigidBody_SetLinearVelocity(EntityID entityID, Vector* force, bool wakeUP)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.SetLinearVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		 actor->SetLinearVelocity(*force, wakeUP);
	}
	static void RigidBody_SetAngularVelocity(EntityID entityID, Vector* force, bool wakeUP)
	{
		if (!ScriptEngine::GetWorldContext()->GetPhysicsEngine()->HasActor(entityID))
		{
			PF_ERROR("RigidBody.SetAngularVelocity - entity is invalid  or does not have rigid body");
			return;
		}
		Count<PhysicsActor> actor = ScriptEngine::GetWorldContext()->GetPhysicsEngine()->GetActor(entityID);
		actor->SetAngularVelocity(*force, wakeUP);
	}

	static void RigidBody_GetAngularVelocity(EntityID entityID, Vector* force)
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		Entity childEntity{ childId,ScriptEngine::GetWorldContext() };
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		Entity childEntity{ childId,ScriptEngine::GetWorldContext() };
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("MeshComponent.GetVisible - entity is invalid ");
			return false;
		}
		#endif

		if (entity.HasComponent<MeshComponent>())
		{
			return entity.GetComponent<MeshComponent>()->Visible;
		}
		PF_ERROR("MeshComponent.GetVisible entity tag: {} ID: {}  does not conatin mesh Compoonent", entity.GetName(), entity.GetEntityID());
	}
	static void MeshComponent_SetVisible(uint64_t entityID,bool visible)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("MeshComponent.GetVisible - entity is invalid ");
			return;
		}
		#endif

		if (entity.HasComponent<MeshComponent>())
		{
			entity.GetComponent<MeshComponent>()->Visible = visible;
			return;
		}
		PF_ERROR("MeshComponent.SetVisible entity tag: {} ID: {}  does not conatin mesh Compoonent", entity.GetName(), entity.GetEntityID());
	}
	#pragma endregion
	
	#pragma region PlayerInputComponent
	static void PlayerInputComponent_SetAction(uint64_t entityID, MonoString* className,MonoString* ActionName, uint32_t inputState, MonoString* meathodName)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerInputComponent.SetAction - entity is invalid or Does not have rigidBody");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerInputComponent>())
			return;

		PlayerInputComponent& playerInput = *entity.GetComponent <PlayerInputComponent>();
		
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerInputComponent.SetMotion - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerInputComponent>())
			return;

		PlayerInputComponent& playerInput = *entity.GetComponent <PlayerInputComponent>();

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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerInputComponent.SetInputState - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerInputComponent>())
			return;

		PlayerInputComponent& playerInput = *entity.GetComponent <PlayerInputComponent>();
		InputManagerMeathods::SetPlayerInput((uint32_t)playerInput.InputPlayer, (PlayerInputState)inputState);
	}

	#pragma endregion 

	#pragma region PlayerHUDComponent
	
	static uint64_t PlayerHUDComponent_GetHUDAssetID(uint64_t entityID, uint32_t index) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.GetHUDAssetID - entity is invalid ");
			return 0;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetHUDAssetID entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return 0;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.IndexHasHUD - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.IndexHasHUD entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return {};
		}
		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			return true;
		}
		return false;
	}

	static bool PlayerHUDComponent_GetVisible(uint64_t entityID, uint32_t tableIndex) {

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.GetVisible - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetVisible entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return {};
		}
		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			return comp.HudTable->GetPanel(tableIndex)->Visible;
		}
		PF_ERROR("PlayerHUDComponent.GetVisible entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
		return false;
	}
	static void PlayerHUDComponent_SetVisible(uint64_t entityID, uint32_t tableIndex, bool* visible) {

		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetVisible - entity is invalid ");
			return ;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetVisible entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return ;
		}
		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
		if (comp.HudTable->HasPanel(tableIndex) && comp.HudTable->GetPanel(tableIndex) != nullptr)
		{
			comp.HudTable->GetPanel(tableIndex)->Visible = *visible;
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetVisible entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
	}

	static bool PlayerHUDComponent_HasButton(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName) 
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.HasButton - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.HasButton entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return false;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetButtonData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
			button.Size = data->Size;
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);

	}
	static UIBaseData PlayerHUDComponent_GetButtonData(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent_GetButtonData - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return {};
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
			data.Size = button.Size;
			return data;
			
		}
		PF_ERROR("PlayerHUDComponent.GetButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
		return {};
	}

	static bool PlayerHUDComponent_HasImageButton(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.HasImageButton - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.HasImageButton entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return false;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent_GetImageButtonData - entity is invalid ");
			return {};
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetImageButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return {};
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
			data.AssetID = (button.Texture != nullptr ) ? button.Texture->GetID() : AssetID(0);
			return data;

		}
		PF_ERROR("PlayerHUDComponent.GetImageButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
		return {};
	}
	static void PlayerHUDComponent_SetImageButtonData(uint64_t entityID, uint32_t tableIndex, MonoString* buttonName, UIImageButtonData* data)
	{
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetImageButtonData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetImageButtonData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
			button.Postion = data->Base.Position ;
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
		PF_ERROR("PlayerHUDComponent.SetImageButtonData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
	}

	static bool PlayerHUDComponent_HasText(uint64_t entityID, uint32_t tableIndex, MonoString* textName) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.HasText - entity is invalid ");
			return false;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.HasText entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return false;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.GetTextData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.GetTextData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
			*textData =ScriptEngine::StringToMono( text.Text);
			return;
		}
		PF_ERROR("PlayerHUDComponent.GetTextData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
	}
	static void PlayerHUDComponent_SetTextData(uint64_t entityID, uint32_t tableIndex, MonoString* textName, UITextData* data, MonoString** textData) {
		Entity entity{ entityID,ScriptEngine::GetWorldContext() };
		#if PF_ENABLE_DEBUG
		if (!entity)
		{
			PF_ERROR("PlayerHUDComponent.SetTextData - entity is invalid ");
			return;
		}
		#endif

		if (!entity.HasComponent<PlayerHUDComponent>())
		{
			PF_ERROR("PlayerHUDComponent.SetTextData entity tag: {} ID: {}  does not conatin PlayerHud Component", entity.GetName(), entity.GetEntityID());
			return;
		}

		PlayerHUDComponent& comp = *entity.GetComponent<PlayerHUDComponent>();
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
			text.Size =data->Base.Size  ;
			text.Text = ScriptEngine::MonoToString(*textData);
			return;
		}
		PF_ERROR("PlayerHUDComponent.SetTextData entity tag: {} ID: {}  table index {} is invalid", entity.GetName(), entity.GetEntityID(), tableIndex);
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
		//World
		{
			PF_ADD_INTERNAL_CALL(World_Instanciate);
			PF_ADD_INTERNAL_CALL(World_TryFindEntityByTag);
			PF_ADD_INTERNAL_CALL(World_DeleteEntity);
			PF_ADD_INTERNAL_CALL(World_GetTimeStep);
		}
		//Entity 
		{
			PF_ADD_INTERNAL_CALL(Entity_HasComponent);
			PF_ADD_INTERNAL_CALL(GetScriptInstance);
			PF_ADD_INTERNAL_CALL(Entity_GetOwner);
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
	}
}
