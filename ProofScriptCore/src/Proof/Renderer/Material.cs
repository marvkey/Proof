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

        private static bool IsPrimitiveOrBuiltin<T>()
        {
            Type t = typeof(T);
            return t.IsPrimitive ||
                   t == typeof(decimal) ||
                   t == typeof(Vector2) ||
                   t == typeof(Vector3) ||
                   t == typeof(Vector4) ||
                   t == typeof(Matrix4);
        }

        internal Material() { m_ID = AssetID.Invalid; }
        internal Material(AssetID handle) { m_ID = handle; }
        public override AssetType Type => AssetType.Material;

        public void SetInput<T>(string name,T data)
        {
           // Optional safety: disallow user-defined structs like MyCustomStruct
            if (!IsPrimitiveOrBuiltin<T>())
                throw new InvalidOperationException($"Type {typeof(T).Name} is not supported.");

            int size = Marshal.SizeOf<T>();
            IntPtr dataPtr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(data, dataPtr, false);
                InternalCalls.Material_SetInput(m_ID.ToUInt64(), name, dataPtr, (uint)size);
            }
            finally
            {
                Marshal.FreeHGlobal(dataPtr);
            }
        }

        public T GetInput<T>(string name)
        {
            // Optional safety: disallow user-defined structs like MyCustomStruct
            if (!IsPrimitiveOrBuiltin<T>())
                throw new InvalidOperationException($"Type {typeof(T).Name} is not supported.");

            int size = Marshal.SizeOf<T>();

            IntPtr dataPtr = InternalCalls.Material_GetInput(m_ID.ToUInt64(),name);
            if (dataPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to retrieve variable data.");
            }
            try
            {
                return Marshal.PtrToStructure<T>(dataPtr);
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Error while retrieving variable data: {ex.Message}", ex);
            }
        }

        public Texture2D GetTexture(string name)
        {
            AssetID assetID = new AssetID(InternalCalls.Material_GetTexture(m_ID.ToUInt64(), name));

            if(!assetID.IsValid())
                return null;

            return new Texture2D(assetID);
        }

        public void SetTexture(string name, Texture2D texture)
        {
            if(texture.ID.IsValid())
                InternalCalls.Material_SetTexture(m_ID.ToUInt64(), name, texture.ID.ToUInt64());
        }
    }
}
