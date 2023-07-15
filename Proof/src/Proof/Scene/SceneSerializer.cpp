#include "Proofprch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <string>
#include <fstream>
#include <filesystem>
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Mesh.h"
#include "Prefab.h"
#include "Material.h"
namespace Proof
{
	#define WRITE_SCRIPT_FIELD(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				out << scriptField.GetValue<Type>();  \
				break
	#define READ_SCRIPT_FIELD(FieldType, Type)             \
		case ScriptFieldType::FieldType:                   \
		{                                                  \
			Type data = scriptField["Data"].as<Type>();    \
			fieldInstance.SetValue(data);                  \
			break;                                         \
	}
	SceneSerializer::SceneSerializer(World* Scene) {
		PF_CORE_ASSERT(Scene, "Scene cannot be nulltptr");
		m_Scene = Scene;
	}
	void SceneSerializer::SerilizeEntity(YAML::Emitter& out, entt::registry64& registry, UUID entityID) {
		out << YAML::BeginMap;// entity
		out << YAML::Key << "Entity" << YAML::Value << entityID;
		{
			PF_CORE_ASSERT(registry.any_of< TagComponent>(entityID), "Does not contian Tag Componnet");
			TagComponent& Tag = registry.get< TagComponent>(entityID);
			auto& tag = Tag.Tag;
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // tag component
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::Key << "tags";
			out << YAML::Flow;
			out << YAML::BeginSeq;
			for (std::string& tag : Tag.m_Tags) {
				out << tag;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // tag component
		}
		{

			PF_CORE_ASSERT(registry.any_of< TransformComponent>(entityID), "Does not contian Transform Component");

			TransformComponent& transfrom = registry.get<TransformComponent>(entityID);
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // transform component
			out << YAML::Key << "Location" << YAML::Value << transfrom.Location;
			out << YAML::Key << "Rotation" << YAML::Value << transfrom.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transfrom.Scale;
			out << YAML::EndMap; // transform component
		}
		{
			PF_CORE_ASSERT(registry.any_of< ChildComponent>(entityID), "Does not contian Child Component");

			ChildComponent& childComponent = registry.get<ChildComponent>(entityID);
			out << YAML::Key << "ChildComponent";
			out << YAML::BeginMap; //child
			out << YAML::Key << "OwnerID" << YAML::Value << childComponent.GetOwnerID();

			out << YAML::Key << "Children";
			out << YAML::Flow;
			out << YAML::BeginSeq;
			for (EntityID simpleEnitty : childComponent.m_Children)
			{
				out << simpleEnitty;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // child component
		}
		{
			if (registry.any_of<MeshComponent>(entityID)) {
				MeshComponent& meshComponent = registry.get<MeshComponent>(entityID);

				out << YAML::Key << "MeshComponent";
				out << YAML::BeginMap; // Mesh component
				out << YAML::Key << "MeshAssetPointerID" << YAML::Value << meshComponent.m_MeshID;
				if (AssetManager::HasAsset(meshComponent.GetMesh()))
				{
					if (*meshComponent.GetMesh()->GetMaterialTable() == *meshComponent.MaterialTable)
						goto leave;
					out << YAML::Key << "MaterialTable"; 
					out << YAML::BeginSeq;//MaterialTbale 
					for (auto& [index, material] : meshComponent.MaterialTable->GetMaterials())
					{
						out << YAML::BeginMap;// material

						// we nned th "" for some reason 
						out << YAML::Key << "Material" << YAML::Key << "";
							
						//id of 0 means default material
						out << YAML::Key << "AssetID" << YAML::Value << material->GetID();
						out << YAML::Key << "Index" << YAML::Value << index;

						out << YAML::EndMap;// material

					}
					out << YAML::EndSeq; // matrailTable
				}
				leave:
				out << YAML::EndMap; // Mesh component
			}
		}
		{
			if (registry.any_of<SpriteComponent>(entityID)) {
				SpriteComponent& sprite = registry.get<SpriteComponent>(entityID);
				out << YAML::Key << "SpriteComponent";
				out << YAML::BeginMap; // Sprite component
				if(sprite.Texture != nullptr)
					out << YAML::Key << "TextureAssetPointerID" << YAML::Value << sprite.Texture->GetID();
				else 
					out << YAML::Key << "TextureAssetPointerID" << YAML::Value << 0;
				out << YAML::Key << "Colour" << YAML::Value << sprite.Colour;
				out << YAML::EndMap; // Sprite component
			}
		}
		{
			
			{
				if (registry.any_of<DirectionalLightComponent>(entityID)) {
					DirectionalLightComponent& directonalLight = registry.get<DirectionalLightComponent>(entityID);
					out << YAML::Key << "DirectionalLightComponent";
					out << YAML::BeginMap; // DirectionalLightComponent
					out << YAML::Key << "Color" << directonalLight.Color;
					out << YAML::Key << "Intensity" << directonalLight.Intensity;
					out << YAML::EndMap; // DirectionalLightComponent
				}
			}

			{
				if (registry.any_of<PointLightComponent>(entityID))
				{
					PointLightComponent& pointLight = registry.get<PointLightComponent>(entityID);
					out << YAML::Key << "PointLightComponent";
					out << YAML::BeginMap; // PointLightComponent
					out << YAML::Key << "Color" << pointLight.Color;
					out << YAML::Key << "Intensity" << pointLight.Intensity;

					out << YAML::Key << "Constant" << pointLight.Constant;
					out << YAML::Key << "Linear" << pointLight.Linear;
					out << YAML::Key << "Quadratic" << pointLight.Quadratic;
					out << YAML::Key << "Radius" << pointLight.Radius;

					out << YAML::EndMap; // PointLightComponent
				}
			}
			{
				if (registry.any_of<ScriptComponent>(entityID))
				{
					ScriptComponent& scriptComponent = registry.get<ScriptComponent>(entityID);
					out << YAML::Key << "ScriptComponent";
					out << YAML::BeginMap; //ScriptComponent
					out << YAML::Key << "Scripts" << YAML::BeginSeq; //scriptSeq
					for (const std::string& scriptName : scriptComponent.ScriptsNames)
					{
						out << YAML::BeginMap;// Script
						out << YAML::Key << "Script" << scriptName;
						PF_ENGINE_INFO("Saving script {}", scriptName);

						out << YAML::Key << "ScriptFields" << YAML::Value;
						out << YAML::BeginSeq; // scriptfields
						Count<ScriptClass> entityClass = ScriptEngine::GetScriptClass(scriptName);
						const auto& fields = entityClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap({ entityID,nullptr });
						for (const auto& [fieldName, field] : fields)
						{
							if (!entityFields.contains(scriptName))
								continue;
							if (!entityFields.at(scriptName).contains(fieldName)) continue;

							out << YAML::BeginMap; // ScriptField
							out << YAML::Key << "Name" << YAML::Value << fieldName;
							out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

							out << YAML::Key << "Data" << YAML::Value;
							ScriptFieldInstance& scriptField = entityFields.at(scriptName).at(fieldName);

							switch (field.Type)
							{
								WRITE_SCRIPT_FIELD(Float, float);
								WRITE_SCRIPT_FIELD(Double, double);
								WRITE_SCRIPT_FIELD(Bool, bool);
								WRITE_SCRIPT_FIELD(Char, char);
								WRITE_SCRIPT_FIELD(Int8_t, int8_t);
								WRITE_SCRIPT_FIELD(Int16_t, int16_t);
								WRITE_SCRIPT_FIELD(Int32_t, int32_t);
								WRITE_SCRIPT_FIELD(Int64_t, int64_t);
								WRITE_SCRIPT_FIELD(Uint8_t, uint8_t);
								WRITE_SCRIPT_FIELD(Uint16_t, uint16_t);
								WRITE_SCRIPT_FIELD(Uint32_t, uint32_t);
								WRITE_SCRIPT_FIELD(Uint64_t, uint64_t);
								//WRITE_SCRIPT_FIELD(Vector2, Vector);
								//WRITE_SCRIPT_FIELD(Vector3, glm::vec3);
								//WRITE_SCRIPT_FIELD(Vector4, glm::vec4);
								WRITE_SCRIPT_FIELD(Entity, uint64_t);
								WRITE_SCRIPT_FIELD(Prefab, uint64_t);
								WRITE_SCRIPT_FIELD(ImageAsset, uint64_t);
							}
							out << YAML::EndMap; // ScriptField
						}
						out << YAML::EndSeq; // scriptfields
						out << YAML::EndMap;//script
					}
					out << YAML::EndSeq;//scriptSeq
					out << YAML::EndMap; // ScriptComponent
				}
			}

			{
				if (registry.any_of<SpotLightComponent>(entityID))
				{
					SpotLightComponent& spotLight = registry.get<SpotLightComponent>(entityID);

					out << YAML::Key << "SpotLightComponent";
					out << YAML::BeginMap; // PointLightComponent
					out << YAML::Key << "Color" << spotLight.Color;

					out << YAML::Key << "Intensity" << spotLight.Intensity;
					out << YAML::Key << "Constant" <<spotLight.Constant;
					out << YAML::Key << "Linear" << spotLight.Linear;
					out << YAML::Key << "Quadratic" << spotLight.Quadratic;
					out << YAML::Key << "Radius" << spotLight.Radius;
					out << YAML::Key << "OuterCutoff" << spotLight.OuterCutOff;
					out << YAML::Key << "CutOff" << spotLight.CutOff;

					out << YAML::EndMap; // PointLightComponent
				}
			}
		}
		{
			if (registry.any_of<TextComponent>(entityID))
			{
				TextComponent& textComponent = registry.get<TextComponent>(entityID);

				out << YAML::Key << "TextComponent";
				out << YAML::BeginMap; // Text Component
				out << YAML::Key << "Color" << textComponent.Colour;
				out << YAML::Key << "Kerning" << textComponent.Kerning;
				out << YAML::Key << "LineSpacing" << textComponent.LineSpacing;
				out << YAML::Key << "Text" << textComponent.Text;
				out << YAML::Key << "UseLocalRotation" << textComponent.UseLocalRotation;
				out << YAML::EndMap; // Text Component
			}
		}
		{
			if (registry.any_of<CameraComponent>(entityID)) {
				CameraComponent& cameraComponent = registry.get<CameraComponent>(entityID);

				out << YAML::Key << "CameraComponent";
				out << YAML::BeginMap; // Camera Componet
				out << YAML::Key << "NearPlane" << cameraComponent.NearPlane;
				out << YAML::Key << "FarPlane" << cameraComponent.FarPlane;
				out << YAML::Key << "FOV" << cameraComponent.FovDeg;
				out << YAML::Key << "Width" << cameraComponent.Width;
				out << YAML::Key << "Height" << cameraComponent.Height;
				out << YAML::Key << "UpVector" << cameraComponent.UPVector;
				out << YAML::Key << "UseLocalRotation" << cameraComponent.UseLocalRotation;
				out << YAML::EndMap; // CameraComponet
			}
		}
		{
			if (registry.any_of<CubeColliderComponent>(entityID)) {
			
				CubeColliderComponent& cubeCollider = registry.get<CubeColliderComponent>(entityID);
				out << YAML::Key << "CubeColliderComponent";
				out << YAML::BeginMap; // CubeColliderComponent
				out << YAML::Key << "IsTrigger" << cubeCollider.IsTrigger;
				out << YAML::Key << "OffsetLocation" << cubeCollider.OffsetLocation;
				out << YAML::Key << "OffsetScale" << cubeCollider.OffsetScale;
				out << YAML::Key << "PhysicsMaterialPointerID" << cubeCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CubeColliderComponent
			}
		}
		{
			if (registry.any_of<SphereColliderComponent>(entityID)) {

				SphereColliderComponent& sphereCollider = registry.get<SphereColliderComponent>(entityID);
				out << YAML::Key << "SphereColliderComponent";
				out << YAML::BeginMap; // SphereColliderComponent
				out << YAML::Key << "IsTrigger" << sphereCollider.IsTrigger;
				out << YAML::Key << "Offset" << sphereCollider.OffsetLocation;
				out << YAML::Key << "Radius" << sphereCollider.Radius;
				out << YAML::Key << "PhysicsMaterialPointerID" << sphereCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // SphereColliderComponent
			}
		}
		{
			if (registry.any_of<CapsuleColliderComponent>(entityID)) {

				CapsuleColliderComponent& CapsuleCollider = registry.get<CapsuleColliderComponent>(entityID);
				out << YAML::Key << "CapsuleColliderComponent";
				out << YAML::BeginMap; // CapsuleColliderComponent
				out << YAML::Key << "IsTrigger" << CapsuleCollider.IsTrigger;
				out << YAML::Key << "Offset" << CapsuleCollider.OffsetLocation;
				out << YAML::Key << "Radius" << CapsuleCollider.Radius;
				out << YAML::Key << "Height" << CapsuleCollider.Height;
				out << YAML::Key << "Direction" << EnumReflection::EnumString<CapsuleDirection>(CapsuleCollider.Direction);
				out << YAML::Key << "PhysicsMaterialPointerID" << CapsuleCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CapsuleColliderComponent
			}
		}
		{
			if (registry.any_of<MeshColliderComponent>(entityID)) {
				MeshColliderComponent& meshCollider = registry.get<MeshColliderComponent>(entityID);
				out << YAML::Key << "MeshColliderComponent";
				out << YAML::BeginMap; // MeshColliderComponent
				out << YAML::Key << "IsTrigger" << meshCollider.IsTrigger;
				out << YAML::Key << "PhysicsMaterialPointerID" << meshCollider.m_PhysicsMaterialPointerID;
				out << YAML::Key << "MeshAssetPointerID" << meshCollider.m_MeshAssetPointerID;
				out << YAML::EndMap; // MeshColliderComponent
			}
		}
		{
			if (registry.any_of<RigidBodyComponent>(entityID)) {
				RigidBodyComponent& rigidBody = registry.get<RigidBodyComponent>(entityID);
				out << YAML::Key << "RigidBodyComponent";
				out << YAML::BeginMap; // RigidBodyComponent
				out << YAML::Key << "Mass" << rigidBody.Mass;
				out << YAML::Key << "LinearDrag" << rigidBody.LinearDrag;
				out << YAML::Key << "AngularDrag" << rigidBody.AngularDrag;
				out << YAML::Key << "Gravity" << rigidBody.Gravity;
				out << YAML::Key << "Type" << EnumReflection::EnumString<RigidBodyType>(rigidBody.m_RigidBodyType);
				out << YAML::Key << "FreezeLocation" <<  rigidBody.FreezeLocation;
				out << YAML::Key << "FreezeRotation" <<  rigidBody.FreezeRotation;
				out << YAML::EndMap; // RigidBodyComponent
			}
		}

		{
			if (registry.any_of<PlayerInputComponent>(entityID))
			{
				PlayerInputComponent& playerInput = registry.get<PlayerInputComponent>(entityID);
				out << YAML::Key << "PlayerInputComponent";
				out << YAML::BeginMap; // PlayerInputComponent
				out << YAML::Key << "InputPlayer" << EnumReflection::EnumString(playerInput.InputPlayer);
				if(playerInput.Player)
					out << YAML::Key << "PrefabID" << playerInput.Player->GetID();
				else
					out << YAML::Key << "PrefabID" << 0;
				out << YAML::EndMap; // PlayerInputComponent
			}
		}
		
		{
			if (registry.any_of<PlayerHUDComponent>(entityID))
			{
				PlayerHUDComponent& hud = registry.get<PlayerHUDComponent>(entityID);
				out << YAML::Key << "PlayerHUDComponent";
				out << YAML::BeginMap; // PlayerHudComponent

				out << YAML::Key << "UiTable";
				out << YAML::BeginSeq;//hudTable
				if (hud.HudTable != nullptr)
				{
					for (auto& [index,panel] : hud.HudTable->GetPanels())
					{
						out << YAML::BeginMap;// hud

						out << YAML::Key << "HUD" << YAML::Key << "";
						AssetID  id = panel != nullptr ? panel->GetID() : AssetID(0);
						out << YAML::Key << "HUDAssetID" << YAML::Value << id;
						if(panel != nullptr)
							out << YAML::Key << "Visible" << YAML::Value << panel->Visible;
						out << YAML::Key << "Index" << YAML::Value << index;

						out << YAML::EndMap;// hud

					}
				}
				out << YAML::EndSeq; // hudTable
				out << YAML::EndMap; // PlayerHudComponent
			}
		}

		{
			if (registry.any_of<ParticleSystemComponent>(entityID))
			{
				ParticleSystemComponent& particleSystemComponent = registry.get<ParticleSystemComponent>(entityID);
				out << YAML::Key << "ParticleSystemComponent";
				out << YAML::BeginMap; // ParticleSystemComponent

				out << YAML::Key << "ParticleHandlerTable";
				out << YAML::BeginSeq;//ParticleHandlerTable
				if (particleSystemComponent.ParticleHandlerTable != nullptr)
				{
					for (auto& [index, particleHandler] : particleSystemComponent.ParticleHandlerTable->GetHandlers())
					{
						out << YAML::BeginMap;// particleHandler

						out << YAML::Key << "ParticleHandler" << YAML::Key << "";
						AssetID  id = 0;

						if (particleHandler != nullptr && particleHandler->GetParticleSystem() != nullptr)
							id = particleHandler->GetParticleSystem()->GetID();
						out << YAML::Key << "ParticleHandlerAssetID" << YAML::Value << id;

						if (particleHandler != nullptr)
							out << YAML::Key << "Visible" << YAML::Value << particleHandler->Visible;
						out << YAML::Key << "Index" << YAML::Value << index;

						out << YAML::EndMap;// particleHandler

					}
				}
				out << YAML::EndSeq; // ParticleHandlerTable
				out << YAML::EndMap; // ParticleSystemComponent
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
					SerilizeEntity(out,m_Scene->m_Registry, entityID);
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
		m_Scene->ForEachEntityBackwards([&](Entity entity) {
			m_Scene->DeleteEntity(entity);
		});
		m_Scene->DeleteEntitiesfromQeue();
		//for (auto entity : m_Scene->m_Registry.view<entt::entity>())
		//{
		//	m_Scene->m_Registry.destroy(entity);
		//}
		auto entities = worldData["Entities"];

		if (!entities)
			return false;

		DeSerilizeEntity(entities, m_Scene, &m_AssetLoadID);
		return true;
	}

	void SceneSerializer::DeSerilizeEntity(YAML::Node& entities, World* world, std::set<AssetID>* assetLoad, bool prefab)
	{
		for (auto entity : entities)
		{
			uint64_t EntID = entity["Entity"].as<uint64_t>();

			std::string Entityname;
			auto tagcomponent = entity["TagComponent"];
			if (tagcomponent)
			{
				Entityname = tagcomponent["Tag"].as<std::string>();
			}

			Entity NewEntity = world->CreateEntity(Entityname, EntID);
			{
				if (tagcomponent)
				{
					if (tagcomponent["tags"])
					{
						auto* tc = NewEntity.GetComponent<TagComponent>();

						for (auto tag : tagcomponent["tags"])
						{
							tc->AddTag(tag.as<std::string>());
						}
					}
				}
			}
			// Transform
			{
				auto transformComponet = entity["TransformComponent"];
				if (transformComponet)
				{
					auto* tc = NewEntity.GetComponent<TransformComponent>();
					tc->Location = transformComponet["Location"].as<Vector>();

					tc->Rotation = transformComponet["Rotation"].as<Vector>();
					tc->Scale = transformComponet["Scale"].as<Vector>();

				}
			}
			// ChildComponent
			{
				auto subEntityComponent = entity["ChildComponent"];
				if (subEntityComponent)
				{
					auto* tc = NewEntity.GetComponent<ChildComponent>();
					tc->m_OwnerID = subEntityComponent["OwnerID"].as<uint64_t>();

					if (subEntityComponent["Children"])
					{
						for (auto entityID : subEntityComponent["Children"])
						{
							uint64_t childID = entityID.as<uint64_t>();
							tc->m_Children.emplace_back(childID);
						}
					}

				}
			}
			// MESH
			{
				auto meshComponent = entity["MeshComponent"];
				if (meshComponent)
				{
					auto& src = *NewEntity.AddComponent<MeshComponent>();
					src.m_MeshID = meshComponent["MeshAssetPointerID"].as<uint64_t>();
					if (meshComponent["MaterialTable"])
					{
						Count<MaterialTable> matTable = Count<MaterialTable>::Create();
						for (auto mat : meshComponent["MaterialTable"])
						{
							AssetID id = mat["AssetID"].as<uint64_t>();
							uint32_t index = mat["Index"].as<uint32_t>();
							if (AssetManager::HasAsset(id))
							{
								matTable->SetMaterial(index, AssetManager::GetAsset<Material>(id));
							}
							else
							{
								matTable->SetMaterial(index, Count<Material>::Create("Default"));

							}
						}
						src.MaterialTable = matTable;
					}
					else
					{
						if (AssetManager::HasAsset(src.m_MeshID))
						{
							src.MaterialTable = Count<MaterialTable>::CreateFrom(AssetManager::GetAsset<Mesh>(src.m_MeshID)->GetMaterialTable());
						}
						else
						{
							src.MaterialTable = Count<MaterialTable>::Create();
						}
					}
				}
			}
			// SPRITE
			{
				auto spriteRendererComponent = entity["SpriteComponent"];
				if (spriteRendererComponent)
				{
					auto& src = *NewEntity.AddComponent<SpriteComponent>();
					uint64_t id = spriteRendererComponent["TextureAssetPointerID"].as<uint64_t>();
					src.Colour = spriteRendererComponent["Colour"].as<glm::vec4>();
					if (id != 0)
					{
						if (assetLoad)
							assetLoad->emplace(id);
						if (AssetManager::HasAsset(id))
						{
							src.Texture = AssetManager::GetAsset<Texture2D>(id);
						}
					}
				}
			}
			// LIGHT
			{

				{
					auto directionalLight = entity["DirectionalLightComponent"];
					if (directionalLight)
					{
						auto& src = *NewEntity.AddComponent<DirectionalLightComponent>();
						src.Color = directionalLight["Color"].as<Vector>();
						src.Intensity = directionalLight["Intensity"].as<float>();
					}
				}

				{
					auto pointLight = entity["PointLightComponent"];
					if (pointLight)
					{
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
					if (spotLight)
					{
						auto& src = *NewEntity.AddComponent<SpotLightComponent>();
						src.Color = spotLight["Color"].as<Vector>();
						src.Intensity = spotLight["Intensity"].as<float>();
						src.Constant = spotLight["Constant"].as<float>();
						src.Linear = spotLight["Linear"].as<float>();
						src.Quadratic = spotLight["Quadratic"].as<float>();
						src.Radius = spotLight["Radius"].as<float>();

						src.CutOff = spotLight["CutOff"].as<float>();
						src.OuterCutOff = spotLight["OuterCutOff"].as<float>();
					}
				}
			}
			// Text
			{

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					auto& src = *NewEntity.AddComponent<TextComponent>();
					src.Colour = textComponent["Color"].as<glm::vec4>();
					src.Kerning = textComponent["Kerning"].as<float>();
					src.LineSpacing = textComponent["LineSpacing"].as<float>();
					src.Text = textComponent["Text"].as<std::string>();
					if(textComponent["UseLocalRotation"])
						src.UseLocalRotation = textComponent["UseLocalRotation"].as<bool>();
				}
			}
			// CAMERA
			{
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& src = *NewEntity.AddComponent<CameraComponent>();
					src.NearPlane = cameraComponent["NearPlane"].as<float>();
					src.FarPlane = cameraComponent["FarPlane"].as<float>();
					src.FovDeg = cameraComponent["FOV"].as<float>();
					src.Width = cameraComponent["Width"].as<uint32_t>();
					src.Height = cameraComponent["Height"].as<uint32_t>();
					src.UseLocalRotation = cameraComponent["UseLocalRotation"].as<bool>();

				}
			}
			// CUBE COLLIDER
			{
				auto cubeColliderComponent = entity["CubeColliderComponent"];
				if (cubeColliderComponent)
				{
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
				if (sphereColliderComponent)
				{
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
				if (capsuleColliderComponent)
				{
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
				if (mehsCollider)
				{
					auto& src = *NewEntity.AddComponent<MeshColliderComponent>();
					src.IsTrigger = mehsCollider["IsTrigger"].as<bool>();
					src.m_PhysicsMaterialPointerID = mehsCollider["PhysicsMaterialPointerID"].as<uint64_t>();
					src.m_MeshAssetPointerID = mehsCollider["MeshAssetPointerID"].as<uint64_t>();
				}
			}
			// RIGID BODY
			{
				auto rigidBodyComponent = entity["RigidBodyComponent"];
				if (rigidBodyComponent)
				{
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

			//PlayerInputComponent
			{
				auto playerInputComponent = entity["PlayerInputComponent"];
				if (playerInputComponent)
				{
					auto& pic = *NewEntity.AddComponent<PlayerInputComponent>();
					pic.InputPlayer = EnumReflection::StringEnum< Players>(playerInputComponent["InputPlayer"].as<std::string>());
					uint64_t assetId = playerInputComponent["PrefabID"].as<uint64_t>();
					if (AssetManager::HasAsset(assetId))
						pic.Player = AssetManager::GetAsset<Prefab>(assetId);
				}
			}
			// PlayerHudComppoent
			{
				auto playerHudComponent = entity["PlayerHUDComponent"];
				if (playerHudComponent)
				{
					auto& phc = *NewEntity.AddComponent<PlayerHUDComponent>();
					Count<UITable> table = Count<UITable>::Create();
					for(auto hud :  playerHudComponent["UiTable"])
					{
						
						AssetID id = hud["HUDAssetID"].as<uint64_t>();
						uint32_t index= hud["Index"].as<uint32_t>();
						if (AssetManager::HasAsset(id))
						{
							table->SetUI(index, AssetManager::GetAsset<UIPanel>(id));
							if (hud["Visible"])
							{
								bool visible = hud["Visible"].as<bool>();
								table->GetPanel(index)->Visible = visible;
							}
						}
						else
						{
							table->SetUI(index, nullptr);
						}
					}
					phc.HudTable = table;
				}
			}
			// ParticleSystemComponent
			{
				auto particleSystemComponent = entity["ParticleSystemComponent"];
				if (particleSystemComponent)
				{
					auto& psc = *NewEntity.AddComponent<ParticleSystemComponent>();
					Count<ParticleHandlerTable> table = Count<ParticleHandlerTable>::Create();
					for (auto  particleHandler : particleSystemComponent["ParticleHandlerTable"])
					{

						AssetID id = particleHandler["ParticleHandlerAssetID"].as<uint64_t>();
						uint32_t index = particleHandler["Index"].as<uint32_t>();
						if (AssetManager::HasAsset(id))
						{
							table->SetHandler(index, Count<ParticleHandler>::Create( AssetManager::GetAsset<ParticleSystem>(id)));
							if (particleHandler["Visible"])
							{
								bool visible = particleHandler["Visible"].as<bool>();
								table->GetHandler(index)->Visible = visible;
							}
						}
						else
						{
							table->SetHandler(index, nullptr);
						}
					}
					psc.ParticleHandlerTable = table;
				}
			}
			//Script Component
			{
				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& scp = *NewEntity.AddComponent<ScriptComponent>();
					auto scripts = scriptComponent["Scripts"];
					for (auto script : scripts)
					{
						std::string scriptName = script["Script"].as<std::string>();
						auto& engineScripts = ScriptEngine::GetScripts();
						if (!engineScripts.contains(scriptName))
							continue;

						scp.ScriptsNames.insert(scriptName);
						if (prefab)
							continue;
						Count<ScriptClass> scriptClass = ScriptEngine::GetScriptClass(scriptName);
						const auto& fields = scriptClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap(NewEntity);

						auto scriptFields = script["ScriptFields"];

						for (auto scriptField : scriptFields)
						{
							std::string fieldName = scriptField["Name"].as<std::string>();
							std::string fieldTypeString = scriptField["Type"].as<std::string>();
							ScriptFieldType type = Utils::ScriptFieldTypeFromString(fieldTypeString);
							if (!fields.contains(fieldName))
							{
								PF_ENGINE_WARN("Entity {} Script {} does not contain {} field", Entity{ EntID,world }.GetName(), scriptName, fieldName);
								continue;
							}
							//creating the field instance adnscript
							ScriptFieldInstance& fieldInstance = entityFields[scriptName][fieldName];

							fieldInstance.Field = fields.at(fieldName);


							switch (type)
							{
								READ_SCRIPT_FIELD(Float, float);
								READ_SCRIPT_FIELD(Double, double);
								READ_SCRIPT_FIELD(Bool, bool);
								READ_SCRIPT_FIELD(Char, char);
								READ_SCRIPT_FIELD(Int8_t, int8_t);
								READ_SCRIPT_FIELD(Int16_t, int16_t);
								READ_SCRIPT_FIELD(Int32_t, int32_t);
								READ_SCRIPT_FIELD(Int64_t, int64_t);
								READ_SCRIPT_FIELD(Uint8_t, uint8_t);
								READ_SCRIPT_FIELD(Uint16_t, uint16_t);
								READ_SCRIPT_FIELD(Uint32_t, uint32_t);
								READ_SCRIPT_FIELD(Uint64_t, uint64_t);
								//READ_SCRIPT_FIELD(Vector2, glm::vec2);
								//READ_SCRIPT_FIELD(Vector3, glm::vec3);
								//READ_SCRIPT_FIELD(Vector4, glm::vec4);
								READ_SCRIPT_FIELD(Entity, uint64_t);
								READ_SCRIPT_FIELD(Prefab, uint64_t);
								READ_SCRIPT_FIELD(ImageAsset, uint64_t);
							}
						}
					}
				}
			}
		}
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