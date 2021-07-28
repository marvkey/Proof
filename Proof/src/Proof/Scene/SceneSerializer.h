#pragma once
#include "World.h"
#include <yaml-cpp/yaml.h>

namespace Proof
{
	class Proof_API SceneSerializer {
	public:
		SceneSerializer(World* Scene);
		/* saving world data into a file*/
		void SerilizeText(const std::string& filePath);
		void SerilizeBinary(const std::string& filepath);

		/* Openign the file and creating a scene from data*/
		bool DeSerilizeText(const std::string& filePath);
		bool DeSerilizeBinary(const std::string& filepath);

	private:
		World* m_Scene;
		void SerilizeEntity(YAML::Emitter& out,Entity entity);
	};
}