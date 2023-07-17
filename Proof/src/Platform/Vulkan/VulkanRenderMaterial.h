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
		virtual ~VulkanRenderMaterial(){Release(); }
		virtual void Set(std::string_view name, Count<class UniformBuffer> buffer);
		virtual void Set(std::string_view name, Count<class StorageBuffer> buffer);
		virtual void Set(std::string_view name, Count<class Image2D> image);
		virtual void Set(std::string_view name, Count<class Texture2D> texture);
		virtual void Set(std::string_view name, const std::vector< Count<class Texture2D>>& images);

		virtual void Set(const std::string& name, float value);
		virtual void Set(const std::string& name, bool value);
		virtual void Set(const std::string& name, int value);
		virtual void Set(const std::string& name, uint32_t value);

		virtual void Set(const std::string& name, const Vector2I& value);
		virtual void Set(const std::string& name, const VectorI& value);
		virtual void Set(const std::string& name, const Vector4I& value);
		
		virtual void Set(const std::string& name, const Vector2& value);
		virtual void Set(const std::string& name, const Vector& value);
		virtual void Set(const std::string& name, const Vector4& value);

		virtual void Set(const std::string& name, const glm::mat3& value);
		virtual void Set(const std::string& name, const glm::mat4& value);
	
		float& GetFloat(const std::string& name);
		bool& GetBool(const std::string& name);
		int& GetInt(const std::string& name);
		uint32_t& GetUint32(const std::string& name);
		Vector2I& GetVector2I(const std::string& name) ;
		VectorI& GetVectorI(const std::string& name);
		Vector4I& GetVector4I(const std::string& name) ;
		Vector2& GetVector2(const std::string& name) ;
		Vector& GetVector(const std::string& name) ;
		Vector4& GetVector4(const std::string& name) ;
		glm::mat3& GetMatrix3(const std::string& name) ;
		glm::mat4& GetMatrix4(const std::string& name) ;

		Count<class Texture2D> TryGetTexture2D(std::string_view name);
		Count<class Image2D> TryGetImage2D(std::string_view name);
		virtual const RenderMaterialConfiguration& GetConfig()const { return m_Config; };

		void Bind(Count < class VulkanRenderCommandBuffer > render, Count<class VulkanComputePass> pass);
		void Bind(Count < class VulkanRenderCommandBuffer > render, Count<class VulkanRenderPass> pass);

	private:
		template<typename T>
		void SetInternal(const std::string& name, T& value)
		{
			const ShaderResourceBufferInfo* decl = FindUniformDeclaration(name);
			PF_CORE_ASSERT(decl, "Could not Find Storage!");

			if (!decl)return;

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

		const ShaderResourceBufferInfo* FindUniformDeclaration(const std::string& name);
		void Build();
		void Release();
	};
}
