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
#include "Shader.h"
#include "CommandBuffer.h"
#include "Font.h"
#include "Proof/Scene/Mesh.h"
#include "MSDFData.h"
#include "Proof/Math/MathInclude.h"
namespace Proof {

	static CameraData s_CurrentCamera;
	void Renderer2D::Init() {
		FrameBufferConfig framebufferSpec;
		framebufferSpec.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
		framebufferSpec.ClearColorOnLoad = false;
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		framebufferSpec.DebugName = "Renderer2D Framebuffer";
		m_FrameBuffer = FrameBuffer::Create(framebufferSpec);


		m_CommandBuffer = RenderCommandBuffer::Create("Renderer2D");
		m_UBCamera = UniformBufferSet::Create(sizeof(CameraData));

		m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		m_WhiteTexture = Renderer::GetWhiteTexture();

		for(int index = 0; index < c_MaxTextureSlots; index++)
			m_QuadTextures[index] = m_WhiteTexture;
		{
			auto vertexArray = VertexArray::Create({ sizeof(Vertex2D) });
			vertexArray->AddData(0, DataType::Vec3, offsetof(Vertex2D, Vertex2D::Position));
			vertexArray->AddData(1, DataType::Vec4, offsetof(Vertex2D, Vertex2D::Color));
			vertexArray->AddData(2, DataType::Vec3, offsetof(Vertex2D, Vertex2D::TexCoords));
			vertexArray->AddData(3, DataType::Float, offsetof(Vertex2D, Vertex2D::TexSlot));

			m_QuadVertexBufferBase = pnew Vertex2D[c_MaxVertexCount];
			m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

			m_QuadVertexBuffer = VertexBuffer::Create(c_MaxVertexCount * sizeof(Vertex2D));

			uint32_t* quadIndices = pnew uint32_t[c_MaxIndexCount];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < c_MaxIndexCount; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}

			m_IndexBuffer = IndexBuffer::Create(quadIndices, c_MaxIndexCount * sizeof(uint32_t));
			pdelete[] quadIndices;

			GraphicsPipelineConfiguration graphicsPipelineConfig;
			graphicsPipelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
			graphicsPipelineConfig.DebugName = "Sprite Pipeline";
			graphicsPipelineConfig.Shader = Renderer::GetShader("Base2D");
			graphicsPipelineConfig.VertexArray = vertexArray;
			graphicsPipelineConfig.CullMode = CullMode::None;
			//graphicsPipelineConfig.DepthTest = false;
			//graphicsPipelineConfig.WriteDepth = false;
			auto graphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);

