using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    public class Material
    {
        internal AssetID m_ID;
        internal Material(AssetID id)
        {
            m_ID = id;
        }

        internal Material()
        {
            m_ID = AssetID.Invalid;
        }
    }
}
