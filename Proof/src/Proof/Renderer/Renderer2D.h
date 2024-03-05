#pragma once
#include "Proof/Math/Math.h"
#include "glm/glm.hpp"
#include <array>
#include "vertex.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "RenderereCustomTypeSet.h"
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
		float FontIndex;
		// we can render multiple fonts so that means multiple textures (0,32)

		//(tex background)
		float TexIndex;
	};
	struct SpriteComponent;
	struct TransformComponent;
	struct AABB;
	class VertexBufferSet;

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

	struct Renderer2DContextSettings
	{
		bool RenderOnTop = false;
	};
	class Renderer2D : RefCounted {
		friend class Camera;
	public:
		Renderer2D();
		~Renderer2D();
		void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Renderer2DContextSettings settigs = Renderer2DContextSettings());
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

		void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color = glm::vec4(1.0f));
		void DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		void DrawAABB(Count<class MeshBase> mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		void DrawAABB(const class SubMesh& subMesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		void DrawAABBSubMeshes(Count<class MeshBase> mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

		void DrawCylinder(glm::vec3 position, glm::vec3 rotationRadians, float height, float radius, glm::vec4 color = glm::vec4(1.0f), bool drawFromBase = true);
		void DrawCapsule(glm::vec3 position, glm::vec3 rotationRadian, float height, float radius, glm::vec4 color = glm::vec4(1.0f));

		void DrawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec4& color = glm::vec4(1.0f));
		//lenght is gottenform the direction
		void DrawRay(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color = glm::vec4(1.0f));

		void DrawArc(float startAngleRadians, float endAngleRadians, glm::vec3 position, glm::vec3 rotationRadians, float radius, glm::vec4 color = glm::vec4(1.0f), bool drawChord = false, bool drawSector = false,uint32_t arcSegments = 32);
		void DrawCircle(const glm::vec3& p0, const glm::vec3& rotation, float radius, const glm::vec4& color);
		void DrawCircle(const glm::mat4& transform, const glm::vec4& color);

		//not working
		void DrawHalfCircle(const glm::vec3& p0, const glm::vec3& rotation, float radius, const glm::vec4& color);
		//not working
		void DrawHalfCircle(const glm::mat4& transform, const glm::vec4& color);
		//not working
		void DrawSemiCircle(const glm::mat4& transform, const glm::vec4& color);
		//not working
		void DrawSemiCircle(const glm::vec3& p0, const glm::vec3& rotation, float radius, const glm::vec4& color);
		//not working
		void DrawDebugHemisphere(const glm::vec3& position, const glm::vec3& rotation, float radius, const glm::vec4& color = glm::vec4(1.0f));

		void DrawDebugSphere(const glm::vec3& position, const glm::vec3& rotation, float radius, const glm::vec4& color = glm::vec4(1.0f));
		void DrawDebugCube(const glm::vec3& position, const glm::vec3& rotation, glm::vec3 size = glm::vec3(1), const glm::vec4& color = glm::vec4(1.0f));
		//void DrawDebugHemisphereFlatBase(const glm::vec3& position, const glm::vec3& rotation, float radius, const glm::vec4& color = glm::vec4(1.0f));
		
		// Thickness is between 0 and 1
		void FillCircle(const glm::vec2& p0, float radius, const glm::vec4& color, float thickness = 0.05f);
		void FillCircle(const glm::vec3& p0, float radius, const glm::vec4& color, float thickness = 0.05f);

		void DrawQuadBillboard(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0,0,0),const glm::vec2& size = glm::vec2(1), const glm::vec4& color = glm::vec4(1));
		void DrawQuadBillboard(const Count<class Texture2D>& texture, glm::vec3 position, const glm::vec3& rotation = glm::vec3(0, 0, 0), const glm::vec2& size = glm::vec2(1), const glm::vec4& tintColor = glm::vec4(1), float tilingFactor = 1.0f);
		void DrawCameraFrustrum(const glm::mat4& viewProjection, const glm::vec4& color);

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
		CameraData m_Camera;
		Count<class RenderCommandBuffer> m_CommandBuffer;
		Count<FrameBuffer> m_FrameBuffer;
		Rendered2DStatistics m_Stats;

		void Render();
		void Reset();
		void Init();

		Count<class Texture2D> m_WhiteTexture;

		Count<class IndexBuffer> m_IndexBuffer;
		glm::vec4 m_QuadVertexPositions[4];
		const uint32_t c_MaxQuadCount = 10000;
		const uint32_t c_MaxVertexCount = c_MaxQuadCount * 4; // times 4 cause each quad holds 4 vertices
		const uint32_t c_MaxIndexCount = c_MaxQuadCount * 6;

		const uint32_t c_MaxLines = 2000;
		const uint32_t c_MaxLineVertices = c_MaxLines * 2;
		const uint32_t c_MaxLineIndices = c_MaxLines * 6;
		static inline const uint32_t c_MaxTextureSlots = 32; //1-31slots

		std::array<Count<Texture2D>, c_MaxTextureSlots> m_QuadTextures;
		Count<class VertexBufferSet> m_QuadVertexBuffer;
		float m_QuadTextureSlotIndex = 1; //0 white texture
		Count<class RenderPass> m_QuadPass;
		Count<class RenderPass> m_QuadOnTopPass;

		uint32_t m_QuadIndexCount = 0;
		RendererCustomTypeSet<Vertex2D*> m_QuadVertexBufferBase = nullptr;
		Vertex2D* m_QuadVertexBufferPtr = nullptr;

		uint32_t m_TextIndexCount = 0;
		RendererCustomTypeSet<TextVertex*> m_TextVertexBufferBase = nullptr;
		TextVertex* m_TextVertexBufferPtr = nullptr;
		Count<class RenderPass> m_TextPass;
		Count<class RenderPass> m_TextOnTopPass;
		std::array<Count<Font>, c_MaxTextureSlots> m_FontTextures;
		Count<VertexBufferSet> m_TextVertexBuffer;
		float m_TextFontSlotIndex = 1; //0 default font

		Count<RenderPass> m_LinePass;
		Count<RenderPass> m_LineOnTopPass;
		Count<VertexBufferSet> m_LineVertexBuffer;
		Count<IndexBuffer> m_LineIndexBuffer;
		uint32_t m_LineIndexCount = 0;
		RendererCustomTypeSet<LineVertex*> m_LineVertexBufferBase;
		LineVertex* m_LineVertexBufferPtr;

		Count<RenderPass> m_CircleRenderPass;
		Count<RenderPass> m_CircleOnTopRenderPass;
		Count<VertexBufferSet> m_CircleVertexBuffer;
		uint32_t m_CircleIndexCount = 0;
		RendererCustomTypeSet<CircleVertex*> m_CircleVertexBufferBase = nullptr;
		CircleVertex* m_CircleVertexBufferPtr = nullptr;

		Count<UniformBufferSet> m_UBCamera = nullptr;
		Renderer2DContextSettings m_ContextSettings;
	};
}

