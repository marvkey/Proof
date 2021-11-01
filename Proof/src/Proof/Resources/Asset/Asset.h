#pragma once
#include "Proof/Core/Core.h"
#include "AssetManager.h"
namespace Proof
{
	using AssetID = uint64_t;
	class Proof_API Asset {
	public:
		Asset() =default;
		virtual void SaveAsset() =0;
		virtual bool LoadAsset(const std::string& FilePath) =0;
		
		const static std::string& GetAssetType() {
			static std::string assetType = "AssetType::None";
			return assetType;
		}

		virtual const std::string& GetAssetTypeVirtual()const {
			return GetAssetType();
		};
		virtual AssetID GetID() {
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
	protected:
		void SetPath(const std::string& NewFilePath) {
			m_SavePath = NewFilePath;
		}
		std::string m_AssetName;
		std::string m_SavePath;
		AssetID m_ID = 0;
		friend class ContentBrowserPanel;
		friend class AssetManager;
	};
}

