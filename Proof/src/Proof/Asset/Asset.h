#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/PlatformUtils.h"
namespace Proof
{
	enum class AssetType {
		None =0,
		Mesh,
		Texture,
		Material,
		World,				// NOT TREATED THE SAME AS OTHER ASSETS
		MeshSourceFile,
		PhysicsMaterial,
		TextureSourceFile,
	};

	using AssetID = UUID;
	class Proof_API Asset {
	public:
		virtual ~Asset() {

		}
		virtual void SaveAsset() =0;
		virtual bool LoadAsset(const std::string& FilePath) =0;
		bool LoadAsset() { return LoadAsset(m_SavePath.string()); }
		
		virtual AssetID GetAssetID() {
			return m_AssetID;
		}
		AssetType GetAssetType() {
			return m_AssetType;
		}
		virtual std::string GetExtension()const = 0;

		std::string GetName()const{
			return Utils::FileDialogs::GetFileName(m_SavePath);
		}
		const std::filesystem::path& GetPath()const{
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
		std::filesystem::path m_SavePath;
		AssetID m_AssetID = 0;
		friend class ContentBrowserPanel;
		friend class AssetManager;
	};
}

