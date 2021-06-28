#include "Proofprch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Proof3D/Renderer/Camera/Camera.h"
#include "Proof3D/Renderer/Camera/OrthagraphicCamera.h"
#include "Proof/Renderer/Buffer.h"

namespace Proof {
	std::array<uint32_t,Renderer2DStorage::s_MaxIndexCount>Renderer2DStorage::QuadIndices = {};
	static uint32_t Offset = 0;

	static glm::mat4 s_Transform;
	static Vertex2D Vertex1,Vertex2,Vertex3,Vertex4;

	static Renderer2DStorage* s_Storage2DData;
	static Renderer2D::Renderer2DStats* s_Renderer2DStats;
	void Renderer2D::Init() {
		s_Renderer2DStats = new Renderer2DStats;
		for (uint32_t i = 0; i < Renderer2DStorage::s_MaxIndexCount; i += 6) {
			Renderer2DStorage::QuadIndices[i + 0] = 0 +Offset;
			Renderer2DStorage::QuadIndices[i + 1] = 1 + Offset;
			Renderer2DStorage::QuadIndices[i + 2] = 3 + Offset;

			Renderer2DStorage::QuadIndices[i + 3] = 1 + Offset;
			Renderer2DStorage::QuadIndices[i + 4] = 2 + Offset;
			Renderer2DStorage::QuadIndices[i + 5] = 3 + Offset;
			Offset += 4;
		}
		s_Storage2DData = new Renderer2DStorage();
		s_Storage2DData->m_WhiteTexture = Texture2D::Create(1,1);

		s_Storage2DData->m_Shader = Shader::Create("DrawQuad",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/ShaderRenderSquare.glsl");
		s_Storage2DData->m_VertexArray = VertexArray::Create();
		s_Storage2DData->m_VertexBuffer = VertexBuffer::Create(s_Storage2DData->s_MaxVertexCount);
		s_Storage2DData->m_IndexBuffer = IndexBuffer::Create(&s_Storage2DData->QuadIndices[0],s_Storage2DData->QuadIndices.size());
		s_Storage2DData->m_VertexBuffer->Bind();
		s_Storage2DData->m_IndexBuffer->Bind();

		s_Storage2DData->m_VertexArray->AttachIndexBuffer(s_Storage2DData->m_IndexBuffer);

		s_Storage2DData->m_VertexArray->AddData(0,3,sizeof(Vertex2D),(void*)offsetof(Vertex2D,Position));
		s_Storage2DData->m_VertexArray->AddData(1,4,sizeof(Vertex2D),(void*)offsetof(Vertex2D,Color));
		s_Storage2DData->m_VertexArray->AddData(2,3,sizeof(Vertex2D),(void*)offsetof(Vertex2D,TexCoords));
		s_Storage2DData->m_VertexArray->AddData(3,1,sizeof(Vertex2D),(void*)offsetof(Vertex2D,TexSlot));

		uint32_t WhiteTextureImage = 0xffffffff;
		s_Storage2DData->m_WhiteTexture->SetData(&WhiteTextureImage,sizeof(uint32_t));
		int32_t Samplers[32];
		for(uint32_t i=0; i< Renderer2DStorage::MaxTextureSlot;i++)
			Samplers[i] =i;
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_Shader->SetIntArray("u_TextureSlot",Samplers,Renderer2DStorage::MaxTextureSlot);

	    s_Storage2DData->m_Textures[0]= s_Storage2DData->m_WhiteTexture;
	}
	void Renderer2D::BeginContext(const glm::mat4& Projection,const Camera& camera) {
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_Shader->SetMat4("u_ViewProjection",Projection);
		s_Storage2DData->m_Shader->SetMat4("u_View",camera.GetCameraView());
		s_Renderer2DStats->m_DrawCalls=0;
		s_Renderer2DStats->m_QuadCount =0;
		Start();
	}
	void Renderer2D::BeginContext(const OrthagraphicCamera& Camera) {
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_Shader->SetMat4("u_ViewProjection",Camera.GetProjectionMatrix());
		s_Storage2DData->m_Shader->SetMat4("u_View",Camera.GetViewMatrix());
		s_Renderer2DStats->m_DrawCalls = 0;
		s_Renderer2DStats->m_QuadCount = 0;
		Start();
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location) {
		DrawQuad(Location,{0.0,0.0,0.0},{1,1,1},{1.0f,1.0f,1.0f,1.0f},s_Storage2DData->m_WhiteTexture);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Size) {
		DrawQuad(Location,{0.0,0.0,0.0},Size,{1.0f,1.0f,1.0f,1.0f},s_Storage2DData->m_WhiteTexture);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec4& Color) {
		DrawQuad(Location,Rotation,{1.0f,1.0f,1.0f},Color,s_Storage2DData->m_WhiteTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec4& Color) {
		DrawQuad(Location,{0.0,0.0,0.0},{1.0f,1.0f,1.0f},Color,s_Storage2DData->m_WhiteTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Location,const Count<Texture2D> texture) {
		DrawQuad(Location,{0.0,0.0,0.0},{1.f,1.f,1.f},{1.0f,1.0f,1.0f,1.0f},texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Size,const glm::vec4& TintColor,Count<Texture2D> texture) {
		DrawQuad(Location,{0.0,0.0,0.0},Size,TintColor,texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec4& TintColor,Count<Texture2D> texture) {
		DrawQuad(Location,{0.0,0.0,0.0},{1.0f,1.0f,1.0f},TintColor,texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color){
		DrawQuad(Location,Rotation,Size,Color,s_Storage2DData->m_WhiteTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation, const glm::vec3& Size,const glm::vec4& Color,const Count<Texture2D> texture2D) {
		if (s_Storage2DData->m_IndexCount >=Renderer2DStorage::s_MaxIndexCount) // reached maxed index size
			Reset();
		float TextureIndex =-1.0f; // no texture index
		for(uint32_t i =0; i<s_Storage2DData->m_TextureSlotIndex;i++){
			if(s_Storage2DData->m_Textures[i]->GetID() == texture2D->GetID()){
				TextureIndex =(float)i;
				break;
			}
		}
		if(TextureIndex ==-1.0f){ /// if the texture does not exist, then we are just going to assing a new texture
			TextureIndex = (float) s_Storage2DData->m_TextureSlotIndex; // assinging new
			s_Storage2DData->m_Textures[s_Storage2DData->m_TextureSlotIndex] =texture2D;
			s_Storage2DData->m_TextureSlotIndex++;
		}

		s_Transform = glm::mat4(1.0f);
		s_Transform = glm::translate(glm::mat4(1.0f),{Location.x,Location.y,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.x),{1.0f,0.0f,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.y),{0.0f,1.0f,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.z),{0.0f,0.0f,1.0f}) *
			glm::scale(glm::mat4(1.0f),{Size.x,Size.y,Size.z});

		Vertex1.Position = s_Transform * glm::vec4(0.5f,0.5f,0.0f,1.0f);
		Vertex1.Color = Color;
		Vertex1.TexCoords = {1.0f,1.0f};
		Vertex1.TexSlot = TextureIndex;

		Vertex2.Position = s_Transform * glm::vec4(0.5f,-0.5f,0.0f,1.0f);
		Vertex2.Color = Color;
		Vertex2.TexCoords = {1.0f,0.0f};
		Vertex2.TexSlot = TextureIndex;

		Vertex3.Position = s_Transform * glm::vec4(-0.5f,-0.5f,0.0f,1.0f);
		Vertex3.Color = Color;
		Vertex3.TexCoords = {0.0f,0.0f};
		Vertex3.TexSlot = TextureIndex;

		Vertex4.Position = s_Transform * glm::vec4(-0.5f,0.5f,0.0f,1.0f);
		Vertex4.Color = Color;
		Vertex4.TexCoords = {0.0f,1.0f};
		Vertex4.TexSlot = TextureIndex;
		/* Gonna test wich is faster this meathod or the second one*/
		s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize] = Vertex1;
		s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize+1] = Vertex2;
		s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize+2] = Vertex3;
		s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize+3] = Vertex4;
		/*
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize],&Vertex1,sizeof(Vertex1));
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize +1],&Vertex2,sizeof(Vertex1));
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize +2],&Vertex3,sizeof(Vertex3));
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize +3],&Vertex4,sizeof(Vertex4));
		*/
		s_Storage2DData->m_IndexCount+=6; 
		s_Storage2DData->m_QuadArraySize += 4;
		s_Renderer2DStats->m_QuadCount+=1;
	}
	void Renderer2D::EndContext() {
		Render();
	}
	void Renderer2D::Reset() {
		Render();
		Start();
	}
	void Renderer2D::Render() {
		if(s_Storage2DData->m_IndexCount ==0)return; // nothing to draw
		s_Storage2DData->m_Shader->UseShader();
		s_Storage2DData->m_VertexBuffer->Bind();
		s_Storage2DData->m_IndexBuffer->Bind();
		for(uint32_t i =0; i<s_Storage2DData->m_TextureSlotIndex;i++){
			s_Storage2DData->m_Textures[i]->BindTexture(i);
		} 
		s_Storage2DData->m_VertexBuffer->AddData(s_Storage2DData->m_QuadArray,s_Storage2DData->m_QuadArraySize *sizeof(Vertex2D));
		RendererCommand::DrawIndexed(s_Storage2DData->m_VertexArray,s_Storage2DData->m_IndexCount);
		s_Renderer2DStats->m_DrawCalls+=1;
	}
	void Renderer2D::Start() {
		s_Storage2DData->m_IndexCount = 0;
		s_Storage2DData->m_TextureSlotIndex = 1;
		s_Storage2DData->m_QuadArraySize =0;
	}
	
	std::vector<Vertex2D> Renderer2D::CreateQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Scale,const glm::vec4& Color,float TexIndex) {
		float Depth =0.0f;
		glm::mat4 Transform = glm::mat4(1.0f);
		Transform = glm::translate(glm::mat4(1.0f),{Location.x,Location.y,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.x),{1.0f,0.0f,0.0f})*
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.y),{0.0f,1.0f,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.z),{0.0f,0.0f,1.0f}) *
			glm::scale(glm::mat4(1.0f),{Scale.x,Scale.y,Scale.z});

		Vertex2D V1,V2,V3,V4;
		V1.Position = Transform*glm::vec4(0.5f,0.5f,Depth,1.0f);
		V1.Color = Color;
		V1.TexCoords ={1.0f,1.0f};
		V1.TexSlot =TexIndex;

		V2.Position = Transform*glm::vec4(0.5f,-0.5f,Depth,1.0f);
		V2.Color = Color;
		V2.TexCoords = {1.0f,0.0f};
		V2.TexSlot = TexIndex;

		V3.Position = Transform * glm::vec4(-0.5f,-0.5f,Depth,1.0f);
		V3.Color = Color;
		V3.TexCoords = {0.0f,0.0f};
		V3.TexSlot = TexIndex;

		V4.Position = Transform * glm::vec4(-0.5f,0.5f,Depth,1.0f);
		V4.Color = Color;
		V4.TexCoords = {0.0f,1.0f};
		V4.TexSlot = TexIndex;

		return {V1,V2,V3,V4};
	}
}