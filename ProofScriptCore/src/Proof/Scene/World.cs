using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
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
        public static Entity Instanciate(Prefab prefab, Transform transform)
        {
            ulong entityID = InternalCalls.World_Instanciate(prefab.ID, transform);

            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }
        public static Entity Instanciate(Prefab prefab, Vector3 location)
        {
            Transform transform;
            transform.Location = location;
            transform.Rotation = new Vector3(0.0f);
            transform.Scale = new Vector3(1.0f);
            ulong entityID = InternalCalls.World_Instanciate(prefab.ID, transform);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }

        public static Entity TryFindEntityByTag(string tag)
        {
            ulong id = InternalCalls.World_TryFindEntityByTag(tag);
            if (id == 0)
                return null;

            return new Entity(id);
        }

        public static void DeleteEntity(Entity entity, bool deleteChildren = true)
        {
            InternalCalls.World_DeleteEntity(entity.ID, deleteChildren);
        }

        public static float GetTimeStep()
        {
            return InternalCalls.World_GetTimeStep();
        }
        public static void Restart()
        {
            InternalCalls.World_Restart();
        }
        public static T[] GetEntityWithType<T>() where T : Entity, new()
        {
            ulong[] list =null;
            InternalCalls.World_ForEachEntityWith(typeof(T).FullName, ref list);

            if (list == null)
            {
                list = new ulong[] { };
                T[] scriptslist = new T[list.Length];
                int index = 0;
                foreach (ulong entityID in list)
                {
                    Entity entity = new Entity(entityID);
                    scriptslist[index] = entity.As<T>();
                    index++;
                }
                return scriptslist;
            }
            {
                T[] scriptslist = new T[list.Length];
                int index = 0;
                foreach (ulong entityID in list)
                {
                    Entity entity = new Entity(entityID);
                    scriptslist[index] = entity.As<T>();
                    index++;
                }
                return scriptslist;
            }
        }
    }
}
