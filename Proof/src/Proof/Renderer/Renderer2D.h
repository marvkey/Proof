#pragma once
#include "Proof3D/Math/Math.h"
#include "glm/glm.hpp"
#include <array>
#include "Proof3D/Scene/Vertex.h"

namespace Proof {
	struct Proof_API Renderer2DStorage {
		Count<class VertexArray> m_VertexArray;
		Count<class Shader> m_Shader;
		Count<class VertexBuffer> m_VertexBuffer;
		Count<class IndexBuffer> m_IndexBuffer;
		Count<class Texture2D>m_WhiteTexture;

		static const uint32_t s_MaxQuadCount = 2000;
		static const uint32_t s_MaxVertexCount = s_MaxQuadCount * sizeof(Vertex2D)*4; // times 4 cause each quad holds 4 vertices
		static const uint32_t s_MaxIndexCount = s_MaxQuadCount * 6;
		static std::array<uint32_t,s_MaxIndexCount>QuadIndices;
		uint32_t m_IndexCount = 0;

		static const uint32_t MaxTextureSlot=32; // 1-31 slots
		std::array<Count<Texture2D>,MaxTextureSlot> m_Textures;
		float m_TextureSlotIndex;
		uint32_t m_QuadArraySize;
		Vertex2D m_QuadArray[s_MaxVertexCount];
	};

	class Proof_API Renderer2D {
		friend class Camera;
	public:
		static void Init();
		static void BeginContext(const glm::mat4& Projection,const Camera& camera);
		static void BeginContext(glm::mat4 Projection,class EditorCamera3D& EditorCamera);

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

		struct Renderer2DStats{
			static uint32_t m_QuadCount;
			static uint32_t m_DrawCalls;
		};
		static void Reset();
	private:
		/* Not using as default rendeer cause it allocates to the heap and we dont need taht waste in performance */
		static std::vector<Vertex2D> CreateQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Scale,const glm::vec4& Color,float TexIndex);
		static void Render();
	};
}

