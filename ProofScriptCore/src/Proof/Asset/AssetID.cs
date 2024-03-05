using Proof;
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
    public struct AssetID
    {
        public static readonly AssetID  Invalid = new AssetID(0);

        private ulong m_ID;

        public AssetID(ulong handle) { m_ID = handle; }

        public bool IsValid() => InternalCalls.AssetID_IsValid(ref this);

        public static implicit operator bool(AssetID assetId)
        {
            return InternalCalls.AssetID_IsValid(ref assetId);
        }

        public static explicit operator ulong(AssetID assetId)
        {
            return assetId.m_ID;
        }

        public ulong ToUInt64()
        {
            return m_ID;
        }
        public override string ToString() => m_ID.ToString();
        public override int GetHashCode() => m_ID.GetHashCode();
    }
}
