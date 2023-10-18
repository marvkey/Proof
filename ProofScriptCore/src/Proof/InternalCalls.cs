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
        internal extern static void RigidBody_GetLinearVelocity(ulong entityID, out Vector3 linearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_SetLinearVelocity(ulong entityID, ref Vector3 linearVelocity, bool autoAwake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_GetAngularVelocity(ulong entityID, out Vector3 linearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_SetAngularVelocity(ulong entityID, ref Vector3 linearVelocity, bool autoAwake);
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
        internal static extern bool AssetID_IsValid(ref AssetID assetHandle);
        #endregion
    }
}
