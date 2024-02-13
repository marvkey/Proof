#include "Proofprch.h"
#include "VulkanDescriptorManager.h"
#include "VulkanShader.h"
#include "VulkanGraphicsContext.h"
#include "VulkanResourceBuffer.h"
#include "VulkanRenderer.h"
#include "VulkanTexutre.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanSampler.h"
namespace Proof 
{
    /**
    *
    * very important 
    * https://stackoverflow.com/questions/50005062/vulkan-descriptor-binding
    * each objcet may need on descirptoer set for multiple 
    */
    static VkDescriptorPoolSize* pool_sizes = new VkDescriptorPoolSize[11]
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


	VulkanDescriptorManager::VulkanDescriptorManager(const VulkanDescriptorManagerConfig& config, bool isRenderTrhead):
        m_Config(config)
	{
        if (isRenderTrhead)
            RT_Build();
        else
        {
            Build();
        }
        WeakCount<VulkanDescriptorManager> instanceWeakCount = this;
        m_ShaderReloadCallbackIndex = m_Config.Shader->AddShaderReloadCallback([instanceWeakCount]
        {
            if (!instanceWeakCount.IsValid())
                return;
            auto descriptorManager = instanceWeakCount.Lock();

            descriptorManager->Release();
            descriptorManager->Build();
        });
	}
    VulkanDescriptorManager::~VulkanDescriptorManager()
    {
        Release();
        m_Config.Shader->RemoveShaderReloadCallback(m_ShaderReloadCallbackIndex);
    }
    void VulkanDescriptorManager::Build()
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance]()
            {
                instance->RT_Build();
            });
    }
	void VulkanDescriptorManager::SetInput(std::string_view name, Count<UniformBuffer> buffer)
	{
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, buffer]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
      
	}
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class TextureCube> buffer, bool isRenderThread)
    {
        if (isRenderThread)
        {
            InvalidateDescriptors();
            auto shader = m_Config.Shader;
            const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
            if (decl)
            {
                m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
            }
            else
            {
                PF_ENGINE_ERROR("Render pass {}, Input {} not found", m_Config.DebugName, name);
                PF_CORE_ASSERT(false);
            }
            return;
        }
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, buffer]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });

    }

    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class StorageBuffer> buffer)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, buffer]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }

    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class UniformBufferSet> buffer)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, buffer]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class StorageBufferSet> buffer)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, buffer]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, images]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(images);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class Texture2D> buffer,bool isRenderThread)
    {
        if (isRenderThread)
        {
            InvalidateDescriptors();
            auto shader = m_Config.Shader;
            const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
            if (decl)
            {
                m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
            }
            else
            {
                PF_ENGINE_ERROR("Render pass {}, Input {} not found", m_Config.DebugName, name);
                PF_CORE_ASSERT(false);
            }
            return;
        }
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, buffer]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(buffer);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }

   
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<Image2D>image)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, image]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(image);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<ImageView> imageView)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, imageView]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(imageView);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, images]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(images);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }

    void VulkanDescriptorManager::SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, imageViews ]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(imageViews);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, Count<class Image> imageView)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, imageView]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    if(imageView)
                        instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(imageView);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::SetInput(std::string_view name, const std::vector< Count<class Image>>& imageViews)
    {
        Count<VulkanDescriptorManager> instance = this;
        Renderer::Submit([instance, name, imageViews]
            {
                instance->InvalidateDescriptors();
                auto shader = instance->m_Config.Shader;
                const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
                if (decl)
                {
                    auto value = instance->m_Inputs[decl->Set][decl->Binding];
                    instance->m_Inputs[decl->Set][decl->Binding] = RenderPassInput(imageViews);
                }
                else
                {
                    PF_ENGINE_ERROR("Render pass {}, Input {} not found", instance->m_Config.DebugName, name);
                    PF_CORE_ASSERT(false);
                }
            });
    }
    void VulkanDescriptorManager::RT_Bind()
    {
        PF_PROFILE_FUNC();
        PF_PROFILE_TAG("",m_Config.DebugName.c_str());
        const uint32_t currentFrame = Renderer::RT_GetCurrentFrameInFlight();

        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
        {
            if (m_LastFrameGrabPool != FrameTime::GetFrameCount())
            {
                for (auto pool : m_UsedPools[Renderer::RT_GetCurrentFrameInFlight()])
                {
                    vkResetDescriptorPool(device, pool, 0);
                    m_FreePools.push_back(pool);
                }
                m_UsedPools[Renderer::RT_GetCurrentFrameInFlight()].clear();
            }
            m_LastFrameGrabPool = FrameTime::GetFrameCount();
        }

        //TODO faster way to do this using delegates to know when resources are updated

        //if (!m_DirtyDescriptorSets[currentFrame])
        //{
        //    for (auto& [set, SetInfo] : m_Inputs)
        //    {
        //        for (auto& [binding, resource] : SetInfo)
        //        {   
        //            if (resource.Input.size() > 1)
        //            {
        //                InvalidateDescriptors();
        //                break;
        //            }
        //        }
        //        //to breka out of current loop
        //        if (m_DirtyDescriptorSets[currentFrame])
        //            break;
        //    }
        //}

        bool newFrame = m_FrameCounter != FrameTime::GetFrameCount();

        auto& descriptorSets = m_WriteDescriptorMap[currentFrame];
        std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>> imageInfos;
        // a unique pos in the list for the image
        uint32_t imageUniquePos = 0;
        
        for (auto& [set, SetInfo] : m_Inputs)
        {
            //only allocate when we are trying to bind agian in the same frame
            
            bool allocated = false;
            if (!newFrame )
            {
              allocated =true;
              AllocateDescriptorSet(currentFrame, set);
            }

            if (m_FreePools.size() > 0)
            {
                if(!allocated)
                    AllocateDescriptorSet(currentFrame, set);
            
            }
            descriptorSets[set].clear();

            for (auto& [binding, resource] : SetInfo)
            {
                VkWriteDescriptorSet& write = descriptorSets[set].emplace_back();
                const std::string& stringName = m_Config.Shader->GetShaderDescriptorSet().at(set).MapBindingToWrite.at(binding);
                write = m_Config.Shader->GetShaderDescriptorSet().at(set).WriteDesriptorSet.at(stringName);
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = m_DescriptorSets[currentFrame][set].Set;

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
                    case Proof::RenderPassResourceType::UniformBufferSet:
                        {
                            write.pBufferInfo = &resource.Input[0].As<VulkanUniformBufferSet>()->GetBuffer(currentFrame).As< VulkanUniformBuffer>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::StorageBuffer:
                        {
                            write.pBufferInfo = &resource.Input[0].As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::StorageBufferSet:
                        {
                            write.pBufferInfo = &resource.Input[0].As<VulkanStorageBufferSet>()->GetBuffer(currentFrame).As< VulkanStorageBuffer>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::Texture2D:
                        {
                            write.pImageInfo = &resource.Input[0].As<VulkanTexture2D>()->GetDescriptorInfoVulkan();
                            PF_CORE_ASSERT(write.pImageInfo);
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::Texture2DSet:
                        {
                            imageUniquePos++;
                            std::vector<VkDescriptorImageInfo> info;
                            //info.resize(resource.Input.size());
                            for (auto image : resource.Input)
                            {
                                //image = resource.Input[0];
                                info.push_back(image.As<VulkanTexture2D>()->GetDescriptorInfoVulkan());
                            }
                            imageInfos[imageUniquePos] = info;
                            write.pImageInfo = imageInfos[imageUniquePos].data();
                            write.descriptorCount = resource.Input.size();
                        }
                        break;
                    case Proof::RenderPassResourceType::TextureCube:
                        {
                            write.pImageInfo = &resource.Input[0].As<VulkanTextureCube>()->GetDescriptorInfoVulkan();
                            PF_CORE_ASSERT(write.pImageInfo);
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::Image2D:
                        {
                            write.pImageInfo = &resource.Input[0].As<VulkanImage2D>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::Image2DSet:
                        {
                            imageUniquePos++;
                            std::vector<VkDescriptorImageInfo> info;
                            //info.resize(resource.Input.size());
                            for (auto& image : resource.Input)
                            {
                                info.push_back(image.As<VulkanImage2D>()->GetDescriptorInfoVulkan());
                            }
                            imageInfos[imageUniquePos] = info;
                            write.pImageInfo = imageInfos[imageUniquePos].data();
                            write.descriptorCount = resource.Input.size();
                        }
                        break;
                    case Proof::RenderPassResourceType::ImageView:
                        {
                            write.pImageInfo = &resource.Input[0].As<VulkanImageView>()->GetDescriptorInfoVulkan();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::ImageViewSet:
                        {
                            imageUniquePos++;
                            std::vector<VkDescriptorImageInfo> info;

                            info.reserve(resource.Input.size());
                            for (auto& image : resource.Input)
                            {
                                info.push_back(image.As<VulkanImageView>()->GetDescriptorInfoVulkan());
                            }
                            imageInfos[imageUniquePos] = info;
                            write.pImageInfo = imageInfos[imageUniquePos].data();
                            write.descriptorCount = resource.Input.size();
                        }
                        break;
                    case Proof::RenderPassResourceType::Image:
                        {
                            write.pImageInfo = (VkDescriptorImageInfo*) resource.Input[0].As<RendererResource>()->GetResourceDescriptorInfo();
                            write.descriptorCount = 1;
                        }
                        break;
                    case Proof::RenderPassResourceType::Sampler:
                        {
                            write.pImageInfo = (VkDescriptorImageInfo*)resource.Input[0].As<RendererResource>()->GetResourceDescriptorInfo();
                            write.descriptorCount = 1;
                        }
                     break;
                    case Proof::RenderPassResourceType::ImageSet:
                        {
                            imageUniquePos++;
                            std::vector<VkDescriptorImageInfo> info;
                            //info.resize(resource.Input.size());
                            for (auto& image : resource.Input)
                            {
                                info.push_back(*(VkDescriptorImageInfo*)resource.Input[0].As<RendererResource>()->GetResourceDescriptorInfo());
                            }
                            imageInfos[imageUniquePos] = info;
                            write.pImageInfo = imageInfos[imageUniquePos].data();
                            write.descriptorCount = resource.Input.size();
                        }
                        break;
                    default:
                        PF_CORE_ASSERT(false,"Type not supported");
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
            if(!setData.empty())
                vkUpdateDescriptorSets(device, (uint32_t)setData.size(), setData.data(), 0, nullptr);
        }
        m_FrameCounter = FrameTime::GetFrameCount();
    }
    void VulkanDescriptorManager::SetGlobalInput(Count<GlobalBufferSet> globalInputs)
    {
        auto shader = m_Config.Shader;

        for (auto& [name, inputData] : globalInputs->GetBuffers())
        {
            const SahderInputDeclaration* decl = shader->GetInputDeclaration(name.data());
            if (!decl)continue;

            RendererResourceType type = inputData.first;

            switch (type)
            {
                case Proof::RendererResourceType::None:
                    break;
                case Proof::RendererResourceType::Sampler:
                    {

                    Count<RenderSampler> storageBufferSet = inputData.second.As<RenderSampler>();
                    m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                break;
                case Proof::RendererResourceType::Image2D:
                    {
                        Count<Image2D> storageBufferSet = inputData.second.As<Image2D>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                case Proof::RendererResourceType::ImageView:
                    {
                        Count<ImageView> storageBufferSet = inputData.second.As<ImageView>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                case Proof::RendererResourceType::Texture2D:
                    {
                        Count<Texture2D> storageBufferSet = inputData.second.As<Texture2D>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                case Proof::RendererResourceType::TextureCube:
                    {
                        Count<TextureCube> storageBufferSet = inputData.second.As<TextureCube>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                case Proof::RendererResourceType::UniformBuffer:
                {
                    Count<UniformBuffer> storageBufferSet = inputData.second.As<UniformBuffer>();
                    m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                }
                    break;
                case Proof::RendererResourceType::UniformBufferSet:
                    {
                        Count<UniformBufferSet> storageBufferSet = inputData.second.As<UniformBufferSet>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                case Proof::RendererResourceType::StorageBuffer:
                    {
                        Count<StorageBuffer> storageBufferSet = inputData.second.As<StorageBuffer>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                case Proof::RendererResourceType::StorageBufferSet:
                    {
                        Count<StorageBufferSet> storageBufferSet = inputData.second.As<StorageBufferSet>();
                        m_Inputs[decl->Set][decl->Binding] = RenderPassInput(storageBufferSet);
                    }
                    break;
                default:
                    PF_CORE_ASSERT(false, "Type not supported");
                    break;
            }
        }
    }
  
    void VulkanDescriptorManager::SetGoalballInputs(Count<GlobalBufferSet> uniformData)
    {
        for (auto set : m_GlobalSets)
        {
            if (set == uniformData)
                return;
        }
        m_GlobalSets.emplace_back(uniformData);
        SetGlobalInput(uniformData);
    }
    void VulkanDescriptorManager::RT_Build()
    {
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();
        m_WriteDescriptorMap.resize(Renderer::GetConfig().FramesFlight);
        m_DescriptorSets.resize(Renderer::GetConfig().FramesFlight);

        auto shader = m_Config.Shader;
        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
                   

        m_DescriptorPool = GrabPool();
        for (auto& [set, shaderDescriptorSet] : shader->GetShaderDescriptorSet())
        {
            if (set > m_Config.LastSet)
                continue;
            for (int frame = 0; frame < Renderer::GetConfig().FramesFlight; frame++)
            {
                AllocateDescriptorSet(frame, set);
            }
        }
        m_DirtyDescriptorSets.resize(Renderer::GetConfig().FramesFlight, true);

        for (auto globalInputs : m_GlobalSets)
        {
            SetGlobalInput(globalInputs);
        }
        m_LastFrameGrabPool = FrameTime::GetFrameCount();
        m_FrameCounter = FrameTime::GetFrameCount();
    }
    void VulkanDescriptorManager::AllocateDescriptorSet(uint32_t frame, uint32_t set)
    {
        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

        VkDescriptorSet vkSet = nullptr;
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = m_DescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_Config.Shader->GetDescriptorResource().at(set).Layout;

        VkResult allocResult = vkAllocateDescriptorSets(device, &allocInfo, &vkSet);
        bool needReallocate = false;

        switch (allocResult) 
        {
        case VK_ERROR_FRAGMENTED_POOL:
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            needReallocate = true;
        }
        if (needReallocate) 
        {
            //allocate a new pool and retry
            m_UsedPools[Renderer::RT_GetCurrentFrameInFlight()].emplace_back(m_DescriptorPool);
            m_DescriptorPool = GrabPool();

            allocInfo.descriptorPool = m_DescriptorPool;
            vkSet = nullptr;
            VK_CHECK_RESULT( vkAllocateDescriptorSets(device, &allocInfo, &vkSet));
        }
        VulkanUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_DESCRIPTOR_SET, fmt::format("{} descriptorSet RenderFrame: {} Set:{}", m_Config.DebugName, frame,set), vkSet);
        m_DescriptorSets[frame][set].Set = vkSet;
    }
    void VulkanDescriptorManager::InvalidateDescriptors()
    {
        const uint32_t framesInFlight = Renderer::GetConfig().FramesFlight;
        for (uint32_t i = 0; i < framesInFlight; i++)
            m_DirtyDescriptorSets[i] = true;
    }
    VkDescriptorPool VulkanDescriptorManager::GrabPool()
    {
        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

        VkDescriptorPool pool = nullptr;

        
        if (m_FreePools.empty() )
        {
            
            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets = 10 * Renderer::GetConfig().FramesFlight;
            pool_info.poolSizeCount = 11;
            pool_info.pPoolSizes = pool_sizes;

            VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &pool));
        }
        else
        {
            pool = m_FreePools.back();
            m_FreePools.pop_back();
        }   
       
        return pool;
    }
    void VulkanDescriptorManager::Release()
    {
        Renderer::SubmitResourceFree([pool = m_DescriptorPool, descriptorSet=m_DescriptorSets ] 
            {
                auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
                for (int frame = 0; frame < descriptorSet.size(); frame++)
                {
                    for (auto [binding, resource] : descriptorSet[frame])
                    {
                        if(resource.Set)
                            VK_CHECK_RESULT(vkFreeDescriptorSets(device, pool, 1, &resource.Set));
                    }
                }
                vkDestroyDescriptorPool(device, pool, nullptr);

            });
        m_WriteDescriptorMap.clear();
        m_DescriptorSets.clear();
        m_DescriptorPool = nullptr;
    }
}

