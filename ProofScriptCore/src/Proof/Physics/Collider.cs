using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    public class Collider
    {
        public ulong EntityID { get; protected set; }
        public bool IsTrigger { get; protected set; }

        private Entity m_Entity;
        private RigidBodyComponent m_RigidBodyComponent;

        public Entity Entity
        {
            get
            {
                if (m_Entity == null)
                    m_Entity = new Entity(EntityID);

                return m_Entity;
            }
        }

        public RigidBodyComponent RigidBody
        {
            get => m_RigidBodyComponent ?? (m_RigidBodyComponent = Entity.GetComponent<RigidBodyComponent>());
        }

        public override string ToString()
        {
            string type = "Collider";

            switch (this)
            {
                case BoxCollider _:
                    type = "BoxCollider";
                    break;
                case SphereCollider _:
                    type = "SphereCollider";
                    break;
                //case CapsuleCollider _:
                //    type = "CapsuleCollider";
                //    break;
                case MeshCollider _:
                    type = "MeshCollider";
                    break;
            }

            return $"Collider({type}, {EntityID}, {IsTrigger})";
        }
    }
    [RegisterCoreClassStruct]
    public class BoxCollider : Collider
    {
        public Vector3 Size { get; protected set; }
        public Vector3 Center { get; protected set; }

        internal BoxCollider()
        {
            EntityID = 0;
            IsTrigger = false;
            Size = Vector3.Zero;
            Center = Vector3.Zero;
        }

        internal BoxCollider(ulong entityID, bool isTrigger, Vector3 size, Vector3 center)
        {
            EntityID = entityID;
            IsTrigger = isTrigger;
            Size = size;
            Center = center;
        }
    }
    [RegisterCoreClassStruct]
    public class SphereCollider : Collider
    {
        public float Radius { get; protected set; }
        public Vector3 Center { get; protected set; }

        internal SphereCollider()
        {
            EntityID = 0;
            IsTrigger = false;
            Radius = 0.0f;
        }

        internal SphereCollider(ulong entityID, bool isTrigger, float radius)
        {
            EntityID = entityID;
            IsTrigger = isTrigger;
            Radius = radius;
        }
    }
    /*
    public class CapsuleCollider : Collider
    {
        public float Radius { get; protected set; }
        public float Height { get; protected set; }
        public Vector3 Center { get; protected set; }

        internal CapsuleCollider()
        {
            EntityID = 0;
            IsTrigger = false;
            Radius = 0.0f;
            Height = 0.0f;
        }

        internal CapsuleCollider(ulong entityID, bool isTrigger, float radius, float height)
        {
            EntityID = entityID;
            IsTrigger = isTrigger;
            Radius = radius;
            Height = height;
        }
    }
    */
    [RegisterCoreClassStruct]
    public class MeshCollider : Collider
    {
        public MeshBase Mesh { get; protected set; }
        public bool IsStaticMesh => InternalCalls.MeshCollider_IsStaticMesh(ref m_MeshHandle);

        private AssetID m_MeshHandle;

        internal MeshCollider()
        {
            EntityID = 0;
            IsTrigger = false;

            m_MeshHandle = AssetID.Invalid;
            Mesh = null;
        }

        internal MeshCollider(ulong entityID, bool isTrigger, AssetID meshHandle)
        {
            EntityID = entityID;
            IsTrigger = isTrigger;

            if (meshHandle.IsValid())
            {
                if (InternalCalls.MeshCollider_IsStaticMesh(ref meshHandle))
                    Mesh = new StaticMesh(meshHandle);
                else
                    Mesh = new DynamicMesh(meshHandle);

                m_MeshHandle = meshHandle;
            }
            else
            {
                m_MeshHandle = AssetID.Invalid;
            }
        }
    }
}
