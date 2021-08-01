#include "Proofprch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <string>
#include <filesystem>
#include "Proof/Resources/ExternalCreations.h"
namespace Proof{

	SceneSerializer::SceneSerializer(World* Scene) {
		PF_CORE_ASSERT(Scene,"Scene cannot be nulltptr");
		m_Scene = Scene;
	}
	void SceneSerializer::SerilizeEntity(YAML::Emitter& out, Entity entity){
		out<<YAML::BeginMap;// entity
		out<<YAML::Key<<"Entity"<< YAML::Value <<entity.GetID();

		for (Component* Comp : *m_Scene->Registry.GetEntities().at(entity.GetID())) {

			TagComponent* Tag = dynamic_cast<TagComponent*>(Comp);

			if(Tag != nullptr){
				auto& tag = Tag->GetName();
				out<<YAML::Key<<"TagComponent";
				out<<YAML::BeginMap; // tag component
				out<<YAML::Key<<"Tag"<<YAML::Value<<tag;
				out << YAML::EndMap; // tag component
				continue;
			}

			TransformComponent* Transfrom = dynamic_cast<TransformComponent*>(Comp);
			if (Transfrom != nullptr) {
				out<<YAML::Key<<"TransformComponent";
				out<<YAML::BeginMap; // transform component
				out<<YAML::Key<<"Location"<<YAML::Value<<Transfrom->Location;
				out<<YAML::Key<<"Rotation"<<YAML::Value<<Transfrom->Rotation;
				out<<YAML::Key<<"Scale"<<YAML::Value<<Transfrom->Scale;
				out << YAML::EndMap; // transform component
				continue;
			}
			MeshComponent* Meshes = dynamic_cast<MeshComponent*>(Comp);
			if (Meshes != nullptr) {
				out<<YAML::Key<<"MeshComponent";
				out << YAML::BeginMap; // Mesh component
				out<<YAML::Key<<"Name"<<YAML::Value<<Meshes->GetName();
				out<<YAML::Key<<"AssetID"<<YAML::Value<<Meshes->GetAssetID();
				out<<YAML::Key<<"LocalLocation"<<Meshes->MeshLocalTransform.Location;
				out<<YAML::Key<<"LocalRotation"<<Meshes->MeshLocalTransform.Rotation;
				out<<YAML::Key<<"LocalScale"<<Meshes->MeshLocalTransform.Scale;
				out<<YAML::Key<<"MaterialPointerID"<<Meshes->GetMaterialPointerID();
				out << YAML::EndMap; // Mesh component
				continue;
			}

			SpriteComponent* Sprite = dynamic_cast<SpriteComponent*>(Comp);
			if (Sprite != nullptr) {
				out << YAML::Key << "SpriteComponent";
				out << YAML::BeginMap; // Sprite component
				out<<YAML::Key<<"Name"<<YAML::Value<<Sprite->GetName();
				out<<YAML::Key<<"AssetID"<<YAML::Value<<Sprite->GetAssetID();
				out<<YAML::Key<<"Colour"<<YAML::Value<<Sprite->Colour;
				out << YAML::Key << "LocalLocation" << Sprite->SpriteTransfrom.Location;
				out << YAML::Key << "LocalRotation" << Sprite->SpriteTransfrom.Rotation;
				out << YAML::Key << "LocalScale" << Sprite->SpriteTransfrom.Scale;
				out << YAML::EndMap; // Sprite component
				continue;
			}
			LightComponent* Light = dynamic_cast<LightComponent*>(Comp);
			if(Light != nullptr){
				out<<YAML::Key<<"LightComponent";
				out<<YAML::BeginMap; // LightComponent;
				out<<YAML::Key<<"Position"<< Light->m_Position;
				out<<YAML::Key<<"Direction"<< Light->m_Direction;

				out<<YAML::Key<<"CutOff"<< Light->m_CutOff;
				out<<YAML::Key<<"OuterCutOff"<< Light->m_OuterCutOff;

				out << YAML::Key << "Constant" << Light->m_Constant;
				out << YAML::Key << "Linear" << Light->m_Linear;
				out << YAML::Key << "Quadratic" << Light->m_Quadratic;

				out << YAML::Key << "Ambient" << Light->m_Ambient;
				out << YAML::Key << "Diffuse" << Light->m_Diffuse;
				out << YAML::Key << "Specular" << Light->m_Specular;

				out << YAML::Key << "LightType" << Light->m_LightType;
				out << YAML::EndMap; // LightComponent
				continue;
			}
		}
		out << YAML::EndMap; // entity
	}

