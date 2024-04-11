#include "Proofprch.h"
#include "AssetCustomDataSerelizer.h"
#include <yaml-cpp/yaml.h>
#include "MeshSourceSavedSettings.h"
#include "AssetCustomDataInfo.h"
#include "../AssetManager.h"
#include "Proof/Scripting/ScriptImportSettings.h"
#include "Proof/Scripting/ScriptField.h"
#include "Proof/Asset/SerializeCommon.h"
#include "Proof/Scripting/ScriptEngine.h"
namespace Proof
{
	void MeshSourceSavedSettingSerializer::Save(const std::filesystem::path& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;//MeshSourceSavedSetting
		out << YAML::Key << "MeshSourceSavedSetting";
		out << YAML::BeginMap;//MeshSources
		out << YAML::Key << "MeshSources" <<YAML::Value << YAML::BeginSeq;

		for(auto& [meshSourceID,meshSourceData] : MeshSourceSavedSettings::s_MeshesSourcesMetaData)
		{
			out << YAML::BeginMap;// meshSource
			out << YAML::Key << "MeshSource" << YAML::Value << meshSourceID;

			out << YAML::Key << "MaterialTable";
			out << YAML::BeginSeq;//MaterialTbale 
			for (auto& [index, materialID] : meshSourceData.MaterialList)
			{

				out << YAML::BeginMap;// material

				// we nned th "" for some reason 
				out << YAML::Key << "Material" << YAML::Key << "";

				//id of 0 means default material
				out << YAML::Key << "AssetID" << YAML::Value << materialID;
				out << YAML::Key << "Index" << YAML::Value << index;

				out << YAML::EndMap;// material

			}
			out << YAML::EndSeq; // matrailTable

			out << YAML::EndMap; //mesh source
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;//MeshSources
		out << YAML::EndMap;//MeshSourceSavedSetting


		std::ofstream stream(Utils::GetAssetCustomDataFileSystemPath(path).string());

		//creae dir
		if (!FileSystem::Exists(Utils::GetAssetCustomDataFileSystemPath(path).string()))
			FileSystem::CreateDirectory(Utils::GetAssetCustomDataFileSystemPath(path).parent_path().string());

		stream << out.c_str();
		stream.close();
	}
	void MeshSourceSavedSettingSerializer::Load(const std::filesystem::path& path)
	{
		if (!FileSystem::Exists(Utils::GetAssetCustomDataFileSystemPath(path).string()))
			return;

		// Open the YAML file for reading
		std::ifstream file(Utils::GetAssetCustomDataFileSystemPath(path).string());
		if (!file.is_open()) {
			PF_CORE_ASSERT(false);
			// Handle error: Unable to open the file
			return;
		}

		// Load the YAML data
		YAML::Node root = YAML::Load(file);

		// Check if the root is a map
		if (root && root["MeshSourceSavedSetting"]) {
			// Access the MeshSources section
			const YAML::Node& meshSourcesSettingsNode = root["MeshSourceSavedSetting"];
			if(meshSourcesSettingsNode)
			{ 
				auto meshSourcesNode = meshSourcesSettingsNode["MeshSources"];

				if (meshSourcesNode)
				{
					// Iterate over each MeshSource entry
					for (const auto& meshSourceEntry : meshSourcesNode) 
					{
						// Access meshSourceID and meshSourceData
						const YAML::Node& meshSourceIDNode = meshSourceEntry["MeshSource"];
						const YAML::Node& materialTableNode = meshSourceEntry["MaterialTable"];

						// Extract data as needed
						if (meshSourceIDNode && materialTableNode) 
						{
							// Example: Extract meshSourceID
							uint64_t meshSourceID = meshSourceIDNode.as<uint64_t>();
							if (!AssetManager::HasAsset(meshSourceID))
								continue;
							if (AssetManager::GetAssetInfo(meshSourceID).Type != AssetType::MeshSourceFile)
								continue;
							// Example: Extract MaterialTable
							if (materialTableNode) 
							{
								for (const auto& materialNode : materialTableNode) 
								{
									// Extract material data
									// Example: Extract AssetID and Index
									uint64_t materialAssetID = materialNode["AssetID"].as<uint64_t>();
									uint64_t materialIndex = materialNode["Index"].as<uint64_t>();
									
									if (AssetManager::HasAsset(materialAssetID))
									{
										if (AssetManager::GetAssetInfo(materialAssetID).Type == AssetType::Material)
										{
											MeshSourceSavedSettings::s_MeshesSourcesMetaData[meshSourceID].ID = meshSourceID;
											MeshSourceSavedSettings::s_MeshesSourcesMetaData[meshSourceID].MaterialList[materialIndex] = materialAssetID;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		// Close the file (optional, as it will be closed when it goes out of scope)
		file.close();
	}
	void ScriptImportSavedSettingSerializer::Save(const std::filesystem::path& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;//ScriptImportSettings
		out << YAML::Key << "ScriptImportSettings";
		out << YAML::BeginMap;//Scripts
		out << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq;
		for (auto& [scriptName, scriptMetaData] : ScriptImportSettings::s_Classes)
		{
			out << YAML::BeginMap;// Script
			out << YAML::Key << "Script" << YAML::Value << scriptName;
			out << YAML::Key << "ScriptName" << YAML::Value << scriptName;

			out << YAML::Key << "Fields" << YAML::Value;
			out << YAML::BeginSeq;//Fields
			for (const auto& [fieldName, fieldStorage] : scriptMetaData.Fields)
			{
				SerializeCommon::SaveScriptField(out, fieldStorage);
			}
			out << YAML::EndSeq; // Fields
			out << YAML::EndMap; //Script
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;//Scripts
		out << YAML::EndMap;//ScriptImportSettings


		std::ofstream stream(Utils::GetAssetCustomDataFileSystemPath(path).string());

		//creae dir
		if (!FileSystem::Exists(Utils::GetAssetCustomDataFileSystemPath(path).string()))
			FileSystem::CreateDirectory(Utils::GetAssetCustomDataFileSystemPath(path).parent_path().string());

		stream << out.c_str();
		stream.close();
	}
	void ScriptImportSavedSettingSerializer::Load(const std::filesystem::path& path)
	{

		if (!FileSystem::Exists(Utils::GetAssetCustomDataFileSystemPath(path).string()))
			return;

		// Open the YAML file for reading
		std::ifstream file(Utils::GetAssetCustomDataFileSystemPath(path).string());
		if (!file.is_open()) {
			PF_CORE_ASSERT(false);
			// Handle error: Unable to open the file
			return;
		}

		// Load the YAML data
		YAML::Node root = YAML::Load(file);

		// Check if the root is a map
		if (!root)
			return; 
		if (!root["ScriptImportSettings"])
			return;

		const YAML::Node& scriptSettingsNode = root["ScriptImportSettings"];

		if (!scriptSettingsNode)
			return;

		auto scriptsMap = scriptSettingsNode["Scripts"];
		for(auto script : scriptsMap)
		{
			const std::string scriptClassModule = script["ScriptName"].as<std::string>("");

			if (!ScriptEngine::IsModuleValid(scriptClassModule))
				continue;
			
			if (!script["Fields"])continue;
			auto scriptFields = script["Fields"];

			for (auto field : scriptFields)
			{
				std::string fieldNameID = field["NameID"].as<std::string>();

				if (ScriptImportSettings::s_Classes[scriptClassModule].Fields.contains(fieldNameID))
				{
					SerializeCommon::LoadScriptField(field, ScriptImportSettings::s_Classes[scriptClassModule].Fields.at(fieldNameID));
				}
			}
		}

		file.close();
	}
}
