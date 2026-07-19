using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static Proof.PlayerInputComponent;

namespace Proof
{
    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public class Entity
	{
		protected Entity() { ID = 0; }

		internal Entity(ulong id)
		{
			ID = id;

        }

        // NOTE: Implemented according to Microsofts official documentation:
        // https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/statements-expressions-operators/how-to-define-value-equality-for-a-type
        public override bool Equals(object obj) => obj is Entity other && Equals(other);

        // NOTE: Implemented according to Microsofts official documentation:
        // https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/statements-expressions-operators/how-to-define-value-equality-for-a-type
        public bool Equals(Entity other)
        {
            if (other is null)
                return false;

            if (ReferenceEquals(this, other))
                return true;

            return ID == other.ID;
        }
        public static bool IsValid(Entity entity)
        {
            if (entity is null)
                return false;

            return InternalCalls.World_IsEntityValid(entity.ID);
        }

        public override int GetHashCode() => (int)ID;

        public static bool operator ==(Entity entityA, Entity entityB) => entityA is null ? entityB is null : entityA.Equals(entityB);
        public static bool operator !=(Entity entityA, Entity entityB) => !(entityA == entityB);

        public static implicit operator bool(Entity entity) => IsValid(entity);


        public readonly ulong ID;

        public event Action<Entity> CollisionEnterEvent;
        public event Action<Entity> CollisionStayEvent;
        public event Action<Entity> CollisionLeaveEvent;

        public event Action<Entity> TriggerEnterEvent;
        public event Action<Entity> TriggerStayEvent;
        public event Action<Entity> TriggerLeaveEvent;

        private TransformComponent m_TransformComponent;

        public Transform WorldTransform
        {
            get
            {
                if (m_TransformComponent == null)
                    m_TransformComponent = GetComponent<TransformComponent>();

                return m_TransformComponent.WorldTransform;

            }
        }
        public TransferMatrix4 WorldTransformMatrix
        {
            get
            {
                if (m_TransformComponent == null)
                    m_TransformComponent = GetComponent<TransformComponent>();

                return m_TransformComponent.WorldTransformMatrix;

            }
        }
       
        public TransformComponent Transform
        {
            get
            {
                if (m_TransformComponent == null)
                    m_TransformComponent = GetComponent<TransformComponent>();

                return m_TransformComponent;
            }
        }

        public Proof.Vector3 Location
        {
            get => Transform.Location;
            set => Transform.Location = value;
        }

        public Proof.Vector3 Rotation
        {
            get => Transform.Rotation;
            set => Transform.Rotation = value;
        }

        public Proof.Vector3 Scale
        {
            get => Transform.Scale;
            set => Transform.Scale = value;
        }

        public Proof.Quaternion RotationQuat
        {
            get => Transform.RotationQuat;
            set => Transform.RotationQuat = value;
        }
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

        public void AddChild(Entity e)
        {
            InternalCalls.Entity_AddChild(ID, e.ID);    
        }


        public bool HasComponent<T>() where T : Component, new()
        {
			Type componentType = typeof(T);
			return InternalCalls.Entity_HasComponent(ID, componentType);
		}

        public T AddComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);

            if(InternalCalls.Entity_AddComponent(ID, componentType) == true)
                return GetComponent<T>();

