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

	static void LoadScriptFieldEnum(YAML::iterator::value_type& scriptField, Count<EnumFieldStorage> enumFieldStorage, ScriptFieldType savedScriptFieldType)
	{
		auto dataNode = scriptField["Data"];
		switch (enumFieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Int8:
				{
					enumFieldStorage->SetValue(static_cast<int8_t>(dataNode.as<int16_t>()));
					break;
				}
			case ScriptFieldType::Int16:
				{
					enumFieldStorage->SetValue(dataNode.as<int16_t>());
					break;
				}
			case ScriptFieldType::Int32:
				{
					enumFieldStorage->SetValue(dataNode.as<int32_t>());
					break;
				}
			case ScriptFieldType::Int64:
				{
					enumFieldStorage->SetValue(dataNode.as<int64_t>());
					break;
				}
			case ScriptFieldType::UInt8:
				{
					enumFieldStorage->SetValue(dataNode.as<uint8_t>());
					break;
				}
			case ScriptFieldType::UInt16:
				{
					enumFieldStorage->SetValue(dataNode.as<uint16_t>());
					break;
				}
			case ScriptFieldType::UInt32:
				{
					enumFieldStorage->SetValue(dataNode.as<uint32_t>());
					break;
				}
			case ScriptFieldType::UInt64:
				{
					enumFieldStorage->SetValue(dataNode.as<uint64_t>());
					break;
				}
		}
	}

	static void LoadScriptFieldStorage(YAML::iterator::value_type& scriptField, Count<FieldStorage> fieldStorage, ScriptFieldType savedScriptFieldType)
	{
		auto dataNode = scriptField["Data"];
		switch (fieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Bool:
				{
					fieldStorage->SetValue(dataNode.as<bool>());
					break;
				}
			case ScriptFieldType::Int8:
				{
					fieldStorage->SetValue(static_cast<int8_t>(dataNode.as<int16_t>()));
					break;
				}
			case ScriptFieldType::Int16:
				{
					fieldStorage->SetValue(dataNode.as<int16_t>());
					break;
				}
			case ScriptFieldType::Int32:
				{
					fieldStorage->SetValue(dataNode.as<int32_t>());
					break;
				}
			case ScriptFieldType::Int64:
				{
					fieldStorage->SetValue(dataNode.as<int64_t>());
					break;
				}
			case ScriptFieldType::UInt8:
				{
					fieldStorage->SetValue(dataNode.as<uint8_t>());
					break;
				}
			case ScriptFieldType::UInt16:
				{
					fieldStorage->SetValue(dataNode.as<uint16_t>());
					break;
				}
			case ScriptFieldType::UInt32:
				{
					fieldStorage->SetValue(dataNode.as<uint32_t>());
					break;
				}
			case ScriptFieldType::UInt64:
				{
					fieldStorage->SetValue(dataNode.as<uint64_t>());
					break;
				}
			case ScriptFieldType::Float:
				{
					fieldStorage->SetValue(dataNode.as<float>());
					break;
				}
			case ScriptFieldType::Double:
				{
					fieldStorage->SetValue(dataNode.as<double>());
					break;
				}
			case ScriptFieldType::String:
				{
					fieldStorage->SetValue(dataNode.as<std::string>());
					break;
				}
			case ScriptFieldType::Vector2:
				{
					fieldStorage->SetValue(dataNode.as<glm::vec2>());
					break;
				}
			case ScriptFieldType::Vector3:
				{
					fieldStorage->SetValue(dataNode.as<glm::vec3>());
					break;
				}
			case ScriptFieldType::Vector4:
				{
					fieldStorage->SetValue(dataNode.as<glm::vec4>());
					break;
				}
			case ScriptFieldType::Prefab:
			case ScriptFieldType::Entity:
			case ScriptFieldType::Mesh:
			case ScriptFieldType::DynamicMesh:
			case ScriptFieldType::Material:
			case ScriptFieldType::PhysicsMaterial:
			case ScriptFieldType::Texture2D:
				{
					fieldStorage->SetValue(dataNode.as<UUID>());
					break;
				}
		}
	}

	static void LoadScriptField(YAML::iterator::value_type& scriptField, Count<ScriptWorld>& scriptWorld, const std::string& classModule, Entity entity)
	{
		std::string fieldNameID = scriptField["NameID"].as<std::string>();
		ScriptFieldType scriptFieldType =EnumReflection::StringEnum<ScriptFieldType>( scriptField["Type"].as<std::string>());

		Count<FieldStorageBase> fieldStorage = scriptWorld->GetEntityClassField(entity, classModule, fieldNameID);

		if (!fieldStorage)
			return;

		if (fieldStorage->GetFieldInfo()->IsArray())
		{

		}
		else if (fieldStorage->GetFieldInfo()->IsEnum())
		{
			
			LoadScriptFieldEnum(scriptField, fieldStorage.As<EnumFieldStorage>(), scriptFieldType);
		}
		else
		{
			LoadScriptFieldStorage(scriptField, fieldStorage.As<FieldStorage>(), scriptFieldType);
		}
	}
	static void SaveScriptFieldEnum(YAML::Emitter& out, Count<EnumFieldStorage> fieldStorage)
	{
		out << YAML::Key << "Data" << YAML::Value;
		switch (fieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Int8:
				{
					out << fieldStorage->GetValue<int8_t>();
					break;
				}
			case ScriptFieldType::Int16:
				{
					out << fieldStorage->GetValue<int16_t>();
					break;
				}
			case ScriptFieldType::Int32:
				{
					out << fieldStorage->GetValue<int32_t>();
					break;
				}
			case ScriptFieldType::Int64:
				{
					out << fieldStorage->GetValue<int64_t>();
					break;
				}
			case ScriptFieldType::UInt8:
				{
					out << fieldStorage->GetValue<uint8_t>();
					break;
				}
			case ScriptFieldType::UInt16:
				{
					out << fieldStorage->GetValue<uint16_t>();
					break;
				}
			case ScriptFieldType::UInt32:
				{
					out << fieldStorage->GetValue<uint32_t>();
					break;
				}
			case ScriptFieldType::UInt64:
				{
					out << fieldStorage->GetValue<uint64_t>();
					break;
				}

				out << "";
				break;
		}
	}

	static void SaveScriptFieldStorage(YAML::Emitter& out, Count<FieldStorage> fieldStorage)
	{
		out << YAML::Key << "Data" << YAML::Value;
		switch (fieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Bool:
				{
					out << fieldStorage->GetValue<bool>();
					break;
				}
			case ScriptFieldType::Int8:
				{
					out << fieldStorage->GetValue<int8_t>();
					break;
				}
			case ScriptFieldType::Int16:
				{
					out << fieldStorage->GetValue<int16_t>();
					break;
				}
			case ScriptFieldType::Int32:
				{
					out << fieldStorage->GetValue<int32_t>();
					break;
				}
			case ScriptFieldType::Int64:
				{
					out << fieldStorage->GetValue<int64_t>();
					break;
				}
			case ScriptFieldType::UInt8:
				{
					out << fieldStorage->GetValue<uint8_t>();
					break;
				}
			case ScriptFieldType::UInt16:
				{
					out << fieldStorage->GetValue<uint16_t>();
					break;
				}
			case ScriptFieldType::UInt32:
				{
					out << fieldStorage->GetValue<uint32_t>();
					break;
				}
			case ScriptFieldType::UInt64:
				{
					out << fieldStorage->GetValue<uint64_t>();
					break;
				}
			case ScriptFieldType::Float:
				{
					out << fieldStorage->GetValue<float>();
					break;
				}
			case ScriptFieldType::Double:
				{
					out << fieldStorage->GetValue<double>();
					break;
				}
			case ScriptFieldType::String:
				{
					out << fieldStorage->GetValue<std::string>();
					break;
				}
			case ScriptFieldType::Vector2:
				{
					out << fieldStorage->GetValue<glm::vec2>();
					break;
				}
			case ScriptFieldType::Vector3:
				{
					out << fieldStorage->GetValue<glm::vec3>();
					break;
				}
			case ScriptFieldType::Vector4:
				{
					out << fieldStorage->GetValue<glm::vec4>();
					break;
				}
			case ScriptFieldType::Prefab:
			case ScriptFieldType::Entity:
			case ScriptFieldType::Mesh:
			case ScriptFieldType::DynamicMesh:
			case ScriptFieldType::Material:
			case ScriptFieldType::PhysicsMaterial:
			case ScriptFieldType::Texture2D:
				{
					out << fieldStorage->GetValue<UUID>();
					break;
				}
			default:
				out << "";
				break;
		}
	}

	static void SaveScriptField(YAML::Emitter& out,Count<FieldStorageBase> fieldStorage)
	{
		if (fieldStorage == nullptr)
			return;

		out << YAML::BeginMap; // ScriptField
		out << YAML::Key << "NameID" << YAML::Value << fieldStorage->GetFieldInfo()->FullName;
		out << YAML::Key << "Type" << YAML::Value << EnumReflection::EnumString( fieldStorage->GetFieldInfo()->Type);
		if (fieldStorage->GetFieldInfo()->IsArray())
		{
			
		}
		else if(fieldStorage->GetFieldInfo()->IsEnum())
		{
			SaveScriptFieldEnum(out, fieldStorage.As<EnumFieldStorage>());
		}
		else
		{
			SaveScriptFieldStorage(out, fieldStorage.As<FieldStorage>());
		}
		out << YAML::EndMap; // ScriptField
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
					if (*dynamicMeshComponent.GetMesh()->GetMaterialTable() == *dynamicMeshComponent.MaterialTable)
						goto leavedynamic;
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
			if (entity.HasComponent<SpriteComponent>()) {
				SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
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
			
				if (entity.HasComponent<SkyLightComponent>())
				{
					SkyLightComponent& skylight = entity.GetComponent<SkyLightComponent>();
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
				if (entity.HasComponent<DirectionalLightComponent>()) {
					DirectionalLightComponent& directonalLight = entity.GetComponent<DirectionalLightComponent>();
					out << YAML::Key << "DirectionalLightComponent";
					out << YAML::BeginMap; // DirectionalLightComponent
					out << YAML::Key << "Color" << directonalLight.Color;
					out << YAML::Key << "Intensity" << directonalLight.Intensity;
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
									SaveScriptField(out, fieldStorage);
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
				out << YAML::Key << "UseLocalRotation" << textComponent.UseLocalRotation;
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
				out << YAML::Key << "PhysicsMaterialPointerID" << cubeCollider.m_PhysicsMaterialPointerID;
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
				out << YAML::Key << "PhysicsMaterialPointerID" << sphereCollider.m_PhysicsMaterialPointerID;
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
				out << YAML::Key << "PhysicsMaterialPointerID" << CapsuleCollider.m_PhysicsMaterialPointerID;
				out << YAML::EndMap; // CapsuleColliderComponent
			}
		}
		{
			if (entity.HasComponent<MeshColliderComponent>()) {
				MeshColliderComponent& meshColliderComponent = entity.GetComponent<MeshColliderComponent>();
				out << YAML::Key << "MeshColliderComponent";
				out << YAML::BeginMap; // MeshColliderComponent
				out << YAML::Key << "ColliderID" << YAML::Value << meshColliderComponent.ColliderID;
				out << YAML::Key << "IsTrigger" << YAML::Value << meshColliderComponent.IsTrigger;
				out << YAML::Key << "UseSharedShape" << YAML::Value << meshColliderComponent.UseSharedShape;
				out << YAML::Key << "PhysicsMaterialPointerID" << YAML::Value << meshColliderComponent.m_PhysicsMaterialPointerID;
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
				out << YAML::Key << "PhysicsMaterialID" << characterController.PhysicsMaterialID;

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
				out << YAML::Key << "InputPlayer" << EnumReflection::EnumString(playerInput.InputPlayer);

				if (CanSaveAsset(playerInput.Player))
					out << YAML::Key << "PrefabID" << 0;
				else
					out << YAML::Key << "PrefabID" << (uint64_t)playerInput.Player->GetID();

				out << YAML::EndMap; // PlayerInputComponent
			}
		}
		
		{
			if (entity.HasComponent<PlayerHUDComponent>())
			{
				PlayerHUDComponent& hud = entity.GetComponent<PlayerHUDComponent>();
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
						Count<MaterialTable> matTable = Count<MaterialTable>::Create();
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
						Count<MaterialTable> matTable = Count<MaterialTable>::Create();
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
							src.MaterialTable = Count<MaterialTable>::CreateFrom(AssetManager::GetAsset<DynamicMesh>(src.m_MeshID)->GetMaterialTable());
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
							//m_AssetLoadID.emplace((UUID)id);
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
						src.ColorTint = skyLight["TintColor"].as<glm::vec3>();
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
						src.Color = directionalLight["Color"].as<glm::vec3>();
						src.Intensity = directionalLight["Intensity"].as<float>();
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
				auto cubeColliderComponent = entity["BoxColliderComponent"];
				if (cubeColliderComponent)
				{
					auto& src = NewEntity.AddComponent<BoxColliderComponent>();
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
					auto src = MeshColliderComponent();

					src.ColliderID = mehsCollider["ColliderID"].as<uint64_t>(0);
					src.IsTrigger = mehsCollider["IsTrigger"].as<bool>(false);
					src.UseSharedShape = mehsCollider["UseSharedShape"].as<bool>(false);
					src.m_PhysicsMaterialPointerID = mehsCollider["PhysicsMaterialPointerID"].as<uint64_t>(0);
					
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

					ccc.PhysicsMaterialID = characterControllerComponent["PhysicsMaterialID"].as<uint64_t>();

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
								// might change to an uint64_t keep i nmind
								LoadScriptField(field, scriptWorld, scriptClassModule, NewEntity);
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