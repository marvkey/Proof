#pragma once
#include "Proof/Core/Core.h"
#include "AssetManager.h"
namespace Proof
{
	class Proof_API Asset {
	public:
		
		Asset() =default;
		virtual void SaveAsset() =0;
		virtual bool LoadAsset(const std::string& FilePath) =0;
		bool LoadAsset() { return LoadAsset(m_SavePath); }
		const static std::string& GetAssetType() {
			static std::string assetType = "AssetType::None";
			return assetType;
		}

		virtual const std::string& GetAssetTypeVirtual()const {
			return GetAssetType();
		};
		virtual UUID GetID() {
			return m_ID;
		}

		virtual uint32_t GetIamgeEdtorID(){
			return 0;
		}// FOR BROWSER PURPOSES
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
		void SetPath(const std::string& newFilePath) {
			m_SavePath = newFilePath;
		}
		std::string m_AssetName;
		std::string m_SavePath;
		UUID m_ID = 0;
		friend class ContentBrowserPanel;
		friend class AssetManager;
	};
}

