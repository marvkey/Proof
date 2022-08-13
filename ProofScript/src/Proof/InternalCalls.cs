using System;
using System.Runtime.CompilerServices;

namespace Proof
{
	public static class InternalCalls
	{
		[System.Runtime.CompilerServices.MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

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
		internal extern static void Rigidbody3DComponent_ApplyLinearImpulse(ulong entityID, ref Vector impulse, ref Vector point, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RandomInt (out Type value,Type intType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float Random_Float(float min, float max);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RandomBool(out bool value);
	}
}
