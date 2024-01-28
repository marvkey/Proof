#pragma once
#include "Proof/Renderer/RenderMaterial.h"
#include "VulkanDescriptorManager.h"
#include "Proof/Core/Buffer.h"

namespace Proof
{
	// always at set 0 for descriptor
	class VulkanRenderMaterial : public RenderMaterial
	{
	public:

		VulkanRenderMaterial(const RenderMaterialConfiguration& config);
		VulkanRenderMaterial(const VulkanRenderMaterial& other);
		virtual ~VulkanRenderMaterial(){Release(); }
		virtual void Set(std::string_view name, Count<class UniformBuffer> buffer);
		virtual void Set(std::string_view name, Count<class StorageBuffer> buffer);
		virtual void Set(std::string_view name, Count<class UniformBufferSet> buffer);
		virtual void Set(std::string_view name, Count<class StorageBufferSet> buffer);

		virtual void Set(std::string_view name, Count<class Texture2D> texture);
		virtual void Set(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		virtual void Set(std::string_view name, Count<class TextureCube> texture);

		void virtual Set(std::string_view name, Count<class ImageView> imageView);
		void virtual Set(std::string_view name, const std::vector< Count<class ImageView>>& imageViews);
		void virtual Set(std::string_view name, Count<class Image2D>image);
		void virtual Set(std::string_view name, const std::vector< Count<class Image2D>>& images);

		void virtual Set(std::string_view name, Count<class Image>image);
		void virtual Set(std::string_view name, const std::vector< Count<class Image>>& images);

		virtual void Set(const std::string& name, float value);
		virtual void Set(const std::string& name, bool value);
		virtual void Set(const std::string& name, int value);
		virtual void Set(const std::string& name, uint32_t value);

		virtual void Set(const std::string& name, const glm::ivec2& value);
		virtual void Set(const std::string& name, const glm::ivec3& value);
		virtual void Set(const std::string& name, const glm::ivec4& value);
		
		virtual void Set(const std::string& name, const glm::vec2& value);
		virtual void Set(const std::string& name, const glm::vec3& value);
		virtual void Set(const std::string& name, const glm::vec4& value);

		virtual void Set(const std::string& name, const glm::mat3& value);
		virtual void Set(const std::string& name, const glm::mat4& value);
	
		float& GetFloat(const std::string& name);
		bool& GetBool(const std::string& name);
		int& GetInt(const std::string& name);
		uint32_t& GetUint32(const std::string& name);
		glm::ivec2& GetVector2I(const std::string& name) ;
		glm::ivec3& GetVectorI(const std::string& name);
		glm::ivec4& GetVector4I(const std::string& name) ;
		glm::vec2& GetVector2(const std::string& name) ;
		glm::vec3& GetVector(const std::string& name) ;
		glm::vec4& GetVector4(const std::string& name) ;
		glm::mat3& GetMatrix3(const std::string& name) ;
		glm::mat4& GetMatrix4(const std::string& name) ;

		Count<class Texture2D> TryGetTexture2D(std::string_view name);
		Count<class Image2D> TryGetImage2D(std::string_view name);
		virtual const RenderMaterialConfiguration& GetConfig()const { return m_Config; };

		void RT_Bind(Count < class VulkanRenderCommandBuffer > render, Count<class VulkanComputePass> pass);
		void RT_Bind(Count < class VulkanRenderCommandBuffer > render, Count<class VulkanRenderPass> pass);

	private:
		template<typename T>
		void SetInternal(const std::string& name, T& value)
		{
			const ShaderResourceBufferInfo* decl = FindUniformDeclaration(name);
			PF_CORE_ASSERT(decl, fmt::format("Could not Find Storage! {}",name));

			if (!decl)return;

			{
				// cannot work because material sets default material and basically
				// when we load a matrial actually date from diskk when we do htis and push to render thread
				// that data is then overriden

				//Buffer data(&value, decl->Size);
				//SetBufferData(data, decl->Offset);
			}
			auto& buffer = m_UniformBufferStorage;
			buffer.SetData(&value, decl->Size, decl->Offset);
		};

		template<typename T>
		T& GetInternal(const std::string& name)
		{
			const ShaderResourceBufferInfo* decl = FindUniformDeclaration(name);
			PF_CORE_ASSERT(decl, "Could not Find Storage!");
			auto& buffer = m_UniformBufferStorage;
			return buffer.Read<T>(decl->Offset);
		}
		Buffer m_UniformBufferStorage;
		RenderMaterialConfiguration m_Config;
		Count< VulkanDescriptorManager> m_DescritptorSetManager;
		std::string m_UniformName;
		const ShaderResourceBufferInfo* FindUniformDeclaration(const std::string& name);
		void Build();
		void Release();

		//pushes onto render thread
		void SetBufferData(Buffer data, uint32_t offset);
		void RT_SetBufferData(Buffer data, uint32_t offset);
	};
}
