#pragma once
#include "Proof3D/Math/Math.h"
#include "glm/glm.hpp"
#include <array>
#include "Proof3D/Scene/Vertex.h"

namespace Proof {
	struct Proof_API Renderer2DStorage {
		Count<class VertexArray> m_VertexArray;
		Count<class Shader> m_Shader;
		std::vector<struct Vertex2D> Vertices;
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class IndexBuffer> m_IndexBuffer;
		Count<class Texture2D>WhiteTexture;
		static const uint32_t MaxQuadCount = 2000;
		static const uint32_t MaxVertexCount = MaxQuadCount * sizeof(Vertex2D)*4;
		static const uint32_t MaxIndexCount = MaxQuadCount * 6;
		static std::array<uint32_t,MaxIndexCount>QuadIndices;
		Count<class VertexBuffer>m_BaseVertexBuffer;
		uint32_t m_IndexCount = 0;
		uint32_t m_DrawCalls = 0;
		uint32_t m_NumQuads = 0;
		static const uint32_t MaxTextureSlot=32; // 1-31 slots
		std::array<Count<Texture2D>,MaxTextureSlot> Textures;
		float TextureSlotIndex;
	};

	class Proof_API Renderer2D {
		friend class Camera;
	public:
		static void Init();
		static void BeginContext(const glm::mat4& Projection,const Camera& camera);
		static void BeginContext(const class OrthagraphicCamera& Camera);
		static void DrawQuad(const glm::vec3& Location);

		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Size);
		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec4& Color);
		static void DrawQuad(const glm::vec3& Location,const glm::vec4& Color);

		static void DrawQuad(const glm::vec3& Location, Count<Texture2D> texture);
		static void DrawQuad(const glm::vec3& Location, const glm::vec4& TintColor,Count<Texture2D> texture);
		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Size,const glm::vec4& TintColor,Count<Texture2D> texture);

		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color);
		
		static void DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color,const Count<Texture2D> texture2D);
		static void EndContext();
	private:
		static std::vector<Vertex2D> CreateQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Scale,const glm::vec4& Color,float TexIndex);
		static void Reset();
		static void Render();
		static void Start();
	};
}

