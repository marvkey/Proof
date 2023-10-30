#include "Proofprch.h"
#include "VulkanRenderMaterial.h"
#include "VulkanResourceBuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Math/Random.h"
#include "VulkanComputePass.h"
#include "VulkanComputePipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer.h"

namespace Proof {
	VulkanRenderMaterial::VulkanRenderMaterial(const RenderMaterialConfiguration& config)
		:m_Config(config)
	{
		Build();
	}

	VulkanRenderMaterial::VulkanRenderMaterial(const VulkanRenderMaterial& other)
	{
		m_Config = other.m_Config;
		Build();
		m_UniformBufferStorage = Buffer::Copy(other.m_UniformBufferStorage);

	}

	void VulkanRenderMaterial::Build()
	{
		VulkanDescriptorManagerConfig descriptorConfig;
		descriptorConfig.DebugName = m_Config.DebugName + " RenderMaterial DescritporManager";
		descriptorConfig.Shader = m_Config.Shader.As<VulkanShader>();
		descriptorConfig.LastSet = 0; //only set 0
		m_DescritptorSetManager = Count<VulkanDescriptorManager>::Create(descriptorConfig);

		auto vk_Shader = m_Config.Shader.As<VulkanShader>();
		uint32_t allocateSize = 0;

		for (auto& [pushName, pushData] : vk_Shader->GetPushConstants())
		{
			if (pushData.stageFlags | VK_SHADER_STAGE_FRAGMENT_BIT || pushData.stageFlags | VK_SHADER_STAGE_COMPUTE_BIT || pushData.stageFlags | VK_SHADER_STAGE_GEOMETRY_BIT ||
				pushData.stageFlags | VK_SHADER_STAGE_VERTEX_BIT)
			{
				m_UniformName = pushName;
				allocateSize = pushData.size;
				break;
			}
		}
		if (allocateSize > 0)
		{
			m_UniformBufferStorage.Allocate(allocateSize);
			m_UniformBufferStorage.Fill(0);
		}
	}

	void VulkanRenderMaterial::Release()
	{
		m_UniformBufferStorage.Release();
	}

	void VulkanRenderMaterial::Set(std::string_view name, Count<class UniformBuffer> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}
	void VulkanRenderMaterial::Set(std::string_view name, Count<class StorageBuffer> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}

	void VulkanRenderMaterial::Set(std::string_view name, Count<class UniformBufferSet> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}

	void VulkanRenderMaterial::Set(std::string_view name, Count<class StorageBufferSet> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}
	void VulkanRenderMaterial::Set(std::string_view name, Count<class Texture2D> texture)
	{
		m_DescritptorSetManager->SetInput(name, texture);
	}
	void VulkanRenderMaterial::Set(std::string_view name, const std::vector< Count<class Texture2D>>& images)
	{
		m_DescritptorSetManager->SetInput(name, images);
	}
	void VulkanRenderMaterial::Set(std::string_view name, Count<class TextureCube> texture)
	{
		m_DescritptorSetManager->SetInput(name, texture);
	}
	void VulkanRenderMaterial::Set(std::string_view name, Count<class ImageView> imageView)
	{
		m_DescritptorSetManager->SetInput(name, imageView);
	}
	void VulkanRenderMaterial::Set(std::string_view name, const std::vector< Count<class ImageView>>& imageViews)
	{
		m_DescritptorSetManager->SetInput(name, imageViews);
	}
	void VulkanRenderMaterial::Set(std::string_view name, Count<class Image2D>image)
	{
		m_DescritptorSetManager->SetInput(name, image);
	}
	void VulkanRenderMaterial::Set(std::string_view name, const std::vector< Count<class Image2D>>& images)
	{
		m_DescritptorSetManager->SetInput(name, images);
	}

	void VulkanRenderMaterial::Set(std::string_view name, Count<class Image>image) 
	{
		m_DescritptorSetManager->SetInput(name, image);

	}
	void VulkanRenderMaterial::Set(std::string_view name, const std::vector< Count<class Image>>& images)
	{
		m_DescritptorSetManager->SetInput(name, images);
	}
	void VulkanRenderMaterial::Set(const std::string& name, float value)
	{
		SetInternal<float>(name,value);
	}
	void VulkanRenderMaterial::Set(const std::string& name, bool value)
	{
		int casted = value;
		SetInternal<int>(name, casted);

	}
	void VulkanRenderMaterial::Set(const std::string& name, int value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, uint32_t value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const Vector2I& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const VectorI& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const Vector4I& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const Vector2& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const Vector& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const Vector4& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const glm::mat3& value)
	{
		SetInternal(name, value);
	}

	void VulkanRenderMaterial::Set(const std::string& name, const glm::mat4& value)
	{
		SetInternal(name, value);
	}

	float& VulkanRenderMaterial::GetFloat(const std::string& name) 
	{
		return GetInternal<float>(name);
	}
	bool& VulkanRenderMaterial::GetBool(const std::string& name)
	{
		return GetInternal<bool>(name);
	}
	int& VulkanRenderMaterial::GetInt(const std::string& name) 
	{
		return GetInternal<int>(name);
	}

