#include "Proofprch.h"
#include "AssetSerelizer.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Scene/SceneSerializer.h"
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
		material->Name = assetData.GetName();
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

		out << YAML::Key << "StaticFriction" << YAML::Value << material->GetStaticFriction();
		out << YAML::Key << "DynamicFriction" << YAML::Value << material->GetDynamicFriction();
		out << YAML::Key << "Bounciness" << YAML::Value << material->GetBounciness();

		out << YAML::Key << "FrictionCombineMode" << YAML::Value << EnumReflection::EnumString(material->GetFrictionCombineMode());
		out << YAML::Key << "BouncinessCombineMode" << YAML::Value << EnumReflection::EnumString(material->GetBouncinessCombineMode());

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

		material->SetStaticFriction(data["StaticFriction"].as<float>());
		material->SetDynamicFriction(data["DynamicFriction"].as<float>());
		material->SetBounciness(data["Bounciness"].as<float>());

		material->SetFrictionCombineMode(EnumReflection::StringEnum<CombineMode>(data["FrictionCombineMode"].as<std::string>()));
		material->SetBouncinessCombineMode(EnumReflection::StringEnum<CombineMode>(data["BouncinessCombineMode"].as<std::string>()));

		SetID(assetData, material);
		return material;
	}

	void MeshAssetSerializer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<Mesh> mesh = asset.As<Mesh>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(mesh->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << mesh->GetID();
		out << YAML::Key << "AssetSource" << YAML::Value << AssetManager::GetAssetInfo(mesh->GetMeshSource()->GetPath()).ID;
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
		uint64_t source = data["AssetSource"].as<uint64_t>();
		
		PF_CORE_ASSERT(AssetManager::HasAsset(source), "Trying to load mesh with meshSource that does not exist");
		Count<Mesh> mesh = Count<Mesh>::Create(AssetManager::GetAsset<MeshSource>(source));
		SetID(assetData, mesh);

		return mesh;
	}

	void MeshSourceAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{

	}
	Count<class Asset> MeshSourceAssetSerializer::TryLoadAsset(const AssetInfo& data) const
	{
		Count<MeshSource> source = Count<MeshSource>::Create(AssetManager::GetAssetFileSystemPath(data.Path).string());
		SetID(data, source);
		return source;
	}

	void PrefabAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<Prefab> prefab = asset.As<Prefab>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(prefab->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << prefab->GetID();
		out << YAML::Key << "EntityOwner" << YAML::Value << prefab->m_BaseEntityID;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		prefab->GetRegistry().each([&](auto entityID) {
			SceneSerializer::SerilizeEntity(out, prefab->m_Registry, entityID);
		});
		out << YAML::Flow;
		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
		found.close();
	}

	Count<class Asset> PrefabAssetSerilizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;
		Count<Prefab> prefab = Count<Prefab>::Create();
		UUID owernID= data["EntityOwner"].as<uint64_t>();
		auto entities = data["Entities"];

		Count<World> world = Count<World>::Create("prefab world");

		SceneSerializer::DeSerilizeEntity(entities, world.Get(), nullptr);

		prefab->SetEntity(Entity{ owernID,world.Get() });
		SetID(assetData, prefab);


		return prefab;
	}

}
