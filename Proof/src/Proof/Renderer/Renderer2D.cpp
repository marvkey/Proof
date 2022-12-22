#include "Proofprch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Renderer/Buffer.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "PipeLineLayout.h"
#include "RenderPass.h"
#include "GraphicsPipeLine.h"
#include "CommandBuffer.h"
namespace Proof {

	static glm::mat4 s_Transform;
	static Vertex2D Vertex1,Vertex2,Vertex3,Vertex4;
	static SpritePipeline* s_SpritePiipeline;
	static Renderer2DStorage* s_Storage2DData;
	enum class DescriptorSet0 {
	//struct
		CameraData = 0,
		//struct
		WorldData = 1,

	};

	enum class DescriptorSet1 {
		TextureArray = 0
	};
	struct CameraData {
		CameraData() {};
		CameraData(const glm::mat4& projection, const glm::mat4& view, const Vector& pos) :
			m_Projection(projection), m_View(view), m_Positon(pos) {
		};
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		Vector m_Positon;
	};

	static CameraData s_CurrentCamera;
	void Renderer2D::Init() {
		s_Storage2DData = new Renderer2DStorage();
		InitDescriptors();
		s_SpritePiipeline = new SpritePipeline();
	}
	
	void Renderer2D::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer> frameBuffer) {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 1.0f, TimerTypes::RendererBase);
		s_CurrentCamera = CameraData{ projection,view,Position };
		s_Storage2DData->CurrentFrameBuffer = frameBuffer;
	}
	void Renderer2D::InitDescriptors() {
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((int)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				//.AddBinding((int)DescriptorSet0::WorldData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.Build();
			s_Storage2DData->Descriptors.insert({ DescriptorSets::Zero,descriptor });
		}

		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::One)
				.AddBinding((int)DescriptorSet1::TextureArray, DescriptorType::ImageSampler, ShaderStage::Fragment,32)
				.Build();
			s_Storage2DData->Descriptors.insert({ DescriptorSets::One,descriptor });
		}
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location) {
		DrawQuad(Location,{0.0,0.0,0.0},{1,1,1},{1.0f,1.0f,1.0f,1.0f}, Renderer::GetWhiteTexture());
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Size) {
		DrawQuad(Location,{0.0,0.0,0.0},Size,{1.0f,1.0f,1.0f,1.0f}, Renderer::GetWhiteTexture());
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec4& Color) {
		DrawQuad(Location,Rotation,{1.0f,1.0f,1.0f},Color, Renderer::GetWhiteTexture());
	}

	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec4& Color) {
		DrawQuad(Location,{0.0,0.0,0.0},{1.0f,1.0f,1.0f},Color, Renderer::GetWhiteTexture());
	}

	void Renderer2D::DrawQuad(const glm::vec3& Location,const Count<Texture2D> texture) {
		DrawQuad(Location,{0.0,0.0,0.0},{1.f,1.f,1.f},{1.0f,1.0f,1.0f,1.0f},texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Size,const glm::vec4& TintColor,Count<Texture2D>& texture) {
		DrawQuad(Location,{0.0,0.0,0.0},Size,TintColor,texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec4& TintColor,Count<Texture2D> texture) {
		DrawQuad(Location,{0.0,0.0,0.0},{1.0f,1.0f,1.0f},TintColor,texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Size,const glm::vec4& Color){
		DrawQuad(Location,Rotation,Size,Color,Renderer::GetWhiteTexture());
	}
	void Renderer2D::DrawQuad(SpriteComponent& Sprite, const TransformComponent& transform){
		DrawQuad({transform.Location},transform.Rotation,transform.Scale,Sprite.Colour,Sprite.GetTexture()!= nullptr ? Sprite.GetTexture() : Renderer::GetWhiteTexture());
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation, const glm::vec3& Size,const glm::vec4& Color,const Count<Texture2D>& texture2D) {
		if (s_Storage2DData->IndexCount >= s_Storage2DData->c_MaxIndexCount){ // reached maxed index size
			Render();
			Reset();
		}
		float TextureIndex =-1.0f; // no texture index
		
		for(uint32_t i =0; i<s_Storage2DData->TextureSlotIndex;i++){
			if(s_Storage2DData->Textures[i]->GetID() == texture2D->GetID()){
				TextureIndex =(float)i;
				break;
			}
		}
		if(TextureIndex ==-1.0f){ /// if the texture does not exist, then we are just going to assing a new texture
			TextureIndex = (float) s_Storage2DData->TextureSlotIndex; // assinging new
			s_Storage2DData->Textures[s_Storage2DData->TextureSlotIndex] =texture2D;
			s_Storage2DData->TextureSlotIndex++;
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
		s_Storage2DData->QuadArray[s_Storage2DData->QuadArraySize] = Vertex1;
		s_Storage2DData->QuadArray[s_Storage2DData->QuadArraySize+1] = Vertex2;
		s_Storage2DData->QuadArray[s_Storage2DData->QuadArraySize+2] = Vertex3;
		s_Storage2DData->QuadArray[s_Storage2DData->QuadArraySize+3] = Vertex4;
		/*
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize],&Vertex1,sizeof(Vertex1));
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize +1],&Vertex2,sizeof(Vertex1));
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize +2],&Vertex3,sizeof(Vertex3));
		memcpy(&s_Storage2DData->m_QuadArray[s_Storage2DData->m_QuadArraySize +3],&Vertex4,sizeof(Vertex4));
		*/
		s_Storage2DData->IndexCount+=6; 
		s_Storage2DData->QuadArraySize += 4;
		
	}
	void Renderer2D::EndContext() {
		Render();
		Reset();
	}
	void Renderer2D::Reset() {
		s_Storage2DData->IndexCount = 0;
		s_Storage2DData->TextureSlotIndex = 1;
		s_Storage2DData->QuadArraySize = 0;
	}
	
	void Renderer2D::Render() {
		if (s_Storage2DData->IndexCount == 0)return; // nothing to draw

		#if 0
		if(s_Storage2DData->m_IndexCount ==0)return; // nothing to draw
		s_Storage2DData->m_Shader->Bind();
		s_Storage2DData->m_VertexBuffer->Bind();
		s_Storage2DData->m_IndexBuffer->Bind();
		for(uint32_t i =0; i<s_Storage2DData->m_TextureSlotIndex;i++){
			s_Storage2DData->m_Textures[i]->Bind(i);
		} 
		s_Storage2DData->m_VertexBuffer->AddData(s_Storage2DData->m_QuadArray,s_Storage2DData->m_QuadArraySize *sizeof(Vertex2D));
		Renderer::DrawIndexed(s_Storage2DData->m_VertexArray,s_Storage2DData->m_IndexCount);
		s_Renderer2DStats->m_DrawCalls+=1;
		#endif
		s_Storage2DData->CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));
		auto descriptor0 = s_Storage2DData->Descriptors[DescriptorSets::Zero];
		auto descriptor1 = s_Storage2DData->Descriptors[DescriptorSets::One];

		descriptor0->WriteBuffer((int)DescriptorSet0::CameraData, s_Storage2DData->CameraBuffer);
		descriptor1->WriteImage((int)DescriptorSet1::TextureArray, s_Storage2DData->Textures);
		Renderer::BeginRenderPass(s_Storage2DData->CommandBuffer, s_SpritePiipeline->RenderPass, s_Storage2DData->CurrentFrameBuffer);
		Renderer::RecordRenderPass(s_SpritePiipeline->RenderPass, [&](Count <CommandBuffer> commandBuffer) {
			s_Storage2DData->VertexBuffer->AddData(s_Storage2DData->QuadArray.data(), s_Storage2DData->QuadArraySize * sizeof(Vertex2D));
			descriptor0->Bind(commandBuffer, s_SpritePiipeline->PipeLineLayout);
			descriptor1->Bind(commandBuffer, s_SpritePiipeline->PipeLineLayout);
			s_Storage2DData->VertexBuffer->Bind(commandBuffer);
			s_Storage2DData->IndexBuffer->Bind(commandBuffer);
			Renderer::DrawArrays(commandBuffer, s_Storage2DData->IndexBuffer->GetCount(),1);
		});
		Renderer::EndRenderPass(s_SpritePiipeline->RenderPass);
		Renderer::SubmitCommandBuffer(s_Storage2DData->CommandBuffer);

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
	Renderer2DStorage::Renderer2DStorage() {
		QuadIndices.resize(c_MaxIndexCount);
		Textures.resize(MaxTextureSlot);
		QuadArray.resize(c_MaxVertexCount);
		uint32_t Offset = 0;
		for (uint32_t i = 0; i < c_MaxIndexCount; i += 6) {
			QuadIndices[i + 0] = 0 +Offset;
			QuadIndices[i + 1] = 1 + Offset;
			QuadIndices[i + 2] = 2 + Offset;

			QuadIndices[i + 3] = 3 + Offset;
			QuadIndices[i + 4] = 3 + Offset;
			QuadIndices[i + 5] = 0 + Offset;
			Offset += 4;
		}

		VertexBuffer = VertexBuffer::Create(c_MaxVertexCount);
		IndexBuffer = IndexBuffer::Create(QuadIndices.data(), QuadIndices.size());
		CameraBuffer = UniformBuffer::Create(sizeof(CameraData), DescriptorSets::Zero, (uint32_t)DescriptorSet0::CameraData);
		CommandBuffer = CommandBuffer::Create();
		uint32_t whiteTextureData = 0xffffffff;

		int32_t Samplers[32];
		for(uint32_t i=0; i< Renderer2DStorage::MaxTextureSlot;i++)
			Samplers[i] =i;
		Textures[0] =  Texture2D::Create(1, 1, ImageFormat::RGBA, &whiteTextureData);

	}
	SpritePipeline::SpritePipeline() {
		auto vertexArray = VertexArray::Create({ sizeof(Vertex2D) });
		vertexArray->AddData(0, DataType::Vec3, offsetof(Vertex2D, Vertex2D::Position));
		vertexArray->AddData(1, DataType::Vec4, offsetof(Vertex2D, Vertex2D::Color));
		vertexArray->AddData(2, DataType::Vec3, offsetof(Vertex2D, Vertex2D::TexCoords));
		vertexArray->AddData(3, DataType::Float, offsetof(Vertex2D, Vertex2D::TexSlot));
		Shader = Shader::GetOrCreate("Base2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Mesh.shader");
		PipeLineLayout = PipeLineLayout::Create(std::vector{ s_Storage2DData->Descriptors[DescriptorSets::Zero],s_Storage2DData->Descriptors[DescriptorSets::One] });
		RenderPass = RenderPass::Create();
		GraphicsPipeline = GraphicsPipeline::Create(Shader, RenderPass, PipeLineLayout, vertexArray);
		RenderPass->SetGraphicsPipeline(GraphicsPipeline);
	}
}