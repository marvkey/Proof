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
		MaterialAsset();
		virtual ~MaterialAsset() {

		}
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		static std::string StaticGetExtension() {
			return "Material.ProofAsset";
		}

		virtual std::string GetExtension()const {
			return "Material.ProofAsset";
		}
		Count<Material> GetMaterial()const;
	private:
		Count<Material> m_Material;
		friend class AssetManager;
	};
}