			RenderPassConfig renderPassConfig;
			renderPassConfig.DebugName = "Sprite RenderPass";
			renderPassConfig.Pipeline = graphicsPipeline;
			renderPassConfig.TargetFrameBuffer = m_FrameBuffer;
			m_QuadPass = RenderPass::Create(renderPassConfig);
			m_QuadPass->SetInput("CameraData", m_UBCamera);

		}
		

		{

			for (int index = 0; index < c_MaxTextureSlots; index++)
				m_FontTextures[index] = Font::GetDefault();
		
			auto vertexArray = VertexArray::Create({ sizeof(TextVertex) });
			vertexArray->AddData(0, DataType::Vec3, offsetof(TextVertex, TextVertex::Positon));
			vertexArray->AddData(1, DataType::Vec4, offsetof(TextVertex, TextVertex::Color));
			vertexArray->AddData(2, DataType::Vec3, offsetof(TextVertex, TextVertex::TexCoord));
			vertexArray->AddData(3, DataType::Float, offsetof(TextVertex, TextVertex::FontIndex));
			vertexArray->AddData(4, DataType::Float, offsetof(TextVertex, TextVertex::TexIndex));


			m_TextVertexBufferBase = pnew TextVertex[c_MaxVertexCount];
			m_TextVertexBufferPtr = m_TextVertexBufferBase;

			m_TextVertexBuffer = VertexBuffer::Create(c_MaxVertexCount * sizeof(Vertex2D));

			GraphicsPipelineConfiguration graphicsPipelineConfig;
			graphicsPipelineConfig.DebugName = "Text Pipeline";
			graphicsPipelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
			graphicsPipelineConfig.Shader = Renderer::GetShader("Text2D");
			graphicsPipelineConfig.VertexArray = vertexArray;
			graphicsPipelineConfig.CullMode = CullMode::None;
			auto graphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);

			RenderPassConfig renderPassConfig("Text RenderPass");
			renderPassConfig.Pipeline = graphicsPipeline;
			renderPassConfig.TargetFrameBuffer = m_FrameBuffer;
			m_TextPass = RenderPass::Create(renderPassConfig);
			m_TextPass->SetInput("CameraData", m_UBCamera);
		}
		//
		{
			auto vertexArray = VertexArray::Create({ sizeof(LineVertex) });
			vertexArray->AddData(0, DataType::Vec3, offsetof(LineVertex, LineVertex::Position));
			vertexArray->AddData(1, DataType::Vec4, offsetof(LineVertex, LineVertex::Color));

			GraphicsPipelineConfiguration graphicsPipelineConfig;
			graphicsPipelineConfig.DebugName = "Line";
			graphicsPipelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
			graphicsPipelineConfig.Shader = Renderer::GetShader("Line2D");
			graphicsPipelineConfig.VertexArray = vertexArray;
			graphicsPipelineConfig.DrawMode = DrawType::Line;
			graphicsPipelineConfig.LineWidth = 2.0f;
			auto graphicsPipeline = GraphicsPipeline::Create(graphicsPipelineConfig);

			RenderPassConfig renderPassConfig("Line");
			renderPassConfig.Pipeline = graphicsPipeline;
			renderPassConfig.TargetFrameBuffer = m_FrameBuffer;
			m_LinePass = RenderPass::Create(renderPassConfig);
			m_LinePass->SetInput("CameraData", m_UBCamera);

			uint32_t* lineIndices = pnew uint32_t[c_MaxLineIndices];
			for (uint32_t i = 0; i < c_MaxLineIndices; i++)
				lineIndices[i] = i;
			m_LineIndexBuffer = IndexBuffer::Create(lineIndices, c_MaxLineIndices);
			pdelete[] lineIndices;

			m_LineVertexBufferBase = pnew LineVertex[c_MaxLineVertices];
			m_LineVertexBufferPtr = m_LineVertexBufferBase;

			m_LineVertexBuffer = VertexBuffer::Create(c_MaxLineVertices * sizeof(LineVertex));
		}
	}
	
	void Renderer2D::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position) {
		PF_PROFILE_FUNC()
		CameraData camera = CameraData{ projection,view,Position };
		//CameraData camera = CameraData{ glm::mat4(1),glm::mat4(1)};

		Buffer buffer(&camera, sizeof(CameraData));
		m_UBCamera->SetData(Renderer::GetCurrentFrame().FrameinFlight, buffer);
		m_Stats = {};
		Renderer::BeginCommandBuffer(m_CommandBuffer);


	}
	Renderer2D::Renderer2D()
	{
		Init();
	}
	Renderer2D::~Renderer2D()
	{
		pdelete[] m_QuadVertexBufferBase;
		pdelete[] m_TextVertexBufferBase;
		pdelete[] m_LineVertexBufferBase;
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
			DrawQuad(transform.Location, transform.GetRotationEuler(), transform.Scale,
				glm::vec4{ Sprite.Colour }, Sprite.Texture);
		}
		else
		{
			DrawQuad( transform.Location,transform.GetRotationEuler(), transform.Scale, glm::vec4{Sprite.Colour}, m_WhiteTexture);
		}
	}
	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		if (m_LineIndexCount >= c_MaxLineIndices)
		{
			Render();
			Reset();
		}

		m_LineVertexBufferPtr->Position = p0;
		m_LineVertexBufferPtr->Color = color;
		m_LineVertexBufferPtr++;

		m_LineVertexBufferPtr->Position = p1;
		m_LineVertexBufferPtr->Color = color;
		m_LineVertexBufferPtr++;

		m_LineIndexCount += 2;

	}
	void Renderer2D::DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec4 min = { aabb.Min.x, aabb.Min.y, aabb.Min.z, 1.0f };
		glm::vec4 max = { aabb.Max.x, aabb.Max.y, aabb.Max.z, 1.0f };

		glm::vec4 corners[8] =
		{
			transform * glm::vec4 { aabb.Min.x, aabb.Min.y, aabb.Max.z, 1.0f },
			transform * glm::vec4 { aabb.Min.x, aabb.Max.y, aabb.Max.z, 1.0f },
			transform * glm::vec4 { aabb.Max.x, aabb.Max.y, aabb.Max.z, 1.0f },
			transform * glm::vec4 { aabb.Max.x, aabb.Min.y, aabb.Max.z, 1.0f },

			transform * glm::vec4 { aabb.Min.x, aabb.Min.y, aabb.Min.z, 1.0f },
			transform * glm::vec4 { aabb.Min.x, aabb.Max.y, aabb.Min.z, 1.0f },
			transform * glm::vec4 { aabb.Max.x, aabb.Max.y, aabb.Min.z, 1.0f },
			transform * glm::vec4 { aabb.Max.x, aabb.Min.y, aabb.Min.z, 1.0f }
		};

		for (uint32_t i = 0; i < 4; i++)
			DrawLine(corners[i], corners[(i + 1) % 4], color);

		for (uint32_t i = 0; i < 4; i++)
			DrawLine(corners[i + 4], corners[((i + 1) % 4) + 4], color);

		for (uint32_t i = 0; i < 4; i++)
			DrawLine(corners[i], corners[i + 4], color);
	
	}
	void Renderer2D::DrawAABB(Count<class Mesh> mesh, const glm::mat4& transform, const glm::vec4& color)
	{
		AABB box = mesh->GetMeshSource()->GetBoundingBox();

		DrawAABB(box, transform,color);
	}

	void Renderer2D::DrawAABBSubMeshes(Count<class Mesh> mesh, const glm::mat4& transform, const glm::vec4& color)
	{
		const auto& meshAssetSubmeshes = mesh->GetMeshSource()->GetSubMeshes();
		auto& submeshes = mesh->GetSubMeshes();
		for (uint32_t submeshIndex : submeshes)
		{
			const SubMesh& submesh = meshAssetSubmeshes[submeshIndex];
			auto& aabb = submesh.BoundingBox;
			auto aabbTransform = transform * submesh.Transform;
			//auto aabbTransform = transform;
			DrawAABB(aabb, aabbTransform,color);
		}
	}
	void Renderer2D::DrawQuad(const glm::vec3& Location,const glm::vec3& Rotation, const glm::vec3& Size,const glm::vec4& Color,const Count<Texture2D>& texture2D)
	{
		glm::mat4 transform =glm::translate(glm::mat4(1.0f), Location)
			* glm::toMat4(glm::quat(Rotation))
			* glm::scale(glm::mat4(1.0f), { Size.x,Size.y,1.0 });

		DrawQuad(transform,Color,texture2D);
		
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& Color, const Count<Texture2D>& texture)
	{
		constexpr size_t quadVertexCount = 4;
		//constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } };

		if (m_QuadIndexCount >= c_MaxIndexCount)
		{ // reached maxed index size
			Render();
			Reset();
		}

		float textureIndex = -1.f;
		for (uint32_t i = 0; i < m_QuadTextureSlotIndex; i++)
		{
			if (m_QuadTextures[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.f)
		{
			if (m_QuadTextureSlotIndex >= c_MaxTextureSlots)
			{
				Render();
				Reset();
			}

			textureIndex = (float)m_QuadTextureSlotIndex;
			m_QuadTextures[m_QuadTextureSlotIndex] = texture;
			m_QuadTextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			glm::vec3 pos = transform * m_QuadVertexPositions[i];
			m_QuadVertexBufferPtr->Position = GlmVecToProof( pos);
			m_QuadVertexBufferPtr->Color = Color;
			m_QuadVertexBufferPtr->TexCoords = textureCoords[i];
			m_QuadVertexBufferPtr->TexSlot = textureIndex;
			//m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			//m_QuadVertexBufferPtr->EntityID = entityID;
			m_QuadVertexBufferPtr++;
		}

		m_QuadIndexCount += 6;

	}
	void Renderer2D::DrawString(const std::string& text, Count<class Font> font, const TextParams& textParam, const glm::mat4& transform)
	{
		if (m_TextIndexCount>= c_MaxIndexCount)
		{ // reached maxed index size
			Render();
			Reset();
		}
		//https://freetype.org/freetype2/docs/tutorial/step2.html
		const auto& fontGeometry =  font->GetMSDFData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Count<Texture2D> fontAtlas = font->GetAtlasTexture();

		//m_Storage2DData->FontTexture = fontAtlas;
		float fontIndex= -1.f;
		for (uint32_t i = 0; i < m_TextFontSlotIndex; i++)
		{
			if (m_FontTextures[i] == font)
			{
				fontIndex = (float)i;
				break;
			}
		}

		if (fontIndex == -1.f)
		{
			if (m_TextFontSlotIndex >= c_MaxTextureSlots)
			{
				Render();
				Reset();
			}

			fontIndex = (float)m_TextFontSlotIndex;
			m_FontTextures[m_TextFontSlotIndex] = font;
			m_TextFontSlotIndex++;
		}

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = -fsScale * metrics.ascenderY;
		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();
		for (size_t i = 0; i < text.size(); i++)
		{
			char character = text[i];

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

			m_TextVertexBufferPtr->Positon = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			m_TextVertexBufferPtr->Color = textParam.Color ;
			m_TextVertexBufferPtr->TexCoord = texCoordMin;
			m_TextVertexBufferPtr->FontIndex = fontIndex;
			m_TextVertexBufferPtr++;

			m_TextVertexBufferPtr->Positon = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			m_TextVertexBufferPtr->Color = textParam.Color;
			m_TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			m_TextVertexBufferPtr->FontIndex = fontIndex;
			m_TextVertexBufferPtr++;

			m_TextVertexBufferPtr->Positon = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			m_TextVertexBufferPtr->Color = textParam.Color;
			m_TextVertexBufferPtr->TexCoord = texCoordMax;
			m_TextVertexBufferPtr->FontIndex = fontIndex;
			m_TextVertexBufferPtr++;

			m_TextVertexBufferPtr->Positon = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			m_TextVertexBufferPtr->Color = textParam.Color;
			m_TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			m_TextVertexBufferPtr->FontIndex = fontIndex;
			m_TextVertexBufferPtr++;

			m_TextIndexCount += 6;

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

		Renderer::EndCommandBuffer(m_CommandBuffer);
		Renderer::SubmitCommandBuffer(m_CommandBuffer);
	}

	void Renderer2D::Reset() {
		//qud
		{

			m_QuadIndexCount = 0;
			m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
			// reseting every textures back to white that has been changed
			for (uint32_t i = 1; i < m_QuadTextureSlotIndex; i++)
				m_QuadTextures[i] = m_WhiteTexture;
			m_QuadTextureSlotIndex = 1;
		}

		// text
		{
			m_TextIndexCount = 0;
			m_TextVertexBufferPtr = m_TextVertexBufferBase;
			for (uint32_t i = 1; i < m_TextFontSlotIndex; i++)
			{
				m_FontTextures[i] = Font::GetDefault();
			}
			m_TextFontSlotIndex = 1;
		}

		//line
		{
			m_LineIndexCount = 0;
			m_LineVertexBufferPtr = m_LineVertexBufferBase;

		}
	}
	
	void Renderer2D::Render() {
		PF_PROFILE_FUNC();

		Timer renderTime;
		
		if (m_QuadIndexCount > 0) // nothing to draw
		{
			PF_PROFILE_FUNC("Renderer2D::Quad Draw");
			
			Timer quadTime;
			uint32_t dataSize = (uint32_t)((uint8_t*)m_QuadVertexBufferPtr - (uint8_t*)m_QuadVertexBufferBase);
			m_QuadVertexBuffer->SetData(m_QuadVertexBufferBase, dataSize);

			std::vector<Count<Texture2D>> textureVec;
			textureVec.resize(m_QuadTextures.size());
			for (uint32_t i = 0; i < m_QuadTextures.size(); i++)
			{
				textureVec[i] = m_QuadTextures[i];
			}
			//std::vector<Count<Texture2D>> textureVec(m_QuadTextures.begin(), m_QuadTextures.end());

			m_QuadPass->SetInput("u_Textures", textureVec);
		

			Renderer::BeginRenderPass(m_CommandBuffer, m_QuadPass);
			m_IndexBuffer->Bind(m_CommandBuffer);
			m_QuadVertexBuffer->Bind(m_CommandBuffer);
			Renderer::DrawElementIndexed(m_CommandBuffer, m_QuadIndexCount);
			Renderer::EndRenderPass(m_QuadPass);

			m_Stats.QuadDrawTime += quadTime.ElapsedMillis();
		}
		if (m_TextIndexCount > 0)
		{
			PF_PROFILE_FUNC("Renderer2D::String Draw");
			Timer textTime;
			uint32_t dataSize = (uint32_t)((uint8_t*)m_TextVertexBufferPtr - (uint8_t*)m_TextVertexBufferBase);
			m_TextVertexBuffer->SetData(m_TextVertexBufferBase, dataSize);

			std::vector<Count<Texture2D>> textureVec;
			textureVec.resize(m_FontTextures.size());
			for (uint32_t i = 0; i < textureVec.size(); i++)
			{
				textureVec[i] = m_FontTextures[i]->GetAtlasTexture();
			}
			m_TextPass->SetInput("u_FontAtlas", textureVec);

			Renderer::BeginRenderPass(m_CommandBuffer, m_TextPass);
			m_IndexBuffer->Bind(m_CommandBuffer);
			m_TextVertexBuffer->Bind(m_CommandBuffer);

			Renderer::DrawElementIndexed(m_CommandBuffer, m_TextIndexCount);
			Renderer::EndRenderPass(m_TextPass);

			m_Stats.TextDrawTime += textTime.ElapsedMillis();

		}

		if (m_LineIndexCount > 0)
		{
			PF_PROFILE_FUNC("Renderer2D::Line Draw");
			Timer lineTime;
			uint32_t dataSize = (uint32_t)((uint8_t*)m_LineVertexBufferPtr - (uint8_t*)m_LineVertexBufferBase);

			m_LineVertexBuffer->SetData(m_LineVertexBufferBase, dataSize);

			Renderer::BeginRenderPass(m_CommandBuffer, m_LinePass);
			m_LineIndexBuffer->Bind(m_CommandBuffer);
			m_LineVertexBuffer->Bind(m_CommandBuffer);
			
			Renderer::DrawElementIndexed(m_CommandBuffer, m_LineIndexCount);
			Renderer::EndRenderPass(m_LinePass);
		}

		m_Stats.TotalRenderTime += renderTime.ElapsedMillis();
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
		Count<IndexBuffer> indexBuffer = IndexBuffer::Create(indices.data(), indices.size() * sizeof(uint32_t));
		return std::make_pair(buffer, indexBuffer);
	}
	void Renderer2D::SetTargetFrameBuffer(Count<class FrameBuffer> framebuffer)
	{
		if (m_FrameBuffer == framebuffer)
			return;
		m_FrameBuffer = framebuffer;
		{
			m_QuadPass->SetTargetFrameBuffer(m_FrameBuffer);
			m_TextPass->SetTargetFrameBuffer(m_FrameBuffer);
			m_LinePass->SetTargetFrameBuffer(m_FrameBuffer);
		}
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
		IndexBuffer = IndexBuffer::Create(QuadIndices.data(), c_MaxIndexCount * sizeof(uint32_t));
		CameraBuffer = UniformBufferSet::Create(sizeof(CameraData));

		TextVertexBuffer = VertexBuffer::Create(c_MaxVertexCount * sizeof(TextVertex));
		uint32_t whiteTextureData = 0xffffffff;
		TextureConfiguration textrureConfig;

		//Buffer buffer()
		Textures.resize(Renderer2DStorage::c_MaxTextureSlot);

		WhiteTexture = Renderer::GetWhiteTexture();

		for(uint32_t i=0; i < Renderer2DStorage::c_MaxTextureSlot;i++)
			Textures[i] = WhiteTexture;
	}
}