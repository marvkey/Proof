#pragma once
#include "Proof/Core/Core.h"
#include"Proof/Math/Vector.h"
#include <glm/glm.hpp>
namespace Proof{

	struct RenderMaterialConfiguration
	{
		std::string DebugName;
		Count<class Shader> Shader;
	};
	class RenderMaterial : public RefCounted
	{
	public:virtual void Set(std::string_view name, Count<class UniformBuffer> buffer0) = 0;
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

		  virtual void Set(const std::string& name, float value) = 0;
		  virtual void Set(const std::string& name, bool value) = 0;
		  virtual void Set(const std::string& name, int value) = 0;
		  virtual void Set(const std::string& name, uint32_t value) = 0;
		  virtual void Set(const std::string& name, const Vector2I& value) = 0;
		  virtual void Set(const std::string& name, const VectorI& value) = 0;
		  virtual void Set(const std::string& name, const Vector4I& value) = 0;
		  virtual void Set(const std::string& name, const Vector2& value) = 0;
		  virtual void Set(const std::string& name, const Vector& value) = 0;
		  virtual void Set(const std::string& name, const Vector4& value) = 0;
		  virtual void Set(const std::string& name, const glm::mat3& value) = 0;
		  virtual void Set(const std::string& name, const glm::mat4& value) = 0;

		  virtual float& GetFloat(const std::string& name) = 0;
		  virtual int& GetInt(const std::string& name) = 0;
		  virtual bool& GetBool(const std::string& name) = 0;
		  virtual uint32_t& GetUint32(const std::string& name) = 0;
		  virtual Vector2I& GetVector2I(const std::string& name) = 0;
		  virtual VectorI& GetVectorI(const std::string& name) = 0;
		  virtual Vector4I& GetVector4I(const std::string& name) = 0;
		  virtual Vector2& GetVector2(const std::string& name) = 0;
		  virtual Vector& GetVector(const std::string& name) = 0;
		  virtual Vector4& GetVector4(const std::string& name) = 0;
		  virtual glm::mat3& GetMatrix3(const std::string& name) = 0;
		  virtual glm::mat4& GetMatrix4(const std::string& name) = 0;

		  virtual Count<class Texture2D> TryGetTexture2D(std::string_view name) = 0;
		  virtual const RenderMaterialConfiguration& GetConfig()const = 0;
		static Count<RenderMaterial> Create(const RenderMaterialConfiguration&config);
	};
}
