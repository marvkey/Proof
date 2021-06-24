#include "Proofprch.h"
#include "VulkanShader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <conio.h>
#include "Platform/Window/CurrentWindow.h"
namespace Proof {
	VulkanShader::VulkanShader(const std::string & VertexPath,const std::string & FragmentPath,bool IsProofDirectory) {
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
		std::ifstream VertexShaderFile;
		std::ifstream FragmentShaderFile;
		std::stringstream vShaderStream,fShaderStream;
		if (IsProofDirectory == false)
			VertexShaderFile.open(VertexPath.c_str());
		else
			VertexShaderFile.open((FolderOfCurrentDirectory + VertexPath).c_str());

		if (IsProofDirectory == false)
			FragmentShaderFile.open(FragmentPath.c_str());
		else
			FragmentShaderFile.open((FolderOfCurrentDirectory + FragmentPath).c_str());

		if (VertexShaderFile.is_open() == true && FragmentShaderFile.is_open() == true) {
			
			vShaderStream << VertexShaderFile.rdbuf();//put to stream
			fShaderStream << FragmentShaderFile.rdbuf();
			VertexShaderFile.close();
			FragmentShaderFile.close();

			VertexShaderSource = vShaderStream.str();
			FragmentShaderSource = fShaderStream.str();
		}
		else {
			PF_ENGINE_ERROR("Shader File Is Incorrect");
			char full[_MAX_PATH];

			if (IsProofDirectory == false) {
				if (_fullpath(full,VertexPath.c_str(),_MAX_PATH) != NULL)
					PF_ENGINE_INFO("Full path is: {}",full);

				if (_fullpath(full,FragmentPath.c_str(),_MAX_PATH) != NULL)
					PF_ENGINE_INFO("Full path is: {}",full);
			}

			else {
				if (_fullpath(full,(FolderOfCurrentDirectory + VertexPath).c_str(),_MAX_PATH) != NULL)
					PF_ENGINE_INFO("Full path is: {}",full);

				if (_fullpath(full,(FolderOfCurrentDirectory + FragmentPath).c_str(),_MAX_PATH) != NULL)
					PF_ENGINE_INFO("Full path is: {}",full);
			}
			return;
		}
		
		auto vertShaderCode = ReadFile(FolderOfCurrentDirectory + VertexPath);
		auto fragShaderCode = ReadFile(FolderOfCurrentDirectory + FragmentPath);

		VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,fragShaderStageInfo};
		vkDestroyShaderModule(Graphics->device,fragShaderModule,nullptr);
		vkDestroyShaderModule(Graphics->device,vertShaderModule,nullptr);
	}
	VulkanShader::VulkanShader(const std::string & ShaderPath,bool IsProofDirectory) {}

	VkShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& Code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize =Code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(Code.data());
		VkShaderModule shaderModule;
		if (vkCreateShaderModule(Graphics->device,&createInfo,nullptr,&shaderModule) != VK_SUCCESS) {
			PF_ENGINE_ERROR("failed to create shader module!");
		}
		return shaderModule;
	}
	std::vector<char> VulkanShader::ReadFile(const std::string & filename) {
		std::ifstream file(filename,std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			PF_ENGINE_ERROR("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(),fileSize);
		file.close();
		return buffer;
	}
}