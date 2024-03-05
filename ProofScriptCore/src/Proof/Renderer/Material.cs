using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    public class Material : Asset
    {
        internal Material() { m_ID = AssetID.Invalid; }
        internal Material(AssetID handle) { m_ID = handle; }
    }
}
