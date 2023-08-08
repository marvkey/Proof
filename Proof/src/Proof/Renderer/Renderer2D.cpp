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
#include "RenderPass.h"
#include "GraphicsPipeLine.h"
#include "CommandBuffer.h"
#include "Font.h"
#include "MSDFData.h"
#include "Proof/Math/MathInclude.h"
namespace Proof {

	static glm::mat4 s_Transform;
	static Vertex2D Vertex1,Vertex2,Vertex3,Vertex4;
	enum class DescriptorSet0 {
	//struct
		CameraData = 0,
		//struct
		WorldData = 1,

	};
	
	static CameraData s_CurrentCamera;
	void Renderer2D::Init() {
		m_Storage2DData = CreateSpecial <Renderer2DStorage>();
		//if(m_SpritePipeline == nullptr)
		//	m_SpritePipeline= CreateSpecial<SpritePipeline>();
		//m_TextPipeline = CreateSpecial<TextPipeline>();

		m_SpritePipeline->RenderPass->SetInput("CameraData", m_Storage2DData->CameraBuffer);
	}
	
	void Renderer2D::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, Count<RenderCommandBuffer>& commdandBuffer) {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 1.0f, TimerTypes::RendererBase);
		s_CurrentCamera = CameraData{ projection,view,Position };
		m_Storage2DData->CurrentFrameBuffer = frameBuffer;
		m_Storage2DData->CommandBuffer = commdandBuffer;
	}
	Renderer2D::Renderer2D(Count<ScreenFrameBuffer>& frameBuffer,bool screenSpace)
	{
		m_ScreenSpace = screenSpace;
		if (m_SpritePipeline == nullptr)
			m_SpritePipeline = CreateSpecial<SpritePipeline>(frameBuffer->GetFrameBuffer());

		m_TextPipeline = CreateSpecial<TextPipeline>(frameBuffer->GetFrameBuffer());
		Init();
	}
	Renderer2D::Renderer2D(Count<ScreenFrameBuffer>& frameBuffer,const std::string& spriteRenderShaderPath)
	{
		m_ScreenSpace = false;
		m_SpritePipeline = CreateSpecial<SpritePipeline>(frameBuffer->GetFrameBuffer(), spriteRenderShaderPath);
		m_TextPipeline = CreateSpecial<TextPipeline>(frameBuffer->GetFrameBuffer());
		
		Init();

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
		if (Sprite.Texture != nullptr)
		{
			DrawQuad(ProofToglmVec(transform.Location), ProofToglmVec(transform.Rotation), ProofToglmVec(transform.Scale),
				glm::vec4{ Sprite.Colour }, Sprite.Texture);
		}
		else
		{

			DrawQuad( ProofToglmVec(transform.Location),ProofToglmVec(transform.Rotation),ProofToglmVec(transform.Scale),glm::vec4{Sprite.Colour}, nullptr);
		}
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation, const glm::vec3& Size,const glm::vec4& Color,const Count<Texture2D>& texture2D) {
		if (m_Storage2DData->IndexCount >= m_Storage2DData->c_MaxIndexCount){ // reached maxed index size
			Render();
			Reset();
		}
		float TextureIndex =-1.0f; // no texture index
		if (texture2D != nullptr)
		{
			for (uint32_t i = 0; i < m_Storage2DData->TextureSlotIndex; i++)
			{
				if (m_Storage2DData->Textures[i] == texture2D)
				{
					TextureIndex = (float)i;
					break;
				}
			}
			if (TextureIndex == -1.0f)
			{ /// if the texture does not exist, then we are just going to assing a new texture
				TextureIndex = (float)m_Storage2DData->TextureSlotIndex; // assinging new
				m_Storage2DData->Textures[m_Storage2DData->TextureSlotIndex] =texture2D ;
				m_Storage2DData->TextureSlotIndex++;
			}
		}
		if (TextureIndex == -1)
			TextureIndex = 0;
		s_Transform = glm::mat4(1.0f);
		if (m_ScreenSpace == false)
		{

			s_Transform = glm::translate(glm::mat4(1.0f), { Location.x,Location.y,Location.z }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1.0f,0.0f,0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0.0f,1.0f,0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0.0f,0.0f,1.0f }) *
				glm::scale(glm::mat4(1.0f), { Size.x,Size.y,Size.z });

			Vertex1.model = s_Transform;
			Vertex2.model = s_Transform;
			Vertex3.model = s_Transform;
			Vertex4.model = s_Transform;

			Vertex1.Position = GlmVecToProof(s_Transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
			Vertex1.Color = Color;
			Vertex1.TexCoords = { 1.0f,1.0f };
			Vertex1.TexSlot = TextureIndex;

			Vertex2.Position = GlmVecToProof(s_Transform * glm::vec4(0.5f, -0.5f, 0.0, 1.0f));
			Vertex2.Color = Color;
			Vertex2.TexCoords = { 1.0f,0.0f };
			Vertex2.TexSlot = TextureIndex;

			Vertex3.Position = GlmVecToProof(s_Transform * glm::vec4(-0.5f, -0.5f, 0.0, 1.0f));
			Vertex3.Color = Color;
			Vertex3.TexCoords = { 0.0f,0.0f };
			Vertex3.TexSlot = TextureIndex;

			Vertex4.Position = GlmVecToProof(s_Transform * glm::vec4(-0.5f, 0.5f, 0.0, 1.0f));
			Vertex4.Color = Color;
			Vertex4.TexCoords = { 0.0f,1.0f };
			Vertex4.TexSlot = TextureIndex;
		}
		else
		{

			s_Transform = glm::translate(glm::mat4(1.0f), { Location.x,Location.y,0 })*
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1.0f,0.0f,0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0.0f,1.0f,0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0.0f,0.0f,1.0f }) *
				glm::scale(glm::mat4(1.0f), { Size.x,Size.y,Size.z });
			Vertex1.Position = GlmVecToProof(s_Transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));

			Vertex1.model = s_Transform;
			Vertex2.model = s_Transform;
			Vertex3.model = s_Transform;
			Vertex4.model = s_Transform;
			Vertex1.Color = Color;
			Vertex1.TexCoords = { 1.0f,1.0f };
			Vertex1.TexSlot = TextureIndex;

			Vertex2.Position = GlmVecToProof(s_Transform * glm::vec4(0.5f, -0.5f, 0.0, 1.0f));
			Vertex2.Color = Color;
			Vertex2.TexCoords = { 1.0f,0.0f };
			Vertex2.TexSlot = TextureIndex;

			Vertex3.Position = GlmVecToProof(s_Transform * glm::vec4(-0.5f, -0.5f, 0.0, 1.0f));
			Vertex3.Color = Color;
			Vertex3.TexCoords = { 0.0f,0.0f };
			Vertex3.TexSlot = TextureIndex;

			Vertex4.Position = GlmVecToProof(s_Transform * glm::vec4(-0.5f, 0.5f, 0.0, 1.0f));
			Vertex4.Color = Color;
			Vertex4.TexCoords = { 0.0f,1.0f };
			Vertex4.TexSlot = TextureIndex;
		}
		m_Storage2DData->QuadArray[m_Storage2DData->QuadArraySize] = Vertex1;
		m_Storage2DData->QuadArray[m_Storage2DData->QuadArraySize+1] = Vertex2;
		m_Storage2DData->QuadArray[m_Storage2DData->QuadArraySize+2] = Vertex3;
		m_Storage2DData->QuadArray[m_Storage2DData->QuadArraySize+3] = Vertex4;

		m_Storage2DData->IndexCount+=6; 
		m_Storage2DData->QuadArraySize += 4;
		
	}
	void Renderer2D::DrawString(const std::string& text, Count<class Font> font, const TextParams& textParam, const glm::mat4& transform)
	{
		if (m_Storage2DData->TextIndexCount >= m_Storage2DData->c_MaxIndexCount)
		{ // reached maxed index size
			Render();
			Reset();
		}
		//https://freetype.org/freetype2/docs/tutorial/step2.html
		const auto& fontGeometry =  font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Count<Texture2D> fontAtlas = font->GetAtlasTexture();

		m_Storage2DData->FontTexture = fontAtlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = -fsScale * metrics.ascenderY;
		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();
		for (size_t i = 0; i < text.size(); i++)
		{
			char32_t character = text[i];

			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight + textParam.LineSpacing;
				continue;
			}
			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < text.size() - 1)
				{
					char nextCharacter = text[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + textParam.Kerning;
				continue;
			}

			if (character == '\t')
			{
				// NOTE(Marv): is this right?
				x += 4.0f * (fsScale * spaceGlyphAdvance + textParam.Kerning);
				continue;
			}
			auto glyph = fontGeometry.getGlyph(character);

			if (!glyph)
				glyph = fontGeometry.getGlyph('?');

			if (!glyph)
				continue;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin(pl, pb);
			glm::vec2 quadMax(pr, pt);


			quadMin *= fsScale, quadMax *= fsScale;

			//ofst location
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);


			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			glm::mat4 copyTransfrom = transform;
			//if (m_ScreenSpace == true)
			//{
			//	copyTransfrom = glm::mat4(1.0f);
			//}
			TextVertex textVertex1, textVertex2, textVertex3, textVertex4;
			textVertex1.Positon = copyTransfrom * glm::vec4(quadMin, 0.0f, 1.0f);
			textVertex1.Color = textParam.Color ;
			textVertex1.TexCoord = texCoordMin;

			textVertex2.Positon = copyTransfrom * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			textVertex2.Color = textParam.Color;
			textVertex2.TexCoord = { texCoordMin.x, texCoordMax.y };

			textVertex3.Positon = copyTransfrom * glm::vec4(quadMax, 0.0f, 1.0f);
			textVertex3.Color = textParam.Color;
			textVertex3.TexCoord = texCoordMax;

			textVertex4.Positon = copyTransfrom * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			textVertex4.Color = textParam.Color;
			textVertex4.TexCoord = { texCoordMax.x, texCoordMin.y };

			m_Storage2DData->TextArray[m_Storage2DData->TextArraySize] = textVertex1;
			m_Storage2DData->TextArray[m_Storage2DData->TextArraySize+1] = textVertex2;
			m_Storage2DData->TextArray[m_Storage2DData->TextArraySize+2] = textVertex3;
			m_Storage2DData->TextArray[m_Storage2DData->TextArraySize+3] = textVertex4;

			m_Storage2DData->TextArraySize+=4;
			m_Storage2DData->TextIndexCount += 6;

			if (i < text.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = text[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + textParam.Kerning;
			}
		}
	}
	void Renderer2D::EndContext() {
		Render();
		Reset();
	}

	void Renderer2D::Reset() {

		m_Storage2DData->TextIndexCount = 0;
		m_Storage2DData->TextArraySize =  0;

		m_Storage2DData->QuadArraySize = 0;
		m_Storage2DData->IndexCount = 0;

		m_Storage2DData->CurrentFrameBuffer = nullptr;
		m_Storage2DData->CommandBuffer = nullptr;

		// reseting every textures back to white that has been changed
		for (uint32_t i = 1; i < m_Storage2DData->TextureSlotIndex; i++)
			m_Storage2DData->Textures[i] = m_Storage2DData->Textures[0];
		m_Storage2DData->TextureSlotIndex = 1;
	}
	
	void Renderer2D::Render() {
		PF_PROFILE_FUNC();

		CameraData screnData;
		screnData.Position = s_CurrentCamera.Position;
		screnData.Projection = glm::mat4(1.0f);
		screnData.ProjectionView = glm::mat4(1.0f);
		
		m_Storage2DData->CameraBuffer->SetData(1,Buffer(((uint8_t*)&s_CurrentCamera, sizeof(CameraData))));

		if (m_Storage2DData->IndexCount > 0) // nothing to draw
		{
			PF_PROFILE_FUNC("Renderer2D::Quad Draw");
			m_Storage2DData->VertexBuffer->SetData(m_Storage2DData->QuadArray.data(), m_Storage2DData->QuadArraySize * sizeof(Vertex2D));
			m_SpritePipeline->RenderPass->SetInput("u_Textures", m_Storage2DData->Textures);

			Renderer::BeginRenderPass(m_Storage2DData->CommandBuffer, m_SpritePipeline->RenderPass);
			m_Storage2DData->VertexBuffer->Bind(m_Storage2DData->CommandBuffer);
			m_Storage2DData->IndexBuffer->Bind(m_Storage2DData->CommandBuffer);
			Renderer::DrawElementIndexed(m_Storage2DData->CommandBuffer, m_Storage2DData->IndexCount, m_Storage2DData->QuadArraySize, 0);
			Renderer::EndRenderPass(m_SpritePipeline->RenderPass);
		}
		//}

		#if 0
		if(m_Storage2DData->TextIndexCount > 0){
			PF_PROFILE_FUNC("Renderer2D::String Draw");
			auto descriptor0 = m_TextPipeline->Descriptors[DescriptorSets::Zero];

			descriptor0->WriteBuffer((int)DescriptorSet0::CameraData, m_Storage2DData->CameraBuffer);
			descriptor0->WriteImage(1, m_Storage2DData->FontTexture);

			//Renderer::RecordRenderPass(m_RenderPass, m_TextPipeline->GraphicsPipeline);
			m_Storage2DData->TextVertexBuffer->SetData(m_Storage2DData->TextArray.data(), m_Storage2DData->TextArraySize * sizeof(TextVertex));
			descriptor0->Bind(m_Storage2DData->CommandBuffer, m_TextPipeline->PipeLineLayout);

			m_Storage2DData->IndexBuffer->Bind(m_Storage2DData->CommandBuffer);
			m_Storage2DData->TextVertexBuffer->Bind(m_Storage2DData->CommandBuffer);

			Renderer::DrawElementIndexed(m_Storage2DData->CommandBuffer, m_Storage2DData->TextIndexCount, m_Storage2DData->TextArraySize);
		}
		if (m_Storage2DData->IndexCount == 0)return; // nothing to draw
		{
			PF_PROFILE_FUNC("Renderer2D::Quad Draw");

			auto descriptor0 = m_SpritePipeline->Descriptors[DescriptorSets::Zero];

			descriptor0->WriteBuffer((int)DescriptorSet0::CameraData, m_Storage2DData->CameraBuffer);
			descriptor0->WriteImage(1, m_Storage2DData->Textures);

			//Renderer::RecordRenderPass(m_RenderPass, m_SpritePipeline->GraphicsPipeline);
			m_Storage2DData->VertexBuffer->SetData(m_Storage2DData->QuadArray.data(), m_Storage2DData->QuadArraySize * sizeof(Vertex2D));
			descriptor0->Bind(m_Storage2DData->CommandBuffer, m_SpritePipeline->PipeLineLayout);
			m_Storage2DData->VertexBuffer->Bind(m_Storage2DData->CommandBuffer);
			m_Storage2DData->IndexBuffer->Bind(m_Storage2DData->CommandBuffer);
			Renderer::DrawElementIndexed(m_Storage2DData->CommandBuffer, m_Storage2DData->IndexCount, m_Storage2DData->QuadArraySize, 0);
		}
		#endif
	}
	
	
	std::vector<Vertex2D> Renderer2D::CreateQuad(const glm::vec3& Location,const glm::vec3& Rotation,const glm::vec3& Scale,const glm::vec4& Color,float TexIndex) {
		glm::mat4 Transform = glm::mat4(1.0f);
		Transform = glm::translate(glm::mat4(1.0f),{Location.x,Location.y,Location .z}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.x),{1.0f,0.0f,0.0f})*
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.y),{0.0f,1.0f,0.0f}) *
			glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.z),{0.0f,0.0f,1.0f}) *
			glm::scale(glm::mat4(1.0f),{Scale.x,Scale.y,Scale.z});

		Vertex2D V1,V2,V3,V4;
		V1.Position = GlmVecToProof( Transform*glm::vec4(0.5f,0.5f,0,1.0f));
		V1.Color = Color;
		V1.TexCoords ={1.0f,1.0f};
		V1.TexSlot =TexIndex;

		V2.Position = GlmVecToProof(Transform*glm::vec4(0.5f,-0.5f, 0,1.0f));
		V2.Color = Color;
		V2.TexCoords = {1.0f,0.0f};
		V2.TexSlot = TexIndex;

		V3.Position = GlmVecToProof(Transform * glm::vec4(-0.5f,-0.5f, 0,1.0f));
		V3.Color = Color;
		V3.TexCoords = {0.0f,0.0f};
		V3.TexSlot = TexIndex;

		V4.Position = GlmVecToProof( Transform * glm::vec4(-0.5f,0.5f, 0,1.0f));
		V4.Color = Color;
		V4.TexCoords = {0.0f,1.0f};
		V4.TexSlot = TexIndex;

		return {V1,V2,V3,V4};
	}
	std::pair<Count<VertexBuffer>, Count<IndexBuffer>> Renderer2D::CreateQuad()
	{
		// refernce from https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/Primitives.cpp

		// keep this it is important for teh brdf
		std::vector<Vertex> vertices = {
			{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.2f}, {1.0f, 1.0f}},
			{{-1.0f, 1.0f, 0.0f}, {0.2f, 1.0f, 0.2f}, {0.0f, 1.0f}},
			{{-1.0f, -1.0f, 0.0f}, {0.2f, 0.2f, 0.2f}, {0.0f, 0.0f}},
			{{1.0f, -1.0f, 0.0f}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.0f}},
		};
		std::vector<uint32_t> indices = {
			0, 1, 2, 0, 2, 3,
		};
		Count<VertexBuffer> buffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
		Count<IndexBuffer> indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
		return std::make_pair(buffer, indexBuffer);
	}
	Renderer2DStorage::Renderer2DStorage() {
		QuadIndices.resize(c_MaxIndexCount);
		QuadArray.resize(c_MaxVertexCount);
		TextArray.resize(c_MaxVertexCount);
		uint32_t Offset = 0;
		for (uint32_t i = 0; i < c_MaxIndexCount; i += 6) {
			QuadIndices[i + 0] = 0 +Offset;
			QuadIndices[i + 1] = 1 + Offset;
			QuadIndices[i + 2] = 2 + Offset;

			QuadIndices[i + 3] = 2 + Offset;
			QuadIndices[i + 4] = 3 + Offset;
			QuadIndices[i + 5] = 0 + Offset;
			Offset += 4;
		}

		VertexBuffer = VertexBuffer::Create(c_MaxVertexCount*sizeof(Vertex2D));
		IndexBuffer = IndexBuffer::Create(QuadIndices.data(), c_MaxIndexCount);
		CameraBuffer = UniformBufferSet::Create(sizeof(CameraData));

		TextVertexBuffer = VertexBuffer::Create(c_MaxVertexCount * sizeof(TextVertex));
		uint32_t whiteTextureData = 0xffffffff;

		Textures.resize(Renderer2DStorage::c_MaxTextureSlot);
		WhiteTexture = Texture2D::Create(&whiteTextureData,TextureConfiguration( ));

		for(uint32_t i=0; i < Renderer2DStorage::c_MaxTextureSlot;i++)
			Textures[i] = WhiteTexture;

	}
	SpritePipeline::SpritePipeline(Count<FrameBuffer> frameBuffer)
	{
		auto vertexArray = VertexArray::Create({ sizeof(Vertex2D) });


		vertexArray->AddData(0, DataType::Vec4, 0);
		vertexArray->AddData(1, DataType::Vec4, (sizeof(glm::vec4) * 1));
		vertexArray->AddData(2, DataType::Vec4, (sizeof(glm::vec4) * 2));
		vertexArray->AddData(3, DataType::Vec4, (sizeof(glm::vec4) * 3));

		vertexArray->AddData(4, DataType::Vec3, offsetof(Vertex2D, Vertex2D::Position));
		vertexArray->AddData(5, DataType::Vec4, offsetof(Vertex2D, Vertex2D::Color));
		vertexArray->AddData(6, DataType::Vec3, offsetof(Vertex2D, Vertex2D::TexCoords));
		vertexArray->AddData(7, DataType::Float, offsetof(Vertex2D, Vertex2D::TexSlot));

		Count<Shader > shader = Renderer::GetShader("Base2D");
		//{
		//	auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
		//		.AddBinding((int)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
		//		.AddBinding((int)1, DescriptorType::ImageSampler, ShaderStage::Fragment, 32)
		//		.Build();
		//	Descriptors.insert({ DescriptorSets::Zero,descriptor });
		//}
		//PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] });

		


		GraphicsPipelineConfig graphicsPipelineConfig;
		graphicsPipelineConfig.DebugName = "Sprite Pipeline";
		graphicsPipelineConfig.Shader = shader;
		graphicsPipelineConfig.VertexArray = vertexArray;
		graphicsPipelineConfig.TargetBuffer = frameBuffer;
		GraphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);

		RenderPassConfig rednerPassConfig("Sprite RenderPass", frameBuffer);
		rednerPassConfig.Pipeline = GraphicsPipeline;
		RenderPass = RenderPass::Create(rednerPassConfig);
	}

	SpritePipeline::SpritePipeline(Count<FrameBuffer> frameBuffer, const std::string& shaderPath)
	{
		/*
		auto vertexArray = VertexArray::Create({ sizeof(Vertex2D) });
		vertexArray->AddData(0, DataType::Vec4, 0);
		vertexArray->AddData(1, DataType::Vec4, (sizeof(glm::vec4) * 1));
		vertexArray->AddData(2, DataType::Vec4, (sizeof(glm::vec4) * 2));
		vertexArray->AddData(3, DataType::Vec4, (sizeof(glm::vec4) * 3));

		vertexArray->AddData(4, DataType::Vec3, offsetof(Vertex2D, Vertex2D::Position));
		vertexArray->AddData(5, DataType::Vec4, offsetof(Vertex2D, Vertex2D::Color));
		vertexArray->AddData(6, DataType::Vec3, offsetof(Vertex2D, Vertex2D::TexCoords));
		vertexArray->AddData(7, DataType::Float, offsetof(Vertex2D, Vertex2D::TexSlot));


		Shader = Shader::GetOrCreate(Utils::FileDialogs::GetFileName(shaderPath), shaderPath);
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((int)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding((int)1, DescriptorType::ImageSampler, ShaderStage::Fragment, 32)
				.Build();
			Descriptors.insert({ DescriptorSets::Zero,descriptor });
		}
		PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] });
		GraphicsPipelineConfig graphicsPipelineConfig;
		graphicsPipelineConfig.DebugName = "Sprite";
		graphicsPipelineConfig.Shader = Shader;
		graphicsPipelineConfig.VertexArray = vertexArray;
		graphicsPipelineConfig.RenderPass = renderPass;
		//graphicsPipelineConfig.PipelineLayout = PipeLineLayout;
		//blendign is very expensive for rendierng 
		//graphicsPipelineConfig.Blend = true;
		GraphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);
		*/
	}

	TextPipeline::TextPipeline(Count<FrameBuffer> frameBuffer)
	{
		/*
		auto vertexArray = VertexArray::Create({ sizeof(TextVertex) });
		vertexArray->AddData(0, DataType::Vec3, offsetof(TextVertex, TextVertex::Positon));
		vertexArray->AddData(1, DataType::Vec4, offsetof(TextVertex, TextVertex::Color));
		vertexArray->AddData(2, DataType::Vec3, offsetof(TextVertex, TextVertex::TexCoord));
		Shader = Shader::GetOrCreate("Text", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Text.shader");
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((int)DescriptorSet0::CameraData, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding(1, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.Build();
			Descriptors.insert({ DescriptorSets::Zero,descriptor });
		}
		PipeLineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] });
		GraphicsPipelineConfig graphicsPipelineConfig;
		graphicsPipelineConfig.DebugName = "Text Vertex";
		graphicsPipelineConfig.Shader = Shader;
		graphicsPipelineConfig.VertexArray = vertexArray;
		graphicsPipelineConfig.RenderPass = renderPass;
		//graphicsPipelineConfig.PipelineLayout = PipeLineLayout;
		GraphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);
		*/
	}

}