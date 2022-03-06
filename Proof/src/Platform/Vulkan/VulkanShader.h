#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Shader.h"
namespace Proof
{
	class Proof_API VulkanShader : public Shader {
		VulkanShader(const std::string& _NameShader, const std::string& VertexPath, const std::string& FragmentPath); // CODE NEEDS TO BE NEATEN UP
	private:
		static std::vector<char> ReadFile(const std::string& filePath);
	};
}