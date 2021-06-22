#pragma once
#include "glm/glm.hpp"
#include "Proof3D/Renderer/Camera/Camera.h"
#include "Proof3D/Math/Math.h"

#include "Proof3D/Scene/Vertex.h"
struct StaticMeshComponent;
namespace Proof {
	struct BatchRenderer3D;
	class Proof_API Renderer3D {
	public:
		static void Init();
		static void ShutDown();
		static void BeginContext(const glm::mat4& Projection,const Camera& camera);
		static void DrawCube(const Vector& Location,const Vector& Scale);
		static void DrawCube(const Vector& Location,const Vector& Scale,const glm::vec4& Color);
		static void DrawCube(const Vector& Location,const Vector& Scale,const Count<Texture2D>& Texture);
		static void DrawCube(const Vector& Location,const Vector& Scale,const Count<Texture2D>& Texture,const glm::vec4& TintColor);
		static void DrawMesh(StaticMeshComponent& MeshComponent);
		static void EndContext();
	private:
		static void DrawCube(const glm::mat4& Transform,const glm::vec4& Color= {1.0f,1.0f,1.0f,1.0f});
		static void DrawCube(const glm::mat4& Transform,Count<Texture2D>Tex,const glm::vec4& TintColor = {1.0f,1.0f,1.0f,1.0f});

	};

	struct BatchRenderer3D {
		BatchRenderer3D(); 
		Vertex Data;
		uint32_t Offset;
		uint32_t NumofVertices = 0;
		const uint32_t MaxVertices = 2000;

		void AttachVertex(std::vector<Vertex>& Vertices);
		void AttachVertex(const Vector& Vertices,const Vector& Normal,const glm::vec2& TexCoords,const Vector& Tangent,const Vector& Bitangent);
		void AttachVertex(const Vertex& vertex);
		void AttachIndex(std::vector<uint32_t>& Indices);
		Count <class VertexArray> m_VertexArrayObject;
		Count<class VertexBuffer> m_VertexBufferObject;
		Count<class IndexBuffer> m_IndexBufferObject;
		Count<class Shader> m_Shader;
		//std::unordered_map<std::string,uint8_t>m_MapTextures;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<glm::vec2> m_Texcoords;
	};
}