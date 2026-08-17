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



    public class PbrSurfaceMaterial
    {
        private readonly Material m_Material;

        private const string AlbedoColorName = "u_MaterialUniform.Albedo";
        private const string MetalnessName = "u_MaterialUniform.Metalness";
        private const string RoughnessName = "u_MaterialUniform.Roughness";
        private const string NormalTextureToggleName = "u_MaterialUniform.NormalTexToggle";
        private const string EmissionOverrideColorToggleName = "u_MaterialUniform.EmissionOverrideColorToggle";
        private const string EmissionName = "u_MaterialUniform.Emission";
        private const string EmissionOverrideColorName = "u_MaterialUniform.EmissionOverrideColor";
        private const string TilingName = "u_MaterialUniform.TextureTiling";
        private const string OffsetName = "u_MaterialUniform.TextureOffset";

        private const string AlbedoMapName = "u_AlbedoMap";
        private const string NormalMapName = "u_NormalMap";
        private const string MetalnessMapName = "u_MetallicMap";
        private const string RoughnessMapName = "u_RoughnessMap";

        public Material Material => m_Material;
        public bool IsValid => m_Material != null && m_Material.ID.IsValid();

        public PbrSurfaceMaterial(Material material)
        {
            m_Material = material;
        }

        private MaterialVariable GetVariable(string name)
        {
            if (!IsValid)
            {
                Log.Error("PbrSurfaceMaterial: Material is invalid.");
                return null;
            }

            return m_Material.GetVariable(name);
        }

        public Vector3 GetAlbedoColor()
        {
            MaterialVariable variable = GetVariable(AlbedoColorName);
            return variable != null ? variable.Get<Vector3>() : default;
        }

        public void SetAlbedo(Vector3 value)
        {
            GetVariable(AlbedoColorName)?.Set(value);
        }

        public float GetMetalness()
        {
            MaterialVariable variable = GetVariable(MetalnessName);
            return variable != null ? variable.Get<float>() : 0.0f;
        }

        public void SetMetalness(float value)
        {
            GetVariable(MetalnessName)?.Set(value);
        }

        public float GetRoughness()
        {
            MaterialVariable variable = GetVariable(RoughnessName);
            return variable != null ? variable.Get<float>() : 0.0f;
        }

        public void SetRoughness(float value)
        {
            GetVariable(RoughnessName)?.Set(value);
        }

        public bool GetNormalTextureToggle()
        {
            MaterialVariable variable = GetVariable(NormalTextureToggleName);
            return variable != null && variable.Get<bool>();
        }

        public void SetNormalTextureToggle(bool value)
        {
            GetVariable(NormalTextureToggleName)?.Set(value);
        }

        public bool GetEmissionOverrideColorToggle()
        {
            MaterialVariable variable = GetVariable(EmissionOverrideColorToggleName);
            return variable != null && variable.Get<bool>();
        }

        public void SetEmissionOverrideColorToggle(bool value)
        {
            GetVariable(EmissionOverrideColorToggleName)?.Set(value);
        }

        public float GetEmission()
        {
            MaterialVariable variable = GetVariable(EmissionName);
            return variable != null ? variable.Get<float>() : 0.0f;
        }

        public void SetEmission(float value)
        {
            GetVariable(EmissionName)?.Set(value);
        }

        public Vector3 GetEmissionOverrideColor()
        {
            MaterialVariable variable = GetVariable(EmissionOverrideColorName);
            return variable != null ? variable.Get<Vector3>() : default;
        }

        public void SetEmissionOverrideColor(Vector3 value)
        {
            GetVariable(EmissionOverrideColorName)?.Set(value);
        }

        public Vector2 GetTiling()
        {
            MaterialVariable variable = GetVariable(TilingName);
            return variable != null ? variable.Get<Vector2>() : default;
        }

        public void SetTiling(Vector2 value)
        {
            GetVariable(TilingName)?.Set(value);
        }

        public Vector2 GetOffset()
        {
            MaterialVariable variable = GetVariable(OffsetName);
            return variable != null ? variable.Get<Vector2>() : default;
        }

        public void SetOffset(Vector2 value)
        {
            GetVariable(OffsetName)?.Set(value);
        }

        public void SetAlbedoMap(Texture2D texture)
        {
            GetVariable(AlbedoMapName)?.SetTexture(texture);
        }

        public void SetNormalMap(Texture2D texture)
        {
            GetVariable(NormalMapName)?.SetTexture(texture);
        }

        public void SetMetalnessMap(Texture2D texture)
        {
            GetVariable(MetalnessMapName)?.SetTexture(texture);
        }

        public void SetRoughnessMap(Texture2D texture)
        {
            GetVariable(RoughnessMapName)?.SetTexture(texture);
        }

        public Texture2D GetAlbedoMap()
        {
            return GetVariable(AlbedoMapName)?.GetTexture();
        }

        public Texture2D GetNormalMap()
        {
            return GetVariable(NormalMapName)?.GetTexture();
        }

        public Texture2D GetMetalnessMap()
        {
            return GetVariable(MetalnessMapName)?.GetTexture();
        }

        public Texture2D GetRoughnessMap()
        {
            return GetVariable(RoughnessMapName)?.GetTexture();
        }
    }
}
