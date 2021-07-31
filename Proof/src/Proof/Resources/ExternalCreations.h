#pragma once
#include "Proof/Resources/Math/Math.h"
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
namespace Proof
{
	YAML::Emitter& operator<<(YAML::Emitter& out,const Vector& v);

	YAML::Emitter& operator<<(YAML::Emitter& out,const glm::vec4& v);
}