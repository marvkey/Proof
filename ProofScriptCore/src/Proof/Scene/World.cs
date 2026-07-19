using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    public static class World
    {
        public static void Play()
        {
            InternalCalls.World_Play();
        }
        public static void Pause()
        {
            InternalCalls.World_Pause();
        }
        public static bool OpenWorld(ulong id)
        {
            return InternalCalls.World_OpenWorld(id);
        }

        public static Entity FindEntityByID(ulong entityID)
        {
            if (!InternalCalls.World_IsEntityValid(entityID))
                return null;

            return new Entity(entityID);
        }
        public static Entity Instantiate(Prefab prefab, Transform transform)
        {
            ulong entityID = InternalCalls.World_Instanciate(prefab.ID.ToUInt64(), transform);

            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }
        public static Entity Instantiate(Prefab prefab, Proof.Vector3 location)
        {
            Transform transform;
            transform.Location = location;
            transform.Rotation = new Proof.Vector3(0.0f);
            transform.Scale = new Proof.Vector3(1.0f);
            ulong entityID = InternalCalls.World_Instanciate(prefab.ID.ToUInt64(), transform);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }
        public static Entity Instantiate(Prefab prefab, Proof.Vector3 location, Entity parent)
        {
            if (parent == null)
                return null;

            Entity InstantiatedEntity = Instantiate(prefab, new Vector3(0));
            parent.AddChild(InstantiatedEntity);

            InstantiatedEntity.Location = location;
            return InstantiatedEntity;
        }
        public static Entity TryFindEntityByTag(string tag)
        {
            ulong id = InternalCalls.World_TryFindEntityByTag(tag);
            if (id == 0)
                return null;

            return new Entity(id);
        }


        public static Entity CreateEntity(string name,Transform transform)
        {
            return new Entity(InternalCalls.World_CreateEntity(name,transform));
        }

        public static Entity CreateEntity(string name)
        {
            return new Entity(InternalCalls.World_CreateEntity(name,new Transform()));
        }

        public static Entity CreateEntity(string name,Entity parent)
        {
            if(Entity.IsValid(parent))
            {
                Entity e =  CreateEntity(name, Transform.Zero);
                parent.AddChild(e);

                return e;
            }

            return null;
        }

        public static Entity CreateEntity(Entity ent,bool includeChildren = true)
        {
            if(Entity.IsValid(ent) == false)
                return null;

            return new Entity( InternalCalls.World_CreateEntityFromEntity(ent.ID,includeChildren ));
        }


        public static void DeleteEntity(Entity entity, bool deleteChildren = true,float time = 0.0f)
        {
            InternalCalls.World_DeleteEntity(entity.ID, deleteChildren,time);
        }

        public static float GetDeltaTime()
        {
            return InternalCalls.World_GetDeltaTime();
        }
        public static void Restart()
        {
            InternalCalls.World_Restart();
        }

    
        public static Entity[] GetAllEntitiesWithScript<T>() where T : Entity, new()
        {
            ulong[] list = null;
             InternalCalls.World_GetEntitiesOfScriptType(typeof(T).FullName,ref list);

             if (list == null || list.Length == 0)
             {
                 return Array.Empty<Entity>();
             }

             Entity[] entityList = new Entity[list.Length];
             for (int i = 0; i < list.Length; i++)
             {
                 entityList[i] = new Entity(list[i]);
             }

             return entityList;
        }
        

    }
}
