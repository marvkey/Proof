#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Math/Math.h"

namespace Proof{
	class Proof_API WorldAsset :public Asset {
		WorldAsset(const std::string& assetPath);
		WorldAsset() :
			Asset(AssetType::World) {

		}

		virtual ~WorldAsset() {

		}
		// NEEDS NO implementation
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		virtual bool IsImageIDNUll() {
			return true;
		}
		static std::string StaticGetExtension() {
			return "ProofWorld";
		}
		virtual std::string GetExtension()const {
			return "ProofWorld";
		}
		virtual uint32_t GetImageID();
	private:
		friend class AssetManager;
	};
}
