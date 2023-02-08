#include "Proofprch.h"
#include "PhysicsMaterialAsset.h"
#include "Proof/Math/Math.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Resources/ExternalCreations.h"
#include "AssetManager.h"
#include "Proof/Utils/PlatformUtils.h"

namespace Proof
{
	PhysicsMaterialAsset::PhysicsMaterialAsset(const std::string& savePath):
		Asset(AssetType::PhysicsMaterial) {
		m_AssetID = AssetManager::CreateID();
		m_SavePath = savePath;
		auto parentDir = std::filesystem::relative(m_SavePath.parent_path(), Application::Get()->GetProject()->GetAssetDirectory());
		m_SavePath = parentDir /= {Utils::FileDialogs::GetFileName(m_SavePath) + "."+GetExtension()};
		m_Material = CreateCount<PhysicsMaterial>();
		SaveAsset();
	}
	PhysicsMaterialAsset::PhysicsMaterialAsset():
		Asset(AssetType::PhysicsMaterial) 
	{
		m_Material = CreateCount<PhysicsMaterial>();
	}
	void PhysicsMaterialAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << m_AssetID;

		out << YAML::Key << "StaticFriction" << YAML::Value << m_Material->StaticFriction;
		out << YAML::Key << "DynamicFriction" << YAML::Value << m_Material->DynamicFriction;
		out << YAML::Key << "Bounciness" << YAML::Value << m_Material->Bounciness;

		out << YAML::Key << "FrictionCombineMode" << YAML::Value << EnumReflection::EnumString(m_Material->FrictionCombineMode);
		out << YAML::Key << "BouncinessCombineMode" << YAML::Value << EnumReflection::EnumString(m_Material->BouncinessCombineMode);

		out << YAML::EndMap;
		std::ofstream found(m_SavePath);
		found << out.c_str();
		found.close();
	}
	bool PhysicsMaterialAsset::LoadAsset(const std::string& filePath) {
		m_SavePath = filePath;
		YAML::Node data = YAML::LoadFile(m_SavePath.string());
		if (!data["AssetType"]) // if there is no scene no
			return false;
		m_AssetID = data["ID"].as<uint64_t>();

		m_Material->StaticFriction = data["StaticFriction"].as<float>();
		m_Material->DynamicFriction = data["DynamicFriction"].as<float>();
		m_Material->Bounciness = data["Bounciness"].as<float>();

		m_Material->FrictionCombineMode = EnumReflection::StringEnum<CombineMode>(data["FrictionCombineMode"].as<std::string>());
		m_Material->BouncinessCombineMode = EnumReflection::StringEnum<CombineMode>(data["BouncinessCombineMode"].as<std::string>());
	
		return true;
	}
	uint32_t PhysicsMaterialAsset::GetImageID() {
		return uint32_t();
	}
	Count<PhysicsMaterial> PhysicsMaterialAsset::GetMaterial() const {
		return m_Material;
	}
}
