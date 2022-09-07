#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Material.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Math/Math.h"

namespace Proof
{
	class Proof_API MaterialAsset:public Asset {
	public:
		MaterialAsset(const std::string& assetPath);
		MaterialAsset():
			Asset(AssetType::Material) 
		{

		}
		virtual ~MaterialAsset() {

		}
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		virtual bool IsImageIDNUll() {
			return true;
		}
		static std::string StaticGetExtension() {
			return "Material.ProofAsset";
		}

		virtual std::string GetExtension()const {
			return "Material.ProofAsset";
		}
		virtual uint32_t GetImageID();
		const Material& GetMaterial()const;
		Material m_Material;
	private:
		friend class AssetManager;
	};
}
