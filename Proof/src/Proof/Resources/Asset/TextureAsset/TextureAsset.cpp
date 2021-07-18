#include "Proofprch.h"
#include "TextureAsset.h"
#include "Proof/Renderer/Renderer3D.h"
namespace Proof{
	Texture2DAsset::Texture2DAsset(const std::string& FilePath) {
		m_Texture =Texture2D::Create(FilePath);
		m_AssetType = AssetType::Texture2DAsset;
	}
	void Texture2DAsset::SaveAsset() {}
	void Texture2DAsset::CreateAsset(const std::string& FilePath) {
	}
	uint32_t Texture2DAsset::GetImageID() {
		return m_Texture != nullptr ? m_Texture->GetID() : InstancedRenderer3D::m_WhiteTexture->GetID();
	}
}