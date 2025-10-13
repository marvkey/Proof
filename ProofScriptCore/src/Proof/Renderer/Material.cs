using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public class MaterialVariable
    {
        private readonly Material m_Material;
        private readonly string m_Name;

        internal MaterialVariable(Material material, string name)
        {
            m_Material = material;
            m_Name = name;
        }

        public string Name => m_Name;
        public Material Material => m_Material;

        public bool IsValid => m_Material != null && !string.IsNullOrEmpty(m_Name);

        // --- Set value directly ---
        public void Set<T>(T value)
        {
            if (!IsValid)
            {
                Log.Error($"MaterialVariable.Set failed: Invalid variable '{m_Name}'");
                return;
            }

            if (!InternalCalls.Material_HasInput(m_Material.ID.ToUInt64(), m_Name))
            {
                Log.Error($"MaterialVariable.Set failed: Material does not contain variable '{m_Name}'");
                return;
            }

            m_Material.SetInput(m_Name, value);
        }

        // --- Get value directly ---
        public T Get<T>()
        {
            if (!IsValid)
            {
                Log.Error($"MaterialVariable.Get failed: Invalid variable '{m_Name}'");
                return default;
            }

            if (!InternalCalls.Material_HasInput(m_Material.ID.ToUInt64(), m_Name))
            {
                Log.Error($"MaterialVariable.Get failed: Material does not contain variable '{m_Name}'");
                return default;
            }

            return m_Material.GetInput<T>(m_Name);
        }

        // --- Texture helpers ---
        public void SetTexture(Texture2D texture)
        {
            if (!IsValid)
            {
                Log.Error($"MaterialVariable.Set(Texture) failed: Invalid variable '{m_Name}'");
                return;
            }

            if (!InternalCalls.Material_HasTexture(m_Material.ID.ToUInt64(), m_Name))
            {
                Log.Error($"MaterialVariable.Set(Texture) failed: Material does not contain texture '{m_Name}'");
                return;
            }

            m_Material.SetTexture(m_Name, texture);
        }

        public Texture2D GetTexture()
        {
            if (!IsValid)
            {
                Log.Error($"MaterialVariable.GetTexture failed: Invalid variable '{m_Name}'");
                return null;
            }

            if (!InternalCalls.Material_HasTexture(m_Material.ID.ToUInt64(), m_Name))
            {
                Log.Error($"MaterialVariable.GetTexture failed: Material does not contain texture '{m_Name}'");
                return null;
            }

            return m_Material.GetTexture(m_Name);
        }
    }


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

        public MaterialVariable GetVariable(string name)
        {
            bool hasInput = InternalCalls.Material_HasInput(m_ID.ToUInt64(), name);
            bool hasTexture = InternalCalls.Material_HasTexture(m_ID.ToUInt64(), name);

            if (!hasInput && !hasTexture)
            {
                Log.Error($"Material.GetVariable failed: Variable '{name}' does not exist in material.");
                return null;
            }

            return new MaterialVariable(this, name);
        }

        public void SetInput<T>(string name, T data)
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

            IntPtr dataPtr = InternalCalls.Material_GetInput(m_ID.ToUInt64(), name);
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

            if (!assetID.IsValid())
                return null;

            return new Texture2D(assetID);
        }

        public void SetTexture(string name, Texture2D texture)
        {
            if (texture.ID.IsValid())
                InternalCalls.Material_SetTexture(m_ID.ToUInt64(), name, texture.ID.ToUInt64());
        }
    }
}
