#include "Proofprch.h"
#include "Proof/Core/Core.h"

#include "ProjectSerilizer.h"
#include "Project.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Input/InputManager.h"
#include <fstream>
namespace Proof
{
	ProjectSerilizer::ProjectSerilizer(Project* project) {
		PF_CORE_ASSERT(project !=  nullptr, "Project cannot be nulltptr");
		m_Project = project;
	}
	void ProjectSerilizer::SerilizeText(const std::string& filePath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << YAML::Key << "Project";
			out << YAML::BeginMap;
			{
				out << YAML::Key << "Name" << m_Project->m_ProjectConfig.Name;
				out << YAML::Key << "AssetDirectory" << m_Project->m_ProjectConfig.AssetDirectory.string();
				out << YAML::Key << "AssetManager" << m_Project->m_ProjectConfig.AssetManager.string();
				out << YAML::Key << "StartWorld" << m_Project->m_ProjectConfig.StartWorld;
			}
			//input
			{
				out << YAML::Key << "Input";
				out << YAML::BeginMap; // Input
				// Action
				{
					out << YAML::Key << "ActionMapping" <<YAML::Value<< YAML::BeginSeq; // Action Mapping
					for (auto& [actionName, action] : InputManager::GetActionMappings())
					{
						out << YAML::BeginMap;// action
						out << YAML::Key << "Action" << actionName;
						out << YAML::Key<<"InputDevices"<< YAML::BeginSeq;// InputDevice
						for (auto& [inputDevice, inputType] : action.Inputs)
						{
							out << YAML::BeginMap;// fdafadfa

							out << YAML::Key << "InputDevice" << EnumReflection::EnumString(inputDevice);


							out << YAML::Key << "Keys";
							out << YAML::Flow;
							out << YAML::BeginSeq;//input device
							for (auto& input : inputType)
							{
								out << input.Key;
							}
							out << YAML::EndSeq; // input device
							out << YAML::EndMap;// fdafadfa


						}
						out << YAML::EndSeq;// inputDevice

						out << YAML::EndMap;// action
					}
					out << YAML::EndSeq; // Action Mapping
				}
				//Motion
				{
					out << YAML::Key << "MotionMapping" << YAML::Value << YAML::BeginSeq; // motion Mapping
					for (auto& [actionName, action] : InputManager::GetMotionMappings())
					{
						out << YAML::BeginMap;// motion
						out << YAML::Key << "Motion" << actionName;
						out << YAML::Key << "InputDevices" << YAML::BeginSeq;// InputDevice
						for (auto& [inputDevice, inputType] : action.Inputs)
						{
							out << YAML::BeginMap;// fdafadfa

							out << YAML::Key << "InputDevice" << EnumReflection::EnumString(inputDevice);


							out << YAML::Key << "Keys";
							out << YAML::Flow;
							out << YAML::BeginSeq;//input device
							for (auto& input : inputType)
							{
								out << input.Key;
							}
							out << YAML::EndSeq; // input device

							{
								out << YAML::Key << "MotionValues";
								out << YAML::Flow;
								out << YAML::BeginSeq;//input device
								for (auto& input : inputType)
								{
									out << input.MotionValue;
								}
								out << YAML::EndSeq; // input device
							}
							out << YAML::EndMap;// fdafadfa


						}
						out << YAML::EndSeq;// inputDevice

						out << YAML::EndMap;// moiton
					}
					out << YAML::EndSeq; // motion Mapping
				}
				out << YAML::EndMap;// input
			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
		std::ofstream foud(filePath);
		foud << out.c_str();
		foud.close();
	}
	bool ProjectSerilizer::DeSerilizeText(const std::string& filePath) {
		if (std::filesystem::exists(filePath) == false)
			PF_CORE_ASSERT(false);
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["Project"])
			return false;
		auto projectData = data["Project"];
		
		m_Project->m_ProjectConfig.Name = projectData["Name"].as<std::string>();
		m_Project->m_ProjectConfig.Project = filePath;
		//m_Project->m_ProjectConfig. = filePath;
		m_Project->m_ProjectConfig.AssetDirectory = projectData["AssetDirectory"].as<std::string>();
		m_Project->m_ProjectConfig.AssetManager = projectData["AssetManager"].as<std::string>();
		m_Project->m_ProjectConfig.StartWorld = projectData["StartWorld"].as<uint64_t>();
		//Input
		if (projectData["Input"])
		{
			auto input = projectData["Input"];
			//action
			{
				auto actionMapping = input["ActionMapping"];
				for (auto action : actionMapping)
				{
					std::string actionName = action["Action"].as < std::string>();
					auto inputDevices = action["InputDevices"];
					InputManager::AddAction(actionName);

					for (auto inputDevice : inputDevices)
					{

						InputDevice device = EnumReflection::StringEnum<InputDevice>(inputDevice["InputDevice"].as<std::string>());

						for (auto key : inputDevice["Keys"])
						{
							int newKey = key.as<int>();
							InputManager::ActionAddKey(actionName, InputType(device, newKey));
						}

					}
				}

			}

			//motion
			{
				auto actionMapping = input["MotionMapping"];
				for (auto action : actionMapping)
				{
					std::string motionName = action["Motion"].as < std::string>();
					auto inputDevices = action["InputDevices"];
					InputManager::AddMotion(motionName);

					for (auto inputDevice : inputDevices)
					{

						InputDevice device = EnumReflection::StringEnum<InputDevice>(inputDevice["InputDevice"].as<std::string>());

						auto motionValues = inputDevice["MotionValues"];
						int currentIterate = 0;
						for (auto key : inputDevice["Keys"])
						{
							int newKey = key.as<int>();
							float val = motionValues[currentIterate].as<float>();
							currentIterate++;
							InputManager::MotionAddKey(motionName, MotionInputType(device, newKey,val));
						}

					}
				}

			}
		}
		return true;
	}
}
