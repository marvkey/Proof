#include "Proofprch.h"
#include "ExternalCreations.h"

namespace Proof
{
	YAML::Emitter& operator<<(YAML::Emitter& out,const Vector& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.X << v.Y << v.Z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out,const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.a << YAML::EndSeq;
		return out;
	}
}