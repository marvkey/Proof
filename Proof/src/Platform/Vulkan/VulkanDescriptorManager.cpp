#include "Proofprch.h"
#include "VulkanDescriptorManager.h"
#include "VulkanShader.h"
#include "VulkanGraphicsContext.h"
#include "VulkanResourceBuffer.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanTexutre.h"
namespace Proof 
{
	VulkanDescriptorManager::VulkanDescriptorManager(const VulkanDescriptorManagerConfig& config)
	{
        m_WriteDescriptorMap.resize(Renderer::GetConfig().FramesFlight);
        m_DescriptorSets.resize(Renderer::GetConfig().FramesFlight);
		m_Config = config;

        Init();
	}
    VulkanDescriptorManager::~VulkanDescriptorManager()
    {
        Release();
    }
	void VulkanDescriptorManager::SetInput(std::string_view name, Count<UniformBuffer> buffer)
	{
        m_Build = false;
        auto shader = m_Config.Shader;
        const SahderInputDeclaration* decl =  shader->GetInputDeclaration(name.data());
        if (decl)
        {
           /// if (!m_Inputs.contains(decl->Set))
           /// {
           ///     PF_ENGINE_ERROR("Descriptor Manager {}, Input {} Descriptro Manager does not contain the set ", m_Config.DebugName, name, decl->Set);
           ///     return;
           /// }
            m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);

           // VkWriteDescriptorSet& write = m_WriteDescriptorMap[Renderer::GetCurrentFrame().FrameinFlight][decl->Set][decl->Binding];
           // write.dstSet = m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight][decl->Set].Set;
           // write.pBufferInfo = &buffer.As<VulkanUniformBuffer>()->GetDescriptorInfo(Renderer::GetCurrentFrame().FrameinFlight);
        }
        else
        {
            PF_ENGINE_ERROR("Render pass {}, Input {} not found", m_Config.DebugName, name);
        }
	}

    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class StorageBuffer> buffer)
    {
        m_Build = false;
        auto shader = m_Config.Shader;

        const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
        if (decl)
        {
            //if (!m_Inputs.contains(decl->Set))
            //{
            //    PF_ENGINE_ERROR("Descriptor Manager {}, Input {} Descriptro Manager does not contain the set ", m_Config.DebugName, name, decl->Set);
            //    return;
            //}
            m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);

            //VkWriteDescriptorSet& write = m_WriteDescriptorMap[Renderer::GetCurrentFrame().FrameinFlight][decl->Set][decl->Binding];
            //write.dstSet = m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight][decl->Set].Set;
            //write.pBufferInfo = &buffer.As<VulkanStorageBuffer>()->GetDescriptorInfo(Renderer::GetCurrentFrame().FrameinFlight);
        }
        else
        {
            PF_ENGINE_ERROR("Render pass {}, Input {} not found", m_Config.DebugName, name);
        }
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images)
    {
        m_Build = false;
        auto shader = m_Config.Shader;
        const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
        if (decl)
        {
            //if (!m_Inputs.contains(decl->Set))
            //{
            //    PF_ENGINE_ERROR("Descriptor Manager {}, Input {} Descriptro Manager does not contain the set ", m_Config.DebugName, name, decl->Set);
            //    return;
            //}
            m_Inputs[decl->Set][decl->Binding] = RenderPassInput(images);
        }
        else
        {
            PF_ENGINE_ERROR("Render pass {}, Input {} not found", m_Config.DebugName, name);
        }
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class Texture2D> buffer)
    {
        m_Build = false;
        auto shader = m_Config.Shader;
        const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
        if (decl)
        {
            //if (!m_Inputs.contains(decl->Set))
            //{
            //    PF_ENGINE_ERROR("Descriptor Manager {}, Input {} Descriptro Manager does not contain the set ", m_Config.DebugName, name, decl->Set);
            //    return;
            //}
            m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
            //VkWriteDescriptorSet& write = m_WriteDescriptorMap[Renderer::GetCurrentFrame().FrameinFlight][decl->Set][decl->Binding];
            //write.dstSet = m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight][decl->Set].Set;
            //write.pImageInfo = &buffer.As<VulkanTexture2D>()->GetImageBufferInfo();
        }
        else
        {
            PF_ENGINE_ERROR("Render pass {}, Input {} not found", m_Config.DebugName, name);
        }
    }
    void VulkanDescriptorManager::Bind()
    {

        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
        auto& descriptorSets= m_WriteDescriptorMap[Renderer::GetCurrentFrame().FrameinFlight];
        std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>> imageInfos;
        uint32_t lastImageInfo = 0;
        for (auto& [set, SetInfo] : m_Inputs)
        {
            for (auto& [binding, resource] : SetInfo)
            {
                VkWriteDescriptorSet& write = descriptorSets[set][binding];
                write.dstSet = m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight][set].Set;
                switch (resource.Type)
                {
                    case Proof::RenderPassResourceType::None:
                        PF_CORE_ASSERT(false);
                        break;
                    case Proof::RenderPassResourceType::UniformBuffer:
                        {
                            write.pBufferInfo = &resource.Input[0].As<VulkanUniformBuffer>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::StorageBuffer:
                        {
                            write.pBufferInfo = &resource.Input[0].As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::Texture2D:
                        {
                            write.pImageInfo = &resource.Input[0].As<VulkanTexture2D>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::TextureCube:
                        {
                           // write.pImageInfo = &resource.Input[0].As<VulkanTextureCube>()->GetDescriptorInfoVulkan();
                        }
                        break;
                    case Proof::RenderPassResourceType::Image2D:
                        {
                            //write.pBufferInfo = resource.Input[0].As<VulkanImage>()->GetDescriptorInfo();
                        }
                        break;
                    case Proof::RenderPassResourceType::Texture2DSet:
                        {
                            lastImageInfo++;
                            std::vector<VkDescriptorImageInfo> info;
                            //info.resize(resource.Input.size());
                            for (auto& image : resource.Input)
                            {
                                info.push_back(image.As<VulkanTexture2D>()->GetDescriptorInfoVulkan());
                            }
                            imageInfos[lastImageInfo] = info;
                            write.pImageInfo = imageInfos[lastImageInfo].data();
                            write.descriptorCount = resource.Input.size();

                        }
                        break;
                    default:
                        break;
                }
            }
        }

        for (auto& [set, setData] : descriptorSets)
        {
            // if that set does not have data n ned to call update on it
            // mainly for render material as they dont bind to sets they dont contain
            // also for graphics pipline wiht a render material 
            // as they should not have any data at set 0
            // so we check if the input contians any data at set 0 if not no need to update
            if (!m_Inputs.contains(set))continue;

            std::vector< VkWriteDescriptorSet> writes;
            writes.resize(setData.size());
            uint32_t index =0;
            for (auto& [binding, write] : setData)
            {
             //   write.dstSet = m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight][set].Set;
                writes[index] = descriptorSets[set][binding];
                index++;
            }
            vkUpdateDescriptorSets(device, writes.size(), writes.data(), 0, nullptr);
        }

        
        m_Build = true;
    }
    void VulkanDescriptorManager::Init()
    {
        auto shader = m_Config.Shader;
        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
        #if 1
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 10 * Renderer::GetConfig().FramesFlight;
        pool_info.poolSizeCount = 10;
        pool_info.pPoolSizes = pool_sizes;

        VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &m_DescriptorPool));
        #else
        std::vector< VkDescriptorPoolSize> largPool;
        // settinf to first descriptor 
        //largPool = shader->GetTypeCounts().at(m_Config.FirstSet);
        // retunrs the index 
        // -1 if not ocntian
        auto poolContains = [](std::vector< VkDescriptorPoolSize>& pool, int type)->int {
            for (int i = 0; i < pool.size(); i++)
            {
                if (pool[i].type == type)
                    return i;
            }
            return -1;
        };

        for (auto& [binding, data] : shader->GetTypeCounts())
        {
            //if (binding == m_Config.FirstSet)
            //    continue;
            for (auto& poolInfo : data)
            {
                int pos = poolContains(largPool, poolInfo.type);
                if (pos == -1)
                {
                    largPool.emplace_back(poolInfo);
                    continue;
                }
                largPool[pos].descriptorCount += poolInfo.descriptorCount;
            }
        }
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = largPool.size() * Renderer::GetConfig().FramesFlight;
        pool_info.poolSizeCount = largPool.size();
        pool_info.pNext = nullptr;
        pool_info.pPoolSizes = largPool.data();
        VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &m_DescriptorPool));
        #endif
        for (auto& [set, shaderDescriptorSet] : shader->GetShaderDescriptorSet())
        {
            if (set > m_Config.LastSet)
                continue;
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            for (auto& [binding, uniformBuffer] : shaderDescriptorSet.UniformBuffers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = uniformBuffer.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                const VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet.at(uniformBuffer.Name);
                for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
                {
                     m_WriteDescriptorMap[frame][set][binding] =  writeDescritporSet;
                }
                //writeDescritporSet = {};
                //writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                //writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                //writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                //writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, storageBuffer] : shaderDescriptorSet.StorageBuffers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = storageBuffer.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                //VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[storageBuffer.Name];
                //writeDescritporSet = {};
                //writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                //writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                //writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                //writeDescritporSet.dstBinding = layoutBinding.binding;

                const VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet.at(storageBuffer.Name);
                for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
                {
                    m_WriteDescriptorMap[frame][set][binding] = writeDescritporSet;
                }
            }

            for (auto& [binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                layoutBinding.descriptorCount = imageSampler.DescriptorCount;
                layoutBinding.stageFlags = imageSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                //VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[imageSampler.Name];
                //writeDescritporSet = {};
                //writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                //writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                //writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                //writeDescritporSet.dstBinding = layoutBinding.binding;

                const VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet.at(imageSampler.Name);
                for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
                {
                    m_WriteDescriptorMap[frame][set][binding] = writeDescritporSet;
                }
            }

            for (auto& [binding, seperateTexture] : shaderDescriptorSet.SeperateTextures)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                layoutBinding.descriptorCount = seperateTexture.DescriptorCount;
                layoutBinding.stageFlags = seperateTexture.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                //VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[seperateTexture.Name];
                //writeDescritporSet = {};
                //writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                //writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                //writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                //writeDescritporSet.dstBinding = layoutBinding.binding;


                const VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet.at(seperateTexture.Name);
                for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
                {
                    m_WriteDescriptorMap[frame][set][binding] = writeDescritporSet;
                }
            }

            for (auto& [binding, seperateSampler] : shaderDescriptorSet.SeperateSamplers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                layoutBinding.descriptorCount = seperateSampler.DescriptorCount;
                layoutBinding.stageFlags = seperateSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                //VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[seperateSampler.Name];
                //writeDescritporSet = {};
                //writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                //writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                //writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                //writeDescritporSet.dstBinding = layoutBinding.binding;


                const VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet.at(seperateSampler.Name);
                for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
                {
                    m_WriteDescriptorMap[frame][set][binding] = writeDescritporSet;
                }
            }

            for (auto& [binding, storageImage] : shaderDescriptorSet.StorageImages)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                layoutBinding.descriptorCount = storageImage.DescriptorCount;
                layoutBinding.stageFlags = storageImage.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                //VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[storageImage.Name];
                //writeDescritporSet = {};
                //writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                //writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                //writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                //writeDescritporSet.dstBinding = layoutBinding.binding;

                const VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet.at(storageImage.Name);
                for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
                {
                    m_WriteDescriptorMap[frame][set][binding] = writeDescritporSet;
                }
            }

            VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo{};
            descriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorLayoutInfo.pNext = nullptr;
            descriptorLayoutInfo.bindingCount = layoutBindings.size();
            descriptorLayoutInfo.pBindings = layoutBindings.data();
            if (layoutBindings.empty())
                continue;
            for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
            {
                VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayoutInfo, nullptr, &m_DescriptorSets[frame][set].Layout));
            }
            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.pNext = nullptr;
            allocInfo.descriptorPool = m_DescriptorPool;
            allocInfo.descriptorSetCount = 1;
            for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
            {
                allocInfo.pSetLayouts = &m_DescriptorSets[frame][set].Layout;

                vkAllocateDescriptorSets(device, &allocInfo, &m_DescriptorSets[frame][set].Set);
            }
        }

      
    }
    void VulkanDescriptorManager::Release()
    {
        
        Renderer::SubmitDatafree([pool = m_DescriptorPool, descriptorSet=m_DescriptorSets ] {
            auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
            for (int frame = 0; frame < descriptorSet.size(); frame++)
            {
                for (auto [binding, resource] : descriptorSet[frame])
                {
                    vkDestroyDescriptorSetLayout(device, resource.Layout, nullptr);
                    VK_CHECK_RESULT(vkFreeDescriptorSets(device, pool, 1, &resource.Set));
                }
            }
            vkDestroyDescriptorPool(device, pool, nullptr);

        });
        m_WriteDescriptorMap.clear();
        m_DescriptorSets.clear();
        m_DescriptorPool = nullptr;
        m_Build = false;
    }
}

