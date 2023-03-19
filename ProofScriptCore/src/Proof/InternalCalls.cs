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

		#region Entity
		[System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(ulong entityID, string scrptName);
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
		#endregion

	}
}