	void SceneSerializer::SerilizeText(const std::string& filePath) {
		m_Scene->m_Path = filePath;
		YAML::Emitter out;
		out<<YAML::BeginMap;
		out<<YAML::Key<<"World"<<YAML::Value<<m_Scene->GetName();
		out<<YAML::Key<<"Entities"<<YAML::Value<<YAML::BeginSeq;
		for( uint32_t ID: m_Scene->Registry.GetAllID()){
			Entity entity ={ID,m_Scene};
			SerilizeEntity(out,entity);
		}
		out<<YAML::EndSeq;
		out<<YAML::EndMap;

		std::ofstream foud(filePath);
		foud<<out.c_str();
	}
	void SceneSerializer::SerilizeBinary(const std::string& filepath) {}
	bool SceneSerializer::DeSerilizeText(const std::string& filePath) {
		m_Scene->m_Path = filePath;

		YAML::Node data = YAML::LoadFile(filePath);
		if(!data["World"]) // if there is no scene no
			return false;

		m_Scene->Name= data["World"].as<std::string>();
		PF_ENGINE_TRACE("Deserilizing World %s",m_Scene->Name.c_str());
		auto entities = data["Entities"];

		if(entities){
			for(auto entity: entities){
				uint32_t EntID = entity["Entity"].as<uint32_t>();

				std::string name;
				auto tagcomponent = entity["TagComponent"];
				if(tagcomponent)
					name =tagcomponent["Tag"].as<std::string>();
				Entity NewEntity = m_Scene->CreateEntity(name,EntID);
				auto transformComponet = entity["TransformComponent"];
				if(transformComponet){
					auto* tc = NewEntity.GetComponent<TransformComponent>();
					tc->Location = transformComponet["Location"].as<glm::vec3>();

					tc->Rotation = transformComponet["Rotation"].as<Vector>();
					tc->Scale = transformComponet["Scale"].as<Vector>();
				}
				
				
				auto meshComponent = entity["MeshComponent"];
				if(meshComponent){
					auto& src = *NewEntity.AddComponent<MeshComponent>();
					src.SetName(meshComponent["Name"].as<std::string>());
					src.AssetID = meshComponent["AssetID"].as<uint32_t>();

					src.MeshLocalTransform.Location = meshComponent["LocalLocation"].as<glm::vec3>();
					src.MeshLocalTransform.Rotation = meshComponent["LocalRotation"].as<glm::vec3>();
					src.MeshLocalTransform.Scale = meshComponent["LocalScale"].as<glm::vec3>();
					src.m_MeshMaterialID = meshComponent["MaterialPointerID"].as<uint32_t>();
				}
				
				auto spriteRendererComponent = entity["SpriteComponent"];
				if (spriteRendererComponent) 				{
					auto& src = *NewEntity.AddComponent<SpriteComponent>();
					src.SetName(spriteRendererComponent["Name"].as<std::string>());
					src.AssetID = spriteRendererComponent["AssetID"].as<uint32_t>();
					src.Colour= spriteRendererComponent["Colour"].as<glm::vec4>();
					src.SpriteTransfrom.Location = spriteRendererComponent["LocalLocation"].as<glm::vec3>();
					src.SpriteTransfrom.Rotation = spriteRendererComponent["LocalRotation"].as<glm::vec3>();
					src.SpriteTransfrom.Scale = spriteRendererComponent["LocalScale"].as<glm::vec3>();
				}

				auto lightComponent = entity["LightComponent"];
				if(lightComponent){
					auto& src = *NewEntity.AddComponent<LightComponent>();
					src.m_Position = lightComponent["Position"].as<Vector>();
					src.m_Direction = lightComponent["Direction"].as<Vector>();

					src.m_CutOff = lightComponent["CutOff"].as<float>();
					src.m_OuterCutOff = lightComponent["OuterCutOff"].as<float>();

					src.m_Constant = lightComponent["Constant"].as<float>();
					src.m_Linear = lightComponent["Linear"].as<float>();
					src.m_Quadratic = lightComponent["Quadratic"].as<float>();

					src.m_Ambient = lightComponent["Ambient"].as<glm::vec3>();
					src.m_Diffuse = lightComponent["Diffuse"].as<glm::vec3>();
					src.m_Specular = lightComponent["Specular"].as<glm::vec3>();

					src.m_LightType = lightComponent["LightType"].as<int>();
				}
			}
		}
		return true;
	}
	bool SceneSerializer::DeSerilizeBinary(const std::string& filepath) {
		PF_CORE_ASSERT(false,"Functon has not been created");
		return false;
	}
}