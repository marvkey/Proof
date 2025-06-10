#include "Proofprch.h"
#include "AssetSerelizer.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/UIRenderer/UIMenu.h"
#include "Proof/Scene/SceneSerializer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Physics/PhysicsMaterial.h"
#include "Proof/Audio/Audio.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Scripting/ScriptFile.h"
#include "Proof/Utils/StringUtils.h"
#include "Proof/Scene/SceneSerializer.h"
#include "MeshImpoter.h"
#include "Proof/Asset/AssetCustomData/MeshSourceSavedSettings.h"
#include "Proof/Input/ElevatedInputSystem/InputAction.h"
#include "Proof/Input/ElevatedInputSystem/InputBindingContext.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedActionKeyMapping.h"
#include "SerializeCommon.h"
namespace Proof {
	void AssetSerializer::SetID(const AssetInfo& data, const Count<class Asset>& asset)
	{
		asset->m_ID = data.ID;
	}
	void TextureAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		/*
		Count<Texture2D> textureAsset = asset.As< Texture2D>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(textureAsset->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << textureAsset->GetID();
		out << YAML::Key << "AssetSource" << YAML::Value << AssetManager::GetAssetInfo(textureAsset->GetPath()).ID;
		out << YAML::EndMap;

		std::ofstream stream(AssetManager::GetAssetFileSystemPath(data.Path).string());
		stream << out.c_str();
		stream.close();
		*/
	}

	Count<class Asset> TextureAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		/*
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
		*/

		auto fullPath = AssetManager::GetAssetFileSystemPath(assetData.Path).string();
		Count<Texture2D> asset = Texture2D::Create(TextureConfiguration(FileSystem::GetFileName(fullPath)), fullPath);
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
		out << YAML::Key << "ShaderName" << YAML::Value << material->GetRenderMaterial()->GetConfig().Shader->GetName();

		out << YAML::Key << "AlbedoColour" << YAML::Value << material->GetAlbedoColor();
		out << YAML::Key << "Roughness" << YAML::Value << material->GetRoughness();
		out << YAML::Key << "Metallness" << YAML::Value << material->GetMetalness();
		out << YAML::Key << "Emission" << YAML::Value << material->GetEmission();
		out << YAML::Key << "Tiling" << YAML::Value << material->GetTiling();
		out << YAML::Key << "NormalTextureToggle" << YAML::Value << material->GetNormalTextureToggle();
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
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(data.Path).string());
		stream << out.c_str();
		stream.close();
	}
	Count<class Asset> MaterialAssetSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;

		std::string shaderName;
		Count<Material> material;

		if (data["ShaderName"] && Renderer::GetShaderLibrary()->HasShader(data["ShaderName"].as<std::string>()))
			material = Count<Material>::Create(assetData.GetName(), Renderer::GetShaderLibrary()->GetShader(data["ShaderName"].as<std::string>()));
		else
			material = Count<Material>::Create(assetData.GetName());

		material->GetAlbedoColor() = data["AlbedoColour"].as<glm::vec3>();

		material->GetMetalness() = data["Metallness"].as<float>();
		material->GetRoughness() = data["Roughness"].as<float>();
		material->GetEmission() = data["Emission"].as<float>(material->GetEmission());
		material->GetNormalTextureToggle() = data["NormalTextureToggle"].as<bool>(false);
		if (data["Tiling"])
		{
			material->GetTiling() = data["Tiling"].as<glm::vec2>();
			material->GetOffset() = data["Offset"].as<glm::vec2>();
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
				material->GetNormalTextureToggle() = data["NormalTextureToggle"].as<bool>(true);
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
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(data.Path).string());
		stream << out.c_str();
		stream.close();
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
		out << YAML::Key << "Translation" << YAML::Value << mesh->GetTranslation();
		out << YAML::Key << "RotationDeg" << YAML::Value << mesh->GetRotationDeg();
		out << YAML::Key << "Scale" << YAML::Value << mesh->GetScale();
		out << YAML::Key << "SubMeshes";
		out << YAML::Flow;

		if (mesh->GetSubMeshes().size() == mesh->GetMeshSource()->GetSubMeshes().size())
			out << YAML::Value << std::vector<uint32_t>();
		else
			out << YAML::Value << mesh->GetSubMeshes();
		out << YAML::EndMap;

		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
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

		mesh->SetTranslation(data["Translation"].as<glm::vec3>(mesh->GetTranslation()));
		mesh->SetRotationDeg(data["RotationDeg"].as<glm::vec3>(mesh->GetRotationDeg()));
		mesh->SetScale(data["Scale"].as<float>(mesh->GetScale()));

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
		out << YAML::Key << "Translation" << YAML::Value << mesh->GetTranslation();
		out << YAML::Key << "RotationDeg" << YAML::Value << mesh->GetRotationDeg();
		out << YAML::Key << "Scale" << YAML::Value << mesh->GetScale();
		out << YAML::Key << "SubMeshes";
		out << YAML::Flow;
		if (mesh->GetSubMeshes().size() == mesh->GetMeshSource()->GetSubMeshes().size())
			out << YAML::Value << std::vector<uint32_t>();
		else
			out << YAML::Value << mesh->GetSubMeshes();
		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
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
		mesh->SetTranslation(data["Translation"].as<glm::vec3>(mesh->GetTranslation()));
		mesh->SetRotationDeg(data["RotationDeg"].as<glm::vec3>(mesh->GetRotationDeg()));
		mesh->SetScale(data["Scale"].as<float>(mesh->GetScale()));

		SetID(assetData, mesh);
		return mesh;
	}
	void MeshSourceAssetSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{

	}
	Count<class Asset> MeshSourceAssetSerializer::TryLoadAsset(const AssetInfo& data) const
	{
		MeshImporter importer(AssetManager::GetAssetFileSystemPath(data.Path));

		bool meshSourceContains = MeshSourceSavedSettings::HasMeshSourceMetaData(data.ID);
		Count<MeshSource> source = importer.ImportToMeshSource();
		SetID(data, source);
		/*
		if (!meshSourceContains)
			importer.UpdateMeshSourceAssetCustomSettings(source);
		if (meshSourceContains)
		{
			const auto& meshSourceMetaData = MeshSourceSavedSettings::GetMeshSourceMetaData(data.ID);
			for (auto [materialIndex, materialID] : meshSourceMetaData.MaterialList)
			{
				if (AssetManager::HasAsset(materialID))
				{
					if (AssetManager::GetAssetInfo(materialID).Type == AssetType::Material)
						source->m_Materials->SetMaterial(materialIndex, AssetManager::GetAsset<Material>(materialID));
				}
				else
					source->m_Materials->SetMaterial(materialIndex, AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Material).As<Material>());

			}
		}
		*/
		return source;
	}

	void PrefabAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<Prefab> prefab = asset.As<Prefab>();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(prefab->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << prefab->GetID();
		out << YAML::Key << "EntityOwner" << YAML::Value << prefab->GetBaseEntity().GetUUID();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto [id, entity] : prefab->m_World->GetEntities())
		{
			SceneSerializer::SerilizeEntity(out, entity);
		}
		out << YAML::Flow;
		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
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

		SceneSerializer::DeSerilizeEntity(entities, world);

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
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
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
	void UIPanelAssetSerilizer::SaveUIElement(YAML::Emitter& out, UIElement element)const
	{
		out << YAML::BeginMap;// Element
		out << YAML::Key << "UIElement" << YAML::Value << element.GetUUID();

		{
			UICoreComponent& coreComponent = element.GetComponent<UICoreComponent>();
			out << YAML::Key << "UICoreComponent";
			out << YAML::BeginMap; // CoreComponent
			out << YAML::Key << "ElementID" << YAML::Value << coreComponent.GetElementID();
			out << YAML::Key << "Name" << YAML::Value << coreComponent.GetName();
			out << YAML::Key << "UIElementType" << YAML::Value << EnumReflection::EnumString(coreComponent.ElementType);
			out << YAML::Key << "Position" << YAML::Value << coreComponent.Transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << coreComponent.Transform.Rotation;
			out << YAML::Key << "Size" << YAML::Value << coreComponent.Transform.Size;
			out << YAML::Key << "AnchorMinimum" << YAML::Value << coreComponent.Transform.Anchor.Minimum;
			out << YAML::Key << "AnchorMaximum" << YAML::Value << coreComponent.Transform.Anchor.Maximum;
			out << YAML::EndMap; // CoreComponent
		}

		{
			if (element.HasComponent<UITextComponent>())
			{
				auto& textComponent = element.GetComponent<UITextComponent>();

				out << YAML::Key << "UITextComponent";
				out << YAML::BeginMap; // UITextComponent
				out << YAML::Key << "Text" << YAML::Value << textComponent.Text;
				out << YAML::Key << "Color" << YAML::Value << textComponent.TextConfig.Color;
				out << YAML::Key << "Kerning" << YAML::Value << textComponent.TextConfig.Kerning;
				out << YAML::Key << "LineSpacing" << YAML::Value << textComponent.TextConfig.LineSpacing;
				out << YAML::EndMap; // UITextComponent
			}
		}
		{
			if (element.HasComponent<UIProggresBarComponent>())
			{
				auto& proggressComponent = element.GetComponent<UIProggresBarComponent>();

				out << YAML::Key << "UIProggresBarComponent";
				out << YAML::BeginMap; // UIProggresBarComponent
				out << YAML::Key << "Proggress" << YAML::Value << proggressComponent.Proggress.GetValue();
				out << YAML::Key << "FillColor" << YAML::Value << proggressComponent.FillColor;
				out << YAML::Key << "BackgroundColor" << YAML::Value << proggressComponent.BackgroundColor;
				out << YAML::EndMap; // UIProggresBarComponent
			}
		}
		out << YAML::EndMap; // Element

	}
	
	void UIPanelAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<UIPanel>uiPanel = asset.As<UIPanel>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(uiPanel->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << uiPanel->GetID();
		out << YAML::Key << "UIElements" << YAML::Value << YAML::BeginSeq;
		
		{
			for (auto& [name, element] : uiPanel->Menu->GetUIElementsMap())
			{
				SaveUIElement(out, element);
			}
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
	}
	void UIPanelAssetSerilizer::DeserilizeUIElements(YAML::Node& uiElements, Count<class UIPanel> asset)const
	{
		Count<UIPanel>uiPanel = asset.As<UIPanel>();
		for (auto uiElement : uiElements)
		{
			//coreComponnet
			UIElement newUIElement;
			{
				auto coreComponent = uiElement["UICoreComponent"];
				if (coreComponent)
				{
					uint64_t id = coreComponent["ElementID"].as<uint64_t>();
					std::string name = coreComponent["Name"].as<std::string>("Empty");
					UIElementType elementType = EnumReflection::StringEnum<UIElementType>(coreComponent["UIElementType"].as<std::string>(" "));

					newUIElement = uiPanel->Menu->CreateElement(name, id, elementType);
					UITransform& transform = newUIElement.GetComponent<UICoreComponent>().Transform;
					transform.Position = coreComponent["Position"].as<glm::vec2>(transform.Position);
					transform.Rotation = coreComponent["Rotation"].as<glm::vec2>(transform.Rotation);
					transform.Size = coreComponent["Size"].as<glm::vec2>(transform.Size);
					transform.Anchor.Minimum = coreComponent["AnchorMinimum"].as<glm::vec2>(transform.Anchor.Minimum);
					transform.Anchor.Maximum = coreComponent["AnchorMaximum"].as<glm::vec2>(transform.Anchor.Maximum);
				}
			}

			{
				auto textComponent = uiElement["UITextComponent"];
				if (textComponent)
				{
					auto& src = newUIElement.GetComponent<UITextComponent>();
					src.TextConfig.Color = textComponent["Color"].as<glm::vec4>();
					src.TextConfig.Kerning = textComponent["Kerning"].as<float>();
					src.TextConfig.LineSpacing = textComponent["LineSpacing"].as<float>();
					src.Text = textComponent["Text"].as<std::string>();
				}
			}

			{
				auto proggressComponent = uiElement["UIProggresBarComponent"];
				if (proggressComponent)
				{
					auto& src = newUIElement.GetComponent<UIProggresBarComponent>();
					src.Proggress = proggressComponent["Proggress"].as<float>(src.Proggress.GetValue());
					src.FillColor = proggressComponent["FillColor"].as<glm::vec4>(src.FillColor);
					src.BackgroundColor = proggressComponent["BackgroundColor"].as<glm::vec4>(src.BackgroundColor);
				}
			}
		}
	}
	Count<class Asset> UIPanelAssetSerilizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;
	
		Count<UIPanel> uiPanel = Count<UIPanel>::Create();

		if (data["UIElements"])
		{
			auto elements = data["UIElements"];
			DeserilizeUIElements(elements, uiPanel);
		}
		SetID(assetData, uiPanel);
		return uiPanel;
	}

	

	void AudioAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		/*
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
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
		*/
	}

	Count<class Asset> AudioAssetSerilizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		/*
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
		*/

		auto fullPath = AssetManager::GetAssetFileSystemPath(assetData.Path).string();
		Count<Audio> asset = Count<Audio>::Create(fullPath);
		SetID(assetData, asset);
		return asset;
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
		out << YAML::Key << "PreviewScale" << YAML::Value << meshCollider->PreviewScale;
		out << YAML::Key << "FlipNormals" << YAML::Value << meshCollider->FlipNormals;
		out << YAML::Key << "CheckZeroAreaTriangles" << YAML::Value << meshCollider->CheckZeroAreaTriangles;
		out << YAML::Key << "AreaTestEpsilon" << YAML::Value << meshCollider->AreaTestEpsilon;
		out << YAML::Key << "ShiftVerticesToOrigin" << YAML::Value << meshCollider->ShiftVerticesToOrigin;
		out << YAML::Key << "AlwaysShareShape" << YAML::Value << meshCollider->AlwaysShareShape;
		out << YAML::Key << "CollisionComplexity" << YAML::Value << EnumReflection::EnumString(meshCollider->CollisionComplexity);

		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
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
		meshCollider->PreviewScale = data["PreviewScale"].as<glm::vec3>(glm::vec3{ 1 });
		meshCollider->CollisionComplexity = EnumReflection::StringEnum<ECollisionComplexity>( data["CollisionComplexity"].as<std::string>());

		SetID(assetData, meshCollider);
		return meshCollider;
	}

	void ScriptFileAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		auto fullPath = AssetManager::GetAssetFileSystemPath(assetData.Path);

		if (FileSystem::Exists(fullPath))
			return;

		Count<ScriptFile> scriptFile = asset.As<ScriptFile>();
		if (scriptFile->GetClasssName().empty())
			return;

		std::string newClassRawTemplate = R"(
