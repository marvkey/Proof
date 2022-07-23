#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/Asset/Asset.h"
#include "Proof/Resources/Math/Math.h"
namespace Proof
{
	class Proof_API PhysicsMaterialAsset :public Asset {
	public:
		PhysicsMaterialAsset(const std::string& assetPath);
		PhysicsMaterialAsset() :
			Asset(AssetType::PhysicsMaterial) {

		}
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		virtual bool IsImageIDNUll() {
			return true;
		}
		static std::string StaticGetExtension() {
			return "PhysicsMaterial.ProofAsset";
		}
		virtual std::string GetExtension()const {
			return "PhysicsMaterial.ProofAsset";
		}
		virtual uint32_t GetImageID();
		const PhysicsMaterial& GetMaterial()const;
		PhysicsMaterial m_Material;
	private:
		friend class AssetManager;
	};
}