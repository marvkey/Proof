using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    public abstract class Asset
    {
        internal AssetID m_ID;
        public AssetID ID => m_ID;
    }
    [RegisterCoreClassStruct]
    public class Texture2D : Asset
    {

        internal Texture2D() { m_ID = AssetID.Invalid; }
        internal Texture2D(AssetID handle) { m_ID = handle; }
    }
}
