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
		bool DeSerilizeText(UUID ID);

		bool DeSerilizeBinary(const std::string& filepath);
		const std::set<UUID>& GetAssetLoadID()const {
			return m_AssetLoadID;
		}

		// for prefab 
		static void SerilizeEntity(YAML::Emitter& out, entt::registry& registry, UUID entityID, entt::entity enttID, bool ispPrefab = false);
		// for prefab 
		static void DeSerilizeEntity(YAML::Node& entities, World* world, std::set<UUID>* assetLoad = nullptr, bool prefab = false);
	private:
		World* m_Scene;
		std::set<UUID> m_AssetLoadID;
	};
}