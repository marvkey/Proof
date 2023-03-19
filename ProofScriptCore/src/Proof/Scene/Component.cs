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

    }
	public enum ForceMode
	{
		Force,				
		Impule,			
		VelocityChange,	
		Acceleration
	};
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

		public void AddForce(Vector force, ForceMode mode = ForceMode.Force, bool autoWake = true)=>
			InternalCalls.RigidBody_AddForce(Entity.ID, force, (int)mode, autoWake);
		public void AddTorque(Vector force, ForceMode mode = ForceMode.Force, bool autoWake = true) => 
			InternalCalls.RigidBody_AddTorque(Entity.ID, force, (int)mode, autoWake);	

	}

}

