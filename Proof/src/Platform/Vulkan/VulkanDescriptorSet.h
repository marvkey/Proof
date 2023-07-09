#pragma once
#include <vulkan/vulkan.h>
#include "Proof/Renderer/Renderer.h"
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Vulkan.h"
namespace Proof
{

    class VulkanDescriptorBuffer;
   
    class VulkanDescriptorSet : public DescriptorSet {
    public:
        struct VulkanDescriptorSetBuilder {
        public:
            VulkanDescriptorSetBuilder(DescriptorSets set) {
                m_Set = set;
            }
            //count is for arrays 
            VulkanDescriptorSetBuilder& AddBinding(uint32_t binding, DescriptorType descriptorType, ShaderStage shaderStage ,uint32_t count = 1);
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> Bindings{};

            Count<VulkanDescriptorSet> Build();
        private:
            DescriptorSets m_Set;
        };
        VkDescriptorSet* GetDescriptorSet(uint32_t index = Renderer::GetCurrentFrame().FrameinFlight) {
            return &m_DescriptorSets[index];
        }
        VulkanDescriptorSet(DescriptorSets set,std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        virtual ~VulkanDescriptorSet();

        const VkDescriptorSetLayout& GetDescriptorSetLayout()const  { return m_DescriptorSetLayout; }
        friend class VulkanRenderer;
        uint32_t GetCount() {
            m_Bindings.size();
        }

        DescriptorSet& WriteBuffer(uint32_t binding, Count<UniformBuffer> buffer);
        DescriptorSet& WriteBuffer(uint32_t binding,  Count<StorageBuffer> buffer);

        DescriptorSet& WriteImage(uint32_t binding, Count<class Texture2D> image);
        DescriptorSet& WriteImage(uint32_t binding, Count<class TextureCube> image);
        DescriptorSet& WriteImage(uint32_t binding, std::vector<Count<class Texture2D>>& image);

        void Overwrite(int frame = Renderer::GetCurrentFrame().FrameinFlight);

        DescriptorSets SetIndex() {
            return m_Set;
        }
        friend class VulkanPipeLineLayout;
        void Bind(Count<class RenderCommandBuffer> commandBuffer, Count<class PipeLineLayout>pipeLineLayout);

    private:
        bool Build(int frame = Renderer::GetCurrentFrame().FrameinFlight);
        VkDescriptorSetLayout m_DescriptorSetLayout;
        Special<class VulkanDescriptorWriter> m_Writer;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
        std::vector<VkDescriptorSet	> m_DescriptorSets;
        DescriptorSets m_Set;

        friend class VulkanDescriptorWriter;
    };

    //class VulkanDescriptorSetManager 
    //{
    //public:
    //    VulkanDescriptorSetManager(Count<VulkanShader> shader);
    //private:
    //    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> Bindings{};
    //};
    class VulkanDescriptorPool {
    public:
        struct Builder {
        public:
            Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& SetMaxSets(uint32_t num) {
                MaxSets = num;
                return *this;
            }

            Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags) {
                PoolFlags = flags;
                return *this;

            }
            uint32_t MaxSets =1000;
            VkDescriptorPoolCreateFlags PoolFlags = 0;
            std::vector<VkDescriptorPoolSize> PoolSizes{};
            Count<VulkanDescriptorPool> Build(VkDevice device);
        };

        VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes, VkDevice& device);

        virtual ~VulkanDescriptorPool();


        bool Allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);
        VkDescriptorPool GetPool() {
            return m_CurrentPool;
        }
        VkDescriptorSet AddTexture(VkSampler sampler, VkImageView image_view, VkImageLayout image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    private:
        void ResetPool();
        friend class VulkanRenderer;
        VkDescriptorPool m_CurrentPool{ VK_NULL_HANDLE };
        std::vector<VkDescriptorPool> m_UsedPools;
        std::vector<VkDescriptorPool> m_FreePools;
        std::vector<VkDescriptorPoolSize> m_PoolSizes{};
        VkDescriptorPool CreatePool(VkDevice device, int count, VkDescriptorPoolCreateFlags flags);
        VkDevice m_Device;
        uint32_t m_MaxSets;
        uint32_t m_PoolFlags;

        VkSampler m_FontSampler;
        VkDescriptorSetLayout m_TextureLayout;
        void InitTextureLayout();
        VkDescriptorPool GrabPool() {
            //there are reusable pools availible
            if (m_FreePools.size() > 0) {
                //grab pool from the back of the vector and remove it from there.
                VkDescriptorPool pool = m_FreePools.back();
                m_FreePools.pop_back();
                return pool;
            }
            else {
                //no pools availible, so create a new one
                return CreatePool(m_Device, m_MaxSets, m_PoolFlags);
            }
        }
        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorWriter {
    public:
        VulkanDescriptorWriter(VulkanDescriptorSet* setLayout, Count<VulkanDescriptorPool> pool);

        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo& bufferInfo);
        VulkanDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo& imageInfo);
        VulkanDescriptorWriter& WriteImage(uint32_t binding, Count< std::vector< VkDescriptorImageInfo>> imageInfo);

        bool Build(VkDescriptorSet& set);
        void Overwrite(VkDescriptorSet& set);
        std::vector<VkWriteDescriptorSet> writes;

    private:
        std::vector< Count< std::vector< VkDescriptorImageInfo>>> m_ImageInfos;
        VulkanDescriptorSet* m_SetLayout;
        Count<VulkanDescriptorPool> m_Pool;
        friend class VulkanRenderer;
    };
	



}
