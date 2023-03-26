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

		public bool HasComponent<T>() where T : Component, new()
        {
			Type componentType = typeof(T);
			return InternalCalls.Entity_HasComponent(ID, componentType);
		}

        public Entity GetOwner()
        {
			InternalCalls.Entity_GetOwner(ID, out ulong owenrId);
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
