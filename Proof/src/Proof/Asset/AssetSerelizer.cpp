#include "Proofprch.h"
#include "AssetSerelizer.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Scene/SceneSerializer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Physics/PhysicsMaterial.h"
#include "Proof/Audio/Audio.h"
#include "Proof/Physics/MeshCollider.h"
#include "MeshImpoter.h"
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
		auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(source).Path);
		//m_Texture = Texture2D::Create(path.string());

		Count<Texture2D> asset = Texture2D::Create(TextureConfiguration(FileSystem::GetFileName(path)), path.string());
		SetID(assetData, asset);
		return asset;
	}
	void MaterialAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		Count<Material> material = asset.As<Material>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(material->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << material->GetID();

		out << YAML::Key << "AlbedoColour" << YAML::Value << material->GetAlbedoColor();
		out << YAML::Key << "Roughness" << YAML::Value << material->GetRoughness();
		out << YAML::Key << "Metallness" << YAML::Value << material->GetMetalness();
		out << YAML::Key << "Tiling" << YAML::Value << material->GetTiling();
		out << YAML::Key << "Offset" << YAML::Value << material->GetOffset();

		if (material->GetAlbedoMap() && material->GetAlbedoMap() != Renderer::GetWhiteTexture())
			out << YAML::Key << "AlbedoTexture" << YAML::Value << material->GetAlbedoMap()->GetID();
		else
			out << YAML::Key << "AlbedoTexture" << YAML::Value << 0;

		if (material->GetNormalMap() && material->GetNormalMap() != Renderer::GetWhiteTexture())
			out << YAML::Key << "NormalTexture" << YAML::Value << material->GetNormalMap()->GetID();
		else
			out << YAML::Key << "NormalTexture" << YAML::Value << 0;

		if (material->GetMetalnessMap() && material->GetMetalnessMap() != Renderer::GetWhiteTexture())
			out << YAML::Key << "MetallicTexture" << YAML::Value << material->GetMetalnessMap()->GetID();
		else
			out << YAML::Key << "MetallicTexture" << YAML::Value << 0;

		if (material->GetRoughnessMap() && material->GetRoughnessMap() != Renderer::GetWhiteTexture())
			out << YAML::Key << "RoughnessTexture" << YAML::Value << material->GetRoughnessMap()->GetID();
		else
			out << YAML::Key << "RoughnessTexture" << YAML::Value << 0;


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
		Count<Material> material = Count<Material>::Create(assetData.GetName());
		material->GetAlbedoColor() = data["AlbedoColour"].as<Vector>();

		material->GetMetalness() = data["Metallness"].as<float>();
		material->GetRoughness() = data["Roughness"].as<float>();

		if (data["Tiling"])
		{
			material->GetTiling() = data["Tiling"].as<Vector2>();
			material->GetOffset() = data["Offset"].as<Vector2>();
		}

		if (data["AlbedoTexture"])
		{
			uint64_t id = data["AlbedoTexture"].as<uint64_t>();
			if (AssetManager::HasAsset(id))
			{
				material->SetAlbedoMap(AssetManager::GetAsset<Texture2D>(id));
			}
		}
		if (data["NormalTexture"])
		{
			uint64_t id = data["NormalTexture"].as<uint64_t>();
			if (AssetManager::HasAsset(id))
			{
				material->SetNormalMap(AssetManager::GetAsset<Texture2D>(id));
			}
		}

		if (data["MetallicTexture"])
		{
			uint64_t id = data["MetallicTexture"].as<uint64_t>();
			if (AssetManager::HasAsset(id))
			{
				material->SetMetalnessMap(AssetManager::GetAsset<Texture2D>(id));
			}
		}


		if (data["RoughnessTexture"])
		{
			uint64_t id = data["RoughnessTexture"].as<uint64_t>();
			if (AssetManager::HasAsset(id))
			{
				material->SetRoughnessMap(AssetManager::GetAsset<Texture2D>(id));
			}
		}


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
		out << YAML::Key << "AssetSource" << YAML::Value << mesh->GetMeshSource()->GetID();
		out << YAML::Key << "SubMeshes";
		out << YAML::Flow;

		if (mesh->GetSubMeshes().size() == mesh->GetMeshSource()->GetSubMeshes().size())
			out << YAML::Value << std::vector<uint32_t>();
		else
			out << YAML::Value << mesh->GetSubMeshes();
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

		auto submeshIndices = data["SubMeshes"].as<std::vector<uint32_t>>(std::vector<uint32_t>());

		Count<Mesh> mesh = Count<Mesh>::Create(AssetManager::GetAsset<MeshSource>(source), submeshIndices);
		SetID(assetData, mesh);
		return mesh;
	}
	void DynamicMeshAssetSerializer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{

		Count<DynamicMesh> mesh = asset.As<DynamicMesh>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(mesh->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << mesh->GetID();
		out << YAML::Key << "AssetSource" << YAML::Value << mesh->GetMeshSource()->GetID();
		out << YAML::Key << "SubMeshes";
		out << YAML::Flow;
		if (mesh->GetSubMeshes().size() == mesh->GetMeshSource()->GetSubMeshes().size())
			out << YAML::Value << std::vector<uint32_t>();
		else
			out << YAML::Value << mesh->GetSubMeshes();
		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
		found.close();
	}

	Count<class Asset> DynamicMeshAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"]) // if there is no scene no
			return nullptr;
		uint64_t source = data["AssetSource"].as<uint64_t>();

		PF_CORE_ASSERT(AssetManager::HasAsset(source), "Trying to load mesh with meshSource that does not exist");
		auto submeshIndices = data["SubMeshes"].as<std::vector<uint32_t>>(std::vector<uint32_t>());
		Count<DynamicMesh> mesh = Count<DynamicMesh>::Create(AssetManager::GetAsset<MeshSource>(source), submeshIndices);
		SetID(assetData, mesh);
		return mesh;
	}
	void MeshSourceAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{

	}
	Count<class Asset> MeshSourceAssetSerializer::TryLoadAsset(const AssetInfo& data) const
	{
		MeshImporter importer(AssetManager::GetAssetFileSystemPath(data.Path));
		Count<MeshSource> source = importer.ImportToMeshSource();
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
		out << YAML::Key << "EntityOwner" << YAML::Value << prefab->m_BaseEntity.GetUUID();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto [id, entity] : prefab->m_World->GetEntities())
		{
			SceneSerializer::SerilizeEntity(out, prefab->m_World->m_Registry, entity.GetUUID(),(entt::entity)entity, true);
		}
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
		UUID owernID = data["EntityOwner"].as<uint64_t>();
		auto entities = data["Entities"];

		Count<World> world = Count<World>::Create("prefab world");

		SceneSerializer::DeSerilizeEntity(entities, world.Get(), nullptr, true);

		prefab->SetEntity(world->GetEntity(owernID));
		SetID(assetData, prefab);

		return prefab;
	}
	void ParticleSystemSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<ParticleSystem> particleSystem = asset.As<ParticleSystem>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(particleSystem->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << particleSystem->GetID();
		out << YAML::Key << "Velocity" << YAML::Value << particleSystem->Velocity;
		out << YAML::Key << "VelocityVariation" << YAML::Value << particleSystem->VelocityVariation;
		out << YAML::Key << "PlayOnAwake" << YAML::Value << particleSystem->PlayOnAwake;
		out << YAML::Key << "ColorBegin" << YAML::Value << particleSystem->ColorBegin;
		out << YAML::Key << "ColorEnd" << YAML::Value << particleSystem->ColorEnd;
		out << YAML::Key << "SizeBegin" << YAML::Value << particleSystem->SizeBegin;
		out << YAML::Key << "SizeEnd" << YAML::Value << particleSystem->SizeEnd;
		out << YAML::Key << "SizeVariation" << YAML::Value << particleSystem->SizeVariation;
		out << YAML::Key << "LifeTime" << YAML::Value << particleSystem->LifeTime;
		out << YAML::Key << "MaxParticles" << YAML::Value << particleSystem->MaxParticles;
		out << YAML::Key << "Loop" << YAML::Value << particleSystem->Loop;
		out << YAML::Key << "Use3D" << YAML::Value << particleSystem->Use3D;
		out << YAML::Key << "SizeBegin3D" << YAML::Value << particleSystem->SizeBegin3D;
		out << YAML::Key << "SizeEnd3D" << YAML::Value << particleSystem->SizeEnd3D;
		out << YAML::Key << "SizeVariation3D" << YAML::Value << particleSystem->SizeVariation3D;
		out << YAML::Key << "Rotation3D" << YAML::Value << particleSystem->Rotation3D;
		if (particleSystem->Texture != nullptr)
			out << YAML::Key << "TextureID" << YAML::Value << (uint64_t)particleSystem->Texture->GetID();
		else
			out << YAML::Key << "TextureID" << YAML::Value << 0;
		//
		{
			out << YAML::Key << "ParticleEmissionEnable" << YAML::Value << particleSystem->Emision.Enabled;
			out << YAML::Key << "ParticleEmissionParticleOverTime" << YAML::Value << particleSystem->Emision.ParticleOverTime;
			out << YAML::Key << "ParticleEmissionSpawnRateDistance" << YAML::Value << particleSystem->Emision.SpawnRateDistance;

		}
		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
		found.close();
	}
	Count<class Asset> ParticleSystemSerilizer::TryLoadAsset(const AssetInfo& assetData)const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;
		Count<ParticleSystem> particleSystem = Count<ParticleSystem>::Create();

		particleSystem->Velocity = data["Velocity"].as<Vector>();
		particleSystem->VelocityVariation = data["VelocityVariation"].as<Vector>();
		particleSystem->ColorBegin = data["ColorBegin"].as<glm::vec4>();
		particleSystem->ColorEnd = data["ColorEnd"].as<glm::vec4>();
		particleSystem->SizeBegin = data["SizeBegin"].as<float>();
		particleSystem->SizeEnd = data["SizeEnd"].as<float>();
		particleSystem->SizeVariation = data["SizeVariation"].as<float>();
		particleSystem->LifeTime = data["LifeTime"].as<float>();
		particleSystem->MaxParticles = data["MaxParticles"].as<uint32_t>();
		particleSystem->Loop = data["Loop"].as<bool>();
		particleSystem->PlayOnAwake = data["PlayOnAwake"].as<bool>();
		if (data["Use3D"])
		{

			particleSystem->Use3D = data["Use3D"].as<bool>();
			particleSystem->SizeBegin3D = data["SizeBegin3D"].as<Vector>();
			particleSystem->SizeEnd3D = data["SizeEnd3D"].as<Vector>();
			particleSystem->SizeVariation3D = data["SizeVariation3D"].as<Vector>();
			particleSystem->Rotation3D = data["Rotation3D"].as<Vector>();
		}

		uint64_t id = data["TextureID"].as<uint64_t>();
		if (AssetManager::HasAsset(id))
		{
			particleSystem->Texture = AssetManager::GetAsset<Texture2D>(id);
		}

		//partilce emmision
		{
			particleSystem->Emision.Enabled = data["ParticleEmissionEnable"].as<bool>();
			particleSystem->Emision.ParticleOverTime = data["ParticleEmissionParticleOverTime"].as<uint32_t>();
			particleSystem->Emision.SpawnRateDistance = data["ParticleEmissionSpawnRateDistance"].as<float>();

		}
		SetID(assetData, particleSystem);
		return particleSystem;
	}
	void UIPanelAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<UIPanel>uiPanel = asset.As<UIPanel>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(uiPanel->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << uiPanel->GetID();
		{
			out << YAML::Key << "Buttons" << YAML::Value << YAML::BeginSeq;
			// Buttons
			for (auto& [name, button] : uiPanel->GetButtons())
			{

				out << YAML::BeginMap;// Button
				out << YAML::Key << "Button" << name;
				out << YAML::Key << "Position" << button.Postion;
				out << YAML::Key << "Size" << button.Size;
				out << YAML::Key << "Rotation" << button.Rotation;
				out << YAML::Key << "Text" << button.Text;
				out << YAML::Key << "TintColor" << button.TintColour;
				out << YAML::Key << "Visible" << button.Visible;
				out << YAML::EndMap;// Button
			}
			out << YAML::EndSeq;
		}

		{
			out << YAML::Key << "ImageButtons" << YAML::Value << YAML::BeginSeq;
			// Buttons
			for (auto& [name, imageButton] : uiPanel->GetImageButtons())
			{

				out << YAML::BeginMap;// Button
				out << YAML::Key << "ImageButton" << name;
				out << YAML::Key << "Position" << imageButton.Postion;
				out << YAML::Key << "Size" << imageButton.Size;
				out << YAML::Key << "Rotation" << imageButton.Rotation;
				out << YAML::Key << "TintColor" << imageButton.TintColour;
				out << YAML::Key << "Visible" << imageButton.Visible;

				AssetID id = (imageButton.Texture == nullptr) ? AssetID(0) : imageButton.Texture->GetID();
				out << YAML::Key << "ImageAssetID" << (uint64_t)id;
				out << YAML::EndMap;// Button
			}
			out << YAML::EndSeq;
		}

		{
			out << YAML::Key << "Texts" << YAML::Value << YAML::BeginSeq;
			// Texts
			for (auto& [name, texts] : uiPanel->GetTexts())
			{

				out << YAML::BeginMap;// Text
				out << YAML::Key << "Text" << name;
				out << YAML::Key << "Position" << texts.Postion;
				out << YAML::Key << "Size" << texts.Size;
				out << YAML::Key << "Rotation" << texts.Rotation;
				out << YAML::Key << "Color" << texts.Param.Color;
				out << YAML::Key << "Input" << texts.Text;
				out << YAML::Key << "Kerning" << texts.Param.Kerning;
				out << YAML::Key << "LineSpacing" << texts.Param.LineSpacing;
				out << YAML::Key << "Visible" << texts.Visible;
				AssetID id = (texts.Font == nullptr) ? AssetID(0) : texts.Font->GetID();
				out << YAML::Key << "Font" << (uint64_t)id;
				out << YAML::EndMap;// Text
			}
			out << YAML::EndSeq;
		}
		out << YAML::EndMap;

		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
		found.close();
	}

	Count<class Asset> UIPanelAssetSerilizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;

		Count<UIPanel> uiPanel = Count<UIPanel>::Create();
		uiPanel->Name = assetData.GetName();
		auto buttons = data["Buttons"];
		for (auto button : buttons)
		{
			UIButton uiButton;
			uiButton.Postion = button["Position"].as<glm::vec2>();
			uiButton.Size = button["Size"].as<glm::vec2>();
			uiButton.Rotation = button["Rotation"].as<glm::vec2>();
			uiButton.Text = button["Text"].as < std::string >();
			uiButton.TintColour = button["TintColor"].as < glm::vec4>();
			if (button["Visible"])
			{
				uiButton.Visible = button["Visible"].as<bool>();
			}
			std::string name = button["Button"].as<std::string>();
			uiPanel->SetButton(uiButton, name);
		}

		auto imageButtons = data["ImageButtons"];

		for (auto imageButton : imageButtons)
		{
			UIButtonImage uiImageButton;
			uiImageButton.Postion = imageButton["Position"].as<glm::vec2>();
			uiImageButton.Size = imageButton["Size"].as<glm::vec2>();
			uiImageButton.Rotation = imageButton["Rotation"].as<glm::vec2>();
			uiImageButton.TintColour = imageButton["TintColor"].as < glm::vec4>();
			if (imageButton["Visible"])
			{
				uiImageButton.Visible = imageButton["Visible"].as<bool>();
			}
			AssetID textureId = imageButton["ImageAssetID"].as<uint64_t>();

			if (AssetManager::HasAsset(textureId))
			{
				uiImageButton.Texture = AssetManager::GetAsset<Texture2D>(textureId);
			}
			std::string name = imageButton["ImageButton"].as<std::string>();
			uiPanel->SetButtonImage(uiImageButton, name);
		}

		auto texts = data["Texts"];

		for (auto text : texts)
		{
			UIText uiText;
			uiText.Postion = text["Position"].as<glm::vec2>();
			uiText.Size = text["Size"].as<glm::vec2>();
			uiText.Rotation = text["Rotation"].as<glm::vec2>();
			uiText.Param.Color = text["Color"].as < glm::vec4>();
			uiText.Text = text["Input"].as < std::string>();
			uiText.Param.Kerning = text["Kerning"].as < float>();
			uiText.Param.LineSpacing = text["LineSpacing"].as<float>();
			if (text["Visible"])
			{
				uiText.Visible = text["Visible"].as<bool>();
			}
			AssetID fontId = text["Font"].as<uint64_t>();

			if (AssetManager::HasAsset(fontId))
			{
				uiText.Font = AssetManager::GetAsset<Font>(fontId);
			}
			std::string name = text["Text"].as<std::string>();
			uiPanel->SetText(uiText, name);
		}
		SetID(assetData, uiPanel);
		return uiPanel;

	}

	void AudioAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<Audio>audio = asset.As<Audio>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(audio->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << audio->GetID();
		if (AssetManager::HasAsset(audio->GetPath()))
		{
			out << YAML::Key << "AudioSource" << YAML::Value << AssetManager::GetAssetInfo(audio->GetPath()).ID;
		}
		else
		{
			out << YAML::Key << "AudioSource" << YAML::Value << 0;
		}
		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
	}

	Count<class Asset> AudioAssetSerilizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;

		Count<Audio> audio;
		
		uint64_t sourceID = data["AudioSource"].as<uint64_t>();
		
		if (AssetManager::HasAsset(sourceID))
		{
			audio = Count<Audio>::Create(AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(sourceID).Path));
		}
		else
		{
			return nullptr;
		}
		
		SetID(assetData, audio);
		return audio;
	}

	

	void MeshColliderAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<MeshCollider> meshCollider = asset.As<MeshCollider>();

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(meshCollider->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << meshCollider->GetID();
		out << YAML::Key << "ColliderMesh" << YAML::Value << meshCollider->ColliderMesh;
		out << YAML::Key << "EnableVertexWelding" << YAML::Value << meshCollider->EnableVertexWelding;
		out << YAML::Key << "VertexWeldTolerance" << YAML::Value << meshCollider->VertexWeldTolerance;
		out << YAML::Key << "FlipNormals" << YAML::Value << meshCollider->FlipNormals;
		out << YAML::Key << "CheckZeroAreaTriangles" << YAML::Value << meshCollider->CheckZeroAreaTriangles;
		out << YAML::Key << "AreaTestEpsilon" << YAML::Value << meshCollider->AreaTestEpsilon;
		out << YAML::Key << "ShiftVerticesToOrigin" << YAML::Value << meshCollider->ShiftVerticesToOrigin;
		out << YAML::Key << "AlwaysShareShape" << YAML::Value << meshCollider->AlwaysShareShape;
		out << YAML::Key << "CollisionComplexity" << YAML::Value << EnumReflection::EnumString(meshCollider->CollisionComplexity);

		out << YAML::EndMap;
		std::ofstream found(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		found << out.c_str();
	}

	Count<class Asset> MeshColliderAssetSerilizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;

		Count<MeshCollider> meshCollider = Count<MeshCollider>::Create();

		meshCollider->ColliderMesh = data["ColliderMesh"].as<uint64_t>();
		meshCollider->EnableVertexWelding = data["EnableVertexWelding"].as<bool>();
		meshCollider->VertexWeldTolerance = data["VertexWeldTolerance"].as<float>();
		meshCollider->FlipNormals = data["FlipNormals"].as<bool>();
		meshCollider->CheckZeroAreaTriangles = data["CheckZeroAreaTriangles"].as<bool>();
		meshCollider->AreaTestEpsilon = data["AreaTestEpsilon"].as<float>();
		meshCollider->ShiftVerticesToOrigin = data["ShiftVerticesToOrigin"].as<bool>();
		meshCollider->AlwaysShareShape = data["AlwaysShareShape"].as<bool>();
		meshCollider->CollisionComplexity = EnumReflection::StringEnum<ECollisionComplexity>( data["CollisionComplexity"].as<std::string>());

		SetID(assetData, meshCollider);
		return meshCollider;
	}

}