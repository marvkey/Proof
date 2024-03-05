using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
	public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

    [RegisterCoreClassStruct]
    public class TagComponent : Component
	{
		public String Tag
		{
			get
			{
				InternalCalls.TagComponent_GetTag(Entity.ID, out string tag);
				return tag;
			}

			set
			{
				InternalCalls.TagComponent_SetTag(Entity.ID, ref value);
			}
		}
	}

    [RegisterCoreClassStruct]
    public class TransformComponent : Component
	{
		public Vector3 Location
		{
			get
			{
				InternalCalls.TransformComponent_GetLocation(Entity.ID, out Vector3 loc);
				return loc;
			}
			set
			{
				InternalCalls.TransformComponent_SetLocation(Entity.ID, ref value);
			}
		}

		public Vector3 Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
				return rotation;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
			}
		}

		public Vector3 Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
			}
		}
		public Vector3 GetFowardVector()
		{
			InternalCalls.TransformComponent_GetFowardVector(Entity.ID, out Vector3 foward);
			return foward;
		}

		public void Translate(Vector3 vec)
		{
			Location += vec;
		}

        /// <summary>
        /// Transform relative to parent entity (Does <b>NOT</b> account for RigidBody transform)
        /// </summary>
        public Transform LocalTransform
        {
            get
            {
                InternalCalls.TransformComponent_GetTransform(Entity.ID, out Transform result);
                return result;
            }

            set => InternalCalls.TransformComponent_SetTransform(Entity.ID, ref value);
        }

        /// <summary>
        /// Transform in world coordinate space (Does <b>NOT</b> account for RigidBody transform)
        /// </summary>
        public Transform WorldTransform
        {
            get
            {
                InternalCalls.TransformComponent_GetWorldSpaceTransform(Entity.ID, out Transform result);
                return result;
            }
        }

    }

    public class TextComponent : Component
	{
		public string Text
		{
			get
			{
				InternalCalls.TextComponent_GetText(Entity.ID, out string text);
				return text;
			}

			set
			{
				InternalCalls.TextComponent_SetText(Entity.ID, ref value);
			}
		}
	}
	public enum ForceMode
	{
		Force,
		Impulse,
		VelocityChange,
		Acceleration
	};
	public class PlayerInputComponent : Component
	{

	}
    [RegisterCoreClassStruct]
    public class MeshComponent : Component
	{
		public bool Visible
		{
			get
			{
				return InternalCalls.MeshComponent_GetVisible(Entity.ID);
			}
			set
			{
				InternalCalls.MeshComponent_SetVisible(Entity.ID, value);
			}
		}
	}

	public class PlayerHUDComponent : Component
	{
		public UIPanel GetPanel(uint index)
		{
			if (InternalCalls.PlayerHUDComponent_IndexHasHUD(Entity.ID, index))
			{
				return new UIPanel(index, Entity);
			}
			return null;
		}
	}
	public class CameraComponent : Component
	{

	}
    [RegisterCoreClassStruct]
    public class ParticleSystemComponent : Component
	{


		public bool HasParticleIndex(uint index)
		{
			return InternalCalls.ParticleSystemComponent_HasParticleIndex(Entity.ID, index);
		}

		public bool ParticleIndexHasParticle(uint index)
		{
			return InternalCalls.ParticleSystemComponent_ParticleIndexHasParticle(Entity.ID, index);
		}

		public ParticleSystem GetParticle(uint index)
		{
			if (ParticleIndexHasParticle(index))
			{
				return new ParticleSystem(index, Entity);
			}
			return null;
		}

		public ParticleSystem[] GetParticles()
		{
			uint[] list = null;
			InternalCalls.ParticleSystemComponent_GetParticles(Entity.ID, ref list);

			if (list == null)
				return null;
			ParticleSystem[] particleSystems = new ParticleSystem[list.Length];
			int index = -1;
			foreach (uint partIndex in list)
			{
				index++;
				if (ParticleIndexHasParticle(partIndex) == false) continue;

				ParticleSystem ps = new ParticleSystem(partIndex, Entity);
				particleSystems[index] = ps;

			}
			return particleSystems;
		}
	}
	public enum RigidBodyType
	{
		Static,
		Dynamic
	}
    [RegisterCoreClassStruct]
    public class RigidBodyComponent : Component
	{
		public float Mass
		{

			get
			{
				InternalCalls.RigidBody_GetMass(Entity.ID, out float mass);
				return mass;
			}

			set
			{
				InternalCalls.RigidBody_SetMass(Entity.ID, ref value);
			}
		}

		public bool Gravity
		{
			get
			{
				return InternalCalls.RigidBody_GetGravity(Entity.ID);
			}
			set
			{
				InternalCalls.RigidBody_SetGravity(Entity.ID, ref value);
			}
		}
		public void AddForce(Vector3 force, ForceMode mode = ForceMode.Force, bool autoWake = true) =>
			InternalCalls.RigidBody_AddForce(Entity.ID, force, (int)mode, autoWake);
		public void AddTorque(Vector3 force, ForceMode mode = ForceMode.Force, bool autoWake = true) =>
			InternalCalls.RigidBody_AddTorque(Entity.ID, force, (int)mode, autoWake);


		public void ClearForce(ForceMode mode = ForceMode.Force)
		{
			InternalCalls.RigidBody_ClearForce(Entity.ID, (int)mode);
		}

		public void ClearTorque(ForceMode mode = ForceMode.Force)
		{
			InternalCalls.RigidBody_ClearTorque(Entity.ID, (int)mode);
		}


		public RigidBodyType BodyType
		{
			get => InternalCalls.RigidBodyComponent_GetBodyType(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetBodyType(Entity.ID, value);
		}

		public bool IsKinematic
		{
			get => InternalCalls.RigidBodyComponent_IsKinematic(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetIsKinematic(Entity.ID, value);
		}

		public Vector3 Location
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetLocation(Entity.ID, out Vector3 translation);
				return translation;
			}

			set => InternalCalls.RigidBodyComponent_SetLocation(Entity.ID, ref value);
		}

		//Radians
		public Vector3 Rotation
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetRotation(Entity.ID, out Vector3 rotationRadians);
				return rotationRadians;
			}

			set => InternalCalls.RigidBodyComponent_SetRotation(Entity.ID, ref value);
		}


		public Vector3 LinearVelocity
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetLinearVelocity(Entity.ID, out Vector3 velocity);
				return velocity;
			}

			set => InternalCalls.RigidBodyComponent_SetLinearVelocity(Entity.ID, ref value);
		}

		public Vector3 AngularVelocity
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetAngularVelocity(Entity.ID, out Vector3 velocity);
				return velocity;
			}

			set => InternalCalls.RigidBodyComponent_SetAngularVelocity(Entity.ID, ref value);
		}

		public float MaxLinearVelocity
		{
			get => InternalCalls.RigidBodyComponent_GetMaxLinearVelocity(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetMaxLinearVelocity(Entity.ID, ref value);
		}

		public float MaxAngularVelocity
		{
			get => InternalCalls.RigidBodyComponent_GetMaxAngularVelocity(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetMaxAngularVelocity(Entity.ID, ref value);
		}

		public float LinearDrag
		{
			get { return InternalCalls.RigidBodyComponent_GetLinearDrag(Entity.ID); }
			set { InternalCalls.RigidBodyComponent_SetLinearDrag(Entity.ID, ref value); }
		}

		public float AngularDrag
		{
			get => InternalCalls.RigidBodyComponent_GetAngularDrag(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetAngularDrag(Entity.ID, ref value);
		}

		public uint Layer
		{
			get => InternalCalls.RigidBodyComponent_GetLayer(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetLayer(Entity.ID, ref value);
		}

		public string LayerName
		{
			get { InternalCalls.RigidBodyComponent_GetLayerName(Entity.ID, out string name); return name; }
			set => InternalCalls.RigidBodyComponent_SetLayerByName(Entity.ID, ref value);
		}

		public bool IsSleeping
		{
			get => InternalCalls.RigidBodyComponent_IsSleeping(Entity.ID);
			set => InternalCalls.RigidBodyComponent_SetIsSleeping(Entity.ID, value);
		}

		public void GetKinematicTarget(out Vector3 targetPosition, out Vector3 targetRotation)
			=> InternalCalls.RigidBodyComponent_GetKinematicTarget(Entity.ID, out targetPosition, out targetRotation);

		public void SetKinematicTarget(Vector3 targetPosition, Vector3 targetRotation)
			=> InternalCalls.RigidBodyComponent_SetKinematicTarget(Entity.ID, ref targetPosition, ref targetRotation);


		public void AddForceAtLocation(Vector3 force, Vector3 location, ForceMode forceMode = ForceMode.Force)
		{
			InternalCalls.RigidBodyComponent_AddForceAtLocation(Entity.ID, ref force, ref location, forceMode);
		}

	}
    [RegisterCoreClassStruct]
    public class BoxColliderComponent : Component
    {
        public Vector3 Size
        {
            get
            {
                InternalCalls.BoxColliderComponent_GetSize(Entity.ID, out Vector3 halfSize);
                return halfSize;
            }

            set => InternalCalls.BoxColliderComponent_SetSize(Entity.ID, ref value);
        }

        public Vector3 Center
        {
            get
            {
                InternalCalls.BoxColliderComponent_GetCenter(Entity.ID, out Vector3 offset);
                return offset;
            }

			set => InternalCalls.BoxColliderComponent_SetCenter(Entity.ID, ref value);
        }

        public bool IsTrigger
        {
            get => InternalCalls.BoxColliderComponent_IsTrigger(Entity.ID);
            set => InternalCalls.BoxColliderComponent_SetTrigger(Entity.ID, value);
        }

        public PhysicsMaterial Material
        {
            get
            {
                return InternalCalls.BoxColliderComponent_GetPhysicsMaterialID(Entity.ID, out AssetID materialHandle)
                    ? new PhysicsMaterial(materialHandle) : null;
            }
        }
    }
    [RegisterCoreClassStruct]
    public class SphereColliderComponent : Component
    {

        public float Radius
        {
            get => InternalCalls.SphereColliderComponent_GetRadius(Entity.ID);
            set => InternalCalls.SphereColliderComponent_SetRadius(Entity.ID, value);
        }

        public Vector3 Center
        {
            get
            {
                InternalCalls.SphereColliderComponent_GetCenter(Entity.ID, out Vector3 offset);
                return offset;
            }

            set => InternalCalls.SphereColliderComponent_SetCenter(Entity.ID, ref value);
        }

        public bool IsTrigger
        {
            get => InternalCalls.SphereColliderComponent_IsTrigger(Entity.ID);
            set => InternalCalls.SphereColliderComponent_SetTrigger(Entity.ID, value);
        }

        public PhysicsMaterial Material
        {
            get => InternalCalls.SphereColliderComponent_GetPhysicsMaterialID(Entity.ID, out AssetID materialHandle)
                ? new PhysicsMaterial(materialHandle) : null;
        }
    }
    /*
    public class CapsuleColliderComponent : Component
    {

        public float Radius
        {
            get => InternalCalls.CapsuleColliderComponent_GetRadius(Entity.ID);
            set => InternalCalls.CapsuleColliderComponent_SetRadius(Entity.ID, value);
        }

        public float Height
        {
            get => InternalCalls.CapsuleColliderComponent_GetHeight(Entity.ID);
            set => InternalCalls.CapsuleColliderComponent_SetHeight(Entity.ID, value);
        }

        public Vector3 Offset
        {
            get
            {
                InternalCalls.CapsuleColliderComponent_GetOffset(Entity.ID, out Vector3 offset);
                return offset;
            }

            set => InternalCalls.CapsuleColliderComponent_SetOffset(Entity.ID, ref value);
        }

        public bool IsTrigger
        {
            get => InternalCalls.CapsuleColliderComponent_IsTrigger(Entity.ID);
            set => InternalCalls.CapsuleColliderComponent_SetTrigger(Entity.ID, value);
        }

        public PhysicsMaterial Material
        {
            get => InternalCalls.CapsuleColliderComponent_GetMaterialHandle(Entity.ID, out AssetID materialHandle)
                ? new PhysicsMaterial(materialHandle) : null;
        }
    }
	*/
    [RegisterCoreClassStruct]
    public class MeshColliderComponent : Component
    {
        public bool IsStaticMesh => InternalCalls.MeshColliderComponent_IsMeshStatic(Entity.ID);

        public AssetID ColliderMeshHandle
        {
            get => InternalCalls.MeshColliderComponent_GetColliderMesh(Entity.ID, out AssetID colliderHandle)
                ? colliderHandle : AssetID.Invalid;
        }

        public bool IsTrigger
        {
            get => InternalCalls.MeshColliderComponent_IsTrigger(Entity.ID);
            set => InternalCalls.MeshColliderComponent_SetTrigger(Entity.ID, value);
        }

        public PhysicsMaterial Material
        {
            get => InternalCalls.MeshColliderComponent_GetMaterialHandle(Entity.ID, out AssetID materialHandle)
                ? new PhysicsMaterial(materialHandle) : null;
        }

		private MeshBase m_ColliderMesh;

        public MeshBase GetColliderMesh()
        {
            if (!ColliderMeshHandle.IsValid())
                return null;

            if (m_ColliderMesh == null || m_ColliderMesh.ID != ColliderMeshHandle)
			{
				if(IsStaticMesh)
				{
					m_ColliderMesh = new StaticMesh(ColliderMeshHandle);
				}
				else
				{
                    m_ColliderMesh = new DynamicMesh(ColliderMeshHandle);
                }
            }

            return m_ColliderMesh;
        }

        
    }
}

