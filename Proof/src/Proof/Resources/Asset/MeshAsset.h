#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/Asset.h"
namespace Proof
{
	class Proof_API MeshAsset: public Asset {
	public:
		MeshAsset(const std::string& FilePath);
		virtual void SaveAsset() override;
		virtual void CreateAsset(const std::string& FilePath)override;
		
		static std::string GetName() {
			return "AssetType::MeshAsset";
		}
		MeshAsset() {
			m_AssetType = AssetType::MeshAsset;
		};
		
		class Model* GetModel()const{
			return m_Model;
		}
		virtual ~MeshAsset(){
			PF_ENGINE_INFO("Mesh Asset Deleted ");
		}
		class Model* m_Model =nullptr;
	private:
	};
}

