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
        public static Entity Instanciate(Prefab prefab, Transform transform)
        {
            ulong entityID = InternalCalls.World_Instanciate(prefab.ID, transform);

            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }
        public static Entity Instanciate(Prefab prefab, Vector location)
        {
            Transform transform;
            transform.Location = location;
            transform.Rotation = new Vector( 0.0f);
            transform.Scale = new Vector(1.0f);
            ulong entityID = InternalCalls.World_Instanciate(prefab.ID, transform);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }
    }
}
