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
	struct TextVertex {
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
		Count<UniformBuffer> CameraBuffer = nullptr;
		Count<class VertexBuffer> VertexBuffer;
		Count<class IndexBuffer> IndexBuffer;	
		Count<class RenderCommandBuffer>CommandBuffer;
		Count<ScreenFrameBuffer> CurrentFrameBuffer ;
		Count<Texture2D> WhiteTexture;
		const uint32_t c_MaxQuadCount = 2000;
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

	struct SpritePipeline {
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
		Count <class PipeLineLayout> PipeLineLayout;
		SpritePipeline(Count <class RenderPass > renderPass);
		SpritePipeline(Count <class RenderPass > renderPass, const std::string& shaderPath);
	};

	struct TextPipeline {
		Count<class GraphicsPipeline> GraphicsPipeline;
		Count<class Shader> Shader;
		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
		Count <class PipeLineLayout> PipeLineLayout;
		TextPipeline(Count <class RenderPass > renderPass);
	};
	
	class Renderer2D {
		friend class Camera;
	public:
		Renderer2D(Count <class RenderPass > renderPass, bool screenSpace = false);
		Renderer2D(Count <class RenderPass > renderPass, const std::string& spriteRenderShaderPath);
		~Renderer2D() {};
		void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commdandBuffer);
		void DrawQuad(const glm::vec3& Location);
		void DrawQuad(const glm::vec3& Location,const glm::vec3& Size);
		void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec4& Color);
		void DrawQuad(const glm::vec3& Location, const glm::vec4& Color);
		void DrawQuad(const glm::vec3& Location, Count<class Texture2D> texture);
		void DrawQuad(const glm::vec3& Location, const glm::vec4& TintColor,Count<Texture2D> texture);
		void DrawQuad(const glm::vec3& Location, const glm::vec3& Size, const glm::vec4& TintColor, Count<Texture2D>& texture);
		void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color);

		void DrawQuad(const glm::vec3& Location, const glm::vec3& Rotation, const glm::vec3& Size, const glm::vec4& Color, const Count<Texture2D>& texture2D);
		void DrawQuad(SpriteComponent& Sprite, const TransformComponent& transform);

		// chagne to u32stirng in the future
		void DrawString(const std::string& text, Count<class Font> font,const TextParams& textparams, const glm::mat4& transform);
		void EndContext();
		static std::vector<Vertex2D> CreateQuad(const glm::vec3& Location, const glm::vec3& Rotation, const glm::vec3& Scale, const glm::vec4& Color, float TexIndex);

		static std::pair<Count<VertexBuffer>, Count<IndexBuffer>> CreateQuad();

		struct Renderer2DStats{
			uint32_t m_QuadCount;
			uint32_t m_DrawCalls;
		};
	private:
		bool m_ScreenSpace = false;
		void Render();
		void Reset();
		void Init();
		Special<SpritePipeline> m_SpritePipeline;
		Special<TextPipeline> m_TextPipeline;
		Count <class RenderPass > m_RenderPass;
		Special< Renderer2DStorage> m_Storage2DData;
		/* Not using as default rendeer cause it allocates to the heap and we dont need taht waste in performance */
	};
}

