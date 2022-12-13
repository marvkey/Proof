#pragma once
#include <vulkan/vulkan.h>
#include "Proof/Renderer/Renderer.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
    /*
        Set=0 descriptor set containing uniform buffer with global, per-frame or per-view data, as well as globally available textures such as shadow map texture array/atlas
        Set=1 descriptor set containing uniform buffer and texture descriptors for per-material data, such as albedo map, Fresnel coefficients, etc.
        Set=2 descriptor set containing dynamic uniform buffer with per-draw data, such as world transform array
    */
    enum class DescriptorSets {
        Zero = 0,
        One,
        Two
    };
    enum class DescriptorSet0 {
        //struct
        CameraData = 0,
        //struct
        WorldData = 1,

    };

    enum class DescriptorSet1 {
        //texture
        AlbedoMap = 0,
        //texture
        NormalMap = 1,
        //texture
        metallicMap = 2,
        //texture
        roughnessMap = 3,
        //texture
        DiffuseMap = 4
    };

    enum class DescriptorSet2 {

    };
    class VulkanDescriptorSet {
    public:
        struct Builder {
        public:
            Builder(DescriptorSets set) {
                m_Set = set;
            }
            Builder& AddBinding(uint32_t binding,VkDescriptorType descriptorType,VkShaderStageFlags stageFlags,uint32_t count = 1);
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

        VulkanDescriptorSet& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorSet& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool Build(int frame = Renderer::GetCurrentFrame().FrameinFlight);
        void Overwrite(int frame = Renderer::GetCurrentFrame().FrameinFlight);

        DescriptorSets SetIndex() {
            return m_Set;
        }
        friend class VulkanPipeLineLayout;
    private:
        VkDescriptorSetLayout m_DescriptorSetLayout;
        Special<class VulkanDescriptorWriter> m_Writer;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
        std::vector<VkDescriptorSet	> m_DescriptorSets;
        DescriptorSets m_Set;

        friend class VulkanDescriptorWriter;
    };

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

        void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

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

        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool Build(VkDescriptorSet& set);
        void Overwrite(VkDescriptorSet& set);
        std::vector<VkWriteDescriptorSet> writes;

    private:
        VulkanDescriptorSet* m_SetLayout;
        Count<VulkanDescriptorPool> m_Pool;
        friend class VulkanRenderer;
    };
  
	class VulkanUniformBuffer {
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t set, uint32_t binding);
		virtual ~VulkanUniformBuffer();
        VkBuffer GetBuffer(int index) {
            return m_UniformBuffers[index].Buffer;
        }
        VkDescriptorBufferInfo GetDescriptorInfo(int index) {
            return {
                m_UniformBuffers[index].Buffer,
                0,
                m_Size,
            };
        }
		void SetData(const void* data, uint32_t size, uint32_t offset = 0, uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
	private:
        // multiple of this cause of frames in flight
        // we do not want to right to a uniform for the next frame
        // while a uniform is still being read by the gpu on the current frame
		std::vector<VulkanBuffer> m_UniformBuffers;

        uint32_t m_Size = 0;
        uint32_t m_Set;
        uint32_t m_Binding;
	};

}
