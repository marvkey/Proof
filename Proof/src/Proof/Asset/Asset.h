#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/PlatformUtils.h"
#include "AssetTypes.h"
namespace Proof
{


	#define ASSET_CLASS_TYPE(type) static AssetType GetStaticType() { return AssetType::type; }\
								virtual AssetType GetAssetType() const override { return GetStaticType(); }
	using AssetID = UUID;
	class Asset : public RefCounted {
	public:
		virtual ~Asset() {

		}
		AssetID GetID() const {//rename to getAssetID
			return m_ID;
		}
		virtual AssetType GetAssetType() const = 0;
	private:
		AssetID m_ID{0};
		friend class ContentBrowserPanel;
		friend class AssetSerializer;
		friend class AssetManager;
		friend class SceneSerializer;
	};
}

