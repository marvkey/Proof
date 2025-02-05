#pragma once
#include <iostream>
#include <vector>
#include<filesystem>
#include "Proof/Utils/FileSystem.h"
#include <unordered_map>
#include "Proof/Core/Assert.h"
#include "Proof/Core/UUID.h"
#include "Proof/Utils/StringUtils.h"
namespace Proof
{
#define ASSET_CLASS_TYPE(type) static AssetType GetStaticType() { return AssetType::type; }\
								virtual AssetType GetAssetType() const override { return GetStaticType(); }

	enum class AssetType 
	{
		None = 0,// for items that 
		Mesh,
		DynamicMesh,
		Texture,
		Material,
		World,				// NOT TREATED THE SAME AS OTHER ASSETS
		MeshSourceFile, // for mesh source fiel and mesh source
		PhysicsMaterial,
		//TextureSourceFile,
		Font,
		//FontSourceFile,
		Prefab,
		UIPanel,
		ParticleSystem,
		Audio,
		//AudioSourceFile,
		MeshCollider,
		ScriptFile,
		InputAction,
		InputBindingContext,
		Skeleton,
		Animation,
		AnimationController,
		EnvironmentMap
	};
	enum class AssetState 
	{
		None = 0,
		Unloaded,
		Ready,
		Loading,
		Invalid
	};
	using AssetID = UUID;

	class Asset;
	namespace Utils 
	{
		// assets that are not converted to a proof format in the editor
		static const std::unordered_set<AssetType> SourceAssets =
		{
			AssetType::Texture,
			AssetType::MeshSourceFile,
			AssetType::Font,
			AssetType::Audio,
		};

		inline bool IsAssetSource(AssetType type)
		{
			return SourceAssets.contains(type);
		}
		static inline const std::unordered_map<AssetType, std::string> AssetTypeMap = 
		{
			{AssetType::None, ""},
			{AssetType::Mesh, ".Mesh.ProofAsset"},
			{AssetType::DynamicMesh, ".DynamicMesh.ProofAsset"},
			//{AssetType::Texture, ".Texture.ProofAsset"},
			{AssetType::Material, ".Material.ProofAsset"},
			{AssetType::World, ".ProofWorld"},
			{AssetType::PhysicsMaterial, ".PhysicsMaterial.ProofAsset"},
			{AssetType::Prefab, ".Prefab.ProofAsset"},
			//{AssetType::TextureSourceFile, ""},
			{AssetType::UIPanel, ".UIPanel.ProofAsset"},
			{AssetType::ParticleSystem, ".ParticleSystem.ProofAsset"},
			{AssetType::Font, ".Font.ProofAsset" },
			{AssetType::Audio, ".Audio.ProofAsset" },
			{AssetType::MeshCollider, ".MeshCollider.ProofAsset" },
			{AssetType::ScriptFile, ".cs" },
			{AssetType::InputAction,".InputAction.ProofAsset"},
			{AssetType::InputBindingContext,".InputBindingContext.ProofAsset"},
			{AssetType::Skeleton,".Skeleton.ProofAsset"},
			{AssetType::Animation,".Animation.ProofAsset"},
			{AssetType::AnimationController,".AnimationController.ProofAsset"},
		};
		std::string GetAssetExtensionString(AssetType type);
		// checks the file extension
		

		static const std::unordered_map<AssetType, std::unordered_set<std::string>> AssetSourceFormats =
		{
			{
				AssetType::MeshSourceFile,
				{
					".fbx",
					".obj",
					".blend",
					".gltf",
					".glb",
					".dae",
				}
			},

			{
				AssetType::Texture,
				{
					".jpeg",
					".jpg",
					".png",
					".tga",
					".big",
					".hdr"
				}
			},

			{
				AssetType::Font,
				{
					".ttf",
				}
			},

			{
				AssetType::Audio,
				{
					".wav",
					".mp3",
					".flac"
				}
			},
		};
		inline bool MeshHasFormat(const std::string& format) 
		{
			return AssetSourceFormats.at(AssetType::MeshSourceFile).contains(Utils::String::ToLower(format));
		}
		inline bool TextureHasFormat(const std::string& format) 
		{
			return AssetSourceFormats.at(AssetType::Texture).contains(Utils::String::ToLower(format));
		}
	
