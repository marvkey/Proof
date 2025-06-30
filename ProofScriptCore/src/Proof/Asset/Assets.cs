using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{

    public enum AssetType
    {
        None = 0,// for items that 
		Mesh,
		DynamicMesh,
		Texture,
		Material,
		World,				// NOT TREATED THE SAME AS OTHER ASSETS
		MeshSourceFile, // for mesh source fiel and mesh source
		PhysicsMaterial,
		//TextureSourceFile,
		Font,
		//FontSourceFile,
		Prefab,
		UIPanel,
		ParticleSystem,
		Audio,
		//AudioSourceFile,
		MeshCollider,
		ScriptFile,
		InputAction,
		InputBindingContext,
		Skeleton,
		Animation,
		AnimationController,
		EnvironmentMap
    };


    [RegisterCoreClassStruct]
    public abstract class Asset
    {
        internal AssetID m_ID;
        public AssetID ID => m_ID;

        // Every derived asset must define this
        public abstract AssetType Type { get; }
    }


    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct AssetKey
    {
        public AssetID ID;
        public AssetType Type;
    }

    [RegisterCoreClassStruct]
    public class Texture2D : Asset
    {

        internal Texture2D() { m_ID = AssetID.Invalid; }
        internal Texture2D(AssetID handle) { m_ID = handle; }

        public override AssetType Type => AssetType.Texture;
    }

    [RegisterCoreClassStruct]
    public class UIPanel : Asset
    {

        internal UIPanel() { m_ID = AssetID.Invalid; }
        internal UIPanel(AssetID handle) { m_ID = handle; }
        public override AssetType Type => AssetType.UIPanel;
    }
}
