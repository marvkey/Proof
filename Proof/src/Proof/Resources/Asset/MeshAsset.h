#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/Asset.h"
#include "Proof/Scene/Mesh.h"
namespace Proof
{
	class Proof_API MeshAsset: public Asset {
	public:
		MeshAsset(const std::string& meshFilePath,const std::string& savePath);
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& FilePath)override;
		
		void ChangeMesh(const std::string& meshFilepath);
		const static std::string& GetAssetType() {
			static std::string assetType = "AssetType::MeshAsset";
			return assetType;
		}
		MeshAsset(){}
		
		class Mesh* GetMesh()const{
			return m_Mesh.get();
		}
		virtual const std::string& GetAssetTypeVirtual()const {
			return GetAssetType();
		};
		virtual ~MeshAsset(){}

		virtual bool IsImageIDNUll(){
			return true;
		}
		virtual uint32_t GetImageID();
		
	private:
		Special<class Mesh> m_Mesh =nullptr;
		friend class AssetManager;
		std::string m_MeshFilePath;
	};
}

