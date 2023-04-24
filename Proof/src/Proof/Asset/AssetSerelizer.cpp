#include "Proofprch.h"
#include "AssetSerelizer.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Scene/SceneSerializer.h"
#include "Proof/Resources/ExternalCreations.h"
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

		SceneSerializer::DeSerilizeEntity(entities, world.Get(), nullptr,true);

		prefab->SetEntity(Entity{ owernID,world.Get() });
		SetID(assetData, prefab);

		return prefab;
	}

	void UIPanelAssetSerilizer::Save(const AssetInfo& assetData, const Count<class Asset>& asset) const
	{
		Count<UIPanel>uiPanel  = asset.As<UIPanel>();

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
				AssetID id = (imageButton.Texture == nullptr) ? AssetID(0) : imageButton.Texture->GetID();
				out << YAML::Key << "ImageAssetID" <<(uint64_t) id;
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
				out << YAML::Key << "Color" << texts.Param.Color ;
				out << YAML::Key << "Input" << texts.Text;
				out << YAML::Key << "Kerning" << texts.Param.Kerning;
				out << YAML::Key << "LineSpacing" << texts.Param.LineSpacing;
				AssetID id = (texts.Font == nullptr) ? AssetID(0) : texts.Font->GetID();
				out << YAML::Key << "Font" << (uint64_t)id;
				out << YAML::EndMap;// Text
			}
			out << YAML::EndSeq;
		}
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

			AssetID fontId = text["Font"].as<uint64_t>();

			if (AssetManager::HasAsset(fontId))
			{
				uiText.Font = AssetManager::GetAsset<Font>(fontId);
			}
			std::string name = texts["Text"].as<std::string>();
			uiPanel->SetText(uiText, name);
		}
		SetID(assetData, uiPanel);
		return uiPanel;

	}
}
