#include "Proofprch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <string>
#include <fstream>
#include <filesystem>
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scripting/ScriptEngine.h"
namespace Proof
{
	SceneSerializer::SceneSerializer(World* Scene) {
		PF_CORE_ASSERT(Scene, "Scene cannot be nulltptr");
		m_Scene = Scene;
	}
	void SceneSerializer::SerilizeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;// entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetID();
		{
			TagComponent* Tag = entity.GetComponent<TagComponent>();
			if (Tag != nullptr) {
				auto& tag = Tag->Tag;
				out << YAML::Key << "TagComponent";
				out << YAML::BeginMap; // tag component
				out << YAML::Key << "Tag" << YAML::Value << tag;
				out << YAML::Key << "tags";
				out << YAML::Flow;
				out << YAML::BeginSeq;
				for (std::string& tag : Tag->m_Tags) {
					out << tag;
				}
				out << YAML::EndSeq;
				out << YAML::EndMap; // tag component
			}
		}
		{
			TransformComponent* Transfrom = entity.GetComponent<TransformComponent>();
			if (Transfrom != nullptr) {
				out << YAML::Key << "TransformComponent";
				out << YAML::BeginMap; // transform component
				out << YAML::Key << "Location" << YAML::Value << Transfrom->Location;
				out << YAML::Key << "Rotation" << YAML::Value << Transfrom->Rotation;
				out << YAML::Key << "Scale" << YAML::Value << Transfrom->Scale;
				out << YAML::EndMap; // transform component
			}
		}
		{
			ChildComponent* childComponent = entity.GetComponent<ChildComponent>();
			if (childComponent != nullptr) {
				out << YAML::Key << "ChildComponent";
				out << YAML::BeginMap; // SubEntityComponet
				out << YAML::Key << "OwnerID" << YAML::Value << childComponent->GetOwnerID();

				out << YAML::Key << "Children";
				out << YAML::Flow;
				out << YAML::BeginSeq;
				for (UUID simpleEnitty : childComponent->m_Children) {
					out << simpleEnitty;
				}
				out << YAML::EndSeq;

				out << YAML::EndMap; // Mesh component
			}
		}
		{
			MeshComponent* Meshes = entity.GetComponent<MeshComponent>();
			if (Meshes != nullptr) {
				out << YAML::Key << "MeshComponent";
				out << YAML::BeginMap; // Mesh component
				out << YAML::Key << "MeshAssetPointerID" << YAML::Value << Meshes->m_MeshAssetPointerID;
				out << YAML::Key << "MaterialPointerID" << Meshes->GetMaterialPointerID();
				out << YAML::EndMap; // Mesh component
			}
		}
		{
			SpriteComponent* Sprite = entity.GetComponent<SpriteComponent>();
			if (Sprite != nullptr) {
				out << YAML::Key << "SpriteComponent";
				out << YAML::BeginMap; // Sprite component
				out << YAML::Key << "TextureAssetPointerID" << YAML::Value << Sprite->m_TextureAssetPointerID;
				out << YAML::Key << "Colour" << YAML::Value << Sprite->Colour;
				out << YAML::EndMap; // Sprite component
			}
		}
		{
			LightComponent* Light = entity.GetComponent<LightComponent>();
			if (Light != nullptr) {
				out << YAML::Key << "LightComponent";
				out << YAML::BeginMap; // LightComponent;
				out << YAML::Key << "CutOff" << Light->m_CutOff;
				out << YAML::Key << "OuterCutOff" << Light->m_OuterCutOff;

				out << YAML::Key << "Constant" << Light->m_Constant;
				out << YAML::Key << "Linear" << Light->m_Linear;
				out << YAML::Key << "Quadratic" << Light->m_Quadratic;

				out << YAML::Key << "Ambient" << Light->m_Ambient;
				out << YAML::Key << "Diffuse" << Light->m_Diffuse;
				out << YAML::Key << "Specular" << Light->m_Specular;

				out << YAML::Key << "LightType" << (int)Light->m_LightType;
				out << YAML::EndMap; // LightComponent
			}
		}
		{
			CameraComponent* cameraComponent = entity.GetComponent<CameraComponent>();
			if (cameraComponent != nullptr) {
				out << YAML::Key << "CameraComponent";
				out << YAML::BeginMap; // Camera Componet
				out << YAML::Key << "AutoSetDimension" << cameraComponent->m_AutoSetDimension;
				out << YAML::Key << "NearPlane" << cameraComponent->m_NearPlane;
				out << YAML::Key << "FarPlane" << cameraComponent->m_FarPlane;
				out << YAML::Key << "FOV" << cameraComponent->m_FovDeg;
				out << YAML::Key << "Width" << cameraComponent->m_Width;
				out << YAML::Key << "Height" << cameraComponent->m_Height;
				out << YAML::Key << "UpVector" << cameraComponent->m_Up;
				out << YAML::EndMap; // CameraComponet
			}
		}
		{
			CubeColliderComponent* cubeCollider = entity.GetComponent<CubeColliderComponent>();
			if (cubeCollider != nullptr) {
				out << YAML::Key << "CubeColliderComponent";
				out << YAML::BeginMap; // CubeColliderComponent
				out << YAML::Key << "IsTrigger" << cubeCollider->IsTrigger;
				out << YAML::Key << "OffsetLocation" << cubeCollider->OffsetLocation;
				out << YAML::Key << "OffsetScale" << cubeCollider->OffsetScale;
				out << YAML::Key << "PhysicsMaterialPointerID" << cubeCollider->m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CubeColliderComponent
			}
		}
		{
			SphereColliderComponent* sphereCollider = entity.GetComponent<SphereColliderComponent>();
			if (sphereCollider != nullptr) {
				out << YAML::Key << "SphereColliderComponent";
				out << YAML::BeginMap; // SphereColliderComponent
				out << YAML::Key << "IsTrigger" << sphereCollider->IsTrigger;
				out << YAML::Key << "Offset" << sphereCollider->OffsetLocation;
				out << YAML::Key << "Radius" << sphereCollider->Radius;
				out << YAML::Key << "PhysicsMaterialPointerID" << sphereCollider->m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // SphereColliderComponent
			}
		}
		{
			CapsuleColliderComponent* CapsuleCollider = entity.GetComponent<CapsuleColliderComponent>();
			if (CapsuleCollider != nullptr) {
				out << YAML::Key << "CapsuleColliderComponent";
				out << YAML::BeginMap; // CapsuleColliderComponent
				out << YAML::Key << "IsTrigger" << CapsuleCollider->IsTrigger;
				out << YAML::Key << "Offset" << CapsuleCollider->OffsetLocation;
				out << YAML::Key << "Radius" << CapsuleCollider->Radius;
				out << YAML::Key << "Height" << CapsuleCollider->Height;
				out << YAML::Key << "Direction" << EnumReflection::EnumString<CapsuleDirection>(CapsuleCollider->Direction);
				out << YAML::Key << "PhysicsMaterialPointerID" << CapsuleCollider->m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CapsuleColliderComponent
			}
		}
		{
			MeshColliderComponent* meshCollider = entity.GetComponent<MeshColliderComponent>();
			if (meshCollider != nullptr) {
				out << YAML::Key << "MeshColliderComponent";
				out << YAML::BeginMap; // MeshColliderComponent
				out << YAML::Key << "IsTrigger" << meshCollider->IsTrigger;
				out << YAML::Key << "PhysicsMaterialPointerID" << meshCollider->m_PhysicsMaterialPointerID;
				out << YAML::Key << "MeshAssetPointerID" << meshCollider->m_MeshAssetPointerID;
				out << YAML::EndMap; // MeshColliderComponent
			}
		}
		{
			RigidBodyComponent* rigidBody = entity.GetComponent<RigidBodyComponent>();
			if (rigidBody != nullptr) {
				out << YAML::Key << "RigidBodyComponent";
				out << YAML::BeginMap; // RigidBodyComponent
				out << YAML::Key << "Mass" << rigidBody->Mass;
				out << YAML::Key << "LinearDrag" << rigidBody->LinearDrag;
				out << YAML::Key << "AngularDrag" << rigidBody->AngularDrag;
				out << YAML::Key << "Gravity" << rigidBody->Gravity;
				out << YAML::Key << "Type" << EnumReflection::EnumString<RigidBodyType>(rigidBody->m_RigidBodyType);
				out << YAML::Key << "FreezeLocation" <<  rigidBody->FreezeLocation;
				out << YAML::Key << "FreezeRotation" <<  rigidBody->FreezeRotation;
				out << YAML::EndMap; // RigidBodyComponent
			}
		}

