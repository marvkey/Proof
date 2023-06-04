using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ImageAsset
    {
        private ulong AssetID;
        //tempory
        public ImageAsset(ulong assetID)
        {
            AssetID = assetID;
        }
    }
}
