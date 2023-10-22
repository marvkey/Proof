using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct Texture
    {
        private ulong AssetID;
        //tempory
        public Texture(ulong assetID)
        {
            AssetID = assetID;
        }
    }
}
