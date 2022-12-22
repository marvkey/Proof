#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Scene/Mesh.h"
namespace Proof
{
	class Proof_API MeshAsset: public Asset {
	public:
		MeshAsset(const std::string& meshFilePath,const std::string& savePath, const std::vector<uint32_t>& excludeIndex = {});
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& FilePath)override;
		
		void ChangeMesh(const std::string& meshFilepath);
		
		virtual ~MeshAsset() {

		}
		MeshAsset():
			Asset(AssetType::Mesh)
		{
		}
		static std::string StaticGetExtension() {
			return "Mesh.ProofAsset";
		}
		virtual std::string GetExtension()const {
			return "Mesh.ProofAsset";
		}
		
		std::vector<uint32_t>& GetDiscardedMesh() {
			return m_DiscardMesh;
		}

		Mesh* GetMesh();
	private:
		std::vector<uint32_t> m_DiscardMesh;
		friend class AssetManager;
		AssetID m_Source;
	};

	class MeshSourceFileAsset : public Asset {
	public:
		MeshSourceFileAsset() :
			Asset(AssetType::MeshSourceFile) {
		}
		MeshSourceFileAsset(const std::string& meshFilePath);
		virtual void SaveAsset(){}
		// the filepath of the actual mesh
		virtual bool LoadAsset(const std::string& FilePath)override;
		Mesh* GetMesh() {
			return m_Mesh.get();
		}
		// so we can view it as like cube.fbx
		virtual std::string GetName()const override{
			return Utils::FileDialogs::GetFullFileName(m_SavePath);
		}
		static std::string StaticGetExtension() {
			return "MeshSourceFileAsset.ProofAsset";
		}
		virtual std::string GetExtension()const;
	private:
		friend class AssetManager;
		Special<class Mesh> m_Mesh = nullptr;
	};
}