using System;
using Proof;

namespace $NAMESPACE_NAME$
{
	public class $CLASS_NAME$ : Entity
	{
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
		}

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
		}
	}
}
)";
		const std::string finalTemplate = Utils::String::ReplaceInString(newClassRawTemplate, { {"$NAMESPACE_NAME$",scriptFile->GetClassNamespace()} , {"$CLASS_NAME$",scriptFile->GetClasssName()} });

		std::ofstream stream(fullPath.string());
		stream << finalTemplate;
		stream.close();
	}

	Count<class Asset> ScriptFileAssetSerilizer::TryLoadAsset(const AssetInfo& data) const
	{
		Count<ScriptFile> file = Count<ScriptFile>::Create();

		SetID(data, file);
		return file;
	}

	void WorldAssetSerializer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		auto fullPath = AssetManager::GetAssetFileSystemPath(assetData.Path);

		Count<World> world = asset.As<World>();
		SceneSerializer scerelizer(world);
		scerelizer.SerilizeText(fullPath.string());
	}

	Count<class Asset> WorldAssetSerializer::TryLoadAsset(const AssetInfo& data) const
	{
		PF_CORE_ASSERT(false, "Cannot World Asset from asset Manager");
		return Count<class Asset>();
	}

	void InputActionSerealizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{

		Count<InputAction> inputAction = asset.As<InputAction>();

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(inputAction->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << inputAction->GetID();
		out << YAML::Key << "OutputType" << YAML::Value << EnumReflection::EnumString( inputAction->OutputType);
		out << YAML::Key << "OutputValueBehavior" << YAML::Value << EnumReflection::EnumString(inputAction->OutputValueBehavior);

		SerializeCommon::SerializeInputInteractions(out,inputAction->Interactions);
		SerializeCommon::SerializeInputCustomizers(out, inputAction->Customizers);

		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
	}

	Count<class Asset> InputActionSerealizer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;

		Count<InputAction> inputAction = Count<InputAction>::Create();

		inputAction->OutputType = EnumReflection::StringEnum<InputActionOutputType>(data["OutputType"].as<std::string>
			(EnumReflection::EnumString(InputActionOutputType::Bool)));

		inputAction->OutputValueBehavior = EnumReflection::StringEnum<InputActionOutputValueBehavior>(data["OutputValueBehavior"].as<std::string>
			(EnumReflection::EnumString(InputActionOutputValueBehavior::MaximumAbsolute)));

		SerializeCommon::LoadInputInteractions(data, inputAction->Interactions);
		SerializeCommon::LoadInputCustomizers(data, inputAction->Customizers);

		SetID(assetData, inputAction);
		return inputAction;
	}

	void InputBindingContextSerializer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{

		Count<InputBindingContext> inputBindingContext = asset.As<InputBindingContext>();

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(inputBindingContext->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << inputBindingContext->GetID();

		out << YAML::Key << "InputBindings" << YAML::Value << YAML::BeginSeq; //InputBindings
		for (auto& binding : inputBindingContext->GetBindings())
		{
			if (!binding.InputAction)
				continue;
			out << YAML::BeginMap;// InputBinding
			out << YAML::Key << "InputActionID" << YAML::Value << binding.InputAction->GetID();
			SerializeCommon::SerializeInputKeyBindings(out, binding.KeyBindings);
			SerializeCommon::SerializeInputInteractions(out, binding.Interactions);
			SerializeCommon::SerializeInputCustomizers(out, binding.Customizers);

			out << YAML::EndMap;
		}
		out << YAML::EndSeq;//InputBindings

		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		stream << out.c_str();
		stream.close();
	}

	Count<class Asset> InputBindingContextSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node data = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!data["AssetType"])
			return nullptr;

		Count<InputBindingContext> inputBindingContext = Count<InputBindingContext>::Create();

		if (!data["InputBindings"])
		{
			SetID(assetData, inputBindingContext);
			return inputBindingContext;
		}
		
		auto inputBindings = data["InputBindings"];

		for (auto inputBinding : inputBindings)
		{
			auto assetID = inputBinding["InputActionID"].as<uint64_t>(0);
			if (!AssetManager::HasAsset(assetID))
				continue;

			auto action = AssetManager::GetAsset<InputAction>(assetID);
			ElevatedActionKeyBinding* binding =  inputBindingContext->AddOrGetBinding(action);

			SerializeCommon::DeserializeInputKeyBindings(inputBinding, binding->KeyBindings);
			SerializeCommon::LoadInputInteractions(inputBinding, binding->Interactions);
			SerializeCommon::LoadInputCustomizers(inputBinding, binding->Customizers);
		}

		out:
		SetID(assetData, inputBindingContext);
		return inputBindingContext;
	}

}