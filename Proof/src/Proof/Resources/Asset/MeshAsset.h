#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/Asset.h"
namespace Proof
{
	class Proof_API MeshAsset: public Asset {
	public:
		MeshAsset(const std::string& FilePath,const std::string& AssetSavePath);
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& FilePath)override;
		
		virtual std::string GetName() {
			return "AssetType::MeshAsset";
		}

		static std::string GetStaticName(){
			return "AssetType::MeshAsset";
		}
		MeshAsset() {
			m_AssetType = AssetType::MeshAsset;
		};
		
		class Model* GetModel()const{
			return m_Model;
		}
		virtual ~MeshAsset(){}

		virtual bool IsImageIDNUll(){
			return true;
		}
		virtual uint32_t GetImageID();
		class Model* GetModel();
	private:
		class Model* m_Model =nullptr;
	};
}