		inline bool FontHasFormat(const std::string& format) 
		{
			return AssetSourceFormats.at(AssetType::Font).contains(Utils::String::ToLower(format));
		}
		inline bool AudioHasFormat(const std::string& format) 
		{
			return AssetSourceFormats.at(AssetType::Audio).contains(Utils::String::ToLower(format));
		}
		inline AssetType GetAssetTypeFromExtension(const std::string& fileFullExtension)
		{
			const std::string fileFullExtensionLower = Utils::String::ToLower(fileFullExtension);

			for (auto& [assetType, formats] : AssetSourceFormats)
			{
				if (formats.contains(fileFullExtensionLower))
					return assetType;
			}

			// Iterate through AssetTypeMap to find a match for the file extension
			for (const auto& pair : AssetTypeMap)
			{
				if (pair.second == fileFullExtensionLower)
				{
					return pair.first;
				}
			}
			return AssetType::None;
		}

		inline AssetType GetAssetTypeFromPath(const std::filesystem::path& path)
		{
			const std::string fileFullExtension = Utils::String::ToLower(FileSystem::GetFullFileExtension(path));
			return GetAssetTypeFromExtension(fileFullExtension);
		}

		bool HasAssetAndAssetType(AssetID id, AssetType type);
		Count<class Proof::Asset> GetAssetFromID(AssetID id);
	}

	template <AssetType Type>
	struct AssetKey 
	{
	public:
		AssetKey() : m_AssetID(0) {}

		AssetKey(AssetID id) 
		{				    
			SetAssetID(id);
		}

		template <class T>
		Count<T> GetAsset()const {
			if (!IsValid())
				return nullptr;

			return Utils::GetAssetFromID(m_AssetID).As<T>();
		}

		

		Count<class Asset> GetAsBaseAsset() const 
		{
			if (!IsValid())
				return nullptr;

			return Utils::GetAssetFromID(m_AssetID);
		}

		AssetID GetAssetID() const {
			return IsValid() ? m_AssetID : AssetID(0);
		}

		bool SetAssetID(AssetID id) {
			if (Utils::HasAssetAndAssetType(id, Type)) {
				m_AssetID = id;
				return true;
			}

			return false;
		}

		bool IsValid() const {
			if (Utils::HasAssetAndAssetType(m_AssetID, Type))
				return true;

			m_AssetID = 0;
			return false;
		}

		operator AssetID() const 
		{
			return GetAssetID();
		}

		operator bool() const 
		{
			return IsValid();
		}

		AssetType GetAssetType()const
		{
			return Type;
		}

		operator uint64_t () { return m_AssetID; }
		operator const uint64_t() const { return m_AssetID; }
		uint64_t Get()const { return m_AssetID; }
	private:
		mutable AssetID m_AssetID = { 0 };
	};


	struct MultiAssetKeys
	{
		MultiAssetKeys(std::initializer_list<AssetType> types)
			: m_SupportedTypes(types) {}

		MultiAssetKeys(AssetID id)
		{
			SetAssetID(id);
		}

		template<typename AssetClass>
		Count<AssetClass> GetAsset() const
		{
			return GetAsBaseAsset().As<AssetClass>();
		}

		Count<class Asset> GetAsBaseAsset() const
		{
			if (IsValid())
				return Utils::GetAssetFromID(m_AssetID);
			return nullptr;
		}

		AssetID GetAssetID() const
		{
			return IsValid() ? m_AssetID : AssetID(0);
		}

		bool SetAssetID(AssetID id);

		bool IsValid() const;

		operator AssetID() const { return m_AssetID; }
		operator bool() const { return IsValid(); }

	private:
		mutable AssetID m_AssetID = { 0 };
		std::unordered_set<AssetType> m_SupportedTypes;
	};
}