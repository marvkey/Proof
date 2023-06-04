﻿using ProofScriptCore.src.Proof.Asset;
using ProofScriptCore.src.Proof.Math;
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
	public class TransformComponent : Component
	{
		public Vector Location
		{
			get
			{
				InternalCalls.TransformComponent_GetLocation(Entity.ID, out Vector loc);
				return loc;
			}
			set
			{
				InternalCalls.TransformComponent_SetLocation(Entity.ID, ref value);
			}
		}

		public Vector Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector rotation);
				return rotation;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
			}
		}

		public Vector Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
			}
		}
        public Vector GetFowardVector()
        {
            InternalCalls.TransformComponent_GetFowardVector(Entity.ID, out Vector foward);
            return foward;
        }

		public void Translate(Vector vec)
		{
			Location += vec;
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
		Impule,			
		VelocityChange,	
		Acceleration
	};
	public class PlayerInputComponent : Component
	{
		
	}

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
    public class ParticleSystemComponent : Component
	{
       

		public bool HasParticleIndex(uint index)
		{
			return InternalCalls.ParticleSystemComponent_HasParticleIndex(Entity.ID, index);
		}

		public bool ParticleIndexHasParticle(uint index)
		{
			return	InternalCalls.ParticleSystemComponent_ParticleIndexHasParticle(Entity.ID, index);
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
			foreach(uint partIndex  in list)
			{
				index++;
                if (ParticleIndexHasParticle(partIndex) == false) continue;

                ParticleSystem ps = new ParticleSystem(partIndex,Entity);
                particleSystems[index] = ps;

            }
			return particleSystems;
        }
    }
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
		public void AddForce(Vector force, ForceMode mode = ForceMode.Force, bool autoWake = true)=>
			InternalCalls.RigidBody_AddForce(Entity.ID, force, (int)mode, autoWake);
		public void AddTorque(Vector force, ForceMode mode = ForceMode.Force, bool autoWake = true) => 
			InternalCalls.RigidBody_AddTorque(Entity.ID, force, (int)mode, autoWake);	


		public void ClearForce(ForceMode mode = ForceMode.Force)
		{
			InternalCalls.RigidBody_ClearForce(Entity.ID, (int)mode);
		}

        public void ClearTorque(ForceMode mode = ForceMode.Force)
        {
            InternalCalls.RigidBody_ClearTorque(Entity.ID, (int)mode);
        }


		public Vector GetLinearVelocity()
		{
			InternalCalls.RigidBody_GetLinearVelocity(Entity.ID, out Vector velocity);
			return velocity;
		}

		public void SetLinearVelocity(Vector velocity, bool wakeUp = true)
		{
			InternalCalls.RigidBody_SetLinearVelocity(Entity.ID, ref velocity, wakeUp);
		}

        public Vector GetAngularVelocity()
        {
            InternalCalls.RigidBody_GetAngularVelocity(Entity.ID, out Vector velocity);
            return velocity;
        }

        public void SetAngularVelocity(Vector velocity, bool wakeUp = true)
        {
            InternalCalls.RigidBody_SetAngularVelocity(Entity.ID, ref velocity, wakeUp);
        }
    }

}

