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
#include "Proof/Renderer/ParticleSystem/ParticleSystem.h"
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
#include "Proof/Animation/Animation.h"
#include "Proof/Animation/AnimationController.h"
#include "Proof/Animation/Skeleton.h"
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

		auto renderMaterial = material->GetRenderMaterial();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(material->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << material->GetID();
		out << YAML::Key << "ShaderName" << YAML::Value << material->GetRenderMaterial()->GetConfig().Shader->GetName();

		if (renderMaterial->GetConfig().Shader->GetName() == "ProofPBR_Static" || renderMaterial->GetConfig().Shader->GetName() == "ProofPBRTransparent_Static")
		{
			PbrSurfaceMaterial surfaceMat(material);

			out << YAML::Key << "AlbedoColour" << YAML::Value << surfaceMat.GetAlbedoColor();
			out << YAML::Key << "Roughness" << YAML::Value << surfaceMat.GetRoughness();
			out << YAML::Key << "Metallness" << YAML::Value << surfaceMat.GetMetalness();
			out << YAML::Key << "Emission" << YAML::Value << surfaceMat.GetEmission();
			out << YAML::Key << "Tiling" << YAML::Value << surfaceMat.GetTiling();
			out << YAML::Key << "NormalTextureToggle" << YAML::Value << surfaceMat.GetNormalTextureToggle();
			out << YAML::Key << "Offset" << YAML::Value << surfaceMat.GetOffset();

			if (surfaceMat.GetAlbedoMap() && surfaceMat.GetAlbedoMap() != Renderer::GetWhiteTexture())
				out << YAML::Key << "AlbedoTexture" << YAML::Value << surfaceMat.GetAlbedoMap()->GetID();
			else
				out << YAML::Key << "AlbedoTexture" << YAML::Value << 0;

			if (surfaceMat.GetNormalMap() && surfaceMat.GetNormalMap() != Renderer::GetWhiteTexture())
				out << YAML::Key << "NormalTexture" << YAML::Value << surfaceMat.GetNormalMap()->GetID();
			else
				out << YAML::Key << "NormalTexture" << YAML::Value << 0;

			if (surfaceMat.GetMetalnessMap() && surfaceMat.GetMetalnessMap() != Renderer::GetWhiteTexture())
				out << YAML::Key << "MetallicTexture" << YAML::Value << surfaceMat.GetMetalnessMap()->GetID();
			else
				out << YAML::Key << "MetallicTexture" << YAML::Value << 0;

			if (surfaceMat.GetRoughnessMap() && surfaceMat.GetRoughnessMap() != Renderer::GetWhiteTexture())
				out << YAML::Key << "RoughnessTexture" << YAML::Value << surfaceMat.GetRoughnessMap()->GetID();
			else
				out << YAML::Key << "RoughnessTexture" << YAML::Value << 0;
			  
			out << YAML::EndMap;
		}
		else
		{
			auto var = material->GetMaterialVariables();

			for (auto [name, type] : var)
			{
				out << YAML::Key << name;
				switch (type)
				{
				case VariableTypes::Bool:
					out << YAML::Value << material->GetRenderMaterial()->GetBool(name);
					break;
				case VariableTypes::Int:
					out << YAML::Value << material->GetRenderMaterial()->GetInt(name);
					break;
				case VariableTypes::Float:
					out << YAML::Value << material->GetRenderMaterial()->GetFloat(name);
					break;
				case VariableTypes::Vec2:
					out << YAML::Value << material->GetRenderMaterial()->GetVector2(name);
					break;
				case VariableTypes::Vec3:
					out << YAML::Value << material->GetRenderMaterial()->GetVector(name);
					break;
				case VariableTypes::Vec4:
					out << YAML::Value << material->GetRenderMaterial()->GetVector4(name);
					break;
				default:
					PF_CORE_ASSERT(false);
					break;
				}
			}


			auto editableTextures = material->GetEditableTextures();

			for(const auto& name : editableTextures)
			{
				out << YAML::Key << name;
				auto texture = material->GetRenderMaterial()->TryGetTexture2D(name);
				if (texture && AssetManager::HasAsset(texture))
					out << YAML::Value << texture->GetID();
				else
					out << YAML::Value << 0;
			}

		}
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

		auto renderMaterial = material->GetRenderMaterial();
		if (renderMaterial->GetConfig().Shader->GetName() == "ProofPBR_Static" || renderMaterial->GetConfig().Shader->GetName() == "ProofPBRTransparent_Static")
		{
			PbrSurfaceMaterial surfaceMat(material);

			surfaceMat.GetAlbedoColor() = data["AlbedoColour"].as<glm::vec3>();

			surfaceMat.GetMetalness() = data["Metallness"].as<float>();
			surfaceMat.GetRoughness() = data["Roughness"].as<float>();
			surfaceMat.GetEmission() = data["Emission"].as<float>(surfaceMat.GetEmission());
			surfaceMat.GetNormalTextureToggle() = data["NormalTextureToggle"].as<bool>(false);
			if (data["Tiling"])
			{
				surfaceMat.GetTiling() = data["Tiling"].as<glm::vec2>();
				surfaceMat.GetOffset() = data["Offset"].as<glm::vec2>();
			}

			if (data["AlbedoTexture"])
			{
				uint64_t id = data["AlbedoTexture"].as<uint64_t>();
				if (AssetManager::HasAsset(id))
				{
					surfaceMat.SetAlbedoMap(AssetManager::GetAsset<Texture2D>(id));
				}
			}
			if (data["NormalTexture"])
			{
				uint64_t id = data["NormalTexture"].as<uint64_t>();
				if (AssetManager::HasAsset(id))
				{
					surfaceMat.SetNormalMap(AssetManager::GetAsset<Texture2D>(id));
					surfaceMat.GetNormalTextureToggle() = data["NormalTextureToggle"].as<bool>(true);
				}
			}

			if (data["MetallicTexture"])
			{
				uint64_t id = data["MetallicTexture"].as<uint64_t>();
				if (AssetManager::HasAsset(id))
				{
					surfaceMat.SetMetalnessMap(AssetManager::GetAsset<Texture2D>(id));
				}
			}


			if (data["RoughnessTexture"])
			{
				uint64_t id = data["RoughnessTexture"].as<uint64_t>();
				if (AssetManager::HasAsset(id))
				{
					surfaceMat.SetRoughnessMap(AssetManager::GetAsset<Texture2D>(id));
				}
			}
		}
		else
		{
			auto variables = material->GetMaterialVariables();

			for(auto& [name, varType] : variables)
			{
				if (data[name])
				{
					switch (varType)
					{
					case VariableTypes::Bool:
						renderMaterial->Set(name, data[name].as<bool>());
						break;
					case VariableTypes::Int:
						renderMaterial->Set(name, data[name].as<int>());
						break;
					case VariableTypes::Float:
						renderMaterial->Set(name, data[name].as<float>());
						break;
					case VariableTypes::Vec2:
						renderMaterial->Set(name, data[name].as<glm::vec2>());
						break;
					case VariableTypes::Vec3:
						renderMaterial->Set(name, data[name].as<glm::vec3>());
						break;
					case VariableTypes::Vec4:
						renderMaterial->Set(name, data[name].as<glm::vec4>());
						break;
					default:
						PF_CORE_ASSERT(false);
						break;
					}
				}
			}

			auto textures = material->GetEditableTextures();

			for(const auto& name : textures)
			{
				if (data[name])
				{
					uint64_t id = data[name].as<uint64_t>();
					if (AssetManager::HasAssetAndAssetType(id,AssetType::Texture))
					{
						renderMaterial->Set(name, AssetManager::GetAsset<Texture2D>(id));
					}
				}
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
		out << YAML::BeginMap; // particle system

		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(particleSystem->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << particleSystem->GetID();

		// Serialize emitters
		out << YAML::Key << "Emitters" << YAML::Value << YAML::BeginSeq; // emitters
		for (size_t i = 0; i < particleSystem->GetEmitterCount(); ++i)
		{
			auto emitter = particleSystem->GetEmitter(i);
			out << YAML::BeginMap;
			out << YAML::Key << "MaxParticles" << YAML::Value << emitter->GetParticleCount();

			// Serialize Initial State
			const auto& init = emitter->ParticleInitialState;
			out << YAML::Key << "InitialState" << YAML::BeginMap;
			out << YAML::Key << "Duration" << YAML::Value << init.Duration;
			out << YAML::Key << "bLooping" << YAML::Value << init.bLooping;
			out << YAML::Key << "StartLifetime" << YAML::Value << init.StartLifetime;
			out << YAML::Key << "StartSpeed" << YAML::Value << init.StartSpeed;
			out << YAML::Key << "StartSize" << YAML::Value << init.StartSize;
			out << YAML::Key << "GravityModifier" << YAML::Value << init.GravityModifier;
			out << YAML::Key << "StartColor" << YAML::Value << init.StartColor;
			out << YAML::Key << "EmitterPosition" << YAML::Value << init.EmitterPosition;
			out << YAML::Key << "FadeOutSpeed" << YAML::Value << init.FadeOutSpeed;
			out << YAML::Key << "EmitterPrevPosition" << YAML::Value << init.EmitterPrevPosition;
			out << YAML::EndMap; // initial state

			const auto& bursts = emitter->Bursts; // or emitter->GetBursts()
			out << YAML::Key << "Bursts" << YAML::Value << YAML::BeginSeq;
			for (const auto& b : bursts)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "StartTime" << YAML::Value << b.StartTime;
				out << YAML::Key << "Count" << YAML::Value << b.Count;
				out << YAML::Key << "Cycles" << YAML::Value << b.Cycles;
				out << YAML::Key << "Interval" << YAML::Value << b.Interval;
				out << YAML::Key << "Probability" << YAML::Value << b.Probability;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;

			// Serialize Emitter Settings
			const auto& settings = emitter->ParticleEmitterSettings;

			// Emission
			out << YAML::Key << "Emission" << YAML::BeginMap;
			out << YAML::Key << "ParticlesPerSecond" << YAML::Value << settings.Emission.ParticlesPerSecond;
			out << YAML::Key << "ParticlesPerDistance" << YAML::Value << settings.Emission.ParticlesPerDistance;
			out << YAML::EndMap;

			// Shape
			out << YAML::Key << "Shape" << YAML::BeginMap;
			out << YAML::Key << "Shape" << YAML::Value << settings.Shape.Shape;
			out << YAML::Key << "RandomizeDirection" << YAML::Value << settings.Shape.RandomizeDirection;
			out << YAML::Key << "SpherizeDirection" << YAML::Value << settings.Shape.SpherizeDirection;
			out << YAML::Key << "RandomizePosition" << YAML::Value << settings.Shape.RandomizePosition;
			out << YAML::Key << "bEnabled" << YAML::Value << settings.Shape.bEnabled;
			out << YAML::Key << "ConeRadius" << YAML::Value << settings.Shape.ConeRadius;
			out << YAML::Key << "ConeAngleDegrees" << YAML::Value << settings.Shape.ConeAngleDegrees;
			out << YAML::Key << "SphereRadius" << YAML::Value << settings.Shape.SphereRadius;
			out << YAML::EndMap;

			// Velocity Over Lifetime
			out << YAML::Key << "VelocityOverLifeTime" << YAML::BeginMap;
			out << YAML::Key << "Linear" << YAML::Value << settings.VelocityOverLifeTime.Linear;
			out << YAML::Key << "SpeedModifier" << YAML::Value << settings.VelocityOverLifeTime.SpeedModifier;
			out << YAML::Key << "Orbital" << YAML::Value << settings.VelocityOverLifeTime.Orbital;
			out << YAML::Key << "Radial" << YAML::Value << settings.VelocityOverLifeTime.Radial;
			out << YAML::Key << "Offset" << YAML::Value << settings.VelocityOverLifeTime.Offset;
			out << YAML::Key << "bEnabled" << YAML::Value << settings.VelocityOverLifeTime.bEnabled;
			out << YAML::EndMap;

			// Color Over Lifetime
			out << YAML::Key << "ColorOverLifeTime" << YAML::BeginMap;
			out << YAML::Key << "bEnabled" << YAML::Value << settings.ColorOverLifeTime.bEnabled;
			out << YAML::Key << "FinalColor" << YAML::Value << settings.ColorOverLifeTime.FinalColor;
			out << YAML::EndMap;

			// Size Over Lifetime
			out << YAML::Key << "SizeOverLifeTime" << YAML::BeginMap;
			out << YAML::Key << "FinalSize" << YAML::Value << settings.SizeOverlifeTime.FinalSize;
			out << YAML::Key << "bEnabled" << YAML::Value << settings.SizeOverlifeTime.bEnabled;
			out << YAML::EndMap;

			// Optional: texture ID
			if (emitter->Texture.IsValid())
				out << YAML::Key << "TextureID" << YAML::Value << (uint64_t)emitter->Texture.GetAssetID();
			else
				out << YAML::Key << "TextureID" << YAML::Value << 0;

			out << YAML::EndMap; // emitter
		}
		out << YAML::EndSeq; // emitters

		out << YAML::EndMap; // particle system

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
		SetID(assetData, particleSystem);

		if (!data["Emitters"])
			return particleSystem;

		for (const auto& emitterNode : data["Emitters"])
		{
			Count<ParticleEmitter> emitter;

			if (emitterNode["MaxParticles"])
			{
				uint32_t maxParticles = emitterNode["MaxParticles"].as<uint32_t>();
				emitter = Count<ParticleEmitter>::Create(maxParticles);
			}
			else
				emitter = Count<ParticleEmitter>::Create();


			// Initial State
			auto& init = emitter->ParticleInitialState;
			if (auto node = emitterNode["InitialState"])
			{
				init.Duration = node["Duration"].as<float>(init.Duration);
				init.bLooping = node["bLooping"].as<int>(init.bLooping);
				init.StartLifetime = node["StartLifetime"].as<float>(init.StartLifetime);
				init.StartSpeed = node["StartSpeed"].as<float>(init.StartSpeed);
				init.StartSize = node["StartSize"].as<glm::vec3>(init.StartSize);
				init.GravityModifier = node["GravityModifier"].as<float>(init.GravityModifier);
				init.StartColor = node["StartColor"].as<glm::vec4>(init.StartColor);
				init.EmitterPosition = node["EmitterPosition"].as<glm::vec3>(init.EmitterPosition);
				init.FadeOutSpeed = node["FadeOutSpeed"].as<float>(init.FadeOutSpeed);
				init.EmitterPrevPosition = node["EmitterPrevPosition"].as<glm::vec3>(init.EmitterPrevPosition);
			}

			// Bursts
			if (auto burstsNode = emitterNode["Bursts"])
			{
				emitter->Bursts.clear();
				if (burstsNode.IsSequence())
				{
					for (const auto& n : burstsNode)
					{
						ParticleBurst b{};
						b.StartTime = n["StartTime"] ? n["StartTime"].as<float>() : 0.0f;
						b.Count = n["Count"] ? n["Count"].as<uint32_t>() : 50;
						b.Cycles = n["Cycles"] ? n["Cycles"].as<uint32_t>() : b.Cycles;
						b.Interval = n["Interval"] ? n["Interval"].as<float>() : b.Interval;
						b.Probability = n["Probability"] ? n["Probability"].as<float>() : b.Probability;
						emitter->Bursts.push_back(b);
					}
				}
			}

			// Emitter Settings
			auto& settings = emitter->ParticleEmitterSettings;

			// Emission
			if (auto node = emitterNode["Emission"])
			{
				settings.Emission.ParticlesPerSecond = node["ParticlesPerSecond"].as<float>(settings.Emission.ParticlesPerSecond);
				settings.Emission.ParticlesPerDistance = node["ParticlesPerDistance"].as<float>(settings.Emission.ParticlesPerDistance);
			}

			// Shape
			if (auto node = emitterNode["Shape"])
			{
				settings.Shape.Shape = node["Shape"].as<int>(settings.Shape.Shape);
				settings.Shape.RandomizeDirection = node["RandomizeDirection"].as<float>(settings.Shape.RandomizeDirection);
				settings.Shape.SpherizeDirection = node["SpherizeDirection"].as<float>(settings.Shape.SpherizeDirection);
				settings.Shape.RandomizePosition = node["RandomizePosition"].as<float>(settings.Shape.RandomizePosition);
				settings.Shape.bEnabled = node["bEnabled"].as<int>(settings.Shape.bEnabled);
				settings.Shape.ConeRadius = node["ConeRadius"].as<float>(settings.Shape.ConeRadius);
				settings.Shape.ConeAngleDegrees = node["ConeAngleDegrees"].as<float>(settings.Shape.ConeAngleDegrees);
				settings.Shape.SphereRadius = node["SphereRadius"].as<float>(settings.Shape.SphereRadius);
			}

			// Velocity Over Lifetime
			if (auto node = emitterNode["VelocityOverLifeTime"])
			{
				settings.VelocityOverLifeTime.Linear = node["Linear"].as<glm::vec3>(settings.VelocityOverLifeTime.Linear);
				settings.VelocityOverLifeTime.SpeedModifier = node["SpeedModifier"].as<float>(settings.VelocityOverLifeTime.SpeedModifier);
				settings.VelocityOverLifeTime.Orbital = node["Orbital"].as<glm::vec3>(settings.VelocityOverLifeTime.Orbital);
				settings.VelocityOverLifeTime.Radial = node["Radial"].as<float>(settings.VelocityOverLifeTime.Radial);
				settings.VelocityOverLifeTime.Offset = node["Offset"].as<glm::vec3>(settings.VelocityOverLifeTime.Offset);
				settings.VelocityOverLifeTime.bEnabled = node["bEnabled"].as<int>(settings.VelocityOverLifeTime.bEnabled);
			}

			// Color Over Lifetime
			if (auto node = emitterNode["ColorOverLifeTime"])
			{
				settings.ColorOverLifeTime.bEnabled = node["bEnabled"].as<int>(settings.ColorOverLifeTime.bEnabled);
				settings.ColorOverLifeTime.FinalColor = node["FinalColor"].as<glm::vec4>(settings.ColorOverLifeTime.FinalColor);
			}

			// Size Over Lifetime
			if (auto node = emitterNode["SizeOverLifeTime"])
			{
				settings.SizeOverlifeTime.FinalSize = node["FinalSize"].as<glm::vec3>(settings.SizeOverlifeTime.FinalSize);
				settings.SizeOverlifeTime.bEnabled = node["bEnabled"].as<int>(settings.SizeOverlifeTime.bEnabled);
			}

			uint64_t texID = emitterNode["TextureID"] ? emitterNode["TextureID"].as<uint64_t>() : 0;
			emitter->Texture = AssetKey<AssetType::Texture>( texID);

			particleSystem->AddEmitter(emitter);
		}

		return particleSystem;
	}
	template<class T, VariableTypes VT>
	void SaveBindableVariable(YAML::Emitter& out, const std::string& name, BindableVariable<T,VT> var)
	{
		out << YAML::Key << name.c_str();
		out << YAML::BeginMap; // name
		out << "Value" << YAML::Value << var.GetValue();
		out << "BindedVariableID" << YAML::Value << var.GetVariableID();
		out << YAML::EndMap; // name
	}

	template<class T, VariableTypes VT>
	void LoadBindableVariable(YAML::Node& node, const std::string& name,BindableVariable<T, VT>& var,Count<VariableSetStorage> storage)
	{
		auto variable = node[name.c_str()];
		if (variable)
		{
			uint64_t id = 0;
			if(variable["BindedVariableID"])
				id = variable["BindedVariableID"].as<uint64_t>(0);
			if (variable["Value"])
			{
				var.SetValue(variable["Value"].as<T>());
			}

			if (storage->GetVariables().contains(id))
			{
				var.SetUseAsVariable(true,storage);
				var.SetVariable(id);
			}
		}
	}

	template<typename T, T Min, T Max, VariableTypes VT>
	void SaveClampedBindableVariable(YAML::Emitter& out, const std::string& name, BindableVariable<ClampedValue<T, Min, Max>, VT>& var)
	{
		out << YAML::Key << name.c_str();
		out << YAML::BeginMap; // name
		out << "Value" << YAML::Value << var.GetValue();
		out << "BindedVariableID" << YAML::Value << var.GetVariableID();
		out << YAML::EndMap; // name
	}

	template<typename T, T Min, T Max, VariableTypes VT>
	void LoadClampedBindableVariable(YAML::Node& node, const std::string& name, BindableVariable<ClampedValue<T, Min, Max>, VT>& var, Count<VariableSetStorage> storage)
	{
		auto variable = node[name.c_str()];
		if (variable)
		{
			uint64_t id = variable["BindedVariableID"].as<uint64_t>(0);
			if (variable["Value"])
			{
				var.SetValue(variable["Value"].as<T>());
			}

			if (storage->GetVariables().contains(id))
			{
				var.SetUseAsVariable(true, storage);
				var.SetVariable(id);
			}
		}
	}
	void SaveStaticAssetBindableVarible(YAML::Emitter& out, const std::string& name, BindableStaticAssetKey& var)
	{
		out << YAML::Key << name.c_str();
		out << YAML::BeginMap; // name
		out << "AssetID" << YAML::Value << var.GetValue().GetAssetID();
		out << "AssetType" << YAML::Value <<EnumReflection::EnumString( var.GetValue().GetExpectedType());
		out << "BindedVariableID" << YAML::Value << var.GetVariableID();
		out << YAML::EndMap; // name
	}
	void LoadStaticAssetBindableVarible(YAML::Node& node, const std::string& name, BindableStaticAssetKey& var, Count<VariableSetStorage> storage)
	{
		auto variable = node[name.c_str()];
		if (variable)
		{
			uint64_t id = variable["BindedVariableID"].as<uint64_t>(0);

			AssetType type = EnumReflection::StringEnum< AssetType>( variable["AssetType"].as<std::string>(""));
			uint64_t assetID = variable["AssetID"].as<uint64_t>(0);

			StaticAssetKey key(type, assetID);
			var.SetValue(key);

			if (storage->GetVariables().contains(id))
			{
				var.SetUseAsVariable(true, storage);
				var.SetVariable(id);
			}
		}
	}


	void SaveBindableVariableString(YAML::Emitter& out, const std::string& name, BindableVariableString& var)
	{
		out << YAML::Key << name.c_str();
		out << YAML::BeginMap; // name
		out << "Value" << YAML::Value << var.GetValue();
		out << "BindedVariableID" << YAML::Value << var.GetVariableID();
		out << YAML::EndMap; // name
	}

	void LoadClampedBindableVariableString(YAML::Node& node, const std::string& name, BindableVariableString& var, Count<VariableSetStorage> storage)
	{
		
		auto variable = node[name.c_str()];
		if (variable && !variable.IsMap())
		{
			var.SetValue(variable.as<std::string>());
		}
		else if (variable)
		{
			uint64_t id = variable["BindedVariableID"].as<uint64_t>(0);
			if (variable["Value"])
			{
				var.SetValue(variable["Value"].as<std::string>());
			}

			if (storage->GetVariables().contains(id))
			{
				var.SetUseAsVariable(true, storage);
				var.SetVariable(id);
			}
		}
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

			out << YAML::Key << "PositionOffset" << YAML::Value << coreComponent.LayoutOffset.Position;


			{
				out << YAML::Key << "ParentHandle" << YAML::Value << coreComponent.m_ParentID;

				out << YAML::Key << "Children";
				out << YAML::Flow;
				out << YAML::BeginSeq;
				for (UUID simpleEnitty : coreComponent.m_Children)
				{
					out << simpleEnitty.Get();
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap; // CoreComponent
		}

		{
			if (element.HasComponent<UIImageComponent>())
			{
				auto& imageComponent = element.GetComponent<UIImageComponent>();

				out << YAML::Key << "UIImageComponent";
				out << YAML::BeginMap; // UIImageComponent
				SaveBindableVariable(out, "TintColor", imageComponent.TintColor);
				SaveStaticAssetBindableVarible(out, "Texture", imageComponent.Texture);
				out << YAML::EndMap; // UIImageComponent
			}
		}

		{
			if (element.HasComponent<UIButtonComponent>())
			{
				auto& buttonComponent = element.GetComponent<UIButtonComponent>();

				out << YAML::Key << "UIButtonComponent";
				out << YAML::BeginMap; // UIButtonComponent
				SaveBindableVariable(out, "TintColor", buttonComponent.TintColor);
				SaveStaticAssetBindableVarible(out,"Texture", buttonComponent.Texture);
				out << YAML::EndMap; // UIButtonComponent
			}
		}

		{
			if (element.HasComponent<UITextComponent>())
			{
				auto& textComponent = element.GetComponent<UITextComponent>();

				out << YAML::Key << "UITextComponent";
				out << YAML::BeginMap; // UITextComponent
				SaveBindableVariableString(out, "Text", textComponent.Text);
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
				//out << YAML::Key << "Proggress" << YAML::Value << proggressComponent.Proggress.GetValue();

				SaveClampedBindableVariable(out, "Proggress", proggressComponent.Proggress);
				SaveBindableVariable(out, "FillColor", proggressComponent.FillColor);
				SaveBindableVariable(out, "BackgroundColor", proggressComponent.BackgroundColor);

				out << YAML::EndMap; // UIProggresBarComponent
			}
		}

		if (element.HasComponent<UIVerticalBoxComponent>())
		{
			auto& verticalBox = element.GetComponent<UIVerticalBoxComponent>();

			out << YAML::Key << "UIVerticalBoxComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Spacing" << YAML::Value << verticalBox.Spacing;
			out << YAML::Key << "DrawBorders" << YAML::Value << verticalBox.DrawBorders;
			out << YAML::Key << "BorderColor" << YAML::Value << verticalBox.BorderColor;
			out << YAML::Key << "BorderThickness" << YAML::Value << verticalBox.BorderThickness;
			out << YAML::EndMap;
		}

		if (element.HasComponent<UIHorizontalBoxComponent>())
		{
			auto& horizontalBox = element.GetComponent<UIHorizontalBoxComponent>();

			out << YAML::Key << "UIHorizontalBoxComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Spacing" << YAML::Value << horizontalBox.Spacing;
			out << YAML::Key << "DrawBorders" << YAML::Value << horizontalBox.DrawBorders;
			out << YAML::Key << "BorderColor" << YAML::Value << horizontalBox.BorderColor;
			out << YAML::Key << "BorderThickness" << YAML::Value << horizontalBox.BorderThickness;
			out << YAML::EndMap;
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

		SerializeCommon::SaveVariableRegistry(out, uiPanel->VariableTable);


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
					newUIElement.GetComponent<UICoreComponent>().LayoutOffset.Position = coreComponent["PositionOffset"].as<glm::vec2>(newUIElement.GetComponent<UICoreComponent>().LayoutOffset.Position);


					newUIElement.GetComponent<UICoreComponent>().m_ParentID = coreComponent["ParentHandle"].as<uint64_t>(0);

					if (coreComponent["Children"])
					{
						for (auto entityID : coreComponent["Children"])
						{
							uint64_t childID = entityID.as<uint64_t>();
							newUIElement.GetComponent<UICoreComponent>().m_Children.emplace_back(childID);
						}
					}
				}
			}
			{
				auto imageComponent = uiElement["UIImageComponent"];
				if (imageComponent)
				{
					auto& src = newUIElement.GetComponent<UIImageComponent>();
					src.TintColor = imageComponent["TintColor"].as<glm::vec4>(glm::vec4(1.0f));

					LoadBindableVariable(imageComponent, "TintColor", src.TintColor, uiPanel->VariableTable->GetVariableSetStorage());
					LoadStaticAssetBindableVarible(imageComponent, "Texture", src.Texture, uiPanel->VariableTable->GetVariableSetStorage());

				}
			}

			{
				auto buttonComponent = uiElement["UIButtomComponent"];
				if (buttonComponent)
				{
					auto& src = newUIElement.GetComponent<UIImageComponent>();
					src.TintColor = buttonComponent["TintColor"].as<glm::vec4>(glm::vec4(1.0f));

					LoadBindableVariable(buttonComponent, "TintColor", src.TintColor, uiPanel->VariableTable->GetVariableSetStorage());
					LoadStaticAssetBindableVarible(buttonComponent, "Texture", src.Texture, uiPanel->VariableTable->GetVariableSetStorage());

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
					LoadClampedBindableVariableString(textComponent, "Text", src.Text, uiPanel->VariableTable->GetVariableSetStorage());
				}
			}

			{
				auto proggressComponent = uiElement["UIProggresBarComponent"];
				if (proggressComponent)
				{
					auto& src = newUIElement.GetComponent<UIProggresBarComponent>();
					src.Proggress = proggressComponent["Proggress"].as<float>(src.Proggress.GetValue());

					LoadClampedBindableVariable(proggressComponent, "Proggress", src.Proggress, uiPanel->VariableTable->GetVariableSetStorage());

					LoadBindableVariable(proggressComponent, "FillColor", src.FillColor, uiPanel->VariableTable->GetVariableSetStorage());
					LoadBindableVariable(proggressComponent, "BackgroundColor", src.BackgroundColor, uiPanel->VariableTable->GetVariableSetStorage());
				}
			}

			{
				auto verticalBox = uiElement["UIVerticalBoxComponent"];
				if (verticalBox)
				{
					auto& src = newUIElement.GetComponent<UIVerticalBoxComponent>();
					src.Spacing = verticalBox["Spacing"].as<float>(src.Spacing);
					src.DrawBorders = verticalBox["DrawBorders"].as<bool>(src.DrawBorders);
					src.BorderColor = verticalBox["BorderColor"].as<glm::vec4>(src.BorderColor);
					src.BorderThickness = verticalBox["BorderThickness"].as<float>(src.BorderThickness);
				}
			}

			{
				auto horizontalBox = uiElement["UIHorizontalBoxComponent"];
				if (horizontalBox)
				{
					auto& src = newUIElement.GetComponent<UIHorizontalBoxComponent>();
					src.Spacing = horizontalBox["Spacing"].as<float>(src.Spacing);
					src.DrawBorders = horizontalBox["DrawBorders"].as<bool>(src.DrawBorders);
					src.BorderColor = horizontalBox["BorderColor"].as<glm::vec4>(src.BorderColor);
					src.BorderThickness = horizontalBox["BorderThickness"].as<float>(src.BorderThickness);
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

		SerializeCommon::LoadVariableRegistry(data, uiPanel->VariableTable);

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

	void AnimationSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		Count<Animation> animation = asset.As<Animation>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(animation->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << animation->GetID();
		out << YAML::Key << "AnimationIndex" << YAML::Value << animation->GetAnimationIndex();
		out << YAML::Key << "MeshSource" << YAML::Value << animation->GetMeshSource()->GetID();
		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(data.Path).string());
		stream << out.c_str();
		stream.close();
	}

	Count<class Asset> AnimationSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node yamlData = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!yamlData["AssetType"])
			return nullptr;

		AssetKey<AssetType::MeshSourceFile> meshSourceKey = (AssetID)yamlData["MeshSource"].as<uint64_t>();
		if (!meshSourceKey.IsValid())
			return nullptr;

		Count<Animation> animation = Count<Animation>::Create(meshSourceKey.GetAsset<MeshSource>(), yamlData["AnimationIndex"].as<uint32_t>());
		SetID(assetData, animation);
		return animation;
	}

	void SkeletonSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		Count<Skeleton> skeleton = asset.As<Skeleton>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(skeleton->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << skeleton->GetID();
		out << YAML::Key << "MeshSource" << YAML::Value << skeleton->GetMeshSource()->GetID();
		out << YAML::EndMap;
		std::ofstream stream(AssetManager::GetAssetFileSystemPath(data.Path).string());
		stream << out.c_str();
		stream.close();
	}

	Count<class Asset> SkeletonSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node yamlData = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!yamlData["AssetType"])
			return nullptr;

		AssetKey<AssetType::MeshSourceFile> meshSourceKey = (AssetID)yamlData["MeshSource"].as<uint64_t>();
		if (!meshSourceKey.IsValid())
			return nullptr;

		Count<Skeleton> skeleton = Count<Skeleton>::Create(meshSourceKey.GetAsset<MeshSource>());
		SetID(assetData, skeleton);
		return skeleton;
	}

	void AnimationControllerSerializer::Save(const AssetInfo& data, const Count<class Asset>& asset) const
	{
		Count<AnimationController> animationController = asset.As<AnimationController>();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(animationController->GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << animationController->GetID();

		// Skeleton Reference
		if (animationController->GetSkeleton())
		{
			out << YAML::Key << "Skeleton" << YAML::Value << animationController->GetSkeleton()->GetID();
		}
		else
		{
			out << YAML::Key << "Skeleton" << YAML::Value << 0; // Null UUID or fallback
		}

		out << YAML::Key << "AnimationStates" << YAML::BeginSeq;

		for (const auto& state : animationController->GetAnimationStates())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "AnimationID" << YAML::Value << state->Animation.GetAssetID();
			out << YAML::Key << "IsLooping" << YAML::Value << state->IsLooping;

			out << YAML::Key << "RootTranslationMask" << YAML::Value << state->RootTranslationMask;
			out << YAML::Key << "RootTranslationExtractMask" << YAML::Value << state->RootTranslationExtractMask;
			out << YAML::Key << "RootRotationMask" << YAML::Value << state->RootRotationMask;
			out << YAML::Key << "RootRotationExtractMask" << YAML::Value << state->RootRotationExtractMask;
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;


		out << YAML::EndMap;

		std::ofstream stream(AssetManager::GetAssetFileSystemPath(data.Path).string());
		stream << out.c_str();
		stream.close();

	}

	Count<class Asset> AnimationControllerSerializer::TryLoadAsset(const AssetInfo& assetData) const
	{
		YAML::Node yamlData = YAML::LoadFile(AssetManager::GetAssetFileSystemPath(assetData.Path).string());
		if (!yamlData["AssetType"])
			return nullptr;


		Count<AnimationController> animationController = Count<AnimationController>::Create();

		// Load Skeleton Asset
		AssetKey<AssetType::Skeleton> skeleton = (UUID)yamlData["Skeleton"].as<uint64_t>(0);
		if (skeleton.IsValid())
		{
			Count<Skeleton> skeletonAsset = skeleton.GetAsset<Skeleton>();
			if (skeletonAsset)
				animationController->SetSkeleton(skeletonAsset);
		}

		// Load animation states
		if (yamlData["AnimationStates"])
		{
			for (const auto& stateNode : yamlData["AnimationStates"])
			{
				AssetKey<AssetType::Animation> animationKey = (UUID)stateNode["AnimationID"].as<uint64_t>(0);
				Count<AnimationState> state = Count<AnimationState>::Create(animationKey);

				if (stateNode["IsLooping"])
					state->IsLooping = stateNode["IsLooping"].as<bool>();

				if (stateNode["RootTranslationMask"])
					state->RootTranslationMask = stateNode["RootTranslationMask"].as<glm::vec3>();
				if (stateNode["RootTranslationExtractMask"])
					state->RootTranslationExtractMask = stateNode["RootTranslationExtractMask"].as<glm::vec3>();
				if (stateNode["RootRotationMask"])
					state->RootRotationMask = stateNode["RootRotationMask"].as<float>();
				if (stateNode["RootRotationExtractMask"])
					state->RootRotationExtractMask = stateNode["RootRotationExtractMask"].as<float>();

				animationController->m_AnimationStates.push_back(state);
			}
		}

		SetID(assetData, animationController);
		return animationController;
	}

}