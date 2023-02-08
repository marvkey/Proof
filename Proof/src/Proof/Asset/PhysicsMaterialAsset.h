#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Material.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Math/Math.h"
namespace Proof
{
	class Proof_API PhysicsMaterialAsset :public Asset {
	public:
		PhysicsMaterialAsset(const std::string& assetPath);
		PhysicsMaterialAsset();
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		virtual ~PhysicsMaterialAsset() {

		}
		static std::string StaticGetExtension() {
			return "PhysicsMaterial.ProofAsset";
		}
		virtual std::string GetExtension()const {
			return "PhysicsMaterial.ProofAsset";
		}
		virtual uint32_t GetImageID();
		Count<PhysicsMaterial> GetMaterial()const;
	private:
		Count<PhysicsMaterial> m_Material;

		friend class AssetManager;
	};
}