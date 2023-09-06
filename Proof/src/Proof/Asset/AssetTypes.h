#pragma once
#include "Proof/Core/Core.h"
#include <iostream>
#include <vector>
#include<filesystem>
namespace Proof
{
	enum class AssetType 
	{
		None = 0,// for items that 
		Mesh,
		Texture,
		Material,
		World,				// NOT TREATED THE SAME AS OTHER ASSETS
		MeshSourceFile, // for mesh source fiel and mesh source
		PhysicsMaterial,
		TextureSourceFile,
		Font,
		FontSourceFile,
		Prefab,
		UIPanel,
		ParticleSystem
	};
	enum class AssetState 
	{
		None = 0,
		Unloaded,
		Ready,
		Loading,
		Invalid
	};
	namespace Utils {
		inline bool IsAssetSource(AssetType type)
		{
			switch (type)
			{
				case AssetType::TextureSourceFile:
				case AssetType::MeshSourceFile:
				case AssetType::FontSourceFile:
					return true;
			}
			return false;
		}
		static inline const std::unordered_map<AssetType, std::string> AssetTypeMap = {
			{AssetType::None, ""},
			{AssetType::Mesh, "Mesh.ProofAsset"},
			{AssetType::Texture, "Texture.ProofAsset"},
			{AssetType::Material, "Material.ProofAsset"},
			{AssetType::World, "ProofWorld"},
			{AssetType::MeshSourceFile, ""},
			{AssetType::PhysicsMaterial, "PhysicsMaterial.ProofAsset"},
			{AssetType::Prefab, "Prefab.ProofAsset"},
			{AssetType::TextureSourceFile, ""},
			{AssetType::UIPanel, "UIPanel.ProofAsset"},
			{AssetType::ParticleSystem, "ParticleSystem.ProofAsset"},
			{AssetType::Font, "Font.ProofAsset" }
		};
		inline std::string GetAssetExtensionString(AssetType type) 
		{
			if (AssetTypeMap.contains(type))
				return AssetTypeMap.at(type);
			PF_CORE_ASSERT(false, fmt::format("Asset Type {} does not have extension", EnumReflection::EnumString(type)).c_str());
		}
		// checks the file extension
		
	// more formats come in the future
		static const std::unordered_set<std::string> MeshSourceFormats =
		{
			"fbx",
			"obj",
			"blend",
			"gltf",
			"glb",
			"blend",
		};

		inline bool MeshHasFormat(const std::string& format) {
			return MeshSourceFormats.contains(format);

		}
		// more formats to come in the future
		static const std::unordered_set< std::string> TextureSourceFormats =
		{
			"jpeg",
			"jpg",
			"png",
			"tga",
			"big",
			"hdr"
		};

		inline bool TextureHasFormat(const std::string& format) {
			return TextureSourceFormats.contains(format);
		}
		static const std::unordered_set< std::string> FontSourceFormats =
		{
			"ttf",
		};
		inline bool FontHasFormat(const std::string& format) {
			return FontSourceFormats.contains(format);
		}

		inline AssetType GetAssetTypeFromPath(const std::filesystem::path& path)
		{
			const std::string fileFullExtension = Utils::FileDialogs::GetFullFileExtension(path);

			if (MeshHasFormat(fileFullExtension))return AssetType::MeshSourceFile;
			if (TextureHasFormat(fileFullExtension))return AssetType::TextureSourceFile;
			if (FontHasFormat(fileFullExtension))return AssetType::FontSourceFile;

			// Iterate through AssetTypeMap to find a match for the file extension
			for (const auto& pair : AssetTypeMap)
			{
				if (pair.second == fileFullExtension)
				{
					return pair.first;
				}
			}
			return AssetType::None;
		}
	}
}