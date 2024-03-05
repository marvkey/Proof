using System;
using System.Runtime.CompilerServices;

namespace Proof
{
	public static class InternalCalls
	{
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ApplyCameraRotate(ulong entityID);
        
        #region Log
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Log_Message(int logType, string message);
        #endregion

        #region Mouse
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Mouse_CaptureMouse(bool caputre);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Mouse_IsMouseCaptured();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Mouse_GetPosition(out Vector2 pos);
        #endregion
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyClicked(int keycode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyPressed(int keycode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyReleased(int keycode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyHold(int keycode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDoubleClick(int keycode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonClicked(int mouseCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonPressed(int mouseCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonReleased(int mouseCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonDoubleClicked(int mouseCode);
        #endregion

        #region Application
        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Application_Shutdown();
        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Application_GetFPS();
        #endregion
        #region World

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
		//returns entity ID
        internal extern static ulong World_Instanciate(ulong prefabID, Transform transform);

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void World_Pause();
        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void World_Play();

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool World_OpenWorld(ulong worldID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        //returns entity ID
        internal extern static ulong World_TryFindEntityByTag(string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern	static void World_DeleteEntity(ulong entityID, bool deleteChildren);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void World_Restart();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float World_GetTimeStep();

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        //returns entity ID of objects with class naem
        internal extern static void World_ForEachEntityWith(string className, ref ulong[] types);

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool World_IsEntityValid(ulong ID);

        #endregion

        #region Entity

        //returns entity of id
        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetChildren(ulong entityID, ref ulong[] types);

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(ulong entityID, string scrptName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetParent(ulong entityID, out ulong ownerID);
        #endregion

        #region TagComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TagComponent_GetTag(ulong entityID, out string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TagComponent_SetTag(ulong entityID, ref string message);
        #endregion

        #region TransformComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetLocation(ulong entityID, out Vector3 location);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetLocation(ulong entityID, ref Vector3 location);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetRotation(ulong entityID, out Vector3 rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetRotation(ulong entityID, ref Vector3 rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetScale(ulong entityID, out Vector3 scale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 Scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetFowardVector(ulong entityID, out Vector3 vec);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetTransform(ulong entityID, out Transform outTransform);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetTransform(ulong entityID, ref Transform inTransform);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetWorldSpaceTransform(ulong entityID, out Transform outTransform);
        #endregion

        #region Physics

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Physics_Raycast(ref RaycastData raycastData, out RaycastHit hit);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Physics_SphereCast(ref SphereCastData sphereCastData, out RaycastHit hit);
        /*
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern RaycastHit2D[] Physics_Raycast2D(ref RaycastData2D raycastData);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Collider[] Physics_OverlapBox(ref Vector3 origin, ref Vector3 halfSize);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Collider[] Physics_OverlapCapsule(ref Vector3 origin, float radius, float halfHeight);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Collider[] Physics_OverlapSphere(ref Vector3 origin, float radius);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int Physics_OverlapBoxNonAlloc(ref Vector3 origin, ref Vector3 halfSize, Collider[] colliders);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int Physics_OverlapCapsuleNonAlloc(ref Vector3 origin, float radius, float halfHeight, Collider[] colliders);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int Physics_OverlapSphereNonAlloc(ref Vector3 origin, float radius, Collider[] colliders);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Physics_GetGravity(out Vector3 gravity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Physics_SetGravity(ref Vector3 gravity);
        */
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Physics_AddRadialImpulse(ref Vector3 origin, float radius, float strength, EFalloffMode falloff, bool velocityChange);

        #endregion

        #region RigidBody
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody_GetGravity(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] 
        internal extern static void RigidBody_SetGravity(ulong entityID, ref bool gravity); 

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_GetMass(ulong entityID, out float outMass);
		[MethodImplAttribute(MethodImplOptions.InternalCall)] 
		internal extern static void RigidBody_SetMass(ulong entityID, ref float mass);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_AddForce(ulong entityID, Vector3 force, int forceMode, bool autoAwake);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_AddTorque(ulong entityID, Vector3 force, int forceMode, bool autoAwake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ClearTorque(ulong entityID, int forceMode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ClearForce(ulong entityID, int forceMode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetLinearVelocity(ulong entityID, out Vector3 linearVelocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetLinearVelocity(ulong entityID, ref Vector3 linearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetAngularVelocity(ulong entityID, out Vector3 linearVelocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetAngularVelocity(ulong entityID, ref Vector3 linearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetBodyType(ulong entityID,RigidBodyType type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static RigidBodyType RigidBodyComponent_GetBodyType(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBodyComponent_IsKinematic(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetIsKinematic(ulong entityID,bool state);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBodyComponent_IsSleeping(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetIsSleeping(ulong entityID, bool state);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetLocation(ulong entityID, out Vector3 location);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetLocation(ulong entityID, ref Vector3 location);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetRotation(ulong entityID, out Vector3 rotation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetRotation(ulong entityID, ref Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetMaxLinearVelocity(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetMaxLinearVelocity(ulong entityID, ref float linearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetMaxAngularVelocity(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetMaxAngularVelocity(ulong entityID, ref float angularVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetLinearDrag(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetLinearDrag(ulong entityID, ref float linearDrag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBodyComponent_GetAngularDrag(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetAngularDrag(ulong entityID, ref float angularDrag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint RigidBodyComponent_GetLayer(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetLayer(ulong entityID, ref uint layer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBodyComponent_GetLayerName(ulong entityID, out string layerName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBodyComponent_SetLayerByName(ulong entityID, ref string layerName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBodyComponent_GetKinematicTarget(ulong entityID, out Vector3 targetPosition, out Vector3 targetRotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBodyComponent_SetKinematicTarget(ulong entityID, ref Vector3 targetPosition, ref Vector3 targetRotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBodyComponent_AddForceAtLocation(ulong entityID, ref Vector3 force, ref Vector3 location, ForceMode forceMode);
        #endregion
        #region BoxColliderComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void BoxColliderComponent_GetSize(ulong entityID, out Vector3 halfSize);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void BoxColliderComponent_SetSize(ulong entityID, ref Vector3 halfSize);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void BoxColliderComponent_GetCenter(ulong entityID, out Vector3 offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void BoxColliderComponent_SetCenter(ulong entityID, ref Vector3 offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool BoxColliderComponent_IsTrigger(ulong entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void BoxColliderComponent_SetTrigger(ulong entityID, bool isTrigger);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool BoxColliderComponent_GetPhysicsMaterialID(ulong entityID, out AssetID outHandle);

        #endregion

        #region SphereColliderComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float SphereColliderComponent_GetRadius(ulong entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SphereColliderComponent_SetRadius(ulong entityID, float radius);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SphereColliderComponent_GetCenter(ulong entityID, out Vector3 offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SphereColliderComponent_SetCenter(ulong entityID, ref Vector3 offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool SphereColliderComponent_IsTrigger(ulong entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SphereColliderComponent_SetTrigger(ulong entityID, bool isTrigger);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool SphereColliderComponent_GetPhysicsMaterialID(ulong entityID, out AssetID outHandle);

        #endregion

        #region MeshColliderComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshColliderComponent_IsMeshStatic(ulong entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshColliderComponent_IsColliderMeshValid(ulong entityID, ref AssetID meshHandle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshColliderComponent_GetColliderMesh(ulong entityID, out AssetID outHandle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshColliderComponent_SetColliderMesh(ulong entityID, ref AssetID meshHandle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshColliderComponent_IsTrigger(ulong entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshColliderComponent_SetTrigger(ulong entityID, bool isTrigger);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshColliderComponent_GetMaterialHandle(ulong entityID, out AssetID outHandle);

        #endregion

        #region PhysicsMaterial

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float PhysicsMaterial_GetStaticFriction(ref AssetID handle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PhysicsMaterial_SetStaticFriction(ref AssetID handle, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float PhysicsMaterial_GetDynamicFriction(ref AssetID handle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PhysicsMaterial_SetDynamicFriction(ref AssetID handle, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float PhysicsMaterial_GetBounciness(ref AssetID handle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PhysicsMaterial_SetBounciness(ref AssetID handle, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern PhysicsMaterialCombineMode PhysicsMaterial_GetFrictionCombineMode(ref AssetID handle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PhysicsMaterial_SetFrictionCombineMode(ref AssetID handle, PhysicsMaterialCombineMode value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern PhysicsMaterialCombineMode PhysicsMaterial_GetBouncinessCombineMode(ref AssetID handle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void PhysicsMaterial_SetBouncinessCombineMode(ref AssetID handle, PhysicsMaterialCombineMode value);

        #endregion
        #region MeshCollider

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshCollider_IsStaticMesh(ref AssetID meshHandle);

        #endregion
        #region MeshBase

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshBase_GetMaterialByIndex(ref AssetID meshHandle, uint index, out AssetID outHandle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint MeshBase_GetMaterialCount(ref AssetID meshHandle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool MeshBase_IsStaticMesh(ref AssetID meshHandle);
        #endregion
        #region TextComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_GetText(ulong entityID, out string Text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetText(ulong entityID, ref string Text);
		#endregion

		#region Random
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Random_RandomBool();


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int Random_RandomInt32(int min, int max);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Random_RandomFloat(float min, float max);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static double Random_RandomDouble(double min, double max);
        #endregion

        #region MeshComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool MeshComponent_GetVisible(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool MeshComponent_SetVisible(ulong entityID, bool value);
        #endregion

        #region PlayerInputComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void PlayerInputComponent_SetAction(ulong entityID,string className,string ActionName,int state,Action func);
        internal extern static void PlayerInputComponent_SetAction(ulong entityID,string className,string ActionName,int state,string funcName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void PlayerInputComponent_SetMotion(ulong entityID, string className,string motionName, Action<float> func);
        internal extern static void PlayerInputComponent_SetMotion(ulong entityID, string className,string motionName, string funcName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayerInputComponent_SetInputState(ulong entityID, int inputState);

        #endregion

        #region PlayerHUDComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool PlayerHUDComponent_IndexHasHUD(ulong entityID, uint tableIndex);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool PlayerHUDComponent_GetVisible(ulong entityID, uint tableIndex);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayerHUDComponent_SetVisible(ulong entityID, uint tableIndex, ref bool visible);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool PlayerHUDComponent_HasButton(ulong entityID, uint tableIndex, string buttonName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static UIBaseData PlayerHUDComponent_GetButtonData(ulong entityID, uint tableIndex, string buttonName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayerHUDComponent_SetButtonData(ulong entityID, uint tableIndex, string buttonName, ref UIBaseData data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool PlayerHUDComponent_HasImageButton(ulong entityID, uint tableIndex, string buttonName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static UiImageButtonData PlayerHUDComponent_GetImageButtonData(ulong entityID, uint tableIndex, string buttonName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayerHUDComponent_SetImageButtonData(ulong entityID, uint tableIndex,string buttonName, ref UiImageButtonData data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool PlayerHUDComponent_HasText(ulong entityID, uint tableIndex, string textname);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayerHUDComponent_GetTextData(ulong entityID, uint tableIndex, string textname, out UITextData textData, out string text);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayerHUDComponent_SetTextData(ulong entityID, uint tableIndex, string textname, ref UITextData textData, ref string text);
        #endregion

        #region ParticleSystemComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ParticleSystemComponent_HasParticleIndex(ulong entityID, uint tableIndex);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ParticleSystemComponent_ParticleIndexHasParticle(ulong entityID, uint tableIndex);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ParticleSystemComponent_GetVisible(ulong entityID, uint tableIndex);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ParticleSystemComponent_SetVisible(ulong entityID, uint tableIndex, ref bool visible);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ParticleSystemComponent_Play(ulong entityID, uint tableIndex);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ParticleSystemComponent_Pause(ulong entityID, uint tableIndex);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ParticleSystemComponent_End(ulong entityID, uint tableIndex);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ParticleSystemComponent_Restart(ulong entityID, uint tableIndex);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int ParticleSystemComponent_GetState(ulong entityID, uint tableIndex);
        
        //returns all particle indexes
        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ParticleSystemComponent_GetParticles(ulong entityID, ref uint[] types);
        #endregion

        #region AssetID

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AssetID_IsValid(ref AssetID AssetID);
        #endregion
    }
}
