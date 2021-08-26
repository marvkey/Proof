#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/Asset/Asset.h"
#include "Proof/Resources/Math/Math.h"

namespace Proof{
	class Proof_API MaterialAsset:public Asset {
	public:
		MaterialAsset(const std::string& FilePath,const std::string& AssetSavePath);
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& FilePath)override;

		MaterialAsset() {
			m_AssetType = AssetType::MaterialAsset;
		};

		virtual ~MaterialAsset() {}

		virtual bool IsImageIDNUll() {
			return true;
		}

		virtual uint32_t GetImageID();

		virtual std::string GetAssetTypeName() {
			return "AssetType::MaterialAsset";
		}

		static std::string GetAssetTypeStaticName() {
			return "AssetType::MaterialAsset";
		}
		Material GetMaterial();
		Material m_Material;
	private:
		friend class AssetManager;

	};
}
