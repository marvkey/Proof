#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{
	enum class AssetType {
		None =0,
		Mesh,
		Texture,
		Material,
		World,				// NOT TREATED THE SAME AS OTHER ASSETS
		MeshSourceFile,
		PhysicsMaterial
	};

	using AssetID = UUID;
	class Proof_API Asset {
	public:
		
		virtual void SaveAsset() =0;
		virtual bool LoadAsset(const std::string& FilePath) =0;
		bool LoadAsset() { return LoadAsset(m_SavePath); }
		
		virtual AssetID GetAssetID() {
			return m_AssetID;
		}
		AssetType GetAssetType() {
			return m_AssetType;
		}
		virtual std::string GetExtension()const = 0;
		virtual uint32_t GetIamgeEdtorID(){
			return 0;
		}
		
		// FOR BROWSER PURPOSES
		virtual bool IsEditorImageNull(){
			return true;
		}

		virtual const std::string& GetName()const{
			return m_AssetName;
		}
		const std::string& GetPath()const{
			return m_SavePath;
		}
		
	protected:
		Asset(AssetType assetType) {
			m_AssetType = assetType;
		}	
		AssetType  m_AssetType;
		void SetPath(const std::string& newFilePath) {
			m_SavePath = newFilePath;
		}
		std::string m_AssetName;
		std::string m_SavePath;
		AssetID m_AssetID = 0;
		friend class ContentBrowserPanel;
		friend class AssetManager;
	};
}

