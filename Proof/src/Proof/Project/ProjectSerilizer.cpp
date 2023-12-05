#include "Proofprch.h"
#include "Proof/Core/Core.h"

#include "ProjectSerilizer.h"
#include "Project.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Input/InputManager.h"
#include <fstream>
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Resources/ExternalCreations.h"
#ifdef CreateDirectory
#undef CreateDirectory
#undef DeleteFile
#undef MoveFile
#undef CopyFile
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable
#endif
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
				out << YAML::Key << "ScriptModuleDirectory" << m_Project->m_ProjectConfig.ScriptModuleDirectory.string();
				out << YAML::Key << "OnCloseStartWorldEditLastOpen" << m_Project->m_ProjectConfig.OnCloseStartWorldEditLastOpen;
				out << YAML::Key << "StartWorldEdit" << m_Project->m_ProjectConfig.StartWorldEdit;
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

			//physics
			{
				out << YAML::Key << "Physics" << YAML::Value;
				out << YAML::BeginMap;

				const auto& physicsSettings = PhysicsEngine::GetSettings();
				out << YAML::Key << "Gravity" << YAML::Value << physicsSettings.Gravity;
				out << YAML::Key << "BroadPhaseType" << YAML::Value <<EnumReflection::EnumString(physicsSettings.BroadPhaseType);
				if (physicsSettings.BroadPhaseType != BroadphaseType::AutomaticBoxPrune)
				{
					out << YAML::Key << "WorldBoundsMin" << YAML::Value << physicsSettings.WorldBoundsMin;
					out << YAML::Key << "WorldBoundsMax" << YAML::Value << physicsSettings.WorldBoundsMax;
					out << YAML::Key << "WorldBoundsSubdivisions" << YAML::Value << physicsSettings.WorldBoundsSubdivisions;
				}
				out << YAML::Key << "FrictionModel" << YAML::Value << (int)physicsSettings.FrictionModel;
				out << YAML::Key << "SolverPositionIterations" << YAML::Value << physicsSettings.SolverIterations;
				out << YAML::Key << "SolverVelocityIterations" << YAML::Value << physicsSettings.SolverVelocityIterations;

				out << YAML::Key << "PhysicsLayers";
				out << YAML::Value << YAML::BeginSeq;
				for (const auto& layer : PhysicsLayerManager::GetLayers())
				{
					out << YAML::BeginMap;
					out << YAML::Key << "Name" << YAML::Value << layer.Name;
					out << YAML::Key << "CollidesWith" << YAML::Value;
					out << YAML::BeginSeq;
					for (const auto& collidingLayer : PhysicsLayerManager::GetLayerCollisions(layer.LayerID))
					{
						out << YAML::BeginMap;
						out << YAML::Key << "Name" << YAML::Value << collidingLayer.Name;
						out << YAML::EndMap;
					}
					out << YAML::EndSeq;

					out << YAML::EndMap;
				}
				out << YAML::EndSeq;

				out << YAML::EndMap;

			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
		std::filesystem::path path = filePath;
		if (!FileSystem::Exists(path.parent_path()))
		{
			FileSystem::CreateDirectory(path.parent_path());
		}
		std::ofstream foud(filePath);
		foud << out.c_str();
		foud.close();
	}
	bool ProjectSerilizer::DeSerilizeText(const std::string& filePath) {
		if (std::filesystem::exists(filePath) == false)
		{
			PF_ENGINE_ERROR("Project serlizer path {} does not exist", filePath);
			PF_CORE_ASSERT(false);
		}
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["Project"])
			return false;
		auto projectData = data["Project"];
		
		m_Project->m_ProjectConfig.Name = projectData["Name"].as<std::string>();
		m_Project->m_ProjectConfig.Project = filePath;
		//m_Project->m_ProjectConfig. = filePath;
		m_Project->m_ProjectConfig.AssetDirectory = projectData["AssetDirectory"].as<std::string>();
		m_Project->m_ProjectConfig.AssetManager = projectData["AssetManager"].as<std::string>();
		if(projectData["ScriptModuleDirectory"])
			m_Project->m_ProjectConfig.ScriptModuleDirectory = projectData["ScriptModuleDirectory"].as<std::string>();
		m_Project->m_ProjectConfig.StartWorld = projectData["StartWorld"].as<uint64_t>();

		if (projectData["OnCloseStartWorldEditLastOpen"])
			m_Project->m_ProjectConfig.OnCloseStartWorldEditLastOpen = projectData["OnCloseStartWorldEditLastOpen"].as<bool>();
		if (projectData["StartWorldEdit"])
			m_Project->m_ProjectConfig.StartWorldEdit = projectData["StartWorldEdit"].as<uint64_t>();

		//Input ScriptModuleDirectory: Resources/Scripts/Binaries
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

		//physics
		{
			auto physicsNode = projectData["Physics"];

			if (physicsNode)
			{
				auto& physicsSettings = PhysicsEngine::GetSettings();

				physicsSettings.Gravity = physicsNode["Gravity"].as<glm::vec3>(physicsSettings.Gravity);
				physicsSettings.BroadPhaseType = physicsNode["BroadPhaseType"] ? EnumReflection::StringEnum<BroadphaseType>( physicsNode["BroadPhaseType"].as<std::string>()) : physicsSettings.BroadPhaseType;

				if (physicsSettings.BroadPhaseType != BroadphaseType::AutomaticBoxPrune)
				{
					physicsSettings.WorldBoundsMin = physicsNode["WorldBoundsMin"] ? physicsNode["WorldBoundsMin"].as<glm::vec3>() : physicsSettings.WorldBoundsMin;
					physicsSettings.WorldBoundsMax = physicsNode["WorldBoundsMax"] ? physicsNode["WorldBoundsMax"].as<glm::vec3>() : physicsSettings.WorldBoundsMax;
					physicsSettings.WorldBoundsSubdivisions = physicsNode["WorldBoundsSubdivisions"] ? physicsNode["WorldBoundsSubdivisions"].as<uint32_t>() : physicsSettings.WorldBoundsSubdivisions;
				}

				physicsSettings.FrictionModel = physicsNode["FrictionModel"] ? (FrictionType)physicsNode["FrictionModel"].as<int>() : physicsSettings.FrictionModel;
				physicsSettings.SolverIterations = physicsNode["SolverPositionIterations"] ? physicsNode["SolverPositionIterations"].as<uint32_t>() : physicsSettings.SolverIterations;
				physicsSettings.SolverVelocityIterations = physicsNode["SolverVelocityIterations"] ? physicsNode["SolverVelocityIterations"].as<uint32_t>() : physicsSettings.SolverVelocityIterations;

				auto physicsLayers = physicsNode["PhysicsLayers"];

				if (physicsLayers)
				{
					for (auto layer : physicsLayers)
						PhysicsLayerManager::AddLayer(layer["Name"].as<std::string>(), false);

					for (auto layer : physicsLayers)
					{
						PhysicsLayer& layerInfo = PhysicsLayerManager::GetLayer(layer["Name"].as<std::string>());
						auto collidesWith = layer["CollidesWith"];
						if (collidesWith)
						{
							for (auto collisionLayer : collidesWith)
							{
								const auto& otherLayer = PhysicsLayerManager::GetLayer(collisionLayer["Name"].as<std::string>());
								PhysicsLayerManager::SetLayerCollision(layerInfo.LayerID, otherLayer.LayerID, true);
							}
						}
					}
				}
			}
		}
		return true;
	}
}