	uint32_t& VulkanRenderMaterial::GetUint32(const std::string& name) 
	{
		return GetInternal<uint32_t>(name);
	}

	Vector2I& VulkanRenderMaterial::GetVector2I(const std::string& name) 
	{
		return GetInternal< Vector2I>(name);
	}

	VectorI& VulkanRenderMaterial::GetVectorI(const std::string& name) 
	{
		return GetInternal<VectorI>(name);
	}

	Vector4I& VulkanRenderMaterial::GetVector4I(const std::string& name) 
	{
		return GetInternal<Vector4I>(name);
	}

	Vector2& VulkanRenderMaterial::GetVector2(const std::string& name) 
	{
		return GetInternal<Vector2>(name);
	}

	Vector& VulkanRenderMaterial::GetVector(const std::string& name) 
	{
		return GetInternal<Vector>(name);
	}

	Vector4& VulkanRenderMaterial::GetVector4(const std::string& name) 
	{
		return GetInternal<Vector4>(name);
	}

	glm::mat3& VulkanRenderMaterial::GetMatrix3(const std::string& name) 
	{
		return GetInternal<glm::mat3>(name);
	}

	glm::mat4& VulkanRenderMaterial::GetMatrix4(const std::string& name) 
	{
		return GetInternal<glm::mat4>(name);
	}

	Count<class Texture2D> VulkanRenderMaterial::TryGetTexture2D(std::string_view name)
	{
		const SahderInputDeclaration* decl = m_Config.Shader.As<VulkanShader>()->GetInputDeclaration(name.data());
		if (decl)
		{
			if (m_DescritptorSetManager->GetInputs().contains(decl->Set))
			{
				if (m_DescritptorSetManager->GetInputs().at(decl->Set).contains(decl->Binding))
				{
					return m_DescritptorSetManager->GetInputs().at(decl->Set).at(decl->Binding).Input[0].As<Texture2D>();
				}
			}
		}
		return nullptr;
	}

	Count<class Image2D> VulkanRenderMaterial::TryGetImage2D(std::string_view name)
	{
		PF_CORE_ASSERT(false);
		return nullptr;
	}

	void VulkanRenderMaterial::RT_Bind(Count<class VulkanRenderCommandBuffer> commandBuffer, Count<VulkanComputePass> computePass)
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("",m_Config.DebugName.c_str());
		auto vk_Shader = m_Config.Shader.As<VulkanShader>();
		m_DescritptorSetManager->RT_Bind();

		auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
		for (auto& [set, setInfo] : frameSet)
		{
			if (set != 0 || setInfo.Set == nullptr)
				continue;

			// set0 is for te material to bind to 
			// basically we have to define a set layout for each descriptor set 0-3
			// but some set may not have data and we do note creata a descriptor set for it
			// so we basically just seeing if thats teh case we dont bind it
			vkCmdBindDescriptorSets(
				commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
				VK_PIPELINE_BIND_POINT_COMPUTE,
				computePass->GetConfig().Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
				(int)set,
				1,
				&setInfo.Set,
				0,
				nullptr);
		}

		for (auto& [pushName, pushData] : vk_Shader->GetPushConstants())
		{
			if (pushData.stageFlags | VK_SHADER_STAGE_COMPUTE_BIT)
			{
				computePass->RT_PushData(pushName, m_UniformBufferStorage.Get());
			}
		}
	}

	void VulkanRenderMaterial::RT_Bind(Count<VulkanRenderCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass)
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("", m_Config.DebugName.c_str());
		auto vk_Shader = m_Config.Shader.As<VulkanShader>();
		m_DescritptorSetManager->RT_Bind();

		auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
		for (auto& [set, setInfo] : frameSet)
		{
			if (set != 0 || setInfo.Set == nullptr)
				continue;

			// set0 is for te material to bind to 
			// basically we have to define a set layout for each descriptor set 0-3
			// but some set may not have data and we do note creata a descriptor set for it
			// so we basically just seeing if thats teh case we dont bind it
			vkCmdBindDescriptorSets(
				commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				renderPass->GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
				(int)set,
				1,
				&setInfo.Set,
				0,
				nullptr);
		}

		for (auto& [pushName, pushData] : vk_Shader->GetPushConstants())
		{
			//if (pushData.stageFlags | VK_SHADER_STAGE_FRAGMENT_BIT || pushData.stageFlags | VK_SHADER_STAGE_COMPUTE_BIT || pushData.stageFlags | VK_SHADER_STAGE_VERTEX_BIT)
			{
				renderPass->RT_PushData(pushName, m_UniformBufferStorage.Get());
			}
		}
	}

	const ShaderResourceBufferInfo* VulkanRenderMaterial::FindUniformDeclaration(const std::string& name)
	{
		auto vk_Shader = m_Config.Shader.As<VulkanShader>();
		return vk_Shader->GetPushConstantInput(m_UniformName, name);
	}

}
