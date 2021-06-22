#pragma once
#include <glm/glm.hpp>
#include "Proof/Renderer/Shader.h"
#include "Proof/Core/Core.h"
#include "vulkan/vulkan.h"
#include "VulkanGraphicsContext.h"
#include "Platform/Window/CurrentWindow.h"
namespace Proof {
	class Proof_API VulkanShader: public Shader {
	public:
		VulkanShader(const std::string& VertexPath,const std::string& FragmentPath,bool IsProofDirectory = false); // CODE NEEDS TO BE NEATEN UP
		VulkanShader(const std::string& ShaderPath,bool IsProofDirectory = false);
	private:
		std::string FolderOfCurrentDirectory = "../Proof/src/";
		VkShaderModule shaderModule;
		VkShaderModule CreateShaderModule(const std::vector<char>&Code);
		Count<VulkanGraphicsContext> Graphics = std::static_pointer_cast<VulkanGraphicsContext>(CurrentWindow::GetGraphicsContext());
		 std::vector<char> ReadFile(const std::string& filename);
	};
}

