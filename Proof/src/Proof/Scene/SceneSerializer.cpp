#include "Proofprch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <string>
#include <fstream>
#include <filesystem>
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Mesh.h"
namespace Proof
{
	SceneSerializer::SceneSerializer(World* Scene) {
		PF_CORE_ASSERT(Scene, "Scene cannot be nulltptr");
		m_Scene = Scene;
	}
	void SceneSerializer::SerilizeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;// entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetEntityID();
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
				for (EntityID simpleEnitty : childComponent->m_Children) {
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
				out << YAML::Key << "MeshAssetPointerID" << YAML::Value << Meshes->m_MeshID;
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
			
			{
				DirectionalLightComponent* directonalLight = entity.GetComponent<DirectionalLightComponent>();
				if (directonalLight != nullptr) {
					out << YAML::Key << "DirectionalLightComponent";
					out << YAML::BeginMap; // DirectionalLightComponent
					out << YAML::Key << "Color" << directonalLight->Color;
					out << YAML::Key << "Intensity" << directonalLight->Intensity;
					out << YAML::EndMap; // DirectionalLightComponent
				}
			}

			{
				PointLightComponent* pointLight= entity.GetComponent<PointLightComponent>();
				if (pointLight != nullptr) {
					out << YAML::Key << "PointLightComponent";
					out << YAML::BeginMap; // PointLightComponent
					out << YAML::Key << "Color" << pointLight->Color;
					out << YAML::Key << "Intensity" << pointLight->Intensity;

					out << YAML::Key << "Constant" << pointLight->Constant;
					out << YAML::Key << "Linear" << pointLight->Linear;
					out << YAML::Key << "Quadratic" << pointLight->Quadratic;
					out << YAML::Key << "Radius" << pointLight->Radius;

					out << YAML::EndMap; // PointLightComponent
				}
			}

			{
				SpotLightComponent* spotLight = entity.GetComponent<SpotLightComponent>();
				if (spotLight != nullptr) {
					out << YAML::Key << "SpotLightComponent";
					out << YAML::BeginMap; // PointLightComponent
					out << YAML::Key << "Color" << spotLight->Color;

					out << YAML::Key << "Intensity" << spotLight->Intensity;
					out << YAML::Key << "Constant" << spotLight->Constant;
					out << YAML::Key << "Linear" << spotLight->Linear;
					out << YAML::Key << "Quadratic" << spotLight->Quadratic;
					out << YAML::Key << "Radius" << spotLight->Radius;
					out << YAML::Key << "OuterCutoff" << spotLight->OuterCutOff;
					out << YAML::Key << "CutOff" << spotLight->CutOff;

					out << YAML::EndMap; // PointLightComponent
				}
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
				out << YAML::EndMap; // ScriptComponent
			}
		}

