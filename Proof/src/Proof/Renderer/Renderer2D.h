#pragma once
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include <array>
#include "vertex.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Texture.h"
#include "UniformBuffer.h"
namespace Proof {
	struct SpriteComponent;
	struct TransformComponent;
	struct Proof_API Renderer2DStorage {
		Count<UniformBuffer> CameraBuffer = nullptr;
		Count<class VertexBuffer> VertexBuffer;
		Count<class IndexBuffer> IndexBuffer;
		Count<class CommandBuffer>CommandBuffer;
		Count<ScreenFrameBuffer> CurrentFrameBuffer ;
		const uint32_t c_MaxQuadCount = 2000;
		const uint32_t c_MaxVertexCount = c_MaxQuadCount * sizeof(Vertex2D)*4; // times 4 cause each quad holds 4 vertices
		const uint32_t c_MaxIndexCount = c_MaxQuadCount * 6;
		std::vector<uint32_t>QuadIndices;
		uint32_t IndexCount = 0;

		const uint32_t MaxTextureSlot=32; // 1-31 slots
		std::vector<Count<Texture2D>> Textures;
		float TextureSlotIndex;
		uint32_t QuadArraySize;
		std::vector<Vertex2D> QuadArray;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		Renderer2DStorage();
	};

	struct SpritePipeline {
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		Count <class PipeLineLayout> PipeLineLayout;
		Count <class RenderPass > RenderPass;
		SpritePipeline();
	};


	class Proof_API Renderer2D {
		friend class Camera;
	public:
		static void Init();
		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer> frameBuffer);

		static void DrawQuad(const glm::vec3& Location);

		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Size);
		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec4& Color);
		static void DrawQuad(const glm::vec3& Location,const glm::vec4& Color);

		static void DrawQuad(const glm::vec3& Location, Count<Texture2D> texture);
		static void DrawQuad(const glm::vec3& Location, const glm::vec4& TintColor,Count<Texture2D> texture);
		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Size,const glm::vec4& TintColor,Count<Texture2D>& texture);

		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color);
		
		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color,const Count<Texture2D>&texture2D);
		static void DrawQuad(SpriteComponent& Sprite, const TransformComponent& transform);
		static void EndContext();

		struct Renderer2DStats{
			uint32_t m_QuadCount;
			uint32_t m_DrawCalls;
		};
		static void Reset();
	private:
		static void InitDescriptors();
		/* Not using as default rendeer cause it allocates to the heap and we dont need taht waste in performance */
		static std::vector<Vertex2D> CreateQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Scale,const glm::vec4& Color,float TexIndex);
		static void Render();
	};
}

