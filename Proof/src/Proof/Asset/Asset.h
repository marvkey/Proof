#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/PlatformUtils.h"
namespace Proof
{
	enum class AssetType {
		None =0,
		Mesh,
		Texture,
		Material,
		World,				// NOT TREATED THE SAME AS OTHER ASSETS
		MeshSourceFile,
		PhysicsMaterial,
		TextureSourceFile,
		Font,
		FontSource,
		Prefab,
		UIPanel,
		ParticleSystem
	};
	enum class AssetState {
		None = 0,
		Unloaded,
		Ready,
		Loading,
		Invalid
	};

	#define ASSET_CLASS_TYPE(type) static AssetType GetStaticType() { return AssetType::type; }\
								virtual AssetType GetAssetType() const override { return GetStaticType(); }
	using AssetID = UUID;
	class Asset {
	public:
		virtual ~Asset() {

		}
		AssetID GetID() {//rename to getAssetID
			return m_ID;
		}
		virtual AssetType GetAssetType() const = 0;
	private:
		AssetID m_ID{0};
		friend class ContentBrowserPanel;
		friend class AssetSerializer;
		friend class AssetManager;
	};
}

