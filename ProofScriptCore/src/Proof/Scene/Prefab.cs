using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    public class Prefab : Asset
    {
        internal Prefab() { m_ID = AssetID.Invalid; }
        internal Prefab(AssetID handle) { m_ID = handle; }
    }
}
