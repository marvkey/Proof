#include "Proofprch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <string>
#include <fstream>
#include <filesystem>
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Core/Application.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Project/Project.h"
#include "Mesh.h"
#include "Prefab.h"
#include "Proof/Scene/World.h"

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
			Type data = scriptField["Data"].as<Type>(0);    \
			fieldInstance.SetValue(data);                  \
			break;                                         \
	}
	SceneSerializer::SceneSerializer(World* Scene) {
		PF_CORE_ASSERT(Scene, "Scene cannot be nulltptr");
		m_Scene = Scene;
	}
	void SceneSerializer::SerilizeEntity(YAML::Emitter& out, entt::registry& registry, UUID entityID, entt::entity enttID, bool ispPrefab )
	{
		out << YAML::BeginMap;// entity
		out << YAML::Key << "Entity" << YAML::Value << entityID;
		{
			PF_CORE_ASSERT(registry.all_of< TagComponent>(enttID), "Does not contian Tag Componnet");
			TagComponent& Tag = registry.get< TagComponent>(enttID);
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

			PF_CORE_ASSERT(registry.all_of< TransformComponent>(enttID), "Does not contian Transform Component");

			TransformComponent& transfrom = registry.get<TransformComponent>(enttID);
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // transform component
			out << YAML::Key << "Location" << YAML::Value << transfrom.Location;
			out << YAML::Key << "Rotation" << YAML::Value << transfrom.GetRotationEuler();
			out << YAML::Key << "Scale" << YAML::Value << transfrom.Scale;
			out << YAML::EndMap; // transform component
		}
		{
			PF_CORE_ASSERT(registry.all_of< HierarchyComponent>(enttID), "Does not contian HierarchyComponent Component");

			HierarchyComponent& hierachyComponent = registry.get<HierarchyComponent>(enttID);
			out << YAML::Key << "HierarchyComponent";
			out << YAML::BeginMap; //HierarchyComponent
			out << YAML::Key << "ParentHandle" << YAML::Value << hierachyComponent.ParentHandle ;

			out << YAML::Key << "Children";
			out << YAML::Flow;
			out << YAML::BeginSeq;
			for (UUID simpleEnitty : hierachyComponent.Children)
			{
				out << simpleEnitty.Get();
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // HierarchyComponent
		}
		{
			if (registry.all_of<MeshComponent>(enttID)) {
				MeshComponent& meshComponent = registry.get<MeshComponent>(enttID);

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
			if (registry.try_get<SpriteComponent>(enttID)!= nullptr) {
				SpriteComponent& sprite = registry.get<SpriteComponent>(enttID);
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
			
				if (registry.all_of<SkyLightComponent>(enttID))
				{
					SkyLightComponent& skylight = registry.get<SkyLightComponent>(enttID);
					out << YAML::Key << "SkyLightComponent";
					out << YAML::BeginMap; // SkyLightComponentComponent
					out << YAML::Key << "TintColor" << skylight.ColorTint;
					out << YAML::Key << "SkyBoxLod" << skylight.SkyBoxLoad;
					out << YAML::Key << "MapRotation" << skylight.MapRotation;
					out << YAML::Key << "Intensity" << skylight.Intensity;
					out << YAML::Key << "DynamicSky" << skylight.DynamicSky;
					out << YAML::Key << "Turbidity" << skylight.Turbidity;
					out << YAML::Key << "Azimuth" << skylight.Azimuth;
					out << YAML::Key << "Inclination" << skylight.Inclination;
					out << YAML::Key << "Image" << skylight.Image;
					out << YAML::EndMap; // SkyLightComponentComponent
				}
			}
			{
				if (registry.all_of<DirectionalLightComponent>(enttID)) {
					DirectionalLightComponent& directonalLight = registry.get<DirectionalLightComponent>(enttID);
					out << YAML::Key << "DirectionalLightComponent";
					out << YAML::BeginMap; // DirectionalLightComponent
					out << YAML::Key << "Color" << directonalLight.Color;
					out << YAML::Key << "Intensity" << directonalLight.Intensity;
					out << YAML::EndMap; // DirectionalLightComponent
				}
			}

			{
				if (registry.all_of<PointLightComponent>(enttID))
				{
					PointLightComponent& pointLight = registry.get<PointLightComponent>(enttID);
					out << YAML::Key << "PointLightComponent";
					out << YAML::BeginMap; // PointLightComponent
					//out << YAML::Key << "Color" << pointLight.Color;
					//out << YAML::Key << "Intensity" << pointLight.Intensity;
					//
					//out << YAML::Key << "Constant" << pointLight.Constant;
					//out << YAML::Key << "Linear" << pointLight.Linear;
					//out << YAML::Key << "Quadratic" << pointLight.Quadratic;
					//out << YAML::Key << "Radius" << pointLight.Radius;

					out << YAML::EndMap; // PointLightComponent
				}
			}
			{
				if (registry.all_of<ScriptComponent>(enttID))
				{
					ScriptComponent& scriptComponent = registry.get<ScriptComponent>(enttID);
					out << YAML::Key << "ScriptComponent";
					out << YAML::BeginMap; //ScriptComponent
					out << YAML::Key << "Scripts" << YAML::BeginSeq; //scriptSeq
					for (const std::string& scriptName : scriptComponent.ScriptsNames)
					{
						if (!ScriptEngine::EntityClassExists(scriptName))
							continue;
						out << YAML::BeginMap;// Script
						out << YAML::Key << "Script" << scriptName;

						if (ScriptEngine::HasScriptFieldMap(entityID) == false || ispPrefab == true)
						{
							out << YAML::EndMap;//script
							continue;
						}
						out << YAML::Key << "ScriptFields" << YAML::Value;
						out << YAML::BeginSeq; // scriptfields
						Count<ScriptClass> entityClass = ScriptEngine::GetScriptClass(scriptName);
						const auto& fields = entityClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap(entityID);
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
								WRITE_SCRIPT_FIELD(Texture, uint64_t);

								case ScriptFieldType::Enum:
									{
										const std::string enumTypeName = ScriptEngine::GetFieldEnumName(field);
										if (!ScriptEngine::GetEnumClasses().contains(enumTypeName))
										{
											out << 0;
											break;
										}
										switch (ScriptEngine::GetEnumClasses().at(enumTypeName).first)
										{
											WRITE_SCRIPT_FIELD(Int8_t, int8_t);
											WRITE_SCRIPT_FIELD(Int16_t, int16_t);
											WRITE_SCRIPT_FIELD(Int32_t, int32_t);
											WRITE_SCRIPT_FIELD(Int64_t, int64_t);
											WRITE_SCRIPT_FIELD(Uint8_t, uint8_t);
											WRITE_SCRIPT_FIELD(Uint16_t, uint16_t);
											WRITE_SCRIPT_FIELD(Uint32_t, uint32_t);
											WRITE_SCRIPT_FIELD(Uint64_t, uint64_t);
											default: break;
										}
										break;
									}
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
				if (registry.all_of<SpotLightComponent>(enttID))
				{
					SpotLightComponent& spotLight = registry.get<SpotLightComponent>(enttID);

					out << YAML::Key << "SpotLightComponent";
					//out << YAML::BeginMap; // PointLightComponent
					//out << YAML::Key << "Color" << spotLight.Color;
					//
					//out << YAML::Key << "Intensity" << spotLight.Intensity;
					//out << YAML::Key << "Constant" <<spotLight.Constant;
					//out << YAML::Key << "Linear" << spotLight.Linear;
					//out << YAML::Key << "Quadratic" << spotLight.Quadratic;
					//out << YAML::Key << "Radius" << spotLight.Radius;
					//out << YAML::Key << "OuterCutoff" << spotLight.OuterCutOff;
					//out << YAML::Key << "CutOff" << spotLight.CutOff;

					out << YAML::EndMap; // PointLightComponent
				}
			}
		}
		{
			if (registry.all_of<TextComponent>(enttID))
			{
				TextComponent& textComponent = registry.get<TextComponent>(enttID);

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
			if (registry.all_of<CameraComponent>(enttID)) {
				CameraComponent& cameraComponent = registry.get<CameraComponent>(enttID);

				out << YAML::Key << "CameraComponent";
				out << YAML::BeginMap; // Camera Componet
				out << YAML::Key << "NearPlane" << cameraComponent.NearPlane;
				out << YAML::Key << "FarPlane" << cameraComponent.FarPlane;
				out << YAML::Key << "FOV" << cameraComponent.FovDeg;
				out << YAML::Key << "UseLocalRotation" << cameraComponent.UseLocalRotation;
				out << YAML::EndMap; // CameraComponet
			}
		}
		{
			if (registry.all_of<CubeColliderComponent>(enttID)) {
			
				CubeColliderComponent& cubeCollider = registry.get<CubeColliderComponent>(enttID);
				out << YAML::Key << "CubeColliderComponent";
				out << YAML::BeginMap; // CubeColliderComponent
				out << YAML::Key << "IsTrigger" << cubeCollider.IsTrigger;
				out << YAML::Key << "Center" << cubeCollider.Center;
				out << YAML::Key << "Size" << cubeCollider.Size;
				out << YAML::Key << "PhysicsMaterialPointerID" << cubeCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CubeColliderComponent
			}
		}
		{
			if (registry.all_of<SphereColliderComponent>(enttID)) {

				SphereColliderComponent& sphereCollider = registry.get<SphereColliderComponent>(enttID);
				out << YAML::Key << "SphereColliderComponent";
				out << YAML::BeginMap; // SphereColliderComponent
				out << YAML::Key << "IsTrigger" << sphereCollider.IsTrigger;
				out << YAML::Key << "Center" << sphereCollider.Center;
				out << YAML::Key << "Radius" << sphereCollider.Radius;
				out << YAML::Key << "PhysicsMaterialPointerID" << sphereCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // SphereColliderComponent
			}
		}
		{
			if (registry.all_of<CapsuleColliderComponent>(enttID)) {

				CapsuleColliderComponent& CapsuleCollider = registry.get<CapsuleColliderComponent>(enttID);
				out << YAML::Key << "CapsuleColliderComponent";
				out << YAML::BeginMap; // CapsuleColliderComponent
				out << YAML::Key << "IsTrigger" << CapsuleCollider.IsTrigger;
				out << YAML::Key << "Center" << CapsuleCollider.Center;
				out << YAML::Key << "Radius" << CapsuleCollider.Radius;
				out << YAML::Key << "Height" << CapsuleCollider.Height;
				out << YAML::Key << "Direction" << EnumReflection::EnumString<CapsuleDirection>(CapsuleCollider.Direction);
				out << YAML::Key << "PhysicsMaterialPointerID" << CapsuleCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CapsuleColliderComponent
			}
		}
		{
			if (registry.all_of<MeshColliderComponent>(enttID)) {
				MeshColliderComponent& meshCollider = registry.get<MeshColliderComponent>(enttID);
				out << YAML::Key << "MeshColliderComponent";
				out << YAML::BeginMap; // MeshColliderComponent
				out << YAML::Key << "IsTrigger" << meshCollider.IsTrigger;
				out << YAML::Key << "PhysicsMaterialPointerID" << meshCollider.m_PhysicsMaterialPointerID;
				out << YAML::Key << "MeshAssetPointerID" << meshCollider.m_MeshAssetPointerID;
				out << YAML::EndMap; // MeshColliderComponent
			}
		}
		{
			if (registry.all_of<RigidBodyComponent>(enttID)) {
				RigidBodyComponent& rigidBody = registry.get<RigidBodyComponent>(enttID);
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
			if (registry.all_of<PlayerInputComponent>(enttID))
			{
				PlayerInputComponent& playerInput = registry.get<PlayerInputComponent>(enttID);
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
			if (registry.all_of<PlayerHUDComponent>(enttID))
			{
				PlayerHUDComponent& hud = registry.get<PlayerHUDComponent>(enttID);
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
			if (registry.all_of<ParticleSystemComponent>(enttID))
			{
				ParticleSystemComponent& particleSystemComponent = registry.get<ParticleSystemComponent>(enttID);
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
		{
			if (registry.all_of<AudioComponent>(enttID))
			{
				AudioComponent& audioComponent = registry.get<AudioComponent>(enttID);
				out << YAML::Key << "AudioComponent";
				out << YAML::BeginMap; // AudioComponent

				out << YAML::Key << "VolumeMultiplier" << YAML::Key << audioComponent.VolumeMultiplier;
				out << YAML::Key << "PitchMultiplier" << YAML::Key << audioComponent.PitchMultiplier;
				out << YAML::Key << "Looping" << YAML::Key << audioComponent.Looping;
				out << YAML::Key << "PlayOnAwake" << YAML::Key << audioComponent.PlayOnAwake;
				out << YAML::Key << "MasterReverbSend" << YAML::Key << audioComponent.MasterReverbSend;
				out << YAML::Key << "LowPassFilter" << YAML::Key << audioComponent.LowPassFilter;
				out << YAML::Key << "HighPassFilter" << YAML::Key << audioComponent.HighPassFilter;
				out << YAML::Key << "SpatializationEnabled" << YAML::Key << audioComponent.SpatializationEnabled;
				out << YAML::Key << "AudioAsset" << YAML::Key << audioComponent.AudioAsset;

				if (audioComponent.SpatializationEnabled)
				{
					out << YAML::Key << "AttenuationModel" << YAML::Key << EnumReflection::EnumString(audioComponent.AttenuationModel);
					out << YAML::Key << "MinGain" << YAML::Key << audioComponent.MinGain;
					out << YAML::Key << "MaxGain" << YAML::Key << audioComponent.MaxGain;
					out << YAML::Key << "MinDistance" << YAML::Key << audioComponent.MinDistance;
					out << YAML::Key << "MaxDistance" << YAML::Key << audioComponent.MaxDistance;
					out << YAML::Key << "ConeInnerAngleInRadians" << YAML::Key << audioComponent.ConeInnerAngleInRadians;
					out << YAML::Key << "ConeOuterAngleInRadians" << YAML::Key << audioComponent.ConeOuterAngleInRadians;
					out << YAML::Key << "ConeOuterGain" << YAML::Key << audioComponent.ConeOuterGain;
					out << YAML::Key << "DopplerFactor" << YAML::Key << audioComponent.DopplerFactor;
					out << YAML::Key << "Rolloff" << YAML::Key << audioComponent.Rolloff;
				}

				out << YAML::EndMap; // AudioComponent

			}
		}
		{
			if (registry.all_of<AudioListenerComponent>(enttID))
			{
				AudioListenerComponent& audioListenerComponent = registry.get<AudioListenerComponent>(enttID);
				out << YAML::Key << "AudioListenerComponent";
				out << YAML::BeginMap; // AudioListenerComponent

				out << YAML::Key << "Active" << YAML::Key << audioListenerComponent.Active;
				out << YAML::Key << "ConeInnerAngleInRadians" << YAML::Key << audioListenerComponent.ConeInnerAngleInRadians;
				out << YAML::Key << "ConeOuterAngleInRadians" << YAML::Key << audioListenerComponent.ConeOuterAngleInRadians;
				out << YAML::Key << "ConeOuterGain" << YAML::Key << audioListenerComponent.ConeOuterGain;

				out << YAML::EndMap; // AudioListenerComponent
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
				out << YAML::Key << "ID" << m_Scene->GetID();

				out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
				m_Scene->m_Registry.each([&](auto entityID) {
					Entity entity = { entityID,m_Scene };
					SerilizeEntity(out,m_Scene->m_Registry, entity.GetUUID(),entityID);
				});
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
		auto parentDir = std::filesystem::path(filePath).parent_path();
		auto savePath = parentDir /= {FileSystem::GetFileName(filePath) + "." + "ProofWorld"};
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

		m_Scene->Name = FileSystem::GetFileName(filePath);
		PF_EC_WARN("Deserilizing World {}", m_Scene->Name.c_str());

		m_Scene->m_ID = worldData["ID"].as<uint64_t>();
		m_Scene->m_Registry.each([&](auto enttiy) {
			Entity entity = { enttiy ,m_Scene };
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

	void SceneSerializer::DeSerilizeEntity(YAML::Node& entities, World* world, std::set<AssetID>* assetLoad, bool isPrefab)
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
						auto& tc = NewEntity.GetComponent<TagComponent>();

						for (auto tag : tagcomponent["tags"])
						{
							tc.AddTag(tag.as<std::string>());
						}
					}
				}
			}
			// Transform
			{
				auto transformComponet = entity["TransformComponent"];
				if (transformComponet)
				{
					TransformComponent& tc = NewEntity.GetComponent<TransformComponent>();
					tc.Location = transformComponet["Location"].as<glm::vec3>();

					tc.SetRotationEuler(transformComponet["Rotation"].as<glm::vec3>());

					tc.Scale = transformComponet["Scale"].as<glm::vec3>();

				}
			}
			// HierarchyComponent
			{
				auto subEntityComponent = entity["HierarchyComponent"];
				if (subEntityComponent)
				{
					HierarchyComponent& tc = NewEntity.GetComponent<HierarchyComponent>();
					tc.ParentHandle = subEntityComponent["ParentHandle"].as<uint64_t>();

					if (subEntityComponent["Children"])
					{
						for (auto entityID : subEntityComponent["Children"])
						{
							uint64_t childID = entityID.as<uint64_t>();
							tc.Children.emplace_back(childID);
						}
					}	 

				}
			}
			// MESH
			{
				auto meshComponent = entity["MeshComponent"];
				if (meshComponent)
				{
					auto& src = NewEntity.AddComponent<MeshComponent>();
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
					auto& src = NewEntity.AddComponent<SpriteComponent>();
					uint64_t id = spriteRendererComponent["TextureAssetPointerID"].as<uint64_t>();
					src.Colour = spriteRendererComponent["Colour"].as<glm::vec4>();
					if (id != 0)
					{
						if (assetLoad)
							assetLoad->emplace((UUID)id);
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
					auto skyLight = entity["SkyLightComponent"];
					if (skyLight)
					{
						auto& src = NewEntity.AddComponent<SkyLightComponent>();
						src.Image = skyLight["Image"].as<uint64_t>();
						src.ColorTint = skyLight["TintColor"].as<Vector>();
						src.SkyBoxLoad = skyLight["SkyBoxLod"].as<float>();
						src.MapRotation = skyLight["MapRotation"].as<float>();
						src.Intensity = skyLight["Intensity"].as<float>();

						src.DynamicSky = skyLight["DynamicSky"].as<bool>();
						src.Turbidity = skyLight["Turbidity"].as<float>();
						src.Azimuth = skyLight["Azimuth"].as<float>();
						src.Inclination = skyLight["Inclination"].as<float>();
						if (AssetManager::HasAsset(src.Image))
						{
							src.LoadMap(src.Image);
						}
					}
				}

				{
					auto directionalLight = entity["DirectionalLightComponent"];
					if (directionalLight)
					{
						auto& src = NewEntity.AddComponent<DirectionalLightComponent>();
						src.Color = directionalLight["Color"].as<Vector>();
						src.Intensity = directionalLight["Intensity"].as<float>();
					}
				}

				{
					auto pointLight = entity["PointLightComponent"];
					if (pointLight)
					{
						auto& src = NewEntity.AddComponent<PointLightComponent>();
						//src.Color = pointLight["Color"].as<Vector>();
						//src.Intensity = pointLight["Intensity"].as<float>();
						//src.Constant = pointLight["Constant"].as<float>();
						//src.Linear = pointLight["Linear"].as<float>();
						//src.Quadratic = pointLight["Quadratic"].as<float>();
						//src.Radius = pointLight["Radius"].as<float>();

					}
				}

				{
					auto spotLight = entity["SpotLightComponent"];
					if (spotLight)
					{
						auto& src = NewEntity.AddComponent<SpotLightComponent>();
						//src.Color = spotLight["Color"].as<Vector>();
						//src.Intensity = spotLight["Intensity"].as<float>();
						//src.Constant = spotLight["Constant"].as<float>();
						//src.Linear = spotLight["Linear"].as<float>();
						//src.Quadratic = spotLight["Quadratic"].as<float>();
						//src.Radius = spotLight["Radius"].as<float>();
						//
						//src.CutOff = spotLight["CutOff"].as<float>();
						//src.OuterCutOff = spotLight["OuterCutOff"].as<float>();
					}
				}
			}
			// Text
			{

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					auto& src = NewEntity.AddComponent<TextComponent>();
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
					auto& src = NewEntity.AddComponent<CameraComponent>();
					src.NearPlane = cameraComponent["NearPlane"].as<float>();
					src.FarPlane = cameraComponent["FarPlane"].as<float>();
					src.FovDeg = cameraComponent["FOV"].as<float>();
					src.UseLocalRotation = cameraComponent["UseLocalRotation"].as<bool>();

				}
			}
			// CUBE COLLIDER
			{
				auto cubeColliderComponent = entity["CubeColliderComponent"];
				if (cubeColliderComponent)
				{
					auto& src = NewEntity.AddComponent<CubeColliderComponent>();
					src.IsTrigger = cubeColliderComponent["IsTrigger"].as<bool>();
					src.Center = cubeColliderComponent["Center"].as<glm::vec3>(src.Center);
					src.Size = cubeColliderComponent["Size"].as<glm::vec3>(src.Size);
					src.m_PhysicsMaterialPointerID = cubeColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}

			// SPHERE COLLIDER
			{
				auto sphereColliderComponent = entity["SphereColliderComponent"];
				if (sphereColliderComponent)
				{
					auto& src = NewEntity.AddComponent<SphereColliderComponent>();
					src.IsTrigger = sphereColliderComponent["IsTrigger"].as<bool>();
					src.Center = sphereColliderComponent["Offset"].as<glm::vec3>(src.Center);
					src.Radius = sphereColliderComponent["Radius"].as<float>();
					src.m_PhysicsMaterialPointerID = sphereColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}
			//CAPSULE COLLIDER
			{

				auto capsuleColliderComponent = entity["CapsuleColliderComponent"];
				if (capsuleColliderComponent)
				{
					auto& src = NewEntity.AddComponent<CapsuleColliderComponent>();
					src.IsTrigger = capsuleColliderComponent["IsTrigger"].as<bool>();
					src.Center = capsuleColliderComponent["Offset"].as<glm::vec3>(src.Center);
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
					auto& src = NewEntity.AddComponent<MeshColliderComponent>();
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
					auto& rgb = NewEntity.AddComponent<RigidBodyComponent>();
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
					auto& pic = NewEntity.AddComponent<PlayerInputComponent>();
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
					auto& phc = NewEntity.AddComponent<PlayerHUDComponent>();
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
			// audio compoennt
			{
				auto audioComponent = entity["AudioComponent"];
				if (audioComponent)
				{
					auto& ac = NewEntity.AddComponent<AudioComponent>();

					ac.VolumeMultiplier = audioComponent["VolumeMultiplier"].as<float>();
					ac.PitchMultiplier = audioComponent["PitchMultiplier"].as<float>();
					ac.Looping = audioComponent["Looping"].as<bool>();
					ac.PlayOnAwake = audioComponent["PlayOnAwake"].as<bool>();
					ac.MasterReverbSend = audioComponent["MasterReverbSend"].as<float>();
					ac.LowPassFilter = audioComponent["LowPassFilter"].as<float>();
					ac.HighPassFilter = audioComponent["HighPassFilter"].as<float>();
					ac.SpatializationEnabled = audioComponent["SpatializationEnabled"].as<bool>();
					ac.AudioAsset = audioComponent["AudioAsset"].as<uint64_t>();

					if (ac.SpatializationEnabled)
					{
						ac.AttenuationModel = EnumReflection::StringEnum<AttenuationModel>(audioComponent["AttenuationModel"].as<std::string>());
						ac.MinGain = audioComponent["MinGain"].as<float>();
						ac.MaxGain = audioComponent["MaxGain"].as<float>();

						ac.MinDistance = audioComponent["MinDistance"].as<float>();
						ac.MaxDistance = audioComponent["MaxDistance"].as<float>();

						ac.ConeInnerAngleInRadians = audioComponent["ConeInnerAngleInRadians"].as<float>();
						ac.ConeOuterAngleInRadians = audioComponent["ConeOuterAngleInRadians"].as<float>();
						ac.ConeOuterGain = audioComponent["ConeOuterGain"].as<float>();

						ac.DopplerFactor = audioComponent["DopplerFactor"].as<float>();
						ac.Rolloff = audioComponent["Rolloff"].as<float>();
					}
				}

			}
			// ParticleSystemComponent
			{
				auto particleSystemComponent = entity["ParticleSystemComponent"];
				if (particleSystemComponent)
				{
					auto& psc = NewEntity.AddComponent<ParticleSystemComponent>();
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
					auto& scp = NewEntity.AddComponent<ScriptComponent>();
					auto scripts = scriptComponent["Scripts"];
					for (auto script : scripts)
					{
						std::string scriptName = script["Script"].as<std::string>();
						auto& engineScripts = ScriptEngine::GetScripts();
						if (!engineScripts.contains(scriptName))
							continue;

						scp.ScriptsNames.insert(scriptName);
						if (isPrefab)
							continue;
						if (!script["ScriptFields"])continue;

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
								PF_ENGINE_WARN("Entity {} Script {} does not contain {} field", world->GetEntity(EntID).GetName(), scriptName, fieldName);
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
								READ_SCRIPT_FIELD(Texture, uint64_t);
								case ScriptFieldType::Enum:
									{
										const std::string enumTypeName = ScriptEngine::GetFieldEnumName(fieldInstance.Field);
										if (!ScriptEngine::GetEnumClasses().contains(enumTypeName))
										{
											fieldInstance.SetValue<uint64_t>(0);
											break;
										}
										switch (ScriptEngine::GetEnumClasses().at(enumTypeName).first)
										{
											READ_SCRIPT_FIELD(Int8_t, int8_t);
											READ_SCRIPT_FIELD(Int16_t, int16_t);
											READ_SCRIPT_FIELD(Int32_t, int32_t);
											READ_SCRIPT_FIELD(Int64_t, int64_t);
											READ_SCRIPT_FIELD(Uint8_t, uint8_t);
											READ_SCRIPT_FIELD(Uint16_t, uint16_t);
											READ_SCRIPT_FIELD(Uint32_t, uint32_t);
											READ_SCRIPT_FIELD(Uint64_t, uint64_t);
											default: break;
										}
										break;
									}
							}
						}
					}
				}
			}
		}
	}

	bool SceneSerializer::DeSerilizeText(UUID ID)
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