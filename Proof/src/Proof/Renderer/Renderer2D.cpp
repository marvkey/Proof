#include "Proofprch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Proof3D/Renderer/Camera/Camera.h"
#include "Proof3D/Renderer/Camera/OrthagraphicCamera.h"


namespace Proof {
	struct Renderer2DStorage;
	std::array<uint32_t,Renderer2DStorage::MaxIndexCount>Renderer2DStorage::QuadIndices = {};
	static uint32_t Offset = 0;
	static Renderer2DStorage* s_Storage2DData;
	void Renderer2D::Init() {
		for (uint32_t i = 0; i < Renderer2DStorage::MaxIndexCount; i += 6) {
			Renderer2DStorage::QuadIndices[i + 0] = 0 +Offset;
			Renderer2DStorage::QuadIndices[i + 1] = 1 + Offset;
			Renderer2DStorage::QuadIndices[i + 2] = 3 + Offset;

			Renderer2DStorage::QuadIndices[i + 3] = 1 + Offset;
			Renderer2DStorage::QuadIndices[i + 4] = 2 + Offset;
			Renderer2DStorage::QuadIndices[i + 5] = 3 + Offset;
			Offset += 4;
		}
		s_Storage2DData = new Renderer2DStorage();
		s_Storage2DData->WhiteTexture = Texture2D::Create(1,1);

		s_Storage2DData->m_Shader = Shader::Create("DrawQuad",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/ShaderRenderSquare.glsl");
		s_Storage2DData->m_VertexArray = VertexArray::Create();
		s_Storage2DData->m_VertexBuffer = VertexBuffer::Create(s_Storage2DData->MaxVertexCount);
		s_Storage2DData->m_IndexBuffer = IndexBuffer::Create(&s_Storage2DData->QuadIndices[0],s_Storage2DData->QuadIndices.size());
		s_Storage2DData->m_VertexBuffer->Bind();
		s_Storage2DData->m_IndexBuffer->Bind();

		s_Storage2DData->m_VertexArray->AttachIndexBuffer(s_Storage2DData->m_IndexBuffer);
		s_Storage2DData->m_VertexArray->AddData(0,3,sizeof(Vertex2D),(void*)offsetof(Vertex2D,Position));
		s_Storage2DData->m_VertexArray->AddData(1,4,sizeof(Vertex2D),(void*)offsetof(Vertex2D,Color));
		s_Storage2DData->m_VertexArray->AddData(2,3,sizeof(Vertex2D),(void*)offsetof(Vertex2D,TexCoords));
		uint32_t WhiteTextureImage = 0xffffffff;
		s_Storage2DData->WhiteTexture->SetData(&WhiteTextureImage,sizeof(uint32_t));
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_Shader->SetInt("m_TextureSlot",0);
	}
	void Renderer2D::BeginContext(const glm::mat4& Projection,const Camera& camera) {
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_Shader->SetMat4("u_ViewProjection",Projection);
		s_Storage2DData->m_Shader->SetMat4("u_View",camera.GetCameraView());
		Start();
	}
	void Renderer2D::BeginContext(const OrthagraphicCamera& Camera) {
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_Shader->SetMat4("u_ViewProjection",Camera.GetProjectionMatrix());
		s_Storage2DData->m_Shader->SetMat4("u_View",Camera.GetViewMatrix());
		Start();
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location) {
		DrawQuad(Location,0.0f,{1,1},{1.0f,1.0f,1.0f,1.0f},s_Storage2DData->WhiteTexture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,float Rotation,const glm::vec2& Size) {
		DrawQuad(Location,Rotation,Size,{1.0f,1.0f,1.0f,1.0f},s_Storage2DData->WhiteTexture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,const glm::vec2& Size) {
		DrawQuad(Location,0.0f,Size,{1.0f,1.0f,1.0f,1.0f},s_Storage2DData->WhiteTexture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,float Rotation) {
		DrawQuad(Location,Rotation,{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f},s_Storage2DData->WhiteTexture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,float Rotation,const glm::vec4& Color) {
		DrawQuad(Location,Rotation,{1.0f,1.0f},Color,s_Storage2DData->WhiteTexture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,const glm::vec2& Size,const glm::vec4& Color) {
		DrawQuad(Location,0.0f,Size,Color,s_Storage2DData->WhiteTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec2& Location,const glm::vec4& Color) {
		DrawQuad(Location,0.0f,{1.0f,1.0f},Color,s_Storage2DData->WhiteTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec2& Location,const Count<Texture2D> texture) {
		DrawQuad(Location,0.0f,{1,1},{1.0f,1.0f,1.0f,1.0f},texture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,const glm::vec2& Size,const glm::vec4& TintColor,Count<Texture2D> texture) {
		DrawQuad(Location,0.0f,Size,TintColor,texture);
	}
	void Renderer2D::DrawQuad(const glm::vec2& Location,const glm::vec4& TintColor,Count<Texture2D> texture) {
		DrawQuad(Location,0.0f,{1.0f,1.0f},TintColor,texture);
	}

	void Renderer2D::DrawQuad(const glm::vec2& Location,float Rotation, const glm::vec2& Size,const glm::vec4& Color,const Count<Texture2D> texture2D) {
		if (s_Storage2DData->m_IndexCount >=Renderer2DStorage::MaxIndexCount ) // reached maxed index size
			Reset();
		texture2D->BindTexture(0);

		auto W1 = CreateQuad(Location,Rotation,Size,Color);
		s_Storage2DData->Vertices.insert(s_Storage2DData->Vertices.end(),W1.begin(),W1.end());
		s_Storage2DData->m_IndexCount+=6;
		s_Storage2DData->m_NumQuads+=1;
	}
	void Renderer2D::EndContext() {
		Render();
	}
	void Renderer2D::Reset() {
		Render();
		Start();
	}
	void Renderer2D::Render() {
		if (s_Storage2DData->m_IndexCount <= 0)return; // IndexCount Empty nothing to draw
		s_Storage2DData->m_VertexBuffer->Bind();
		s_Storage2DData->m_IndexBuffer->Bind();
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_VertexBuffer->AddData(&s_Storage2DData->Vertices[0],s_Storage2DData->Vertices.size() * sizeof(Vertex2D));
		RendererCommand::DrawIndexed(s_Storage2DData->m_VertexArray,s_Storage2DData->m_IndexCount);
		s_Storage2DData->m_DrawCalls +=1;
		s_Storage2DData->Vertices.clear();
	}
	void Renderer2D::Start() {
		s_Storage2DData->m_IndexCount =0;
		s_Storage2DData->m_DrawCalls =0;
		s_Storage2DData->m_NumQuads = 0;
	}
	
	std::vector<Vertex2D> Renderer2D::CreateQuad(const glm::vec2& Location,float Rotation,const glm::vec2& Scale,const glm::vec4& Color) {
		float Depth =0.0f;
		glm::mat4 Transform = glm::mat4(1.0f);
		Transform = glm::translate(glm::mat4(1.0f),{Location.x,Location.y,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation),{0.0f,0.0f,1.0f})*
			glm::scale(glm::mat4(1.0f),{Scale.x,Scale.y,1.0f});

		Vertex2D V1,V2,V3,V4;
		V1.Color = Color;
		V1.Position = Transform*glm::vec4(0.5f,0.5f,Depth,1.0f);
		V1.TexCoords ={1.0f,1.0f};

		V2.Color = Color;
		V2.Position = Transform*glm::vec4(0.5f,-0.5f,Depth,1.0f);
		V2.TexCoords = {1.0f,0.0f};

		V3.Color = Color;
		V3.Position = Transform * glm::vec4(-0.5f,-0.5f,Depth,1.0f);
		V3.TexCoords = {0.0f,0.0f};

		V4.Color = Color;
		V4.Position = Transform * glm::vec4(-0.5f,0.5f,Depth,1.0f);
		V4.TexCoords = {0.0f,1.0f};

		return {V1,V2,V3,V4};
	}
}