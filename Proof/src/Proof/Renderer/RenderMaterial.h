#pragma once
#include "Proof/Core/Core.h"
#include"Proof/Math/Vector.h"
#include <glm/glm.hpp>
namespace Proof {

	struct RenderMaterialConfiguration
	{
		std::string DebugName;
		Count<class Shader> Shader;
	};
	class RenderMaterial : public RefCounted
	{
	public:
		virtual void Set(std::string_view name, Count<class UniformBuffer> buffer0) = 0;
		virtual void Set(std::string_view name, Count<class StorageBuffer> buffer) = 0;
		virtual void Set(std::string_view name, Count<class UniformBufferSet> buffer) = 0;
		virtual void Set(std::string_view name, Count<class StorageBufferSet> buffer) = 0;
		virtual void Set(std::string_view name, Count<class Texture2D> texture) = 0;
		virtual void Set(std::string_view name, Count<class TextureCube> texture) = 0;
		virtual void Set(std::string_view name, const std::vector<Count<class Texture2D>>& images) = 0;
		void virtual Set(std::string_view name, Count<class ImageView> imageView) = 0;
		void virtual Set(std::string_view name, const std::vector< Count<class ImageView>>& imageViews) = 0;
		void virtual Set(std::string_view name, Count<class Image2D>image) = 0;
		void virtual Set(std::string_view name, const std::vector< Count<class Image2D>>& images) = 0;

		void virtual Set(std::string_view name, Count<class Image>image) = 0;
		void virtual Set(std::string_view name, const std::vector< Count<class Image>>& images) = 0;

		virtual void Set(const std::string& name, float value) = 0;
		virtual void Set(const std::string& name, bool value) = 0;
		virtual void Set(const std::string& name, int value) = 0;
		virtual void Set(const std::string& name, uint32_t value) = 0;
		virtual void Set(const std::string& name, const glm::ivec2& value) = 0;
		virtual void Set(const std::string& name, const glm::ivec3& value) = 0;
		virtual void Set(const std::string& name, const glm::ivec4& value) = 0;
		virtual void Set(const std::string& name, const glm::vec2& value) = 0;
		virtual void Set(const std::string& name, const glm::vec3& value) = 0;
		virtual void Set(const std::string& name, const glm::vec4& value) = 0;
		virtual void Set(const std::string& name, const glm::mat3& value) = 0;
		virtual void Set(const std::string& name, const glm::mat4& value) = 0;

		virtual float& GetFloat(const std::string& name) = 0;
		virtual int& GetInt(const std::string& name) = 0;
		virtual bool& GetBool(const std::string& name) = 0;
		virtual uint32_t& GetUint32(const std::string& name) = 0;
		virtual glm::ivec2& GetVector2I(const std::string& name) = 0;
		virtual glm::ivec3& GetVectorI(const std::string& name) = 0;
		virtual glm::ivec4& GetVector4I(const std::string& name) = 0;
		virtual glm::vec2& GetVector2(const std::string& name) = 0;
		virtual glm::vec3& GetVector(const std::string& name) = 0;
		virtual glm::vec4& GetVector4(const std::string& name) = 0;
		virtual glm::mat3& GetMatrix3(const std::string& name) = 0;
		virtual glm::mat4& GetMatrix4(const std::string& name) = 0;

		virtual Count<class Texture2D> TryGetTexture2D(std::string_view name) = 0;
		virtual const RenderMaterialConfiguration& GetConfig()const = 0;
		static Count<RenderMaterial> Create(const RenderMaterialConfiguration& config);
		static Count<RenderMaterial> Create(const std::string& debugame, Count<class Shader> Shader );
	};
}
