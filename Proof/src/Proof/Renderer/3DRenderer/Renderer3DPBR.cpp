#include "Proofprch.h"
#include "Renderer3DPBR.h"
#include "Renderer3DCore.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Renderer.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include "Proof/Core/FrameTime.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "../UniformBuffer.h"
#include "Proof/Scene/Component.h"
#include "Platform/OpenGL/OpenGLRenderer/OpenGLRenderer3DPBR.h"
#include "Platform/Vulkan/VulkanRenderer/VulkanRenderer.h"
namespace Proof{

	void Renderer3DPBR::Init() {
		PF_PROFILE_FUNC();

		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::Init();
		else
			VulkanRenderer::Init();
	}
	void Renderer3DPBR::BeginContext(EditorCamera& editorCamera,Count<ScreenFrameBuffer>& frameBuffer,RendererData& renderSpec) {
		BeginContext(editorCamera.m_Projection, editorCamera.m_View, editorCamera.m_Positon, frameBuffer, renderSpec);
	}
	void Renderer3DPBR::BeginContext(const glm::mat4& projection,const glm::mat4& view,const Vector<float>& Position,Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 1.0f,TimerTypes::Renderer);
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::BeginContext(projection,view,Position, frameBuffer, renderSpec);
	}

	void Renderer3DPBR::Draw(class MeshComponent& meshComponent, const glm::mat4& positionMatrix) {
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Mesh ID",meshComponent.GetMeshAssetID());

		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::Draw(meshComponent,positionMatrix);
	}
	void Renderer3DPBR::Draw(class LightComponent& lightComponent, class TransformComponent& transform) {
		PF_PROFILE_FUNC();
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::Draw(lightComponent,transform);
	}
	void Renderer3DPBR::DrawDebugMesh(Mesh* mesh, const glm::mat4& transform){
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::DrawDebugMesh(mesh, transform);
	}
	PhysicalBasedRenderer* Renderer3DPBR::GetRenderer(){
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			return OpenGLRenderer3DPBR::GetRenderer();
		return nullptr;
	}
	void Renderer3DPBR::EndContext() {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__,0,TimerTypes::Renderer);
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::EndContext();
	}
	void Renderer3DPBR::Reset() {
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			OpenGLRenderer3DPBR::Reset();
	}

	void Renderer3DPBR::Destroy() {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			VulkanRenderer::Destroy();
	}
		
	DeferedRenderingData::DeferedRenderingData() {
		MeshShader = Shader::GetOrCreate("MeshShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/MeshGeometry.glsl");
		LightShader = Shader::GetOrCreate("LightShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/LighteningPass.glsl");
		Gbuffer = FrameBuffer::Create();
		Gbuffer->Bind();
		GPosition = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::Float,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,0,GPosition->GetID());

		GNormal = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::Float,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,1,GNormal->GetID());

		GAlbedo = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::UnsignedByte,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,2,GAlbedo->GetID());

		GMaterial = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::UnsignedByte,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,3,GMaterial->GetID());

		unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4,attachments);
		RenderBuffer = RenderBuffer::Create(RenderBufferAttachment::DepthComponent,CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight());
		Gbuffer->AttachRenderBuffer(FrameBufferAttachmentType::DepthAttachment,RenderBuffer->GetID());
		Gbuffer->UnBind();
	}
	DeferedRenderingData::DeferedRenderingData(uint32_t width, uint32_t height) {
		MeshShader = Shader::GetOrCreate("MeshShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/MeshGeometry.glsl");
		LightShader = Shader::GetOrCreate("LightShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/LighteningPass.glsl");
		Gbuffer = FrameBuffer::Create();
		Gbuffer->Bind();
		GPosition = Texture2D::Create(width, height, DataFormat::RGBA, InternalFormat::RGBA16F, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, type::Float, false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D, 0, GPosition->GetID());

		GNormal = Texture2D::Create(width, height, DataFormat::RGBA, InternalFormat::RGBA16F, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, type::Float, false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D, 1, GNormal->GetID());

		GAlbedo = Texture2D::Create(width, height, DataFormat::RGBA, InternalFormat::RGBA, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, type::UnsignedByte, false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D, 2, GAlbedo->GetID());

		GMaterial = Texture2D::Create(width, height, DataFormat::RGBA, InternalFormat::RGBA16F, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, TextureBaseTypes::Nearest, type::UnsignedByte, false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D, 3, GMaterial->GetID());

		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
		RenderBuffer = RenderBuffer::Create(RenderBufferAttachment::DepthComponent,width, height);
		Gbuffer->AttachRenderBuffer(FrameBufferAttachmentType::DepthAttachment, RenderBuffer->GetID());
		Gbuffer->UnBind();
	}

	FowardRenderingData::FowardRenderingData() {
		m_Shader = Shader::GetOrCreate("FowardRendererShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/FowardRender/Mesh.glsl");
	}

}