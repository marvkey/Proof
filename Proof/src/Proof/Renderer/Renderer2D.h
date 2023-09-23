#pragma once
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include <array>
#include "vertex.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Texture.h"
#include "UniformBuffer.h"
namespace Proof {

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		float Thickness;
		glm::vec2 LocalPosition;
		glm::vec4 Color;
	};
	struct TextVertex 
	{
		glm::vec3 Positon;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		// we can render multiple fonts so that means multiple textures (0,32)

		//(tex background)
		float TexIndex;
	};
	struct SpriteComponent;
	struct TransformComponent;
	struct Renderer2DStorage {
		Count<UniformBufferSet> CameraBuffer = nullptr;
		Count<class VertexBuffer> VertexBuffer;
		Count<class IndexBuffer> IndexBuffer;	
		Count<Texture2D> WhiteTexture;
		const uint32_t c_MaxQuadCount = 10000;
		const uint32_t c_MaxVertexCount = c_MaxQuadCount *4; // times 4 cause each quad holds 4 vertices
		const uint32_t c_MaxIndexCount = c_MaxQuadCount * 6;
		std::vector<uint32_t>QuadIndices;
		uint32_t IndexCount = 0;

		const uint32_t c_MaxTextureSlot=32; // 1-31 slots
		std::vector<Count<Texture2D>> Textures;
		float TextureSlotIndex = 1; //0 white texture
		uint32_t QuadArraySize = 0;
		std::vector<Vertex2D> QuadArray;

		Count<class VertexBuffer> TextVertexBuffer;
		uint32_t TextIndexCount = 0;
		uint32_t TextArraySize= 0;
		std::vector<TextVertex> TextArray;

		Count<Texture2D> FontTexture;
		Renderer2DStorage();
	};
	struct Rendered2DStatistics
	{
		float QuadDrawTime = 0.0f;
		float TextDrawTime = 0.0f;
		float TotalRenderTime = 0.0f;
	};
	class Renderer2D : RefCounted {
		friend class Camera;
	public:
		Renderer2D();
		~Renderer2D();
		void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position);
		void DrawQuad(const glm::vec3& Location);
		void DrawQuad(const glm::vec3& Location,const glm::vec3& Size);
		void DrawQuad(const glm::vec3& Location,const glm::vec3& RotationRadians,const glm::vec4& Color);
		void DrawQuad(const glm::vec3& Location, const glm::vec4& Color);
		void DrawQuad(const glm::vec3& Location, Count<class Texture2D> texture);
		void DrawQuad(const glm::vec3& Location, const glm::vec4& TintColor,Count<Texture2D> texture);
		void DrawQuad(const glm::vec3& Location, const glm::vec3& Size, const glm::vec4& TintColor, Count<Texture2D>& texture);
		void DrawQuad(const glm::vec3& Location,const glm::vec3& RotationRadians,const glm::vec3& Size,const glm::vec4& Color);

		void DrawQuad(const glm::vec3& Location, const glm::vec3& RotationRadians, const glm::vec3& Size, const glm::vec4& Color, const Count<Texture2D>& texture2D);
		void DrawQuad(const glm::mat4& transform, const glm::vec4& Color, const Count<Texture2D>& texture2D);
		void DrawQuad(SpriteComponent& Sprite, const TransformComponent& transform);

		// chagne to u32stirng in the future
		void DrawString(const std::string& text, Count<class Font> font,const TextParams& textparams, const glm::mat4& transform);
		void EndContext();
		static std::vector<Vertex2D> CreateQuad(const glm::vec3& Location, const glm::vec3& RotationRadians, const glm::vec3& Scale, const glm::vec4& Color, float TexIndex);

		static std::pair<Count<VertexBuffer>, Count<IndexBuffer>> CreateQuad();

		void SetTargetFrameBuffer(Count<class FrameBuffer> framebuffer);
		Count<class FrameBuffer> GetTargetFrameBuffer() {
			return m_FrameBuffer;
		};
		Rendered2DStatistics GetStats() { return m_Stats; }
	private:
		Count<class RenderCommandBuffer> m_CommandBuffer;
		Count<FrameBuffer> m_FrameBuffer;
		Rendered2DStatistics m_Stats;
		Count<class RenderPass> m_TextPass;

		void Render();
		void Reset();
		void Init();

		Count<class Texture2D> m_WhiteTexture;

		Count<class IndexBuffer> m_IndexBuffer;
		glm::vec4 m_QuadVertexPositions[4];
		const uint32_t c_MaxQuadCount = 10000;
		const uint32_t c_MaxVertexCount = c_MaxQuadCount * 4; // times 4 cause each quad holds 4 vertices
		const uint32_t c_MaxIndexCount = c_MaxQuadCount * 6;

		static inline const uint32_t c_MaxTextureSlots = 32; //1-31slots

		std::array<Count<Texture2D>, c_MaxTextureSlots> m_QuadTextures;
		Count<class VertexBuffer> m_QuadVertexBuffer;
		float m_QuadTextureSlotIndex = 1; //0 white texture
		Count<class RenderPass> m_QuadPass;

		uint32_t m_QuadIndexCount = 0;
		Vertex2D* m_QuadVertexBufferBase = nullptr;
		Vertex2D* m_QuadVertexBufferPtr = nullptr;
		Count<UniformBufferSet> m_UBCamera = nullptr;

	};
}