		{
			ScriptComponent* scriptComponent = entity.GetComponent<ScriptComponent>();
			if (scriptComponent != nullptr) {
				out << YAML::Key << "ScriptComponent";
				out << YAML::BeginMap; // ScriptComponent
				out << YAML::Key << "Scripts" << YAML::BeginSeq;

				for (auto& script : scriptComponent->m_Scripts) {
					out << YAML::BeginMap;// Script
					out << YAML::Key << "ScriptID" << script.ClassName;
					out << YAML::Key << "FieldSize" << script.Fields.size();
					int fieldIndex = 0;
					for (auto& field : script.Fields) {
						out << YAML::Key << fmt::format("Field{}",fieldIndex);
						out << YAML::BeginMap; //Field
						out << YAML::Key << "Name" << field.Name;
						out << YAML::Key << "Type" << EnumReflection::EnumString(field.Type);
						switch (field.Type) {
							case Proof::ProofMonoType::None:
								break;
							case Proof::ProofMonoType::Bool:
								out << YAML::Key << "Data" << field.Data._Cast<bool>();
								break;
							case Proof::ProofMonoType::Char:
								break;
							case Proof::ProofMonoType::String:
								break;
							case Proof::ProofMonoType::Uint8_t:
								out << YAML::Key << "Data" << *field.Data._Cast<uint8_t>();
								break;
							case Proof::ProofMonoType::Uint16_t:
								out << YAML::Key << "Data" << *field.Data._Cast<uint16_t>();
								break;
							case Proof::ProofMonoType::Uint32_t:
								out << YAML::Key << "Data" << *field.Data._Cast<uint32_t>();
								break;
							case Proof::ProofMonoType::Uint64_t:
								out << YAML::Key << "Data" << *field.Data._Cast<uint64_t>();
								break;
							case Proof::ProofMonoType::Int8_t:
								out << YAML::Key << "Data" << *field.Data._Cast<int8_t>();
								break;
							case Proof::ProofMonoType::Int16_t:
								out << YAML::Key << "Data" << *field.Data._Cast<int16_t>();
								break;
							case Proof::ProofMonoType::Int32_t:
								out << YAML::Key << "Data" << *field.Data._Cast<int32_t>();
								break;
							case Proof::ProofMonoType::Int64_t:
								out << YAML::Key << "Data" << *field.Data._Cast<int64_t>();
								break;
							case Proof::ProofMonoType::Float:
								out << YAML::Key << "Data" << *field.Data._Cast<float>();
								break;
							case Proof::ProofMonoType::Double:
								out << YAML::Key << "Data" << *field.Data._Cast<double>();
								break;
							case Proof::ProofMonoType::Class:
								break;
							case Proof::ProofMonoType::Array:
								break;
							case Proof::ProofMonoType::Enum:
								break;
							default:
								break;
						}
						out << YAML::EndMap; // Field
						fieldIndex++;
					}
					out << YAML::EndMap;// Script
				}
				out << YAML::EndSeq; //Scripts
				out << YAML::EndMap; // ScriptComponent
			}
		}

