using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public abstract class MeshBase :  Asset
    {

        public Material GetMaterial(uint index)
        {
            if (!InternalCalls.MeshBase_GetMaterialByIndex(ref m_ID, index, out AssetID materialHandle))
                return null;

            return new Material(materialHandle);
        }

        public uint GetMaterialCount() => InternalCalls.MeshBase_GetMaterialCount(ref m_ID);
        public bool IsStaticMesh() => InternalCalls.MeshBase_IsStaticMesh(ref m_ID);

    }
    public class StaticMesh : MeshBase
    {
        internal StaticMesh() { m_ID = AssetID.Invalid; }
        internal StaticMesh(AssetID handle) { m_ID = handle; }

        public Material BaseMaterial => GetMaterial(0);
    }

    public class DynamicMesh : MeshBase
    {
        internal DynamicMesh() { m_ID = AssetID.Invalid; }
        internal DynamicMesh(AssetID handle) { m_ID = handle; }

        public Material BaseMaterial => GetMaterial(0);
    }
}
