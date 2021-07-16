#include "Proofprch.h"
#include "TextureAsset.h"

namespace Proof{
	Texture2DAsset::Texture2DAsset(const std::string& FilePath) {
		m_Texture =Texture2D::Create(FilePath);
		m_AssetType = AssetType::Texture2DAsset;
	}
	void Texture2DAsset::SaveAsset() {}
	void Texture2DAsset::CreateAsset(const std::string& FilePath) {
	}
}