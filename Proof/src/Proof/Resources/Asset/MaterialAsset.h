#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/Asset/Asset.h"
#include "Proof/Resources/Math/Math.h"

namespace Proof{
	class Proof_API MaterialAsset:public Asset {
	public:
		MaterialAsset(const std::string& assetPath);
		MaterialAsset()=default;
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		virtual bool IsImageIDNUll() {
			return true;
		}
		virtual uint32_t GetImageID();
		const static std::string& GetAssetType(){
			static std::string assetType ="AssetType::MaterialAsset";
			return assetType;
		}
		virtual const std::string& GetAssetTypeVirtual()const {
			return GetAssetType();
		};
		const Material& GetMaterial()const;
		Material m_Material;
	private:
		friend class AssetManager;
	};
}
