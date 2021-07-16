#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{
	class Proof_API Asset {
	public:
		Asset(){};
		Asset(const std::string& FilePath) {};
		virtual void SaveAsset() =0;
		virtual void CreateAsset(const std::string& FilePath) =0;
		virtual ~Asset(){};
		static enum class AssetType {
			None,
			Texture2DAsset,
			MeshAsset
		};
		AssetType GetAssetType(){return m_AssetType;}
	protected:
		AssetType m_AssetType  = AssetType::None;
	};
}