		out << YAML::EndMap; // entity
	}

	void SceneSerializer::SerilizeText(const std::string& filePath) {
		PF_PROFILE_FUNC();

		m_Scene->m_Path = filePath;
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "World" << YAML::Value << m_Scene->GetName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { (uint64_t)entityID,m_Scene };
			SerilizeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream foud(filePath);
		foud << out.c_str();
	}
	void SceneSerializer::SerilizeBinary(const std::string& filepath) {}
	bool SceneSerializer::DeSerilizeText(const std::string& filePath) {
		PF_PROFILE_FUNC();

		m_Scene->m_Path = filePath;
		if (std::filesystem::exists(filePath) == false)
			PF_CORE_ASSERT(false);
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["World"]) // if there is no scene no
			return false;

		m_Scene->Name = data["World"].as<std::string>();
		PF_WARN("Deserilizing World {}", m_Scene->Name.c_str());
		auto entities = data["Entities"];

		if (!entities)
			return false;
		for (auto entity : entities) {
			uint64_t EntID = entity["Entity"].as<uint64_t>();

			std::string name;
			auto tagcomponent = entity["TagComponent"];
			if (tagcomponent) {
				name = tagcomponent["Tag"].as<std::string>();
			}

			Entity NewEntity = m_Scene->CreateEntity(name, EntID);
			{
				if (tagcomponent) {
					if (tagcomponent["tags"]) {
						auto* tc = NewEntity.GetComponent<TagComponent>();

						for (auto tag : tagcomponent["tags"]) {
							tc->AddTag(tag.as<std::string>());
						}
					}
				}
			}
			// Transform
			{
				auto transformComponet = entity["TransformComponent"];
				if (transformComponet) {
					auto* tc = NewEntity.GetComponent<TransformComponent>();
					tc->Location = transformComponet["Location"].as<glm::vec3>();

					tc->Rotation = transformComponet["Rotation"].as<Vector<>>();
					tc->Scale = transformComponet["Scale"].as<Vector<>>();

				}
			}
			// ChildComponent
			{
				auto subEntityComponent = entity["ChildComponent"];
				if (subEntityComponent) {
					auto* tc = NewEntity.GetComponent<ChildComponent>();
					tc->m_OwnerID = subEntityComponent["OwnerID"].as<uint64_t>();

					if (subEntityComponent["Children"]) {
						for (auto entityID : subEntityComponent["Children"]) {
							uint64_t childID = entityID.as<uint64_t>();
							tc->m_Children.emplace_back(childID);
						}
					}

				}
			}
			// MESH
			{
				auto meshComponent = entity["MeshComponent"];
				if (meshComponent) {
					auto& src = *NewEntity.AddComponent<MeshComponent>();
					src.m_MeshAssetPointerID = meshComponent["MeshAssetPointerID"].as<uint64_t>();
					src.m_MeshMaterialID = meshComponent["MaterialPointerID"].as<uint64_t>();
					if (src.m_MeshAssetPointerID != 0) {
						m_AssetLoadID.emplace(src.m_MeshAssetPointerID);
					}
					if (src.m_MeshMaterialID != 0) {
						m_AssetLoadID.emplace(src.m_MeshMaterialID);
					}
				}
			}
			// SPRITE
			{
				auto spriteRendererComponent = entity["SpriteComponent"];
				if (spriteRendererComponent) {
					auto& src = *NewEntity.AddComponent<SpriteComponent>();
					src.m_TextureAssetPointerID = spriteRendererComponent["TextureAssetPointerID"].as<uint64_t>();
					src.Colour = spriteRendererComponent["Colour"].as<glm::vec4>();
					if (src.m_TextureAssetPointerID != 0) {
						m_AssetLoadID.emplace(src.m_TextureAssetPointerID);
					}
				}
			}
			// LIGHT
			{
				auto lightComponent = entity["LightComponent"];
				if (lightComponent) {
					auto& src = *NewEntity.AddComponent<LightComponent>();
					src.m_CutOff = lightComponent["CutOff"].as<float>();
					src.m_OuterCutOff = lightComponent["OuterCutOff"].as<float>();

					src.m_Constant = lightComponent["Constant"].as<float>();
					src.m_Linear = lightComponent["Linear"].as<float>();
					src.m_Quadratic = lightComponent["Quadratic"].as<float>();

					src.m_Ambient = lightComponent["Ambient"].as<glm::vec3>();
					src.m_Diffuse = lightComponent["Diffuse"].as<glm::vec3>();
					src.m_Specular = lightComponent["Specular"].as<glm::vec3>();

					src.m_LightType = (LightComponent::LightType)lightComponent["LightType"].as<int>();

				}
			}
			// CAMERA
			{
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& src = *NewEntity.AddComponent<CameraComponent>();
					src.m_AutoSetDimension = cameraComponent["AutoSetDimension"].as<bool>();
					src.m_NearPlane = cameraComponent["NearPlane"].as<float>();
					src.m_FarPlane = cameraComponent["FarPlane"].as<float>();
					src.m_FovDeg = cameraComponent["FOV"].as<float>();
					if (src.m_AutoSetDimension == false) {
						src.m_Width = cameraComponent["Width"].as<uint32_t>();
						src.m_Height = cameraComponent["Height"].as<uint32_t>();
					}

				}
			}
			// CUBE COLLIDER
			{
				auto cubeColliderComponent = entity["CubeColliderComponent"];
				if (cubeColliderComponent) {
					auto& src = *NewEntity.AddComponent<CubeColliderComponent>();
					src.IsTrigger = cubeColliderComponent["IsTrigger"].as<bool>();
					src.OffsetLocation = cubeColliderComponent["OffsetLocation"].as<Vector<float>>();
					src.OffsetScale = cubeColliderComponent["OffsetScale"].as<Vector<float>>();
					src.m_PhysicsMaterialPointerID = cubeColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}

			// SPHERE COLLIDER
			{
				auto sphereColliderComponent = entity["SphereColliderComponent"];
				if (sphereColliderComponent) {
					auto& src = *NewEntity.AddComponent<SphereColliderComponent>();
					src.IsTrigger = sphereColliderComponent["IsTrigger"].as<bool>();
					src.OffsetLocation = sphereColliderComponent["Offset"].as<Vector<float>>();
					src.Radius = sphereColliderComponent["Radius"].as<float>();
					src.m_PhysicsMaterialPointerID = sphereColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}
			//CAPSULE COLLIDER
			{

				auto capsuleColliderComponent = entity["CapsuleColliderComponent"];
				if (capsuleColliderComponent) {
					auto& src = *NewEntity.AddComponent<CapsuleColliderComponent>();
					src.IsTrigger = capsuleColliderComponent["IsTrigger"].as<bool>();
					src.OffsetLocation = capsuleColliderComponent["Offset"].as<Vector<float>>();
					src.Radius = capsuleColliderComponent["Radius"].as<float>();
					src.Height = capsuleColliderComponent["Height"].as<float>();
					src.Direction = EnumReflection::StringEnum<CapsuleDirection>(capsuleColliderComponent["Direction"].as<std::string>());
					src.m_PhysicsMaterialPointerID = capsuleColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}
			//Mesh COLLIDER
			{

				auto mehsCollider = entity["MeshColliderComponent"];
				if (mehsCollider) {
					auto& src = *NewEntity.AddComponent<MeshColliderComponent>();
					src.IsTrigger = mehsCollider["IsTrigger"].as<bool>();
					src.m_PhysicsMaterialPointerID = mehsCollider["PhysicsMaterialPointerID"].as<uint64_t>();
					src.m_MeshAssetPointerID = mehsCollider["MeshAssetPointerID"].as<uint64_t>();
				}
			}
			// RIGID BODY
			{
				auto rigidBodyComponent = entity["RigidBodyComponent"];
				if (rigidBodyComponent) {
					auto& rgb = *NewEntity.AddComponent<RigidBodyComponent>();
					rgb.Mass = rigidBodyComponent["Mass"].as<float>();
					rgb.LinearDrag = rigidBodyComponent["LinearDrag"].as<float>();
					rgb.AngularDrag = rigidBodyComponent["AngularDrag"].as<float>();
					rgb.Gravity = rigidBodyComponent["Gravity"].as<bool>();
					rgb.FreezeLocation = rigidBodyComponent["FreezeLocation"].as<Vector<bool>>();
					rgb.FreezeRotation = rigidBodyComponent["FreezeRotation"].as<Vector<bool>>();
					rgb.m_RigidBodyType = EnumReflection::StringEnum<RigidBodyType>(rigidBodyComponent["Type"].as<std::string>());
				}
			}

			//Script Component
			{
				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent) {
					auto& scp = *NewEntity.AddComponent<ScriptComponent>();

					auto scripts = scriptComponent["Scripts"];
					for (auto script : scripts) {
						std::string scriptName = script["ScriptID"].as<std::string>();
						ScriptData scriptData{ scriptName };
						uint32_t numFieldSize = script["FieldSize"].as<int>();

						for (uint32_t i = 0; i < numFieldSize; i++) {
							ScriptField scriptField;
							auto field = script[fmt::format("Field{}", i)];
							scriptField.Name = field["Name"].as<std::string>();
							//at release we do not to do this casue the binary are already built and set for us whe 
							if (ScriptEngine::IsFieldAvailable(scriptName, scriptField.Name) == false)
								continue;
							scriptField.Type = EnumReflection::StringEnum<ProofMonoType>(field["Type"].as<std::string>());
							switch (scriptField.Type) {
								case Proof::ProofMonoType::None:
									break;
								case Proof::ProofMonoType::Bool:
									scriptField.Data = field["Data"].as<bool>();
									break;
								case Proof::ProofMonoType::Char:
									break;
								case Proof::ProofMonoType::String:
									break;
								case Proof::ProofMonoType::Uint8_t:
									scriptField.Data = field["Data"].as<uint8_t>();
									break;
								case Proof::ProofMonoType::Uint16_t:
									scriptField.Data = field["Data"].as<uint16_t>();
									break;
								case Proof::ProofMonoType::Uint32_t:
									scriptField.Data = field["Data"].as<uint32_t>();
									break;
								case Proof::ProofMonoType::Uint64_t:
									scriptField.Data = field["Data"].as<uint64_t>();
									break;
								case Proof::ProofMonoType::Int8_t:
									scriptField.Data = field["Data"].as<int8_t>();
									break;
								case Proof::ProofMonoType::Int16_t:
									scriptField.Data = field["Data"].as<int16_t>();
									break;
								case Proof::ProofMonoType::Int32_t:
									scriptField.Data = field["Data"].as<int32_t>();
									break;
								case Proof::ProofMonoType::Int64_t:
									scriptField.Data = field["Data"].as<int64_t>();
									break;
								case Proof::ProofMonoType::Float:
									scriptField.Data = field["Data"].as<float>();
									break;
								case Proof::ProofMonoType::Double:
									scriptField.Data = field["Data"].as<double>();
									break;
								case Proof::ProofMonoType::Class:
									break;
								case Proof::ProofMonoType::Array:
									break;
								case Proof::ProofMonoType::Enum:
									break;
								default:
									break;
							}
							scriptData.Fields.emplace_back(scriptField);
						}
						scp.m_Scripts.emplace_back(scriptData);
					}
				}
			}
		}
		return true;
	}
	bool SceneSerializer::DeSerilizeBinary(const std::string& filepath) {
		PF_CORE_ASSERT(false, "Functon has not been created");
		return false;
	}
}