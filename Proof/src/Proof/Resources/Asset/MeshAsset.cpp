#include "Proofprch.h"
#include "MeshAsset.h"
#include "Proof3D/Scene/Model.h"
#include "Proof/Renderer/Renderer3D.h"
namespace Proof
{
	MeshAsset::MeshAsset(const std::string& FilePath) {
		m_Model = new Model(FilePath);

		m_AssetType = AssetType::MeshAsset;
	}
	void MeshAsset::SaveAsset() {}
	void MeshAsset::CreateAsset(const std::string& FilePath) {}
	uint32_t MeshAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
}