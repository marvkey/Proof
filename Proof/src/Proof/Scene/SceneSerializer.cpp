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
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scripting/ScriptWorld.h"
#include "Proof/Scripting/ScriptField.h"
#include "Proof/Scripting/ScriptFile.h"
#include "Proof/Asset/SerializeCommon.h"

#include "Proof/Scene/WaterSystem/WaterSystem.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Scene/WaterSystem/GerstnerWave.h"
#include "Proof/Scene/WaterSystem/FFTWave/FFTWave.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Utils/VariableSystem/Variable.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedPlayer.h"
#include "Proof/Input/ElevatedInputSystem/InputBindingContext.h"
#include "Material.h"
#include "TerrainRenderer/TerrainRenderer.h"
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

	
	
	static bool CanSaveAsset(AssetID id)
	{
		if (!AssetManager::HasAsset(id))
			return false;

		if (AssetManager::GetAssetInfo(id).RuntimeAsset)
		{
			if (!AssetManager::IsDefaultAsset(id))
				return false;
		}

		return true;
	}


	static bool CanSaveAsset(Count<Asset> asset)
	{
		if (!AssetManager::HasAsset(asset))
			return false;

		if (AssetManager::GetAssetInfo(asset).RuntimeAsset)
		{
			if (!AssetManager::IsDefaultAsset(asset->GetID()))
				return false;
		}

		return true;
	}
	SceneSerializer::SceneSerializer(Count<World> world) {
		PF_CORE_ASSERT(world, "Scene cannot be nulltptr");
		m_World = world;
	}
	void SceneSerializer::SerilizeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;// entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		{
			PF_CORE_ASSERT(entity.HasComponent< TagComponent>(), "Does not contian Tag Componnet");
			TagComponent& Tag = entity.GetComponent<TagComponent>();
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

			PF_CORE_ASSERT(entity.HasComponent<TransformComponent>(), "Does not contian Transform Component");

			TransformComponent& transfrom = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // transform component
			out << YAML::Key << "Location" << YAML::Value << transfrom.Location;
			out << YAML::Key << "Rotation" << YAML::Value << transfrom.GetRotationEuler();
			out << YAML::Key << "Scale" << YAML::Value << transfrom.Scale;
			out << YAML::EndMap; // transform component
		}
		{
			PF_CORE_ASSERT(entity.HasComponent<HierarchyComponent>(), "Does not contian HierarchyComponent Component");

			HierarchyComponent& hierachyComponent = entity.GetComponent<HierarchyComponent>();
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
			if (entity.HasComponent<MeshComponent>()) {
				MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();

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

						if (!CanSaveAsset(material.As<Asset>()))
							continue;
						out << YAML::BeginMap;// material

						// we nned th "" for some reason 
						out << YAML::Key << "Material" << YAML::Key << "";
							
						//id of 0 means default material
						out << YAML::Key << "AssetID" << YAML::Value << material.As<Asset>()->GetID();
						out << YAML::Key << "Index" << YAML::Value << index;

						out << YAML::EndMap;// material

					}
					out << YAML::EndSeq; // matrailTable
				}
				leave:
				out << YAML::EndMap; // Mesh component
			}
		}
		//dynamic Mesh
		{

			if (entity.HasComponent<DynamicMeshComponent>())
			{
				DynamicMeshComponent& dynamicMeshComponent = entity.GetComponent<DynamicMeshComponent>();

				out << YAML::Key << "DynamicMeshComponent";
				out << YAML::BeginMap; // DynamicMesh component
				out << YAML::Key << "DynamicMeshAssetPointerID" << YAML::Value << dynamicMeshComponent.m_MeshID;
				out << YAML::Key << "CastShadow" << YAML::Value << dynamicMeshComponent.CastShadow;
				out << YAML::Key << "Visible" << YAML::Value << dynamicMeshComponent.Visible;
				out << YAML::Key << "SubMeshIndex" << YAML::Value << dynamicMeshComponent.m_SubmeshIndex;
				if (AssetManager::HasAsset(dynamicMeshComponent.GetMesh()))
				{
					//if (*dynamicMeshComponent.GetMesh()->GetMaterialTable() == *dynamicMeshComponent.MaterialTable)
					//	goto leavedynamic;

					bool canSaveMaterialTable = false;
					auto meshMaterialTable = dynamicMeshComponent.GetMesh()->GetMaterialTable();
					for (auto [index, material] : dynamicMeshComponent.MaterialTable->GetMaterials())
					{
						//if(!meshMaterialTable->HasMaterial(index))
							//goto leavedynamic;

						if (meshMaterialTable->HasMaterial(index))
						{
							if (meshMaterialTable->GetMaterial(index) != material)
							{
								canSaveMaterialTable = true;
								break;
							}
							else
								canSaveMaterialTable = false;
						}
						else
						{
							canSaveMaterialTable = true;
							break;
						}
							

					}
					out << YAML::Key << "MaterialTable";
					out << YAML::BeginSeq;//MaterialTbale 
					for (auto& [index, material] : dynamicMeshComponent.MaterialTable->GetMaterials())
					{
						if (!CanSaveAsset(material.As<Asset>()))
							continue;
						out << YAML::BeginMap;// material

						// we nned th "" for some reason 
						out << YAML::Key << "Material" << YAML::Key << "";

						//id of 0 means default material
						out << YAML::Key << "AssetID" << YAML::Value << material.As<Asset>()->GetID();
						out << YAML::Key << "Index" << YAML::Value << index;

						out << YAML::EndMap;// material

					}
					out << YAML::EndSeq; // matrailTable
				}
				leavedynamic:
				out << YAML::EndMap; // DynamicMesh component
			}
		}

		{
			if (entity.HasComponent<TerrainComponent>())
			{
				const auto& terrainComponent = entity.GetComponent<TerrainComponent>();
				auto terrain = terrainComponent.Terrain;

				if (terrain)
				{
					out << YAML::Key << "TerrainComponent";
					out << YAML::BeginMap; // terrain component

					out << YAML::Key << "Seed" << YAML::Value << terrain->Seed;
					//out << YAML::Key << "MapSize" << YAML::Value << terrain->MapSize;
					out << YAML::Key << "TerrainScale" << YAML::Value << terrain->TerrainScale;

					out << YAML::Key << "NoiseParams";
					out << YAML::BeginMap; // noise
					{

						out << YAML::Key << "Scale" << YAML::Value << terrain->NoiseParams.Scale;
						out << YAML::Key << "Octaves" << YAML::Value << terrain->NoiseParams.Octaves;
						out << YAML::Key << "Persistence" << YAML::Value << terrain->NoiseParams.Persistence;
						out << YAML::Key << "Lacunarity" << YAML::Value << terrain->NoiseParams.Lacunarity;
						out << YAML::Key << "Offset" << YAML::Value << YAML::Flow << YAML::BeginSeq << terrain->NoiseParams.Offset.x << terrain->NoiseParams.Offset.y << YAML::EndSeq;
					}
					out << YAML::EndMap; // noise params

					SerializeCommon::SerializeInterpolationCurve(out, "Curve", terrain->Curve);

					out << YAML::EndMap; // ter
				}
			}
		}
		{
			if (entity.HasComponent<SpriteComponent>()) {
				SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
				out << YAML::Key << "SpriteComponent";
				out << YAML::BeginMap; // Sprite component
				out << YAML::Key << "TextureAssetPointerID" << YAML::Value << sprite.Texture.GetAssetID();
				out << YAML::Key << "Colour" << YAML::Value << sprite.Colour;
				out << YAML::EndMap; // Sprite component
			}
		}
		{
			{
			
				if (entity.HasComponent<SkyLightComponent>())
				{
					SkyLightComponent& skylight = entity.GetComponent<SkyLightComponent>();
					out << YAML::Key << "SkyLightComponent";
					out << YAML::BeginMap; // SkyLightComponentComponent
					out << YAML::Key << "TintColor" << skylight.ColorTint;
					out << YAML::Key << "SkyBoxLod" << skylight.SkyBoxLoad;
					out << YAML::Key << "MapRotation" << skylight.MapRotation;
					out << YAML::Key << "Intensity" << skylight.Intensity;
					out << YAML::Key << "EnvironmentState" << EnumReflection::EnumString(skylight.Environment->GetEnvironmentState());


					{
						auto texturData = skylight.Environment->GetTextureData();
						out << YAML::Key << "TextureDataImage" << texturData.Image;
					}
					{
						auto hosekData = skylight.Environment->GetHosekWilkieDataSkyData();
						out << YAML::Key << "HosekWilkieTurbidity" << hosekData.Turbidity;
						out << YAML::Key << "HosekWilkieGroundReflectance" << hosekData.GroundReflectance;
					}
					{
						PreethamSkyData data = skylight.Environment->GetPreethamSkyData();
						out << YAML::Key << "PreethamTurbidity" << data.Turbidity;
					}
					out << YAML::EndMap; // SkyLightComponentComponent
				}
			}
			{
				if (entity.HasComponent<DirectionalLightComponent>()) {
					DirectionalLightComponent& directonalLight = entity.GetComponent<DirectionalLightComponent>();
					out << YAML::Key << "DirectionalLightComponent";
					out << YAML::BeginMap; // DirectionalLightComponent
					out << YAML::Key << "Color" << directonalLight.Color;
					out << YAML::Key << "Intensity" << directonalLight.Intensity;
					out << YAML::Key << "CastShadow" << directonalLight.CastShadow;
					out << YAML::Key << "ShadowStrength" << directonalLight.ShadowStrength;
					out << YAML::Key << "CastSoftShadow" << directonalLight.CastSoftShadow;
					out << YAML::Key << "ShadowSoftness" << directonalLight.ShadowSoftness;
					out << YAML::EndMap; // DirectionalLightComponent
				}
			}

			{
				if (entity.HasComponent<PointLightComponent>())
				{
					PointLightComponent& pointLight = entity.GetComponent<PointLightComponent>();
					out << YAML::Key << "PointLightComponent";
					out << YAML::BeginMap; // PointLightComponent
					out << YAML::Key << "Color" << pointLight.Color;
					out << YAML::Key << "Intensity" << pointLight.Intensity;
					out << YAML::Key << "MinRadius" << pointLight.MinRadius;
					out << YAML::Key << "Radius" << pointLight.Radius;
					out << YAML::Key << "Falloff" << pointLight.Falloff;
					out << YAML::Key << "CastsShadows" << pointLight.CastsShadows;
					out << YAML::Key << "SoftShadows" << pointLight.SoftShadows;
					out << YAML::Key << "ShadowStrength" << pointLight.ShadowStrength;
					out << YAML::Key << "ShadowSoftness" << pointLight.ShadowSoftness;
					out << YAML::EndMap; // PointLightComponent
				}
			}


			{
				if (entity.HasComponent<SpotLightComponent>())
				{
					SpotLightComponent& spotLight = entity.GetComponent<SpotLightComponent>();

					out << YAML::Key << "SpotLightComponent";
					out << YAML::BeginMap; // SpotLightComponent

					out << YAML::Key << "Color" << spotLight.Color;
					out << YAML::Key << "Intensity" << spotLight.Intensity;
					out << YAML::Key << "Range" << spotLight.Range;
					out << YAML::Key << "Angle" << spotLight.Angle;
					out << YAML::Key << "AngleAttenuation" << spotLight.AngleAttenuation;
					out << YAML::Key << "Falloff" << spotLight.Falloff;
					out << YAML::Key << "CastsShadows" << spotLight.CastsShadows;
					out << YAML::Key << "SoftShadows" << spotLight.SoftShadows;
					out << YAML::Key << "ShadowStrength" << spotLight.ShadowStrength;
					out << YAML::Key << "ShadowSoftness" << spotLight.ShadowSoftness;

					out << YAML::EndMap; // PointLightComponent
				}
			}
			{
				#if 1
				if (entity.HasComponent<ScriptComponent>())
				{
					ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();
					Count<ScriptWorld> scriptWorld = entity.GetCurrentWorld()->GetScriptWorld();

					if (scriptWorld->IsEntityScriptInstantiated(entity))
					{
						ScriptClassesContainerMetaData* classesContainer = scriptWorld->GetEntityClassesContainer(entity);
						if (classesContainer->GetClassesMetaData().size() > 0)
						{

							out << YAML::Key << "ScriptComponent";
							out << YAML::BeginMap; //ScriptComponent

							out << YAML::Key << "Scripts" << YAML::BeginSeq; //scriptSeq

							for (const auto& [scriptName, scritpMetaData] : classesContainer->GetClassesMetaData())
							{
								out << YAML::BeginMap;// Script
								out << YAML::Key << "Script" << scriptName;

								out << YAML::Key << "ScriptClassName" << scritpMetaData.className;
								out << YAML::Key << "ScriptName" << scriptName;
								out << YAML::Key << "Fields" << YAML::Value;
								out << YAML::BeginSeq; // scriptfields
								for (const auto& [fieldName, fieldStorage] : scritpMetaData.Fields)
								{
									SerializeCommon::SaveScriptField(out, fieldStorage);
								}
								out << YAML::EndSeq; // ScriptField

								out << YAML::EndMap;//script

							}
							out << YAML::EndSeq;//scriptSeq
							out << YAML::EndMap; // ScriptComponent
						}
					}
				}
				#endif
			}

		}
		{
			if (entity.HasComponent<TextComponent>())
			{
				TextComponent& textComponent = entity.GetComponent<TextComponent>();

				out << YAML::Key << "TextComponent";
				out << YAML::BeginMap; // Text Component
				out << YAML::Key << "Color" << textComponent.Colour;
				out << YAML::Key << "Kerning" << textComponent.Kerning;
				out << YAML::Key << "LineSpacing" << textComponent.LineSpacing;
				out << YAML::Key << "Text" << textComponent.Text;
				out << YAML::Key << "Visible" << textComponent.Visible;
				out << YAML::Key << "UseLocalRotation" << textComponent.UseLocalRotation;
				out << YAML::Key << "RenderInViewSpace" << textComponent.RenderInViewSpace;
				out << YAML::EndMap; // Text Component
			}
		}
		{
			if (entity.HasComponent<CameraComponent>()) {
				CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();

				out << YAML::Key << "CameraComponent";
				out << YAML::BeginMap; // Camera Componet
				out << YAML::Key << "NearPlane" << cameraComponent.NearPlane;
				out << YAML::Key << "FarPlane" << cameraComponent.FarPlane;
				out << YAML::Key << "FOV" << cameraComponent.FovDeg;
				out << YAML::Key << "UseLocalRotation" << cameraComponent.UseLocalRotation;
				out << YAML::Key << "ActiveForRendering" << cameraComponent.ActiveForRendering;
				out << YAML::EndMap; // CameraComponet
			}
		}
		{
			if (entity.HasComponent<BoxColliderComponent>()) {
			
				BoxColliderComponent& cubeCollider = entity.GetComponent<BoxColliderComponent>();
				out << YAML::Key << "BoxColliderComponent";
				out << YAML::BeginMap; // BoxColliderComponent
				out << YAML::Key << "IsTrigger" << cubeCollider.IsTrigger;
				out << YAML::Key << "Center" << cubeCollider.Center;
				out << YAML::Key << "Size" << cubeCollider.Size;
				out << YAML::Key << "PhysicsMaterialPointerID" << cubeCollider.PhysicsMaterialKey.GetAssetID();
				out << YAML::EndMap; // BoxColliderComponent
			}
		}
		{
			if (entity.HasComponent<SphereColliderComponent>()) {

				SphereColliderComponent& sphereCollider = entity.GetComponent<SphereColliderComponent>();
				out << YAML::Key << "SphereColliderComponent";
				out << YAML::BeginMap; // SphereColliderComponent
				out << YAML::Key << "IsTrigger" << sphereCollider.IsTrigger;
				out << YAML::Key << "Center" << sphereCollider.Center;
				out << YAML::Key << "Radius" << sphereCollider.Radius;
				out << YAML::Key << "PhysicsMaterialPointerID" << sphereCollider.PhysicsMaterialKey.GetAssetID();
				out << YAML::EndMap; // SphereColliderComponent
			}
		}
		{
			if (entity.HasComponent<CapsuleColliderComponent>()) {

				CapsuleColliderComponent& CapsuleCollider = entity.GetComponent<CapsuleColliderComponent>();
				out << YAML::Key << "CapsuleColliderComponent";
				out << YAML::BeginMap; // CapsuleColliderComponent
				out << YAML::Key << "IsTrigger" << CapsuleCollider.IsTrigger;
				out << YAML::Key << "Center" << CapsuleCollider.Center;
				out << YAML::Key << "Radius" << CapsuleCollider.Radius;
				out << YAML::Key << "Height" << CapsuleCollider.Height;
				out << YAML::Key << "Direction" << EnumReflection::EnumString<CapsuleDirection>(CapsuleCollider.Direction);
				out << YAML::Key << "PhysicsMaterialPointerID" << CapsuleCollider.PhysicsMaterialKey.GetAssetID();
				out << YAML::EndMap; // CapsuleColliderComponent
			}
		}
		{
			if (entity.HasComponent<MeshColliderComponent>()) {
				MeshColliderComponent& meshColliderComponent = entity.GetComponent<MeshColliderComponent>();
				out << YAML::Key << "MeshColliderComponent";
				out << YAML::BeginMap; // MeshColliderComponent
				out << YAML::Key << "ColliderID" << YAML::Value << meshColliderComponent.ColliderKey.GetAssetID();
				out << YAML::Key << "IsTrigger" << YAML::Value << meshColliderComponent.IsTrigger;
				out << YAML::Key << "UseSharedShape" << YAML::Value << meshColliderComponent.UseSharedShape;
				out << YAML::Key << "PhysicsMaterialPointerID" << YAML::Value << meshColliderComponent.PhysicsMaterialKey.GetAssetID();
				//out << YAML::Key << "MeshAssetPointerID" << meshCollider.m_MeshAssetPointerID;
				out << YAML::EndMap; // MeshColliderComponent
			}
		}
		{
			if (entity.HasComponent<RigidBodyComponent>()) {
				RigidBodyComponent& rigidBody = entity.GetComponent<RigidBodyComponent>();
				out << YAML::Key << "RigidBodyComponent";
				out << YAML::BeginMap; // RigidBodyComponent
				out << YAML::Key << "Mass" << rigidBody.Mass;
				out << YAML::Key << "LinearDrag" << rigidBody.LinearDrag;
				out << YAML::Key << "PhysicsLayerID" << rigidBody.PhysicsLayerID;
				out << YAML::Key << "AngularDrag" << rigidBody.AngularDrag;
				out << YAML::Key << "Gravity" << rigidBody.Gravity;
				out << YAML::Key << "Type" << EnumReflection::EnumString<RigidBodyType>(rigidBody.RigidBodyType);
				out << YAML::Key << "FreezeLocation" <<  rigidBody.FreezeLocation;
				out << YAML::Key << "FreezeRotation" <<  rigidBody.FreezeRotation;
				out << YAML::Key << "Kinematic" << rigidBody.Kinematic;
				out << YAML::Key <<"CollisionDetection"<<EnumReflection::EnumString<CollisionDetectionType>( rigidBody.CollisionDetection);

				out << YAML::EndMap; // RigidBodyComponent
			}
		}

		{
			if (entity.HasComponent<CharacterControllerComponent>())
			{
				CharacterControllerComponent& characterController = entity.GetComponent<CharacterControllerComponent>();
				out << YAML::Key << "CharacterControllerComponent";
				out << YAML::BeginMap; // CharacterControllerComponent
				out << YAML::Key << "SlopeLimitRadians" << characterController.SlopeLimitRadians;
				out << YAML::Key << "StepOffset" << characterController.StepOffset;
				out << YAML::Key << "SkinOffset" << characterController.SkinOffset;
				out << YAML::Key << "GravityEnabled" << characterController.GravityEnabled;
				out << YAML::Key << "GravityScale" << characterController.GravityScale;
				out << YAML::Key << "MinMoveDistance" << characterController.MinMoveDistance;
				out << YAML::Key << "WalkableMode" <<  EnumReflection::EnumString(characterController.WalkableMode);
				out << YAML::Key << "PhysicsMaterialID" << characterController.PhysicsMaterialKey.GetAssetID();

				out << YAML::Key << "ColliderType" << EnumReflection::EnumString(characterController.ColliderType);
				out << YAML::Key << "Center" << characterController.Center;
				out << YAML::Key << "Radius" << characterController.Radius;
				out << YAML::Key << "Height" << characterController.Height;

				out << YAML::Key << "Size" << characterController.Size;
				out << YAML::EndMap; // CharacterControllerComponent
			}
		}
		{
			if (entity.HasComponent<PlayerInputComponent>())
			{
				PlayerInputComponent& playerInput = entity.GetComponent<PlayerInputComponent>();
				out << YAML::Key << "PlayerInputComponent";
				out << YAML::BeginMap; // PlayerInputComponent
				//out << YAML::Key << "InputPlayer" << EnumReflection::EnumString(playerInput.InputPlayer);


				out << YAML::Key << "InputBindings";
				out << YAML::BeginSeq;//InputBindings
				{
					if (playerInput.Player != nullptr)
					{
						for (auto& inputBinding : playerInput.Player->GetInputBindingContextList())
						{
							out << YAML::BeginMap;// InputBindingContextHandler
							out << YAML::Key << "InputBindingContextHandler" << YAML::Key << "";

							out << YAML::Key << "InputBindingID" << YAML::Value << inputBinding.InputBindingContext->GetID();
							out << YAML::Key << "Active" << YAML::Value << inputBinding.Active;

							out << YAML::EndMap;// InputBindingContextHandler

						}
					}
				}
				out << YAML::EndSeq; // InputBindings

				out << YAML::EndMap; // PlayerInputComponent
			}
		}

		{
			if (entity.HasComponent<PlayerStartComponent>())
			{
				PlayerStartComponent& playerStart = entity.GetComponent<PlayerStartComponent>();
				out << YAML::Key << "PlayerStartComponent";
				out << YAML::BeginMap; // PlayerStartComponent

				out << YAML::Key << "InputPlayer" << YAML::Value << EnumReflection::EnumString(playerStart.InputPlayer);
				out << YAML::Key << "PlayerPrefab" << YAML::Value << playerStart.Player.GetAssetID();
				out << YAML::Key << "DontSpawn" << YAML::Value << playerStart.DontSpawn;
				out << YAML::EndMap; // PlayerStartComponent

			}
		}

		{
		#if 1
			if (entity.HasComponent<PlayerHUDComponent>())
			{
				
				PlayerHUDComponent& hud = entity.GetComponent<PlayerHUDComponent>();
				out << YAML::Key << "PlayerHUDComponent";
				out << YAML::BeginMap; // PlayerHudComponent

				out << YAML::Key << "UITable";

				out << YAML::BeginSeq;//hudTable
				if (hud.HudTable != nullptr)
				{
					for (const auto& layer : hud.HudTable->GetLayers())
					{
						out << YAML::BeginMap;// Layer
						out << YAML::Key << "Layer" << YAML::Key << layer.Name;
						out << YAML::Key << "Visible" << YAML::Key << layer.Visible;

						out << YAML::Key << "Panels";
						out << YAML::BeginSeq;//Panels

						for (auto& panel : layer.GetUIPanels())
						{
							out << YAML::BeginMap;// panel

							AssetID assetID = panel->GetUIPanel() == nullptr ? (AssetID)0 : panel->GetUIPanel()->GetID();
							out << YAML::Key << "PanelID" << YAML::Value << assetID;
							out << YAML::Key << "Visible" << YAML::Value << panel->Visible;
							
							if(assetID != 0)
								SerializeCommon::SaveVariableRegistryInstance(out, panel->GetVariableRegistryInstance());
							out << YAML::EndMap;// panel
						}
						out << YAML::EndSeq;//Panels
						out << YAML::EndMap;// laayer
					}
				}
				out << YAML::EndSeq; // UITableTable
				out << YAML::EndMap; // PlayerHudComponent
			}
		#endif
		}

		{
			if (entity.HasComponent<ParticleSystemComponent>())
			{
				ParticleSystemComponent& particleSystemComponent = entity.GetComponent<ParticleSystemComponent>();
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
			if (entity.HasComponent<AudioComponent>())
			{
				AudioComponent& audioComponent = entity.GetComponent<AudioComponent>();
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
			if (entity.HasComponent<AudioListenerComponent>())
			{
				AudioListenerComponent& audioListenerComponent = entity.GetComponent<AudioListenerComponent>();
				out << YAML::Key << "AudioListenerComponent";
				out << YAML::BeginMap; // AudioListenerComponent

				out << YAML::Key << "Active" << YAML::Key << audioListenerComponent.Active;
				out << YAML::Key << "ConeInnerAngleInRadians" << YAML::Key << audioListenerComponent.ConeInnerAngleInRadians;
				out << YAML::Key << "ConeOuterAngleInRadians" << YAML::Key << audioListenerComponent.ConeOuterAngleInRadians;
				out << YAML::Key << "ConeOuterGain" << YAML::Key << audioListenerComponent.ConeOuterGain;

				out << YAML::EndMap; // AudioListenerComponent
			}

		}
		//water
		{
			if (entity.HasComponent<WaterComponent>())
			{
				WaterComponent& waterComponent = entity.GetComponent<WaterComponent>();
				out << YAML::Key << "WaterComponent";
				out << YAML::BeginMap; // WaterComponent
				out << YAML::Key << "WaveType" << YAML::Key << EnumReflection::EnumString(waterComponent.Water->GetWaveType());
				out << YAML::Key << "Density" << YAML::Key << waterComponent.Water->Density;

				switch (waterComponent.Water->GetWaveType())
				{
					case WaveType::GerstnerWave:
					{
						const GerstnerWave::GerstnerWaveInfo& waterDataInfo = waterComponent.Water->GetWave().As<GerstnerWave>()->GerstnerData;

						out << YAML::Key << "WaveCount " << YAML::Key << waterDataInfo.WaveCount;
						out << YAML::Key << "WaveDistribution" << YAML::Key << waterDataInfo.WaveDistribution;
						out << YAML::Key << "WaveDirection" << YAML::Key << waterDataInfo.WaveDirection;
						out << YAML::Key << "WaveSpread" << YAML::Key << waterDataInfo.WaveSpread;

						out << YAML::Key << "Color" << YAML::Key << waterDataInfo.Color;
						out << YAML::Key << "Speed" << YAML::Key << waterDataInfo.Speed;

						out << YAML::Key << "MinMaxWavelength" << YAML::Key << waterDataInfo.MinMaxWavelength;
						out << YAML::Key << "WavelengthFalloff" << YAML::Key << waterDataInfo.WavelengthFalloff;

						out << YAML::Key << "MinMaxAmplitude" << YAML::Key << waterDataInfo.MinMaxAmplitude;
						out << YAML::Key << "AmplitudeFallOff" << YAML::Key << waterDataInfo.AmplitudeFallOff;

						out << YAML::Key << "MinMaxSteepness" << YAML::Key << waterDataInfo.MinMaxSteepness;
						out << YAML::Key << "SteepnessFallOff" << YAML::Key << waterDataInfo.SteepnessFallOff;

						out << YAML::Key << "Seed" << YAML::Key << waterDataInfo.Seed;
						out << YAML::Key << "RandomSeed" << YAML::Key << waterDataInfo.RandomSeed;
						out << YAML::Key << "PlaneSize" << YAML::Key << waterDataInfo.PlaneSize;
					}
					break;
					case WaveType::FastFourierTransformWave:
					{
						const FFTWave::FFTWaveInfo& waterDataInfo = waterComponent.Water->GetWave().As<FFTWave>()->WaveInfo;
						out << YAML::Key << "OceanSize" << YAML::Value << EnumReflection::EnumString(waterDataInfo.OceanSize);
						out << YAML::Key << "NumCascades" << YAML::Value << waterDataInfo.NumCascades;
						out << YAML::Key << "NormalStrength" << YAML::Value << waterDataInfo.NormalStrength;
						out << YAML::Key << "Roughness" << YAML::Value << waterDataInfo.Roughness;
						out << YAML::Key << "WaterColor" << YAML::Value << waterDataInfo.WaterColor;
						out << YAML::Key << "FoamColor" << YAML::Value << waterDataInfo.FoamColor;

						out << YAML::Key << "DisplacementScale" << YAML::Value << waterDataInfo.DisplacementScale;
						out << YAML::Key << "NormalScale" << YAML::Value << waterDataInfo.NormalScale;
						out << YAML::Key << "WindSpeedScale" << YAML::Value << waterDataInfo.WindSpeedScale;
						out << YAML::Key << "SwellScale" << YAML::Value << waterDataInfo.SwellScale;
						out << YAML::Key << "DetailScale" << YAML::Value << waterDataInfo.DetailScale;
						out << YAML::Key << "SpreadScale" << YAML::Value << waterDataInfo.SpreadScale;

						out << YAML::Key << "FoamTimeScale" << YAML::Value << waterDataInfo.FoamTimeScale;
						out << YAML::Key << "WhitecapScale" << YAML::Value << waterDataInfo.WhitecapScale;
						out << YAML::Key << "FoamGrowthScale" << YAML::Value << waterDataInfo.FoamGrowthScale;
						out << YAML::Key << "FoamDecayScale" << YAML::Value << waterDataInfo.FoamDecayScale;

						out << YAML::Key << "Cascades";
						out << YAML::BeginSeq;//Cascades
						{
							for (const Count<FFTWaveCascade>& cascade : waterComponent.Water->GetWave().As<FFTWave>()->GetCascades())
							{
								out << YAML::BeginMap;// cascade

								out << YAML::Key << "Cascade" << YAML::Key << cascade->GetCascadeIndex();

								const auto& cascadeSettings = cascade->Settings;

								out << YAML::Key << "TileLength" << YAML::Value << cascadeSettings.TileLength;
								out << YAML::Key << "DisplacementScale" << YAML::Value << cascadeSettings.DisplacementScale;
								out << YAML::Key << "NormalScale" << YAML::Value << cascadeSettings.NormalScale;
								out << YAML::Key << "WindSpeed" << YAML::Value << cascadeSettings.WindSpeed;
								out << YAML::Key << "WindDirection" << YAML::Value << cascadeSettings.WindDirection;
								out << YAML::Key << "FetchLength" << YAML::Value << cascadeSettings.FetchLength;
								out << YAML::Key << "Swell" << YAML::Value << cascadeSettings.Swell;
								out << YAML::Key << "Detail" << YAML::Value << cascadeSettings.Detail;
								out << YAML::Key << "Spread" << YAML::Value << cascadeSettings.Spread;
								out << YAML::Key << "FoamTimeScale" << YAML::Value << cascadeSettings.FoamTimeScale;
								out << YAML::Key << "Whitecap" << YAML::Value << cascadeSettings.Whitecap;
								out << YAML::Key << "FoamAmount" << YAML::Value << cascadeSettings.FoamAmount;

								out << YAML::EndMap;// cascade
							}
						}
						out << YAML::EndSeq; // Cascade

						break;
					}
					default:
						break;
				}
			


				out << YAML::EndMap; // WaterComponent
			}

		}

		//buoyancy
		{
			if (entity.HasComponent<BuoyancyComponent>())
			{
				BuoyancyComponent& buoyancyComponent = entity.GetComponent<BuoyancyComponent>();
				out << YAML::Key << "BuoyancyComponent";
				out << YAML::BeginMap; // buoyancyComponent

				
				out << YAML::Key << "Density" << buoyancyComponent.Density;
				out << YAML::Key << "VoxelRelativeSize" << buoyancyComponent.VoxelRelativeSize.GetValue();
				out << YAML::Key << "DragInWater" << buoyancyComponent.DragInWater;
				out << YAML::Key << "AngularDragInWater" << buoyancyComponent.AngularDragInWater;
				out << YAML::Key << "WaveDisplacementFactor" << buoyancyComponent.WaveDisplacementFactor.GetValue();
				out << YAML::Key << "AlwaysStayUpright" << buoyancyComponent.AlwaysStayUpright;


				out << YAML::Key << "Floaters";
				out << YAML::BeginSeq;//Floaters
				{
					for (auto& [entityID,floater] : buoyancyComponent.Floaters)
					{
						out << YAML::BeginMap;// floater

						out << YAML::Key << "Floater" << YAML::Key << "";

						out << YAML::Key << "EntityID" << YAML::Value << entityID;
						out << YAML::Key << "SubmersionDepth" << YAML::Value << floater.SubmersionDepth;
						out << YAML::Key << "Drag" << YAML::Value << floater.Drag;
						out << YAML::Key << "AngularDrag" << YAML::Value << floater.AngularDrag;
						out << YAML::Key << "BuoyancyStrength" << YAML::Value << floater.BuoyancyStrength;

						out << YAML::EndMap;// floater
					}
					
				}
				out << YAML::EndSeq; // Floaters

				out << YAML::EndMap; // buoyancyComponent
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
				out << YAML::Key << "ID" << m_World->GetID();

				out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
				for (auto [id, entity] : m_World->GetEntities())
				{
					SerilizeEntity(out, entity);
				}
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
			PF_CORE_ASSERT(false,fmt::format("Deerilize scenePath:{} not exist", filePath));
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["World"]) // if there is no scene no
			return false;

		auto worldData = data["World"];

		m_World->Name = FileSystem::GetFileName(filePath);
		PF_EC_WARN("Deserilizing World {}", m_World->Name.c_str());

		m_World->m_ID = worldData["ID"].as<uint64_t>();

		for(auto [id, entity] :m_World->GetEntities())
			m_World->DeleteEntity(entity);

		m_World->DeleteEntitiesfromQeue();

		auto entities = worldData["Entities"];
		if (!entities)
			return false;

		DeSerilizeEntity(entities, m_World);
		// one time error where we saved and somehtign bad occured so some entities are holdin children that dont exist
		auto hiearchyView = m_World->m_Registry.view<HierarchyComponent>();
		for (auto entity : hiearchyView)
		{
			HierarchyComponent& hierarchyComponent = hiearchyView.get<HierarchyComponent>(entity);
			std::vector<UUID>& children = hierarchyComponent.Children;
			if (hierarchyComponent.Children.size() > 0)
			{

				for (auto it = children.begin(); it != children.end(); )
				{
					if (!m_World->HasEntity(*it))
						it = children.erase(it);
					else
						++it;
				}
			}
		}
		return true;
	}

	void SceneSerializer::DeSerilizeEntity(YAML::Node& entities, Count<World> world)
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

			{
				auto terrainComponent = entity["TerrainComponent"];

				if (terrainComponent)
				{
					auto& trc = NewEntity.AddComponent<TerrainComponent>();
					auto terrain = Count<TerrainRenderer>::Create(); // or Create/Make if you have a factory
					trc.Terrain = terrain;


					terrain->Seed = terrainComponent["Seed"].as<int>(0);
					//terrain->MapSize = terrainComponent["MapSize"].as<int>(terrain->MapSize);
					terrain->TerrainScale = terrainComponent["TerrainScale"].as<float>(terrain->TerrainScale);

					const auto& noiseNode = terrainComponent["NoiseParams"];
					terrain->NoiseParams.Scale = noiseNode["Scale"].as<float>(terrain->NoiseParams.Scale);
					terrain->NoiseParams.Octaves = noiseNode["Octaves"].as<int>(terrain->NoiseParams.Octaves.GetValue());
					terrain->NoiseParams.Persistence = noiseNode["Persistence"].as<float>(terrain->NoiseParams.Persistence.GetValue());
					terrain->NoiseParams.Lacunarity = noiseNode["Lacunarity"].as<float>(terrain->NoiseParams.Lacunarity);
					terrain->NoiseParams.Offset = noiseNode["Offset"].as<glm::vec2>(terrain->NoiseParams.Offset);
					terrain->RegenerateTerrainMesh();

					SerializeCommon::LoadInterpolationCurve(terrainComponent, "Curve", terrain->Curve);
				}

			}
			// MESH
			{
				auto meshComponent = entity["MeshComponent"];
				if (meshComponent)
				{
					auto& src = NewEntity.AddComponent<MeshComponent>();
					src.m_MeshID = meshComponent["MeshAssetPointerID"].as<uint64_t>();
					//src.SetMesh( meshComponent["MeshAssetPointerID"].as<uint64_t>());
					if (meshComponent["MaterialTable"])
					{
						Count<MaterialTable> matTable = Count<MaterialTable>::Create(false);
						for (auto mat : meshComponent["MaterialTable"])
						{
							AssetID id = mat["AssetID"].as<uint64_t>();
							uint32_t index = mat["Index"].as<uint32_t>();
							if (AssetManager::HasAsset(id))
							{
								matTable->SetMaterial(index, AssetManager::GetAsset<Material>(id));
							}
							//else
							//{
							//	matTable->SetMaterial(index, Count<Material>::Create("Default"));
							//
							//}
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

			//DYNAMIC MESH
			{
				auto dynamicMeshComponent = entity["DynamicMeshComponent"];

				if (dynamicMeshComponent)
				{
					auto& src = NewEntity.AddComponent<DynamicMeshComponent>();
					
					src.m_MeshID = dynamicMeshComponent["DynamicMeshAssetPointerID"].as<uint64_t>(0);
					src.CastShadow = dynamicMeshComponent["CastShadow"].as<bool>();
					src.Visible = dynamicMeshComponent["Visible"].as<bool>();
					src.m_SubmeshIndex =dynamicMeshComponent["SubMeshIndex"].as<uint32_t>();

					if (dynamicMeshComponent["MaterialTable"])
					{
						Count<MaterialTable> matTable = Count<MaterialTable>::Create(false);
						for (auto mat : dynamicMeshComponent["MaterialTable"])
						{
							AssetID id = mat["AssetID"].as<uint64_t>();
							uint32_t index = mat["Index"].as<uint32_t>();
							if (AssetManager::HasAsset(id))
							{
								matTable->SetMaterial(index, AssetManager::GetAsset<Material>(id));
							}
							//else
							//{
							//	matTable->SetMaterial(index, Count<Material>::Create("Default"));
							//
							//}
						}
						src.MaterialTable = matTable;
					}
					else
					{
						if (AssetManager::HasAsset(src.m_MeshID))
						{
							//src.MaterialTable = Count<MaterialTable>::CreateFrom(AssetManager::GetAsset<DynamicMesh>(src.m_MeshID)->GetMaterialTable());
							src.MaterialTable = AssetManager::GetAsset<DynamicMesh>(src.m_MeshID)->GetMaterialTableBasedOnSubMeshIndex(src.m_SubmeshIndex);
						}
						else
						{
							src.MaterialTable = Count<MaterialTable>::Create();
						}
					}
				}
			}

			{

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
							//m_AssetLoadID.emplace((UUID)id);
						if (AssetManager::HasAsset(id))
						{
							src.Texture = AssetManager::GetAsset<Texture2D>(id)->GetID();
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
						src.ColorTint = skyLight["TintColor"].as<glm::vec3>(src.ColorTint);
						src.SkyBoxLoad = skyLight["SkyBoxLod"].as<float>(0);
						src.MapRotation = skyLight["MapRotation"].as<float>(0);
						src.Intensity = skyLight["Intensity"].as<float>(1);
						auto state = EnumReflection::StringEnum<EnvironmentState>( skyLight["EnvironmentState"].as<std::string>(EnumReflection::EnumString(EnvironmentState::HosekWilkie)));
						switch (state)
						{
						case Proof::EnvironmentState::HosekWilkie:
							{
								HosekWilkieSkyData data;
								data.Turbidity = skyLight["HosekWilkieTurbidity"].as<float>(data.Turbidity);
								data.GroundReflectance = skyLight["HosekWilkieGroundReflectance"].as<float>(data.GroundReflectance);
								src.Environment->Update(data);
							}
							break;
						case Proof::EnvironmentState::PreethamSky:
						{

							PreethamSkyData data;
							data.Turbidity = skyLight["PreethamTurbidity"].as<float>(data.Turbidity);
							src.Environment->Update(data);
						}
							break;
						case Proof::EnvironmentState::EnvironmentTexture:
							{
								EnvironmentTextureData data;
								data.Image = skyLight["TextureDataImage"].as<uint64_t>(0);
								src.Environment->Update(data);
							}
							break;
						default:
							break;
						}
						
					}
				}

				{
					auto directionalLight = entity["DirectionalLightComponent"];
					if (directionalLight)
					{
						auto& src = NewEntity.AddComponent<DirectionalLightComponent>();
						src.Color = directionalLight["Color"].as<glm::vec3>();
						src.Intensity = directionalLight["Intensity"].as<float>(src.Intensity);
						src.CastShadow = directionalLight["CastShadow"].as<bool>(src.CastShadow);
						src.ShadowStrength = directionalLight["ShadowStrength"].as<float>(src.ShadowStrength);
						src.CastSoftShadow = directionalLight["CastSoftShadow"].as<bool>(src.CastSoftShadow);
						src.ShadowSoftness = directionalLight["ShadowSoftness"].as<float>(src.ShadowSoftness);
					}
				}

				{
					auto pointLight = entity["PointLightComponent"];
					if (pointLight)
					{
						auto& src = NewEntity.AddComponent<PointLightComponent>();
						src.Color = pointLight["Color"].as<glm::vec3>(src.Color);
						src.Intensity = pointLight["Intensity"].as<float>(src.Intensity);
						src.MinRadius = pointLight["MinRadius"].as<float>(src.MinRadius);
						src.Radius = pointLight["Radius"].as<float>(src.Radius);
						src.Falloff = pointLight["Falloff"].as<float>(src.Falloff);
						src.CastsShadows = pointLight["CastsShadows"].as<bool>(src.CastsShadows);
						src.SoftShadows = pointLight["SoftShadows"].as<bool>(src.SoftShadows);
						src.ShadowStrength = pointLight["ShadowStrength"].as<float>(src.ShadowStrength);
						src.ShadowSoftness = pointLight["ShadowSoftness"].as<float>(src.ShadowSoftness);

					}
				}

				{
					auto spotLight = entity["SpotLightComponent"];
					if (spotLight)
					{
						auto& src = NewEntity.AddComponent<SpotLightComponent>();
						src.Color = spotLight["Color"].as<glm::vec3>(src.Color);
						src.Intensity = spotLight["Intensity"].as<float>(src.Intensity);
						src.Range = spotLight["Range"].as<float>(src.Range);
						src.Angle = spotLight["Angle"].as<float>(src.Angle);
						src.AngleAttenuation = spotLight["AngleAttenuation"].as<float>(src.AngleAttenuation);
						src.Falloff = spotLight["Falloff"].as<float>(src.Falloff);
						src.CastsShadows = spotLight["CastsShadows"].as<bool>(src.CastsShadows);
						src.SoftShadows = spotLight["SoftShadows"].as<bool>(src.SoftShadows);
						src.ShadowStrength = spotLight["ShadowStrength"].as<float>(src.ShadowStrength);
						src.ShadowSoftness = spotLight["ShadowSoftness"].as<float>(src.ShadowSoftness);
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

					src.Visible = textComponent["Visible"].as<bool>(src.Visible);
					src.RenderInViewSpace = textComponent["RenderInViewSpace"].as<bool>(src.RenderInViewSpace);
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
					src.ActiveForRendering = cameraComponent["ActiveForRendering"].as<bool>(src.ActiveForRendering);

				}
			}
			// CUBE COLLIDER
			{
				auto cubeColliderComponent = entity["BoxColliderComponent"];
				if (cubeColliderComponent)
				{
					auto& src = NewEntity.AddComponent<BoxColliderComponent>();
					src.IsTrigger = cubeColliderComponent["IsTrigger"].as<bool>();
					src.Center = cubeColliderComponent["Center"].as<glm::vec3>(src.Center);
					src.Size = cubeColliderComponent["Size"].as<glm::vec3>(src.Size);
					src.PhysicsMaterialKey = (AssetID)cubeColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
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
					src.PhysicsMaterialKey = (AssetID)sphereColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
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
					src.PhysicsMaterialKey = (AssetID)capsuleColliderComponent["PhysicsMaterialPointerID"].as<uint64_t>();
				}
			}
			//Mesh COLLIDER
			{

				auto mehsCollider = entity["MeshColliderComponent"];
				if (mehsCollider)
				{
					auto src = MeshColliderComponent();

					src.ColliderKey = (AssetID)mehsCollider["ColliderID"].as<uint64_t>(0);
					src.IsTrigger = mehsCollider["IsTrigger"].as<bool>(false);
					src.UseSharedShape = mehsCollider["UseSharedShape"].as<bool>(false);
					src.PhysicsMaterialKey = (AssetID)mehsCollider["PhysicsMaterialPointerID"].as<uint64_t>(0);
					
					//src.m_MeshAssetPointerID = mehsCollider["MeshAssetPointerID"].as<uint64_t>();
					// 
					//doing this so we dont generate a default collider if collider is not 0
					NewEntity.AddComponent<MeshColliderComponent>(src);

				}
			}
			// RIGID BODY
			{
				auto rigidBodyComponent = entity["RigidBodyComponent"];
				if (rigidBodyComponent)
				{
					auto& rgb = NewEntity.AddComponent<RigidBodyComponent>();
					rgb.Mass = rigidBodyComponent["Mass"].as<float>();
					rgb.PhysicsLayerID = rigidBodyComponent["PhysicsLayerID"].as<uint32_t>(rgb.PhysicsLayerID);
					rgb.LinearDrag = rigidBodyComponent["LinearDrag"].as<float>();
					rgb.AngularDrag = rigidBodyComponent["AngularDrag"].as<float>();
					rgb.Gravity = rigidBodyComponent["Gravity"].as<bool>();
					rgb.FreezeLocation = rigidBodyComponent["FreezeLocation"].as<VectorTemplate<bool>>();
					rgb.FreezeRotation = rigidBodyComponent["FreezeRotation"].as<VectorTemplate<bool>>();
					rgb.Kinematic = rigidBodyComponent["Kinematic"].as<bool>(rgb.Kinematic);
					rgb.RigidBodyType = EnumReflection::StringEnum<RigidBodyType>(rigidBodyComponent["Type"].as<std::string>());
					rgb.CollisionDetection = EnumReflection::StringEnum<CollisionDetectionType>(rigidBodyComponent["CollisionDetection"].
						as<std::string>(EnumReflection::EnumString(CollisionDetectionType::Discrete)));
				}
			}
			//Character Controller
			{
				auto characterControllerComponent = entity["CharacterControllerComponent"];

				if (characterControllerComponent)
				{
					auto& ccc = NewEntity.AddComponent<CharacterControllerComponent>();

					ccc.SlopeLimitRadians = characterControllerComponent["SlopeLimitRadians"].as<float>();
					ccc.StepOffset = characterControllerComponent["StepOffset"].as<float>();
					ccc.SkinOffset = characterControllerComponent["SkinOffset"].as<float>();

					ccc.GravityEnabled = characterControllerComponent["GravityEnabled"].as<bool>();
					ccc.GravityScale = characterControllerComponent["GravityScale"].as<float>();

					ccc.MinMoveDistance = characterControllerComponent["MinMoveDistance"].as<float>();
					ccc.WalkableMode = EnumReflection::StringEnum<CharacterControllerNonWalkableMode>( characterControllerComponent["GravityScale"].as<std::string>());

					ccc.PhysicsMaterialKey = (AssetID)characterControllerComponent["PhysicsMaterialID"].as<uint64_t>();

					ccc.ColliderType = EnumReflection::StringEnum<CharacterControllerType>(characterControllerComponent["ColliderType"].as<std::string>());
					ccc.Center = characterControllerComponent["Center"].as<glm::vec3>();
					ccc.Radius = characterControllerComponent["Radius"].as<float>();
					ccc.Height = characterControllerComponent["Height"].as<float>();

					ccc.Size = characterControllerComponent["Size"].as<glm::vec3>();
				}
			}
			//PlayerInputComponent
			{
				auto playerInputComponent = entity["PlayerInputComponent"];
				if (playerInputComponent)
				{
					auto& pic = NewEntity.AddComponent<PlayerInputComponent>();
					//pic.InputPlayer = EnumReflection::StringEnum< Players>(playerInputComponent["InputPlayer"].as<std::string>());

					for (auto inputBinding : playerInputComponent["InputBindings"])
					{
						AssetID id = inputBinding["InputBindingID"].as<uint64_t>(0);
						bool active = inputBinding["Active"].as<bool>(false);

						if (AssetManager::HasAsset(id) && AssetManager::GetAssetInfo(id).Type == AssetType::InputBindingContext)
						{
							pic.Player->AddInputBinding(AssetManager::GetAsset<InputBindingContext>(id));

							const InputBindingContextInstance* instnace = pic.Player->GetInputBindingContextInstance(AssetManager::GetAsset<InputBindingContext>(id));
							pic.Player->SetInputBindingActive(instnace->InputBindingContext, active);
						}
					}
				}
			}

			// PlayerStartComponent
			{
			
				auto playerStartComponent = entity["PlayerStartComponent"];
				if (playerStartComponent)
				{
					auto& psc = NewEntity.AddComponent<PlayerStartComponent>();
					psc.InputPlayer = EnumReflection::StringEnum<Players>(playerStartComponent["InputPlayer"].as<std::string>());
					psc.DontSpawn = playerStartComponent["DontSpawn"].as<bool>();
					psc.Player = playerStartComponent["PlayerPrefab"].as<AssetID>();
				}
			}
			// PlayerHudComppoent
			{
			#if 1
				auto playerHudComponent = entity["PlayerHUDComponent"];
				if (playerHudComponent)
				{
					auto& phc = NewEntity.AddComponent<PlayerHUDComponent>();

					Count<UITable> table = Count<UITable>::Create();
					table->ClearLayers();
					for (auto layer : playerHudComponent["UITable"])
					{
						auto& newLayer = table->AddLayer(layer["Layer"].as<std::string>("Unnamed"));
						newLayer.Visible = layer["Visible"].as<bool>(true);

						auto panels = layer["Panels"];

						for (auto panel : panels)
						{
							Count<UIPanelInstance> newPanelInstance = newLayer.PushUI();
							AssetKey<AssetType::UIPanel> uiAsset = panel["PanelID"].as<AssetID>();

							if (uiAsset.IsValid())
								newPanelInstance->SetPanelInstance(uiAsset.GetAsset<UIPanel>());
							newPanelInstance->Visible = panel["Visible"].as<bool>(true);

							if(uiAsset.IsValid())
								SerializeCommon::LoadVariableRegistryInstance(panel, newPanelInstance->GetVariableRegistryInstance());
						}

					}

					phc.HudTable = table;
				}
			#endif
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
			//Water Component
			{
				auto waterComponent = entity["WaterComponent"];
				if (waterComponent)
				{

					auto waveType = EnumReflection::StringEnum<WaveType>(waterComponent["WaveType"].as<std::string>(EnumReflection::EnumString(WaveType::GerstnerWave)));


					Count<Water> water = Count<Water>::Create(waveType);

					water->Density = waterComponent["Density"].as<float>(water->Density);
					NewEntity.AddComponent<WaterComponent>(water);

					switch (waveType)
					{
						case Proof::WaveType::GerstnerWave:
						{
							GerstnerWave::GerstnerWaveInfo& waterDataInfo = water->GetWave().As<GerstnerWave>()->GerstnerData;
							waterDataInfo.WaveCount = waterComponent["WaveCount"].as<uint32_t>(waterDataInfo.WaveCount);
							waterDataInfo.WaveDirection = waterComponent["WaveDirection"].as<glm::vec2>(waterDataInfo.WaveDirection);
							waterDataInfo.WaveSpread = waterComponent["WaveSpread"].as<float>(waterDataInfo.WaveSpread);
							waterDataInfo.WaveDistribution = waterComponent["WaveDistribution"].as<float>(waterDataInfo.WaveDistribution);

							waterDataInfo.Color = waterComponent["Color"].as<glm::vec4>(waterDataInfo.Color);
							waterDataInfo.Speed = waterComponent["Speed"].as<float>(waterDataInfo.Speed);

							waterDataInfo.MinMaxWavelength = waterComponent["MinMaxWavelength"].as<glm::vec2>(waterDataInfo.MinMaxWavelength);
							waterDataInfo.WavelengthFalloff = waterComponent["WavelengthFalloff"].as<float>(waterDataInfo.WavelengthFalloff);

							waterDataInfo.MinMaxAmplitude = waterComponent["MinMaxAmplitude"].as<glm::vec2>(waterDataInfo.MinMaxAmplitude);
							waterDataInfo.AmplitudeFallOff = waterComponent["AmplitudeFallOff"].as<float>(waterDataInfo.AmplitudeFallOff);

							waterDataInfo.MinMaxSteepness = waterComponent["MinMaxSteepness"].as<glm::vec2>(waterDataInfo.MinMaxSteepness);
							waterDataInfo.SteepnessFallOff = waterComponent["SteepnessFallOff"].as<float>(waterDataInfo.SteepnessFallOff);

							waterDataInfo.Seed = waterComponent["Seed"].as<int>(waterDataInfo.Seed);
							waterDataInfo.RandomSeed = waterComponent["RandomSeed"].as<bool>(waterDataInfo.RandomSeed);
							waterDataInfo.PlaneSize = waterComponent["PlaneSize"].as<float>(waterDataInfo.PlaneSize);
						}
							break;
						case Proof::WaveType::FastFourierTransformWave:
						{
							FFTWave::FFTWaveInfo& waterDataInfo = water->GetWave().As<FFTWave>()->WaveInfo;

							waterDataInfo.OceanSize = EnumReflection::StringEnum<FFTWaveMapSize>(waterComponent["OceanSize"].as<std::string>(EnumReflection::EnumString(waterDataInfo.OceanSize)));
							waterDataInfo.NumCascades = waterComponent["NumCascades"].as<int>(waterDataInfo.NumCascades);
							waterDataInfo.NormalStrength = waterComponent["NormalStrength"].as<float>(waterDataInfo.NormalStrength);
							waterDataInfo.Roughness = waterComponent["Roughness"].as<float>(waterDataInfo.Roughness);
							waterDataInfo.WaterColor = waterComponent["WaterColor"].as<glm::vec4>(waterDataInfo.WaterColor);
							waterDataInfo.FoamColor = waterComponent["FoamColor"].as<glm::vec3>(waterDataInfo.FoamColor);

							waterDataInfo.DisplacementScale = waterComponent["DisplacementScale"].as<float>(waterDataInfo.DisplacementScale);
							waterDataInfo.NormalScale = waterComponent["NormalScale"].as<float>(waterDataInfo.NormalScale);
							waterDataInfo.WindSpeedScale = waterComponent["WindSpeedScale"].as<float>(waterDataInfo.WindSpeedScale);
							waterDataInfo.SwellScale = waterComponent["SwellScale"].as<float>(waterDataInfo.SwellScale);
							waterDataInfo.DetailScale = waterComponent["DetailScale"].as<float>(waterDataInfo.DetailScale);
							waterDataInfo.SpreadScale = waterComponent["SpreadScale"].as<float>(waterDataInfo.SpreadScale);

							waterDataInfo.FoamTimeScale = waterComponent["FoamTimeScale"].as<float>(waterDataInfo.FoamTimeScale);
							waterDataInfo.WhitecapScale = waterComponent["WhitecapScale"].as<float>(waterDataInfo.WhitecapScale);
							waterDataInfo.FoamGrowthScale = waterComponent["FoamGrowthScale"].as<float>(waterDataInfo.FoamGrowthScale);
							waterDataInfo.FoamDecayScale = waterComponent["FoamDecayScale"].as<float>(waterDataInfo.FoamDecayScale);

							if (waterComponent["Cascades"])
							{
								uint32_t cascadeindex = 0; 
								for (auto csc : waterComponent["Cascades"])
								{
									FFTWaveCascade::WaveCascadeParameters& cascadeSettings = water->GetWave().As<FFTWave>()->GetCascades().at(cascadeindex)->Settings;

									cascadeSettings.TileLength = csc["TileLength"].as<glm::vec2>(cascadeSettings.TileLength);
									cascadeSettings.DisplacementScale = csc["DisplacementScale"].as<float>(cascadeSettings.DisplacementScale);
									cascadeSettings.NormalScale = csc["NormalScale"].as<float>(cascadeSettings.NormalScale);
									cascadeSettings.WindSpeed = csc["WindSpeed"].as<float>(cascadeSettings.WindSpeed);
									cascadeSettings.WindDirection = csc["WindDirection"].as<float>(cascadeSettings.WindDirection);
									cascadeSettings.FetchLength = csc["FetchLength"].as<float>(cascadeSettings.FetchLength);
									cascadeSettings.Swell = csc["Swell"].as<float>(cascadeSettings.Swell);
									cascadeSettings.Detail = csc["Detail"].as<float>(cascadeSettings.Detail);
									cascadeSettings.Spread = csc["Spread"].as<float>(cascadeSettings.Spread);
									cascadeSettings.FoamTimeScale = csc["FoamTimeScale"].as<float>(cascadeSettings.FoamTimeScale);
									cascadeSettings.Whitecap = csc["Whitecap"].as<float>(cascadeSettings.Whitecap);
									cascadeSettings.FoamAmount = csc["FoamAmount"].as<float>(cascadeSettings.FoamAmount);

									cascadeindex++;
								}
							}


						}
							break;
						default:
							break;
					}


				
				}

			}

			// buoyancyComponent
			{
				auto buoyancyComponent = entity["BuoyancyComponent"];
				if (buoyancyComponent)
				{
					BuoyancyComponent& bycc = NewEntity.AddComponent<BuoyancyComponent>();


					bycc.Density = buoyancyComponent["Density"].as<float>(bycc.Density);
					bycc.VoxelRelativeSize = buoyancyComponent["VoxelRelativeSize"].as<float>(bycc.VoxelRelativeSize.GetValue());
					bycc.DragInWater = buoyancyComponent["DragInWater"].as<float>(bycc.DragInWater);
					bycc.AngularDragInWater = buoyancyComponent["AngularDragInWater"].as<float>(bycc.AngularDragInWater);
					bycc.WaveDisplacementFactor = buoyancyComponent["WaveDisplacementFactor"].as<float>(bycc.WaveDisplacementFactor.GetValue());
					bycc.AlwaysStayUpright = buoyancyComponent["AlwaysStayUpright"].as<bool>(bycc.AlwaysStayUpright);

					for (auto byc : buoyancyComponent["Floaters"])
					{

						float submersionDepth = byc["SubmersionDepth"].as<float>();
						float buoyancyStrength = byc["BuoyancyStrength"].as<float>();

						BuoyancyComponent::EntityFloater entFloater;
						entFloater.EntityUUID = byc["EntityID"].as<uint64_t>();
						entFloater.Floater.SubmersionDepth = byc["SubmersionDepth"].as<float>();
						entFloater.Floater.Drag = byc["Drag"].as<float>();
						entFloater.Floater.AngularDrag = byc["AngularDrag"].as<float>();
						entFloater.Floater.BuoyancyStrength = byc["BuoyancyStrength"].as<float>();

						bycc.Floaters.emplace_back(entFloater);
						
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
				#if 1
				Count<ScriptWorld> scriptWorld = world->GetScriptWorld();
				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					if (scriptComponent["Scripts"])
					{
						auto& scp = NewEntity.AddComponent<ScriptComponent>();
						auto scripts = scriptComponent["Scripts"];
						for (auto script : scripts)
						{
							const std::string scriptClassModule = script["ScriptClassName"].as<std::string>("");

							if (!ScriptEngine::IsModuleValid(scriptClassModule))
								continue;

							if (!scriptWorld->IsEntityScriptInstantiated(NewEntity))
							{
								scp.ScriptMetadates.push_back({ scriptClassModule,nullptr });
								scriptWorld->InstantiateScriptEntity(NewEntity);
								if (!scriptWorld->IsEntityScriptInstantiated(NewEntity))
								{
									scp.ScriptMetadates.pop_back();
									continue;
								}
									
							}
							else
							{
								scriptWorld->ScriptEntityPushScript(NewEntity, scriptClassModule);
							}


							if (!script["Fields"])continue;
							auto scriptFields = script["Fields"];

							for (auto field : scriptFields)
							{
								std::string fieldNameID = field["NameID"].as<std::string>();

								Count<FieldStorageBase> fieldStorage = scriptWorld->GetEntityClassField(NewEntity, scriptClassModule, fieldNameID);
								if(fieldStorage)
									SerializeCommon::LoadScriptField(field, fieldStorage);
							}
						}
					}
				}
				#endif
				#if 0
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
				#endif
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