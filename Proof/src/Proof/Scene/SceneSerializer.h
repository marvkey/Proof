#pragma once
#include <yaml-cpp/yaml.h>
#include <set>
#include <entt/entt.hpp>
namespace Proof
{
	class World;
	class Proof_API SceneSerializer {
	public:
		SceneSerializer(World* Scene);
		/* saving world data into a file*/
		void SerilizeText(const std::string& filePath);
		void SerilizeBinary(const std::string& filepath);

		/* Openign the file and creating a scene from data*/
		bool DeSerilizeText(const std::string& filePath);
		bool DeSerilizeText(AssetID ID);

		bool DeSerilizeBinary(const std::string& filepath);
		const std::set<AssetID>& GetAssetLoadID()const {
			return m_AssetLoadID;
		}

		// for prefab 
		static void SerilizeEntity(YAML::Emitter& out, entt::basic_registry<uint64_t>& registry, UUID entityID);
		// for prefab 
		static void DeSerilizeEntity(YAML::Node& entities, World* world, std::set<AssetID>* assetLoad = nullptr, bool prefab = false);
	private:
		World* m_Scene;
		std::set<AssetID> m_AssetLoadID;
	};
}