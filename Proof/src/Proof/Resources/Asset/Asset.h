#pragma once
#include "Proof/Core/Core.h"
#include "AssetManager.h"
namespace Proof
{
	class Proof_API Asset {
	public:
		Asset() =default;
		Asset(const std::string& FilePath,const std::string& AssetSavePath) {
			m_ID = AssetManager::CreateID();
			m_PathOfPointerToFile =FilePath;
			m_Path =AssetSavePath;
		};
		virtual void SaveAsset() =0;
		virtual bool LoadAsset(const std::string& FilePath) =0;
		virtual ~Asset(){
			
		};
		/*
		DO NOT CHANGE THE ORDER OF THESE BECAUSE WHENWE LOAD THEM IN WE LOAD
		* THEM BY THIER NUMBER CASTED TO AND INT SO 
		* IF ORDER IS CHANGED COULD BE LOADING A MESH ASSET BUT AS A TEXTURE2D
		*/ 
		static enum class AssetType {
			None,
			Texture2DAsset,
			MeshAsset,
			MaterialAsset
		};
		AssetType GetAssetType(){return m_AssetType;}
		virtual std::string GetAssetTypeName(){
			return "AssetType::None";
		}
		uint64_t GetID(){
			return m_ID;
		}

		virtual uint32_t GetImageID() = 0;// FOR BROWSER PURPOSES
		virtual bool IsImageIDNUll() = 0;

		const std::string GetAssetName(){
			return m_AssetName;
		}
	protected:
		AssetType m_AssetType  = AssetType::None;
		uint64_t m_ID = 0;
		std::string m_Path;
		std::string m_PathOfPointerToFile;
		std::string m_AssetName;

	private:
		void SetPath(const std::string& NewFilePath){
			m_Path = NewFilePath;
		}
		friend class ContentBrowserPanel;
		friend class AssetManager;
	};
}

