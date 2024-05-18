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
		AnimationController
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
	}
}