		out << YAML::EndMap; // entity
	}

	void SceneSerializer::SerilizeText(const std::string& filePath) {
		PF_PROFILE_FUNC();

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << YAML::Key << "World";
			out << YAML::BeginMap;
			{
				out << YAML::Key << "ID" << m_Scene->m_WorldID;

				out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
				m_Scene->m_Registry.each([&](auto entityID) {
					Entity entity = { (uint64_t)entityID,m_Scene };
					SerilizeEntity(out, entity);
				});
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
		auto parentDir = std::filesystem::path(filePath).parent_path();
		auto savePath = parentDir /= {Utils::FileDialogs::GetFileName(filePath) + "." + "ProofWorld"};
		std::ofstream foud(savePath.string());
		foud << out.c_str();
	}
	void SceneSerializer::SerilizeBinary(const std::string& filepath) {}
	bool SceneSerializer::DeSerilizeText(const std::string& filePath) {
		PF_PROFILE_FUNC();

		if (std::filesystem::exists(filePath) == false)
			PF_CORE_ASSERT(false);
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["World"]) // if there is no scene no
			return false;

		auto worldData = data["World"];

		m_Scene->Name = Utils::FileDialogs::GetFileName(filePath);
		PF_EC_WARN("Deserilizing World {}", m_Scene->Name.c_str());

		m_Scene->m_WorldID = worldData["ID"].as<uint64_t>();
		auto entities = worldData["Entities"];

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
					tc->Location = transformComponet["Location"].as<Vector>();

					tc->Rotation = transformComponet["Rotation"].as<Vector>();
					tc->Scale = transformComponet["Scale"].as<Vector>();

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
					src.m_MeshID = meshComponent["MeshAssetPointerID"].as<uint64_t>();
					src.MaterialTable = AssetManager::GetAsset<Mesh>(src.m_MeshID)->GetMaterialTable()->Copy();
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
			
				{
					auto directionalLight = entity["DirectionalLightComponent"];
					if (directionalLight) {
						auto& src = *NewEntity.AddComponent<DirectionalLightComponent>();
						src.Color = directionalLight["Color"].as<Vector>();
						src.Intensity = directionalLight["Intensity"].as<float>();
					}
				}

				{
					auto pointLight = entity["PointLightComponent"];
					if (pointLight) {
						auto& src = *NewEntity.AddComponent<PointLightComponent>();
						src.Color = pointLight["Color"].as<Vector>();
						src.Intensity = pointLight["Intensity"].as<float>();
						src.Constant = pointLight["Constant"].as<float>();
						src.Linear = pointLight["Linear"].as<float>();
						src.Quadratic = pointLight["Quadratic"].as<float>();
						src.Radius = pointLight["Radius"].as<float>();

					}
				}

				{
					auto spotLight = entity["SpotLightComponent"];
					if (spotLight) {
						auto& src = *NewEntity.AddComponent<SpotLightComponent>();
						src.Color = spotLight["Color"].as<Vector>();
						src.Intensity = spotLight["Intensity"].as<float>();
						src.Constant = spotLight["Constant"].as<float>();
						src.Linear = spotLight["Linear"].as<float>();
						src.Quadratic = spotLight["Quadratic"].as<float>();
						src.Radius = spotLight["Radius"].as<float>();

						src.CutOff = spotLight["CutOff"].as<float>();
						src.OuterCutOff= spotLight["OuterCutOff"].as<float>();
					}
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
					src.OffsetLocation = cubeColliderComponent["OffsetLocation"].as<Vector>();
					src.OffsetScale = cubeColliderComponent["OffsetScale"].as<Vector>();
					src.m_PhysicsMaterialPointerID = cubeColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}

			// SPHERE COLLIDER
			{
				auto sphereColliderComponent = entity["SphereColliderComponent"];
				if (sphereColliderComponent) {
					auto& src = *NewEntity.AddComponent<SphereColliderComponent>();
					src.IsTrigger = sphereColliderComponent["IsTrigger"].as<bool>();
					src.OffsetLocation = sphereColliderComponent["Offset"].as<Vector>();
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
					src.OffsetLocation = capsuleColliderComponent["Offset"].as<Vector>();
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
					rgb.FreezeLocation = rigidBodyComponent["FreezeLocation"].as<VectorTemplate<bool>>();
					rgb.FreezeRotation = rigidBodyComponent["FreezeRotation"].as<VectorTemplate<bool>>();
					rgb.m_RigidBodyType = EnumReflection::StringEnum<RigidBodyType>(rigidBodyComponent["Type"].as<std::string>());
				}
			}

			//Script Component
			{
				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent) {
					std::string enumClassName;
					auto& scp = *NewEntity.AddComponent<ScriptComponent>();
				}
			}
		}
		return true;
	}
	bool SceneSerializer::DeSerilizeText(AssetID ID)
	{
		auto assetInfo = AssetManager::GetAssetInfo(ID);
		auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path).string();
		return DeSerilizeText(path);
	}
	bool SceneSerializer::DeSerilizeBinary(const std::string& filepath) {
		PF_CORE_ASSERT(false, "Functon has not been created");
		return false;
	}
}