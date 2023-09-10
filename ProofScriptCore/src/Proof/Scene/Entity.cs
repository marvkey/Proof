using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [StructLayout(LayoutKind.Sequential)]
    public class Entity
	{
		protected Entity() { ID = 0; }

		internal Entity(ulong id)
		{
			ID = id;
		}
		public readonly ulong ID;
		public void ApplyCameraRotate()
		{
			InternalCalls.ApplyCameraRotate(ID);
        }
		public Entity[] GetChildren()
		{
            ulong[] list = null;
            InternalCalls.Entity_GetChildren(ID, ref list);

            if (list == null)
                return null;

            Entity[] entityList = new Entity[list.Length];
            int index = 0;
            foreach (ulong entityID in list)
            {
                Entity entity = new Entity(entityID);
                entityList[index] = entity;
                index++;
            }
            return entityList;
        }
		public bool HasComponent<T>() where T : Component, new()
        {
			Type componentType = typeof(T);
			return InternalCalls.Entity_HasComponent(ID, componentType);
		}

		public void SetAction(string ActionName, InputState state, Action func) 
		{
			if (!HasComponent<PlayerInputComponent>())
				return;
			if (func == null) return;
			InternalCalls.PlayerInputComponent_SetAction(ID, this.GetType().FullName, ActionName, (int)state, func.Method.Name);
		}

		public void SetInputState(PlayerInputState state)
		{
			if (!HasComponent<PlayerInputComponent>())
				return;
            InternalCalls.PlayerInputComponent_SetInputState(ID, (int)state);

        }
		public void SetMotion(string MotionName,Action<float> func)
		{
			if (!HasComponent<PlayerInputComponent>())
				return;
            if (func == null) return;

            InternalCalls.PlayerInputComponent_SetMotion(ID, this.GetType().FullName,MotionName, func.Method.Name);
		}
        public Entity GetOwner()
        {
			InternalCalls.Entity_GetParent(ID, out ulong owenrId);
			if (owenrId == 0)	
				return null;

			return new Entity(owenrId);
        }
        public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Entity = this };
			return component;
		}
        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.GetScriptInstance(ID, typeof(T).FullName);
            return instance as T;
        }
        public string Name
		{
			get
			{
				return GetComponent<TagComponent>().Tag;
			}
			set
			{
                InternalCalls.TagComponent_SetTag(ID, ref value);
            }
        }
	}
}
