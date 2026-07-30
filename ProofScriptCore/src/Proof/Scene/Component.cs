using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
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

		public bool HasSubTag(string tag)
		{
			return InternalCalls.TagComponent_HasSubTag(Entity.ID, out tag);
        }
    }

    [RegisterCoreClassStruct]
    public class TransformComponent : Component
	{
		public Proof.Vector3 Location
		{
			get
			{
				InternalCalls.TransformComponent_GetLocation(Entity.ID, out Proof.Vector3 loc);
				return loc;
			}
			set
			{
				InternalCalls.TransformComponent_SetLocation(Entity.ID, ref value);
			}
		}

		public Proof.Vector3 Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(Entity.ID, out Proof.Vector3 rotation);
				return rotation;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
			}
		}

        public Proof.Quaternion RotationQuat
        {
            get
            {
                InternalCalls.TransformComponent_GetRotationQuat(Entity.ID, out Proof.Quaternion rotation);
                return rotation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotationQuat(Entity.ID, ref value);
            }
        }

        public Proof.Vector3 Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(Entity.ID, out Proof.Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
			}
		}

		// local space
		public Proof.Vector3 Forward
		{
			get
			{

                InternalCalls.TransformComponent_GetForwardVector(Entity.ID, out Proof.Vector3 foward);
                return foward;
            }
			set
			{
				InternalCalls.TransformComponent_SetForwardVector(Entity.ID, ref value);
			}
		}
        // local space
        public Proof.Vector3 Right
        {
            get
            {

                InternalCalls.TransformComponent_GetRightVector(Entity.ID, out Proof.Vector3 foward);
                return foward;
            }
            set
            {
				InternalCalls.TransformComponent_SetRightVector(Entity.ID, ref value);
            }
        }
        // local space
        public Proof.Vector3 Up
        {
            get
            {

                InternalCalls.TransformComponent_GetUpVector(Entity.ID, out Proof.Vector3 foward);
                return foward;
            }
            set
            {
                InternalCalls.TransformComponent_SetUpVector(Entity.ID, ref value);
            }
        }

		// Todo move to c++ side and recheck teh definiton
        public Vector3 TransformDirection(Vector3 localDir)
        {
            Quaternion rotateQuat = Quaternion.EularToQuat(glm.Radians(WorldTransform.Rotation));
            Vector3 rotated = rotateQuat * localDir;
            rotated *= WorldTransform.Scale;
            return rotated;
        }

        public void Translate(Proof.Vector3 Vector)
		{
			Location += Vector;
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

        public void Rotate(Proof.Vector3 eulers)
        {
            // Get the current rotation
            Proof.Vector3 currentRotation = Rotation;

            currentRotation += eulers;
            // Update the rotation
            Rotation = currentRotation;
        }


        public TransferMatrix4 WorldTransformMatrix
        {
	        get
	        {
		        InternalCalls.TransformComponent_GetWorldSpaceTransformMatrix(Entity.ID, out TransferMatrix4 matrix);
				return matrix;
	        }
        }


        public TransferMatrix4 TransformMatrix
        {
	        get
	        {
		        InternalCalls.TransformComponent_GetTransformMatrix(Entity.ID, out TransferMatrix4 matrix);
				return matrix;
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

		public bool Visible
		{
			get
			{
                return InternalCalls.TextComponent_GetVisible(Entity.ID);
            }
			set
			{
                InternalCalls.TextComponent_SetVisible(Entity.ID,value);

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

    [StructLayout(LayoutKind.Sequential)]
    public struct PlayerInputBindingContextInstanceRaw
    {
        public ulong ID; //InputBindingContextID
        public uint Priority;
        public bool Active;
    };

    [RegisterCoreClassStruct]
    public class PlayerInputComponent : Component
	{
        public delegate void VoidActionWithInput(InputActionOutput input);

        public void BindAction(InputAction action, InteractionEvent interactionEvent, VoidActionWithInput callback)
        {
            if (callback == null) return;
            InternalCalls.PlayerInputComponent_BindAction(Entity.ID, action.ID, interactionEvent, callback.Target, callback.Method.Name);
        }

		
		public InputBindingData GetBindingData(InputBindingContext BindingContext)
		{
            PlayerInputBindingContextInstanceRaw rawData = InternalCalls.PlayerInputComponent_GetInputBindingContextInstance(this.Entity.ID, BindingContext.ID);

            return new InputBindingData(this, rawData.Priority,rawData.Active, new AssetID(rawData.ID));
        }

		void AddInputBinding(InputBindingContext bindingContext)
		{
			InternalCalls.PlayerInputComponent_AddInputBinding(Entity.ID, bindingContext.ID);
        }

        void AddInputBindingPriority(InputBindingContext bindingContext,uint priority)
        {
            InternalCalls.PlayerInputComponent_AddInputBindingByPriority(Entity.ID, bindingContext.ID,priority);
        }

		void RemoveInputBinding(InputBindingContext bindingContext)
		{
			InternalCalls.PlayerInputComponent_RemoveInputBindingContext(Entity.ID, bindingContext.ID);
		}

		void RemoveInputBindingByPriority(uint priority)
		{
            InternalCalls.PlayerInputComponent_RemoveInputBindingContextByPriority(Entity.ID, priority);

        }
    }

	public struct InputBindingData
	{
		public InputBindingData(PlayerInputComponent input, uint priority, bool active, AssetID id)
		{
			m_PlayerInput = input;
			BindingContext = new InputBindingContext(id);
		}

		public InputBindingContext BindingContext;
		public int Priority
		{
			get
			{
				return InternalCalls.PlayerInputComponent_GetInputBindingContextPriority(m_PlayerInput.Entity.ID,BindingContext.ID);
			}
		}
		public bool Active
		{

            get
            {
                return InternalCalls.PlayerInputComponent_GetInputBindingContextInstance(m_PlayerInput.Entity.ID,BindingContext.ID).Active;
            }

            set
            {
				InternalCalls.PlayerInputComponent_SetInputBindingContextActive(m_PlayerInput.Entity.ID, BindingContext.ID, value);
            }
        }

        private PlayerInputComponent m_PlayerInput;
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

		public StaticMesh Mesh
		{
			get
			{
				return new StaticMesh( InternalCalls.MeshComponent_GetMesh(Entity.ID));
			}

			set
			{
				InternalCalls.MeshComponent_SetMesh(Entity.ID,value.ID ,true);
			}
		}
	}

    [RegisterCoreClassStruct]

	public class DynamicMeshComponent : Component
	{
		public bool Visible
		{
			get
			{
				return InternalCalls.DynamicMeshComponent_GetVisible(Entity.ID);
			}
			set
			{
				InternalCalls.DynamicMeshComponent_SetVisible(Entity.ID, value);
			}
		}

		public uint SubMeshIndex
		{
			get
			{
				return InternalCalls.DynamicMeshComponent_GetSubMeshIndex(Entity.ID);
			}
			set
			{
				InternalCalls.DynamicMeshComponent_SetSubMeshIndex(Entity.ID, value);
			}
		}

		public DynamicMesh Mesh
		{
			get
			{
				return new DynamicMesh( InternalCalls.DynamicMeshComponent_GetMesh(Entity.ID));
			}

			set
			{
				InternalCalls.DynamicMeshComponent_SetMesh(Entity.ID,value.ID ,true);
			}
		}
	}

	[RegisterCoreClassStruct]
	public class DirectionalLightComponent : Component
	{
		public Vector3 Color
		{
			get
			{
				return InternalCalls.DirectionalLightComponent_GetColor(Entity.ID);
			}
			set
			{
				InternalCalls.DirectionalLightComponent_SetColor(Entity.ID, value);
			}
		}

		public float Intensity
		{
			get
			{
				return InternalCalls.DirectionalLightComponent_GetIntensity(Entity.ID);
			}
			set
			{
				InternalCalls.DirectionalLightComponent_SetIntensity(Entity.ID, value);
			}
		}

		public bool CastShadow
		{
			get
			{
				return InternalCalls.DirectionalLightComponent_GetCastShadow(Entity.ID);
			}
			set
			{
				InternalCalls.DirectionalLightComponent_SetCastShadow(Entity.ID, value);
			}
		}

		public float ShadowStrength
		{
			get
			{
				return InternalCalls.DirectionalLightComponent_GetShadowStrength(Entity.ID);
			}
			set
			{
				InternalCalls.DirectionalLightComponent_SetShadowStrength(Entity.ID, value);
			}
		}

		public bool CastSoftShadow
		{
			get
			{
				return InternalCalls.DirectionalLightComponent_GetCastSoftShadow(Entity.ID);
			}
			set
			{
				InternalCalls.DirectionalLightComponent_SetCastSoftShadow(Entity.ID, value);
			}
		}

		public float ShadowSoftness
		{
			get
			{
				return InternalCalls.DirectionalLightComponent_GetShadowSoftness(Entity.ID);
			}
			set
			{
				InternalCalls.DirectionalLightComponent_SetShadowSoftness(Entity.ID, value);
			}
		}
	}
	[RegisterCoreClassStruct]
	public class PointLightComponent : Component
	{
		public Vector3 Color
		{
			get
			{
				return InternalCalls.PointLightComponent_GetColor(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetColor(Entity.ID, value);
			}
		}

		public float Intensity
		{
			get
			{
				return InternalCalls.PointLightComponent_GetIntensity(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetIntensity(Entity.ID, value);
			}
		}

		public float MinRadius
		{
			get
			{
				return InternalCalls.PointLightComponent_GetMinRadius(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetMinRadius(Entity.ID, value);
			}
		}

		public float Radius
		{
			get
			{
				return InternalCalls.PointLightComponent_GetRadius(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetRadius(Entity.ID, value);
			}
		}

		public float Falloff
		{
			get
			{
				return InternalCalls.PointLightComponent_GetFalloff(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetFalloff(Entity.ID, value);
			}
		}

		public bool CastsShadows
		{
			get
			{
				return InternalCalls.PointLightComponent_GetCastsShadows(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetCastsShadows(Entity.ID, value);
			}
		}

		public bool SoftShadows
		{
			get
			{
				return InternalCalls.PointLightComponent_GetSoftShadows(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetSoftShadows(Entity.ID, value);
			}
		}

		public float ShadowStrength
		{
			get
			{
				return InternalCalls.PointLightComponent_GetShadowStrength(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetShadowStrength(Entity.ID, value);
			}
		}

		public float ShadowSoftness
		{
			get
			{
				return InternalCalls.PointLightComponent_GetShadowSoftness(Entity.ID);
			}
			set
			{
				InternalCalls.PointLightComponent_SetShadowSoftness(Entity.ID, value);
			}
		}
	}

	[RegisterCoreClassStruct]
	public class SpotLightComponent : Component
	{
		public Vector3 Color
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetColor(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetColor(Entity.ID, value);
			}
		}

		public float Intensity
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetIntensity(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetIntensity(Entity.ID, value);
			}
		}

		public float Range
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetRange(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetRange(Entity.ID, value);
			}
		}

		public float Angle
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetAngle(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetAngle(Entity.ID, value);
			}
		}

		public float AngleAttenuation
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetAngleAttenuation(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetAngleAttenuation(Entity.ID, value);
			}
		}

		public float Falloff
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetFalloff(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetFalloff(Entity.ID, value);
			}
		}

		public bool CastsShadows
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetCastsShadows(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetCastsShadows(Entity.ID, value);
			}
		}

		public bool SoftShadows
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetSoftShadows(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetSoftShadows(Entity.ID, value);
			}
		}

		public float ShadowStrength
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetShadowStrength(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetShadowStrength(Entity.ID, value);
			}
		}

		public float ShadowSoftness
		{
			get
			{
				return InternalCalls.SpotLightComponent_GetShadowSoftness(Entity.ID);
			}
			set
			{
				InternalCalls.SpotLightComponent_SetShadowSoftness(Entity.ID, value);
			}
		}
	}
    [RegisterCoreClassStruct]
	public class PlayerHUDComponent : Component
	{
        public UILayer Getlayer(string name)
		{
            return InternalCalls.PlayerHUDComponent_UITableGetLayerByName(Entity.ID, name);
        }

        public UILayer GetLayer(uint index)
        {
            return InternalCalls.PlayerHUDComponent_UITableGetLayer(Entity.ID, index);
        }

        public UIPanelInstance GetPanelInstance(uint layerIndex, UIPanel panel)
        {
            InternalCalls.PlayerHUDComponent_UITableLayerGetPanelInstance(Entity.ID, layerIndex, panel.ID, out UIPanelInstance instance);

            return null;
        }

        public UIPanelInstance GetPanelInstanceByIndex(uint layerIndex, uint panelIndex)
		{
            InternalCalls.PlayerHUDComponent_UITableLayerGetPanelInstanceByIndex(Entity.ID, layerIndex, panelIndex, out UIPanelInstanceRaw instance);
			return null;
        }

        public void SetPanelVisible(uint layerIndex, UIPanel panel, bool visible)
        {
            InternalCalls.PlayerHUDComponent_UITableLayerSetPanelInstanceVisible(Entity.ID, layerIndex, panel.ID, visible);
        }

        public bool GetPanelVisible(uint layerIndex, UIPanel panel)
        {
            return InternalCalls.PlayerHUDComponent_UITableLayerGetPanelInstanceVisible(Entity.ID, layerIndex, panel.ID);
        }

        public void PushPanel(string layerName, UIPanel panel, bool visible)
		{
            InternalCalls.PlayerHUDComponent_UITableLayerPushPanelByName(Entity.ID, layerName, panel.ID, visible);
        }

        public void PushPanel(uint layerIndex, UIPanel panel, bool visible)
        {
            InternalCalls.PlayerHUDComponent_UITableLayerPushPanel(Entity.ID, layerIndex, panel.ID, visible);
        }

        public void RemovePanel(uint layerIndex, UIPanel panel)
        {
            InternalCalls.PlayerHUDComponent_UITableLayerRemovePanel(Entity.ID, layerIndex, panel.ID);
        }

        public void RemovePanel(string layerName, UIPanel panel)
		{
            InternalCalls.PlayerHUDComponent_UITableLayerRemovePanelByName(Entity.ID, layerName, panel.ID);
        }

        public Variable GetRegistryVariable(uint layerIndex, UIPanel panel, string varName)
        {
			InternalCalls.PlayerHUDComponent_UITableLayerPanelInstanceGetRegistryVariable(Entity.ID, layerIndex, panel.ID, varName,out VariableRaw var);

			if(!panel.ID.IsValid())return null;
            if (var.VariableUUID != 0)
            {
                return new Variable(var.VariableUUID, (VariableTypes)var.Type, var.StorageHandle);
            }
            return null;
        }

        public Variable GetRegistryVariable(string layerName, UIPanel panel, string varName)
		{
            InternalCalls.PlayerHUDComponent_UITableLayerPanelInstanceGetRegistryVariableByName(Entity.ID, layerName, panel.ID, varName, out VariableRaw var);
            if (var.VariableUUID != 0)
            {
                return new Variable(var.VariableUUID, (VariableTypes)var.Type, var.StorageHandle);
            }

            return null;
        }
        public Variable GetRegistryVariableByPanelIndex(uint layerIndex, uint panelIndex, string varName)
        {
            InternalCalls.PlayerHUDComponent_UITableLayerGetPanelInstanceByIndex(Entity.ID, layerIndex, panelIndex, out UIPanelInstanceRaw panel);
			AssetID id = new AssetID(panel.AssetPanelID);
            if (!id.IsValid())
            {
				return null;
            }

            InternalCalls.PlayerHUDComponent_UITableLayerPanelInstanceGetRegistryVariable(Entity.ID, layerIndex, id, varName, out VariableRaw var);
			if(var.VariableUUID != 0)
			{
                return new Variable(var.VariableUUID, (VariableTypes)var.Type, var.StorageHandle);
            }

			return null;
        }

		
    }

    [RegisterCoreClassStruct]
    public class ParticleEffectComponent : Component
	{


		public bool HasParticleIndex(uint index)
		{
			return InternalCalls.ParticleEffectComponent_HasParticleIndex(Entity.ID, index);
		}

		public bool ParticleIndexHasParticle(uint index)
		{
			return InternalCalls.ParticleEffectComponent_ParticleIndexHasParticle(Entity.ID, index);
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
			InternalCalls.ParticleEffectComponent_GetParticles(Entity.ID, ref list);

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
		public void AddForce(Proof.Vector3 force, ForceMode mode = ForceMode.Force, bool autoWake = true) =>
			InternalCalls.RigidBody_AddForce(Entity.ID, force, (int)mode, autoWake);
		public void AddTorque(Proof.Vector3 force, ForceMode mode = ForceMode.Force, bool autoWake = true) =>
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

		public Proof.Vector3 Location
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetLocation(Entity.ID, out Proof.Vector3 translation);
				return translation;
			}

			set => InternalCalls.RigidBodyComponent_SetLocation(Entity.ID, ref value);
		}

		//Radians
		public Proof.Quaternion Rotation
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetRotation(Entity.ID, out Quaternion rotationRadians);
				return rotationRadians;
			}

			set => InternalCalls.RigidBodyComponent_SetRotation(Entity.ID, ref value);
		}


		public Proof.Vector3 LinearVelocity
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetLinearVelocity(Entity.ID, out Proof.Vector3 velocity);
				return velocity;
			}

			set => InternalCalls.RigidBodyComponent_SetLinearVelocity(Entity.ID, ref value);
		}

		public Proof.Vector3 Velocity
		{
			get
			{
				return LinearVelocity;
			}
			set
			{
				LinearVelocity = value;
			}
		}
		public Proof.Vector3 AngularVelocity
		{
			get
			{
				InternalCalls.RigidBodyComponent_GetAngularVelocity(Entity.ID, out Proof.Vector3 velocity);
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

		public void GetKineMatrixicTarget(out Proof.Vector3 targetPosition, out Proof.Vector3 targetRotation)
			=> InternalCalls.RigidBodyComponent_GetKineMatrixicTarget(Entity.ID, out targetPosition, out targetRotation);

		public void SetKineMatrixicTarget(Proof.Vector3 targetPosition, Proof.Vector3 targetRotation)
			=> InternalCalls.RigidBodyComponent_SetKineMatrixicTarget(Entity.ID, ref targetPosition, ref targetRotation);


		public void AddForceAtLocation(Proof.Vector3 force, Proof.Vector3 location, ForceMode forceMode = ForceMode.Force)
		{
			InternalCalls.RigidBodyComponent_AddForceAtLocation(Entity.ID, ref force, ref location, forceMode);
		}

		public void Translate(Proof.Vector3 translation)   
		{
			InternalCalls.RigidBodyComponent_Translate(Entity.ID, ref translation);
		}

        public void Rotate(Proof.Quaternion quat)
        {
            InternalCalls.RigidBodyComponent_Rotate(Entity.ID, ref quat);
        }
    }
    [RegisterCoreClassStruct]
    public class BoxColliderComponent : Component
    {
        public Proof.Vector3 Size
        {
            get
            {
                InternalCalls.BoxColliderComponent_GetSize(Entity.ID, out Proof.Vector3 halfSize);
                return halfSize;
            }

            set => InternalCalls.BoxColliderComponent_SetSize(Entity.ID, ref value);
        }

        public Proof.Vector3 Center
        {
            get
            {
                InternalCalls.BoxColliderComponent_GetCenter(Entity.ID, out Proof.Vector3 offset);
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
                return InternalCalls.BoxColliderComponent_GetPhysicsMaterialID(Entity.ID, out AssetID MatrixerialHandle)
                    ? new PhysicsMaterial(MatrixerialHandle) : null;
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

        public Proof.Vector3 Center
        {
            get
            {
                InternalCalls.SphereColliderComponent_GetCenter(Entity.ID, out Proof.Vector3 offset);
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
            get => InternalCalls.SphereColliderComponent_GetPhysicsMaterialID(Entity.ID, out AssetID MatrixerialHandle)
                ? new PhysicsMaterial(MatrixerialHandle) : null;
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

        public Proof.Vector3 Offset
        {
            get
            {
                InternalCalls.CapsuleColliderComponent_GetOffset(Entity.ID, out Proof.Vector3 offset);
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
            get => InternalCalls.CapsuleColliderComponent_GetMaterialHandle(Entity.ID, out AssetID MatrixerialHandle)
                ? new PhysicsMaterial(MatrixerialHandle) : null;
        }
    }
	*/

    [RegisterCoreClassStruct]
	public class CameraComponent : Component
	{
		public bool UseLocalRotation
		{
			get => InternalCalls.CameraComponent_GetLocalRotation(Entity.ID);
			set => InternalCalls.CameraComponent_SetLocalRotation(Entity.ID,value);
        }
		public bool ActiveForRendering
		{
			get => InternalCalls.CameraComponent_GetActiveForRendering(Entity.ID);	
			set => InternalCalls.CameraComponent_SetActiveForRendering(Entity.ID, value);
		}
    }
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
            get => InternalCalls.MeshColliderComponent_GetMaterialHandle(Entity.ID, out AssetID MatrixerialHandle)
                ? new PhysicsMaterial(MatrixerialHandle) : null;
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

    [StructLayout(LayoutKind.Sequential)]
    struct BoidFlockData
    {
	    // --- Core Movement ---
	    public float MinSpeed;            // Prevents stalling
	    public float MaxSpeed;            // Max chase/orbit speed
	    public float MaxSteerForce;       // How sharp turns can be

	    // --- Flocking Behavior ---
	    public float PerceptionRadius;    // Sensing range for nearby boids
	    public float AvoidanceRadius;     // Minimum distance between boids

	    public float AlignWeight;         // Strength of alignment (same direction)
	    public float CohesionWeight;      // Pull toward flock center
	    public float SeperateWeight;      // Push away from close neighbors

	    // --- Target / Following ---
	    public float TargetWeight;        // Pull toward target
	    public ulong TargetEntity;      // The entity being followed/orbited

	    // --- Collision Avoidance ---
	    public uint LayerID;
	    public float BoundsRadius;
	    public float AvoidCollisionWeight;
	    public float CollisionAvoidDst;

	    // --- Orbiting Behavior ---
	    public bool Orbit;               // Enable orbiting mode
	    public float OrbitRadius;         // Distance to maintain around target
	    public float OrbitWandering;      // How much randomness/noise in orbit path
    }
    [RegisterCoreClassStruct]
    public class BoidFlockComponent : Component
    {
	     private BoidFlockData m_Data; // persistent cached struct

        private void SyncFromNative() => InternalCalls.BoidFlockComponent_GetSettings(Entity.ID, out m_Data);
        private void SyncToNative() => InternalCalls.BoidFlockComponent_SetSettings(Entity.ID, ref m_Data);

        // ============ All Properties ============

        public float MinSpeed
        {
            get { SyncFromNative(); return m_Data.MinSpeed; }
            set { SyncFromNative(); m_Data.MinSpeed = value; SyncToNative(); }
        }

        public float MaxSpeed
        {
            get { SyncFromNative(); return m_Data.MaxSpeed; }
            set { SyncFromNative(); m_Data.MaxSpeed = value; SyncToNative(); }
        }

        public float PerceptionRadius
        {
            get { SyncFromNative(); return m_Data.PerceptionRadius; }
            set { SyncFromNative(); m_Data.PerceptionRadius = value; SyncToNative(); }
        }

        public float AvoidanceRadius
        {
            get { SyncFromNative(); return m_Data.AvoidanceRadius; }
            set { SyncFromNative(); m_Data.AvoidanceRadius = value; SyncToNative(); }
        }

        public float MaxSteerForce
        {
            get { SyncFromNative(); return m_Data.MaxSteerForce; }
            set { SyncFromNative(); m_Data.MaxSteerForce = value; SyncToNative(); }
        }

        public float AlignWeight
        {
            get { SyncFromNative(); return m_Data.AlignWeight; }
            set { SyncFromNative(); m_Data.AlignWeight = value; SyncToNative(); }
        }

        public float CohesionWeight
        {
            get { SyncFromNative(); return m_Data.CohesionWeight; }
            set { SyncFromNative(); m_Data.CohesionWeight = value; SyncToNative(); }
        }

        public float SeperateWeight
        {
            get { SyncFromNative(); return m_Data.SeperateWeight; }
            set { SyncFromNative(); m_Data.SeperateWeight = value; SyncToNative(); }
        }

        public float TargetWeight
        {
            get { SyncFromNative(); return m_Data.TargetWeight; }
            set { SyncFromNative(); m_Data.TargetWeight = value; SyncToNative(); }
        }

        public uint LayerID
        {
            get { SyncFromNative(); return m_Data.LayerID; }
            set { SyncFromNative(); m_Data.LayerID = value; SyncToNative(); }
        }

        public float BoundsRadius
        {
            get { SyncFromNative(); return m_Data.BoundsRadius; }
            set { SyncFromNative(); m_Data.BoundsRadius = value; SyncToNative(); }
        }

        public float AvoidCollisionWeight
        {
            get { SyncFromNative(); return m_Data.AvoidCollisionWeight; }
            set { SyncFromNative(); m_Data.AvoidCollisionWeight = value; SyncToNative(); }
        }

        public float CollisionAvoidDst
        {
            get { SyncFromNative(); return m_Data.CollisionAvoidDst; }
            set { SyncFromNative(); m_Data.CollisionAvoidDst = value; SyncToNative(); }
        }

        public Entity Target
        {
	        get
	        {
		        SyncFromNative();
		        Entity target = World.FindEntityByID(m_Data.TargetEntity);
		        return target;
	        }
            set { SyncFromNative(); m_Data.TargetEntity = value.ID; SyncToNative(); }
        }

        // ============ Boid List Management ============

        public void AddBoid(Entity entity)
        {
	        if(entity != null)
				InternalCalls.BoidFlockComponent_AddBoid(Entity.ID, entity.ID);
		        
        }

        public void RemoveBoid(Entity entity)
        {
	        if(entity != null)
		        InternalCalls.BoidFlockComponent_RemoveBoid(Entity.ID, entity.ID);
        }
    }
    
}

