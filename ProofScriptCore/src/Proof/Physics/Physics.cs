using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    
    [Flags]
    public enum PhysicsActorConstraint : uint
    {
        None = 0,

        LocationX = 1 << 0,
        LocationY = 1 << 1,
        LocationZ = 1 << 2,
        AllLocation = LocationX | LocationY | LocationZ,

        RotationX = 1 << 3,
        RotationY = 1 << 4,
        RotationZ = 1 << 5,
        AllRotation = RotationX | RotationY | RotationZ,

        All = AllLocation | AllRotation
    } 
    [StructLayout(LayoutKind.Sequential)]
    public struct RaycastHit
    {
        public ulong EntityID { get; internal set; }
        public Proof.Vector3 Position { get; internal set; }
        public Proof.Vector3 Normal { get; internal set; }
        public float Distance { get; internal set; }
        public Collider HitCollider { get; internal set; }

        public Entity Entity => World.FindEntityByID(EntityID);
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct RaycastData
    {
        public Proof.Vector3 Origin;
        public Proof.Vector3 Direction;
        public float MaxDistance;
        public Type[] RequiredComponents;
        public ulong[] ExcludedEntities;
    } 

    [StructLayout(LayoutKind.Sequential)]
    public struct SphereCastData
    {
        public Proof.Vector3 Origin;
        public Proof.Vector3 Direction;
        public float Radius;
        public float MaxDistance;
        public Type[] RequiredComponents;
        public ulong[] ExcludedEntities;
    }
    public enum EFalloffMode { Constant, Linear }
    [RegisterCoreClassStruct]
    public static class Physics
    {
        /*
        public static Vector3 Gravity
        {
            get
            {
                InternalCalls.Physics_GetGravity(out Proof.Vector3 gravity);
                return gravity;
            }

            set => InternalCalls.Physics_SetGravity(ref value);
        }
        */
        

        /// <summary>
        /// Adds a radial impulse to the scene. Any entity within the radius of the origin will be pushed/pulled according to the strength.
        /// You'd use this method for something like an explosion.
        /// </summary>
        /// <param name="origin">The origin of the impulse in world space</param>
        /// <param name="radius">The radius of the area affected by the impulse (1 unit = 1 meter radius)</param>
        /// <param name="strength">The strength of the impulse</param>
        /// <param name="falloff">The falloff method used when calculating force over distance</param>
        /// <param name="velocityChange">Setting this value to <b>true</b> will make this impulse ignore an actors mass</param>
        public static void AddRadialImpulse(Proof.Vector3 origin, float radius, float strength, EFalloffMode falloff = EFalloffMode.Constant, bool velocityChange = false)
            => InternalCalls.Physics_AddRadialImpulse(ref origin, radius, strength, falloff, velocityChange);

        public static bool RayCast(RaycastData raycastData, out RaycastHit hit) => InternalCalls.Physics_RayCast(ref raycastData, out hit);
        public static bool RayCast(Proof.Vector3 origin, Proof.Vector3 direction, float maxDistance, out RaycastHit hit, params Type[] componentFilters)
        {
            s_RaycastData.Origin = origin;
            s_RaycastData.Direction = direction;
            s_RaycastData.MaxDistance = maxDistance;
            s_RaycastData.RequiredComponents = componentFilters;
            return InternalCalls.Physics_RayCast(ref s_RaycastData, out hit);
        }

        public static bool RayCastLayer(RaycastData raycastData, string layerName,out RaycastHit hit) => InternalCalls.Physics_RayCastLayer(ref raycastData, out hit, layerName);
        public static bool RayCastLayer(Proof.Vector3 origin, Proof.Vector3 direction, float maxDistance, string layerName,out RaycastHit hit, params Type[] componentFilters)
        {
            s_RaycastData.Origin = origin;
            s_RaycastData.Direction = direction;
            s_RaycastData.MaxDistance = maxDistance;
            s_RaycastData.RequiredComponents = componentFilters;
            return InternalCalls.Physics_RayCastLayer(ref s_RaycastData, out hit, layerName);
        }

        public static bool SphereCast(SphereCastData spherecastData, out RaycastHit hit) => InternalCalls.Physics_SphereCast(ref spherecastData, out hit);
        public static bool SphereCast(Proof.Vector3 origin, Proof.Vector3 direction, float radius, float maxDistance, out RaycastHit hit, params Type[] componentFilters)
        {
            s_SphereCastData.Origin = origin;
            s_SphereCastData.Direction = direction;
            s_SphereCastData.Radius = radius;
            s_SphereCastData.MaxDistance = maxDistance;
            s_SphereCastData.RequiredComponents = componentFilters;
            return InternalCalls.Physics_SphereCast(ref s_SphereCastData, out hit);
        }

        public static Entity[] OverlapSphere(Proof.Vector3 origin, float radius)
        {
            Entity[] entities = InternalCalls.Physics_OverlapSphere(ref origin, radius);
            if(entities == null)
                return Array.Empty<Entity>();

            return entities;
        }
        /*      
        public static RaycastHit2D[] Raycast2D(RaycastData2D raycastData) => InternalCalls.Physics_Raycast2D(ref raycastData);
        public static RaycastHit2D[] Raycast2D(Proof.Vector2 origin, Proof.Vector2 direction, float maxDistance, params Type[] componentFilters)
        {
            s_RaycastData2D.Origin = origin;
            s_RaycastData2D.Direction = direction;
            s_RaycastData2D.MaxDistance = maxDistance;
            s_RaycastData2D.RequiredComponents = componentFilters;
            return InternalCalls.Physics_Raycast2D(ref s_RaycastData2D);
        }
        */

        /*
        public static Collider[] OverlapBox(Proof.Vector3 origin, Proof.Vector3 halfSize) => InternalCalls.Physics_OverlapBox(ref origin, ref halfSize);
        public static Collider[] OverlapCapsule(Proof.Vector3 origin, float radius, float halfHeight) => InternalCalls.Physics_OverlapCapsule(ref origin, radius, halfHeight);
        public static Collider[] OverlapSphere(Proof.Vector3 origin, float radius) => InternalCalls.Physics_OverlapSphere(ref origin, radius);

        public static int OverlapBoxNonAlloc(Proof.Vector3 origin, Proof.Vector3 halfSize, Collider[] colliders) => InternalCalls.Physics_OverlapBoxNonAlloc(ref origin, ref halfSize, colliders);
        public static int OverlapCapsuleNonAlloc(Proof.Vector3 origin, float radius, float halfHeight, Collider[] colliders) => InternalCalls.Physics_OverlapCapsuleNonAlloc(ref origin, radius, halfHeight, colliders);
        public static int OverlapSphereNonAlloc(Proof.Vector3 origin, float radius, Collider[] colliders) => InternalCalls.Physics_OverlapSphereNonAlloc(ref origin, radius, colliders);
        */
        private static RaycastData s_RaycastData;
        private static SphereCastData s_SphereCastData;
    }
}
