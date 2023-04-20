using ProofScriptCore.src.Proof.Math;
using System;
using System.Runtime.CompilerServices;

namespace Proof
{
	public static class InternalCalls
	{
		#region Log
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Log_Message(int logType, string message);
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

        #region World

        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
		//returns entity ID
        internal extern static ulong World_Instanciate(ulong prefabID, Transform transform);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        //returns entity ID
        internal extern static ulong World_TryFindEntityByTag(string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern	static void World_DeleteEntity(ulong entityID, bool deleteChildren);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float World_GetTimeStep();
        #endregion

        #region Entity
        [System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(ulong entityID, string scrptName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetOwner(ulong entityID, out ulong ownerID);
        #endregion

        #region TagComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TagComponent_GetTag(ulong entityID, out string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TagComponent_SetTag(ulong entityID, ref string message);
        #endregion

        #region TransformComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetLocation(ulong entityID, out Vector location);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetLocation(ulong entityID, ref Vector location);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetRotation(ulong entityID, out Vector rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetRotation(ulong entityID, ref Vector rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetScale(ulong entityID, out Vector scale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetScale(ulong entityID, ref Vector Scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetFowardVector(ulong entityID, out Vector vec);
        #endregion

        #region RigidBody

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_GetMass(ulong entityID, out float outMass);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_SetMass(ulong entityID, ref float mass);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_AddForce(ulong entityID, Vector force, int forceMode, bool autoAwake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody_AddTorque(ulong entityID, Vector force, int forceMode, bool autoAwake);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ClearTorque(ulong entityID, int forceMode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ClearForce(ulong entityID, int forceMode);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_GetLinearVelocity(ulong entityID, out Vector linearVelocity);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_SetLinearVelocity(ulong entityID, ref Vector linearVelocity, bool autoAwake);
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
        #endregion
    }
}
