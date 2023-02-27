#include "Proofprch.h"
#include "AssetSerelizer.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scene/Mesh.h"
namespace Proof {
	void AssetSerializer::SetID(const AssetInfo& data, const Count<class Asset>& asset)
	{
		asset->m_ID = data.ID;
	}
	void TextureAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		Count<Texture2D> textureAsset = asset.As< Texture2D>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(textureAsset->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << textureAsset->GetID();
		out << YAML::Key << "AssetSource" << YAML::Value << AssetManager::GetAssetInfo(textureAsset->GetPath()).ID;
		out << YAML::EndMap;

		std::ofstream found(AssetManager::GetAssetFileSystemPath(data.Path).string());
		found << out.c_str();
		found.close();
	}

	Count<class Asset> TextureAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());

		if (!data["AssetType"]) // if there is no scene no
			return nullptr;

		//m_AssetID = data["ID"].as<uint64_t>();
		uint64_t source = data["AssetSource"].as<uint64_t>();
		auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(source).Path.string());
		//m_Texture = Texture2D::Create(path.string());

		Count<Texture2D> asset = Texture2D::Create(path.string());
		SetID(assetData, asset);
		return asset;
	}
	void MaterialAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		auto material = asset.As<Material>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(material->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << material->GetID();

		out << YAML::Key << "AlbedoColour" << YAML::Value << material->Colour;
		out << YAML::Key << "Roughness" << YAML::Value << material->Roughness;
		out << YAML::Key << "Metallness" << YAML::Value << material->Metallness;
		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(data.Path).string());
		found << out.c_str();
		found.close();
	}
	Count<class Asset> MaterialAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"]) 
			return nullptr;
		Count<Material> material = Count<Material>::Create();
		//m_AssetID = data["ID"].as<uint64_t>();
		material->Colour = data["AlbedoColour"].as<Vector>();

		material->Metallness = data["Metallness"].as<float>();
		material->Roughness = data["Roughness"].as<float>();

		SetID(assetData, material);
		return material;
	}

	void PhysicsMaterialAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		Count<PhysicsMaterial> material = asset.As<PhysicsMaterial>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(material->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << material->GetID();

		out << YAML::Key << "StaticFriction" << YAML::Value << material->StaticFriction;
		out << YAML::Key << "DynamicFriction" << YAML::Value << material->DynamicFriction;
		out << YAML::Key << "Bounciness" << YAML::Value << material->Bounciness;

		out << YAML::Key << "FrictionCombineMode" << YAML::Value << EnumReflection::EnumString(material->FrictionCombineMode);
		out << YAML::Key << "BouncinessCombineMode" << YAML::Value << EnumReflection::EnumString(material->BouncinessCombineMode);

		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(data.Path).string());
		found << out.c_str();
		found.close();
	}
	Count<class Asset> PhysicsMaterialAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());

		if (!data["AssetType"]) // if there is no scene no
			return nullptr;

		//m_AssetID = data["ID"].as<uint64_t>();
		Count<PhysicsMaterial> material = Count<PhysicsMaterial>::Create();

		material->StaticFriction = data["StaticFriction"].as<float>();
		material->DynamicFriction = data["DynamicFriction"].as<float>();
		material->Bounciness = data["Bounciness"].as<float>();

		material->FrictionCombineMode = EnumReflection::StringEnum<CombineMode>(data["FrictionCombineMode"].as<std::string>());
		material->BouncinessCombineMode = EnumReflection::StringEnum<CombineMode>(data["BouncinessCombineMode"].as<std::string>());

		SetID(assetData, material);
		return Count<class Asset>();
	}

	void MeshAssetSerializer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<Mesh> mesh = asset.As<Mesh>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(mesh->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << mesh->GetID();
		out << YAML::Key << "AssetSource" << YAML::Value << AssetManager::GetAssetInfo(mesh->GetPath()).ID;
		out << YAML::Key << "SubMeshes";
		out << YAML::Flow;
	
		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
		found.close();
	}

	Count<class Asset> MeshAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"]) // if there is no scene no
			return nullptr;
		//m_AssetID = data["ID"].as<uint64_t>();
		uint64_t source = data["AssetSource"].as<uint64_t>();
		
		auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(source).Path);
		Count<Mesh> mesh = Count<Mesh>::Create(path.string());

		return mesh;
	}

}