            return null;
        }
       

        public bool RemoveComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_RemoveComponent(ID, componentType);
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
        public Entity GetParent()
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

        public T GetComponentInHierarchy<T>() where T : Component, new ()
        {
            var component = GetComponent<T>();
            if (component != null)
                return component;

            var children = GetChildren();
            if (children == null)
                return null;

            foreach (var child in children)
            {
                if (child == null)
                    continue;

                var found = child.GetComponentInHierarchy<T>();
                if (found != null)
                    return found;
            }

            return null;
        }
        public T GetScript<T>() where T : Entity, new()
        {
            object instance = InternalCalls.GetScriptInstance(ID, typeof(T).FullName);
            if(instance == null)
                return null;
            return instance as T;
        }

        public T AddScript<T>() where T : Entity, new()
        {
            object instance = InternalCalls.AddScriptInstance(ID, typeof(T).FullName);
            if(instance == null)
                return null;
            return instance as T;
        }

        public bool HasScript<T>() where T : Entity, new()
        {
            return GetScript<T>() != null;
        }

        public T GetScriptInChildren<T>() where T : Entity, new()
        {
            // Check if this entity has the script
            if (HasScript<T>())
                return GetScript<T>();

            // Get all direct children
            Entity[] children = GetChildren();

            // Recursively check children and their descendants
            foreach (var child in children)
            {
                if (child.HasScript<T>())
                    return child.GetScript<T>();

                // Recurse deeper into the hierarchy
                var found = child.GetScriptInChildren<T>();
                if (found != null)
                    return found;
            }

            // None found
            return null;
        }
        
        public T GetScriptInstance<T>() where T : Entity, new()
        {
            object instance = InternalCalls.GetScriptInstanceOfType(ID, typeof(T).FullName);
            if (instance == null)
                return null;
            return instance as T;
        }
        
        public T GetScriptInstanceInChildren<T>() where T : Entity, new()
        {
            // Check this entity first
            if (HasScriptInstance<T>())
                return GetScriptInstance<T>();

            // Get direct children
            Entity[] children = GetChildren();

            // Recursively check children
            foreach (var child in children)
            {
                if (child.HasScriptInstance<T>())
                    return child.GetScriptInstance<T>();

                // Recurse deeper
                var found = child.GetScriptInstanceInChildren<T>();
                if (found != null)
                    return found;
            }

            // None found in this branch
            return null;
        }

        public bool HasScriptInstance<T>() where T : Entity, new()
        {
            return GetScriptInstance<T>() != null;
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


        public bool HasSubTag(string name)
        {
            return GetComponent<TagComponent>().HasSubTag(name);
        }

        public void Destroy(bool includeChildren = true)
        {
            World.DeleteEntity(this,includeChildren);   
        }

        protected virtual void OnCreate() { }
        protected virtual void OnUpdate(float ts) { }
        protected virtual void OnPostUpdate(float ts) { }
        protected virtual void OnPhysicsUpdate(float fGYuxedPhysicsDeltaTime) { }

        protected virtual void OnDestroy() { }

        private void OnCollisionEnterInternal(ulong id) => CollisionEnterEvent?.Invoke(new Entity(id));
        private void OnCollisionStayInternal(ulong id) => CollisionStayEvent?.Invoke(new Entity(id));
        private void OnCollisionLeaveInternal(ulong id) => CollisionLeaveEvent?.Invoke(new Entity(id));

        private void OnTriggerEnterInternal(ulong id) => TriggerEnterEvent?.Invoke(new Entity(id));
        private void OnTriggerStayInternal(ulong id) => TriggerStayEvent?.Invoke(new Entity(id));
        private void OnTriggerLeaveInternal(ulong id) => TriggerLeaveEvent?.Invoke(new Entity(id));

        public static void BindInputAction(PlayerInputComponent input, InputAction action, InteractionEvent evt, VoidActionWithInput callback)
        {
            if(action != null)
                input.BindAction(action, evt, callback);
        }

        public void Invoke(Action callback, float time)
        {
            string className = this.GetType().FullName; // e.g., "CharacterMovement"
            string methodName = callback.Method.Name;

            InternalCalls.World_Invoke(this.ID, className, methodName, time, false, 0.0f);
        }

        public void InvokeRepeat(Action callback, float time, float repeatTime)
        {
            string className = this.GetType().FullName;
            string methodName = callback.Method.Name;

            InternalCalls.World_Invoke(this.ID, className, methodName, time, true, repeatTime);
        }

        public void InvokeRepeat(Action callback, float time)
        {
            string className = this.GetType().FullName;
            string methodName = callback.Method.Name;

            InternalCalls.World_Invoke(this.ID, className, methodName, time, true, time);
        }
    }
}
