#include "Proofprch.h"
#include "MeshAsset.h"
#include "Proof3D/Scene/Model.h"
namespace Proof
{
	MeshAsset::MeshAsset(const std::string& FilePath) {
		m_Model = new Model(FilePath);

		m_AssetType = AssetType::MeshAsset;
	}
	void MeshAsset::SaveAsset() {}
	void MeshAsset::CreateAsset(const std::string& FilePath) {}
}