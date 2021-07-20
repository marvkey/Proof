#include "Proofprch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>
namespace YAML
{
	template<>
	struct convert<Proof::Vector> {
		static Node encode(const Proof::Vector& rhs) {
			Node node;
			node.push_back(rhs.X);
			node.push_back(rhs.Y);
			node.push_back(rhs.Z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node,Proof::Vector& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			rhs.Z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node,glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node,glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}
namespace Proof{
	YAML::Emitter& operator<<(YAML::Emitter& out,const Vector& v){
		out<<YAML::Flow;
		out<<YAML::BeginSeq<<v.X<<v.Y<<v.Z<<YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out,const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.a<<YAML::EndSeq;
		return out;
	}
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
		}
		out << YAML::EndMap; // entity
	}

	void SceneSerializer::SerilizeText(const std::string& filePath) {
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
		
		YAML::Node data = YAML::LoadFile(filePath);
		if(!data["World"]) // if there is no scene no
			return false;

		std::string sceneName = data["World"].as<std::string>();
		PF_ENGINE_TRACE("Deserilizing World %s",sceneName.c_str());
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
			}
		}
		return true;
	}
	bool SceneSerializer::DeSerilizeBinary(const std::string& filepath) {
		PF_CORE_ASSERT(false,"Functon has not been created");
		return false;
	}
}