#include "Proofprch.h"
#include "OpenGLRenderer3DPBR.h"
#include "Proof/Renderer/Shader.h"

#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/3DRenderer/Renderer3DCore.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/RendererBase.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include "Proof/Core/FrameTime.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Scene/Component.h"
namespace Proof
{
	static Count<Shader>s_DebugShader;
	Count<class Texture2D>PhysicalBasedRenderer::m_WhiteTexture;
	bool OpenGLRenderer3DPBR::s_InsideContext = false;
	Count<ScreenFrameBuffer> OpenGLRenderer3DPBR::s_RenderFrameBuffer = nullptr;
	RendererData* OpenGLRenderer3DPBR::s_RendererData = nullptr;
	static uint32_t Temp2 = (sizeof(glm::vec4));
	static uint32_t Temp3 = (sizeof(glm::vec4) * 2);
	static uint32_t Temp4 = (sizeof(glm::vec4) * 3);
	static uint32_t Temp5 = (sizeof(glm::vec4) * 4);
	static PhysicalBasedRenderer* s_PBRInstance;
	static glm::mat4 ModelMatrix;
	static Material s_DefaultMaterial;
	static std::vector<uint32_t> s_DifferentID;
	static Count<Shader> s_CurrentLightShader;
	uint32_t NumDirLights = 0;
	uint32_t NumPointLights = 0;
	uint32_t NumSpotLights = 0;
	uint32_t NumLights = 0;
	static CameraData s_CurrentCamera;
	static DrawType s_WorldDrawType = DrawType::Triangles;
	static glm::mat4 s_Projection;
	static glm::mat4 s_ViewPosition;
	void OpenGLRenderer3DPBR::Init() {
		s_PBRInstance = new PhysicalBasedRenderer();
		s_PBRInstance->m_Shader = Shader::GetOrCreate("NewPBRSHADER", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/PhysicalBasedRenderer.glsl");
		s_PBRInstance->m_VertexBuffer = VertexBuffer::Create(PhysicalBaseRendererGuide::s_MaxMesh * sizeof(PhysicalBasedRendererVertex));// can only store that amount of transforms
		s_PBRInstance->m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTextureImage = 0xffffffff;
		s_PBRInstance->m_WhiteTexture->SetData(&WhiteTextureImage, sizeof(uint32_t));
		s_DebugShader = Shader::GetOrCreate("DebugShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/DebugMesh.glsl");
	}

	void OpenGLRenderer3DPBR::BeginContext(EditorCamera& editorCamera, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		BeginContext(editorCamera.m_Projection, editorCamera.m_View, editorCamera.m_Positon, frameBuffer, renderSpec);
	}
	void OpenGLRenderer3DPBR::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		s_CurrentCamera = { projection,view,Position };
		Renderer3DCore::s_CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));
		PF_CORE_ASSERT(s_InsideContext == false, "Cannot start a new Render Context if Previous Render COntext is not closed");
		s_InsideContext = true;
		s_RenderFrameBuffer = frameBuffer;
		s_RendererData = &renderSpec;
		s_RendererData->Reset();
		if (s_RendererData->RenderSettings.Technique == RenderTechnique::DeferedRendering)
			InitilizeDeferedRendering();
		else if (s_RendererData->RenderSettings.Technique == RenderTechnique::FowardRendering)
			InitilizeFowardRendering();
		s_Projection = projection;
		s_ViewPosition = view;
	}
	void OpenGLRenderer3DPBR::Draw(MeshComponent& meshComponent, const glm::mat4& positionMatrix) {
		#if 0
		int meshPointerId = meshComponent.GetMeshAssetID();
		if (meshPointerId == 0)return; // means that therer is no mesh attahced
		int usingMaterial = meshComponent.HasMaterial();
		if (s_PBRInstance->SceneHasAmountMeshes(meshPointerId) == true) {
			const auto& Map = s_PBRInstance->m_AmountMeshes.find(meshPointerId);
			Map->second += 1;
			auto InstanceSize = s_PBRInstance->m_MeshesEndingPositionIndexTransforms.find(meshPointerId);

			ModelMatrix = positionMatrix;
			PhysicalBasedRendererVertex temp(ModelMatrix, meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : s_DefaultMaterial, usingMaterial);
			s_PBRInstance->m_Transforms.insert(s_PBRInstance->m_Transforms.begin() + InstanceSize->second, temp);
			InstanceSize->second++;
			return;
		}

		s_PBRInstance->m_AmountMeshes.insert({ meshPointerId,1 });
		s_PBRInstance->m_Meshes.insert({ meshPointerId,meshComponent });
		s_PBRInstance->m_MeshesEndingPositionIndexTransforms.insert({ meshPointerId,s_PBRInstance->m_Transforms.size() + 1 });
		s_DifferentID.emplace_back(meshPointerId);
		ModelMatrix = positionMatrix;

		PhysicalBasedRendererVertex temp(positionMatrix, meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : s_DefaultMaterial, usingMaterial);
		s_PBRInstance->m_Transforms.emplace_back(temp);
		#endif
	}
	void OpenGLRenderer3DPBR::DrawDebugMesh(Mesh* mesh, const glm::mat4& transform) {
		if (mesh == nullptr)return;
		s_DebugShader->Bind();
		s_DebugShader->SetMat4("model", transform);
		s_DebugShader->SetMat4("projection", s_Projection);
		s_DebugShader->SetMat4("view", s_ViewPosition);
		for (SubMesh& subeMeshes : mesh->meshes) {
			subeMeshes.m_VertexArrayObject->Bind();
			subeMeshes.m_IndexBufferObject->Bind();
			//glPolygonMode(GL_FRONT_AND_BACK, (int)GL_LINES);
			Renderer::DrawElementIndexed(subeMeshes.m_VertexArrayObject, 1, DrawType::Lines);
		}
	}
	void OpenGLRenderer3DPBR::SubmitDirectionalLight(const DirectionalLightComponent& lightComponent, class TransformComponent& transform){
		s_CurrentLightShader->Bind();
		std::string mumberDirectionalLightstring = "v_DirectionalLight[" + std::to_string(NumDirLights) + "]";
		//s_CurrentLightShader->SetVec3(mumberDirectionalLightstring + ".Direction", transform.GetWorldRotation());
		s_CurrentLightShader->SetVec3(mumberDirectionalLightstring + ".Ambient", lightComponent.Color);
		s_CurrentLightShader->SetFloat(mumberDirectionalLightstring + ".Intensity", lightComponent.Intensity);
		NumDirLights++;
		s_CurrentLightShader->UnBind();

	}
	void OpenGLRenderer3DPBR::SubmitPointLight(class PointLightComponent& lightComponent, TransformComponent& transform) {
		s_CurrentLightShader->Bind();
		std::string numberPointLightstring = "v_PointLight[" + std::to_string(NumPointLights) + "]";
		//s_CurrentLightShader->SetVec3(numberPointLightstring + ".Position", transform.GetWorldLocation());
		s_CurrentLightShader->SetVec3(numberPointLightstring + ".Ambient", lightComponent.Color);
		s_CurrentLightShader->SetFloat(numberPointLightstring + +".Constant", lightComponent.Constant);
		s_CurrentLightShader->SetFloat(numberPointLightstring + +".Linear", lightComponent.Linear);
		s_CurrentLightShader->SetFloat(numberPointLightstring + +".Quadratic", lightComponent.Quadratic);
		s_CurrentLightShader->SetFloat(numberPointLightstring + +".Radius", lightComponent.Radius);
		s_CurrentLightShader->SetFloat(numberPointLightstring + ".Intensity", lightComponent.Intensity);
		NumPointLights++;
		s_CurrentLightShader->UnBind();
	}
	void OpenGLRenderer3DPBR::SubmitSpotLight(class SpotLightComponent& lightComponent, TransformComponent& transform) {
		s_CurrentLightShader->Bind();
		std::string numberSpotLightstring = "v_SpotLight[" + std::to_string(NumSpotLights) + "]";
		//s_CurrentLightShader->SetVec3(numberSpotLightstring + ".Direction", { transform.GetWorldRotation() });
		//s_CurrentLightShader->SetVec3(numberSpotLightstring + ".Position", { transform.GetWorldLocation() });
		s_CurrentLightShader->SetVec3(numberSpotLightstring + ".Ambient", lightComponent.Color);
		s_CurrentLightShader->SetFloat(numberSpotLightstring +".Constant", lightComponent.Constant);
		s_CurrentLightShader->SetFloat(numberSpotLightstring +".Linear", lightComponent.Linear);
		s_CurrentLightShader->SetFloat(numberSpotLightstring +".Quadratic", lightComponent.Quadratic);
		s_CurrentLightShader->SetFloat(numberSpotLightstring + ".Radius", lightComponent.Radius);
		s_CurrentLightShader->SetFloat(numberSpotLightstring + ".Intensity", lightComponent.Intensity);
		s_CurrentLightShader->SetFloat(numberSpotLightstring + ".CutOff", glm::cos(glm::radians(lightComponent.CutOff)));
		s_CurrentLightShader->SetFloat(numberSpotLightstring + ".OuterCutOff", glm::cos(glm::radians(lightComponent.OuterCutOff)));
		NumSpotLights++;
		s_CurrentLightShader->UnBind();
	}
	PhysicalBasedRenderer* OpenGLRenderer3DPBR::GetRenderer() {
		return  s_PBRInstance;
	}
	void OpenGLRenderer3DPBR::EndContext() {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 0, TimerTypes::RendererBase);
		Render();
		Reset();
		s_RenderFrameBuffer->UnBind();
		s_RenderFrameBuffer = nullptr;
		s_InsideContext = false;
		s_RendererData = nullptr;
		s_PBRInstance->m_Shader->UnBind();
	}
	void OpenGLRenderer3DPBR::Reset() {
		s_RendererData->Stats.AmountDirectionalLight += NumDirLights;
		s_RendererData->Stats.AmountPointLight += NumPointLights;
		s_RendererData->Stats.AmountSpotLight += NumSpotLights;
		s_RendererData->Stats.AmountLight = s_RendererData->Stats.AmountDirectionalLight + s_RendererData->Stats.AmountPointLight + s_RendererData->Stats.AmountSpotLight;
		s_RendererData->Stats.Instances += s_DifferentID.size();

		s_PBRInstance->m_AmountMeshes.clear();
		s_PBRInstance->m_Meshes.clear();
		s_PBRInstance->m_MeshesEndingPositionIndexTransforms.clear();
		s_DifferentID.clear();
		s_PBRInstance->m_Transforms.clear();
		NumDirLights = 0;
		NumPointLights = 0;
		NumSpotLights = 0;
		NumLights = 0;
		s_CurrentLightShader = nullptr;
	}

	unsigned int quadVAO1 = 0;
	unsigned int quadVBO1;
	void renderQuad11() {
		if (quadVAO1 == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,1.0f,0.0f,0.0f,1.0f,
				-1.0f,-1.0f,0.0f,0.0f,0.0f,
				1.0f,1.0f,0.0f,1.0f,1.0f,
				1.0f,-1.0f,0.0f,1.0f,0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO1);
			glGenBuffers(1, &quadVBO1);
			glBindVertexArray(quadVAO1);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO1);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	void OpenGLRenderer3DPBR::Render() {
		PF_PROFILE_FUNC()
		PF_SCOPE_TIME_THRESHHOLD_TYPE(__FUNCTION__, 0, TimerTypes::RendererBase);
		if (s_RendererData->RenderSettings.Technique == RenderTechnique::DeferedRendering)
			DeferedRender();
		else
			FowardRenderer();
	}

	void OpenGLRenderer3DPBR::InitilizeDeferedRendering() {
		s_CurrentLightShader = s_PBRInstance->m_DeferedRendering.LightShader;
	}

	void OpenGLRenderer3DPBR::DeferedRender() {
		DeferedRendererRenderMesh();
		DeferedRendererRenderLight();
		//if (s_DifferentID.size() == 0)
		//	return;
		//s_RenderFrameBuffer->Bind();
		//s_PBRInstance->m_DeferedRendering.Gbuffer->Bind();
		s_PBRInstance->m_DeferedRendering.Gbuffer->WriteBuffer(s_RenderFrameBuffer->GetFrameBufferID());
		//s_RenderFrameBuffer->Bind();
		glBlitFramebuffer(0, 0, s_RenderFrameBuffer->GetFrameWidth(), s_RenderFrameBuffer->GetFrameHeight(), 0, 0, s_RenderFrameBuffer->GetFrameHeight(), s_RenderFrameBuffer->GetFrameWidth(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		s_RenderFrameBuffer->UnBind();
		//s_RendererData->DeferedData.m_PositionTexture = GetRenderer()->m_DeferedRendering.GPosition->GetID();
		//s_RendererData->DeferedData.m_NormalTexture = GetRenderer()->m_DeferedRendering.GNormal->GetID();
		//s_RendererData->DeferedData.m_AlbedoTexture = GetRenderer()->m_DeferedRendering.GAlbedo->GetID();
	}
	void OpenGLRenderer3DPBR::DeferedRendererRenderLight() {
		if (NumDirLights == 0 && NumSpotLights == 0 && NumPointLights == 0)return;
		s_RenderFrameBuffer->Bind();
		Renderer::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		s_PBRInstance->m_DeferedRendering.LightShader->Bind();

		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gPosition", 0);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gNormal", 1);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gAlbedoSpec", 2);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gMaterialSpec", 3);

		s_PBRInstance->m_DeferedRendering.GPosition->Bind(0);
		s_PBRInstance->m_DeferedRendering.GNormal->Bind(1);
		s_PBRInstance->m_DeferedRendering.GAlbedo->Bind(2);
		s_PBRInstance->m_DeferedRendering.GMaterial->Bind(3);

		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("v_NrDirectionalLight", NumDirLights);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("v_NrPointLight", NumPointLights);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("v_NrSpotLight", NumSpotLights);
		renderQuad11();
		s_RenderFrameBuffer->UnBind();
	}

	void OpenGLRenderer3DPBR::DeferedRendererRenderMesh() {
		if (s_DifferentID.size() == 0)return;
		s_RenderFrameBuffer->Bind();
		Renderer::SetClearColor();
		Renderer::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		s_RenderFrameBuffer->UnBind();

		s_PBRInstance->m_DeferedRendering.Gbuffer->Bind();
		Renderer::SetViewPort(s_RenderFrameBuffer->GetFrameWidth(), s_RenderFrameBuffer->GetFrameHeight());
		Renderer::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		uint32_t sizeOffset = 0;
		s_PBRInstance->m_DeferedRendering.MeshShader->Bind();
		for (uint32_t i = 0; i < s_DifferentID.size(); i++) {
			uint32_t TempID = s_DifferentID[i];
			const auto& TempMesh = s_PBRInstance->m_Meshes.find(TempID);
			const auto& TempAmountMeshes = s_PBRInstance->m_AmountMeshes.find(TempID);
			s_PBRInstance->m_VertexBuffer->Bind();
			s_PBRInstance->m_VertexBuffer->AddData(&s_PBRInstance->m_Transforms[sizeOffset], TempAmountMeshes->second * sizeof(PhysicalBasedRendererVertex));

			if (TempMesh->second.GetMaterial() != nullptr) {

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("albedoMap", 0);
				if (TempMesh->second.GetMaterial()->AlbedoTexture != nullptr)
					TempMesh->second.GetMaterial()->AlbedoTexture->Bind(0);
				else
					s_PBRInstance->m_WhiteTexture->Bind(0);

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("normalMap", 1);
				if (TempMesh->second.GetMaterial()->NormalTexture != nullptr)
					TempMesh->second.GetMaterial()->NormalTexture->Bind(1);
				else
					s_PBRInstance->m_WhiteTexture->Bind(1);

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("metallicMap", 2);
				if (TempMesh->second.GetMaterial()->MetallicTexture != nullptr)
					TempMesh->second.GetMaterial()->MetallicTexture->Bind(2);
				else
					s_PBRInstance->m_WhiteTexture->Bind(2);

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("roughnessMap", 3);
				if (TempMesh->second.GetMaterial()->RoughnessTexture != nullptr)
					TempMesh->second.GetMaterial()->RoughnessTexture->Bind(3);
				else
					s_PBRInstance->m_WhiteTexture->Bind(3);
			}
			else {
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("albedoMap", 0);
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("normalMap", 1);
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("metallicMap", 2);
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("roughnessMap", 3);
				s_PBRInstance->m_WhiteTexture->Bind(0);
				s_PBRInstance->m_WhiteTexture->Bind(1);
				s_PBRInstance->m_WhiteTexture->Bind(2);
				s_PBRInstance->m_WhiteTexture->Bind(3);
			}


			if (TempMesh->second.GetMeshSource()->m_FaceCulling == true)
				Renderer::Enable(ProofRenderTest::CullFace);
			if (TempMesh->second.GetMeshSource()->m_Enabled == true) {
				#if 0
				for (SubMesh& mesh : TempMesh->second.GetMeshSource()->meshes) {
					if (mesh.m_Enabled == false)
						continue;

					if (TempMesh->second.HasMaterial() == false) {
						s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("DiffuseShader", 7);
						if (mesh.m_Textures.size() > 0)
							mesh.m_Textures[0]->Bind(7);
						else
							s_PBRInstance->m_WhiteTexture->Bind(7);
					}

					s_RendererData->Stats.DrawCalls++;
					mesh.m_VertexArrayObject->Bind();
					mesh.m_IndexBufferObject->Bind();
					Renderer::DrawElementIndexed(mesh.m_VertexArrayObject, TempAmountMeshes->second, s_RendererData->RenderSettings.Draw);
				}
				#endif
			}
			if (TempMesh->second.GetMeshSource()->m_FaceCulling == true)
				Renderer::Disable(ProofRenderTest::CullFace); // rename to render settings
			sizeOffset += TempAmountMeshes->second;
		}
		s_PBRInstance->m_DeferedRendering.MeshShader->UnBind();
		s_PBRInstance->m_DeferedRendering.Gbuffer->UnBind();
	}
	void OpenGLRenderer3DPBR::InitilizeFowardRendering() {
		s_CurrentLightShader = s_PBRInstance->m_FowardRendering.m_Shader;
	}
	void OpenGLRenderer3DPBR::FowardRenderer() {
		if (s_DifferentID.size() == 0)return;
		s_RenderFrameBuffer->Bind();
		Renderer::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		Renderer::SetClearColor();
		uint32_t sizeOffset = 0;
		s_PBRInstance->m_FowardRendering.m_Shader->Bind();
		s_PBRInstance->m_FowardRendering.m_Shader->SetInt("AmountLight", NumLights);
		s_PBRInstance->m_FowardRendering.m_Shader->SetInt("v_NrDirectionalLight", NumDirLights);
		s_PBRInstance->m_FowardRendering.m_Shader->SetInt("v_NrPointLight", NumPointLights);
		s_PBRInstance->m_FowardRendering.m_Shader->SetInt("v_NrSpotLight", NumSpotLights);
		for (uint32_t i = 0; i < s_DifferentID.size(); i++) {
			uint32_t TempID = s_DifferentID[i];
			const auto& TempMesh = s_PBRInstance->m_Meshes.find(TempID);
			const auto& TempAmountMeshes = s_PBRInstance->m_AmountMeshes.find(TempID);

			s_PBRInstance->m_VertexBuffer->Bind();
			s_PBRInstance->m_VertexBuffer->AddData(&s_PBRInstance->m_Transforms[sizeOffset], TempAmountMeshes->second * sizeof(PhysicalBasedRendererVertex));
			if (TempMesh->second.GetMaterial() != nullptr) {
				s_PBRInstance->m_FowardRendering.m_Shader->Bind();
				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("albedoMap", 0);
				if (TempMesh->second.GetMaterial()->AlbedoTexture != nullptr)
					TempMesh->second.GetMaterial()->AlbedoTexture->Bind(0);
				else
					s_PBRInstance->m_WhiteTexture->Bind(0);

				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("normalMap", 1);
				if (TempMesh->second.GetMaterial()->NormalTexture != nullptr)
					TempMesh->second.GetMaterial()->NormalTexture->Bind(1);
				else
					s_PBRInstance->m_WhiteTexture->Bind(1);

				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("metallicMap", 2);
				if (TempMesh->second.GetMaterial()->MetallicTexture != nullptr)
					TempMesh->second.GetMaterial()->MetallicTexture->Bind(2);
				else
					s_PBRInstance->m_WhiteTexture->Bind(2);

				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("roughnessMap", 3);
				if (TempMesh->second.GetMaterial()->RoughnessTexture != nullptr)
					TempMesh->second.GetMaterial()->RoughnessTexture->Bind(3);
				else
					s_PBRInstance->m_WhiteTexture->Bind(3);
			}
			else {
				s_PBRInstance->m_FowardRendering.m_Shader->Bind();
				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("albedoMap", 0);
				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("normalMap", 1);
				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("metallicMap", 2);
				s_PBRInstance->m_FowardRendering.m_Shader->SetInt("roughnessMap", 3);
				s_PBRInstance->m_WhiteTexture->Bind(0);
				s_PBRInstance->m_WhiteTexture->Bind(1);
				s_PBRInstance->m_WhiteTexture->Bind(2);
				s_PBRInstance->m_WhiteTexture->Bind(3);

			}
			// draw in wireframe
			//glPolygonMode(GL_FRONT_AND_BACK,(int)GL_LINES); // keeps this need to put into our game
			if (TempMesh->second.GetMeshSource()->m_FaceCulling == true)
				Renderer::Enable(ProofRenderTest::CullFace);
			if (TempMesh->second.GetMeshSource()->m_Enabled == true) {
				#if 0
				for (SubMesh& mesh : TempMesh->second.GetMeshSource()->meshes) {
					if (mesh.m_Enabled == false)
						continue;
					if (TempMesh->second.HasMaterial() == false) {
						s_PBRInstance->m_FowardRendering.m_Shader->SetInt("DiffuseShader", 7);
						if (mesh.m_Textures.size() > 0)
							mesh.m_Textures[0]->Bind(7);
						else
							s_PBRInstance->m_WhiteTexture->Bind(7);
					}
					s_RendererData->Stats.DrawCalls++;
					mesh.m_VertexArrayObject->Bind();
					mesh.m_IndexBufferObject->Bind();
					Renderer::DrawElementIndexed(mesh.m_VertexArrayObject, TempAmountMeshes->second, s_RendererData->RenderSettings.Draw);
				}
				#endif
			}
			if (TempMesh->second.GetMeshSource()->m_FaceCulling == true)
				Renderer::Disable(ProofRenderTest::CullFace); // rename to render stettings
			sizeOffset += TempAmountMeshes->second;
		}
		s_PBRInstance->m_Shader->UnBind();
		s_RenderFrameBuffer->UnBind();
	}

	void OpenGLRenderer3DPBR::PBRWithFowardRender() {
		if (s_DifferentID.size() == 0)return;
		s_RenderFrameBuffer->Bind();
		Renderer::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		Renderer::SetClearColor();
		uint32_t sizeOffset = 0;
		s_PBRInstance->m_Shader->Bind();
		s_PBRInstance->m_Shader->SetInt("AmountLight", NumLights);
		for (uint32_t i = 0; i < s_DifferentID.size(); i++) {
			uint32_t TempID = s_DifferentID[i];
			const auto& TempMesh = s_PBRInstance->m_Meshes.find(TempID);
			const auto& TempAmountMeshes = s_PBRInstance->m_AmountMeshes.find(TempID);

			s_PBRInstance->m_VertexBuffer->Bind();
			s_PBRInstance->m_VertexBuffer->AddData(&s_PBRInstance->m_Transforms[sizeOffset], TempAmountMeshes->second * sizeof(PhysicalBasedRendererVertex));
			if (TempMesh->second.GetMaterial() != nullptr) {
				s_PBRInstance->m_Shader->Bind();
				s_PBRInstance->m_Shader->SetInt("albedoMap", 0);
				if (TempMesh->second.GetMaterial()->AlbedoTexture != nullptr)
					TempMesh->second.GetMaterial()->AlbedoTexture->Bind(0);
				else
					s_PBRInstance->m_WhiteTexture->Bind(0);

				s_PBRInstance->m_Shader->SetInt("normalMap", 1);
				if (TempMesh->second.GetMaterial()->NormalTexture != nullptr)
					TempMesh->second.GetMaterial()->NormalTexture->Bind(1);
				else
					s_PBRInstance->m_WhiteTexture->Bind(1);

				s_PBRInstance->m_Shader->SetInt("metallicMap", 2);
				if (TempMesh->second.GetMaterial()->MetallicTexture != nullptr)
					TempMesh->second.GetMaterial()->MetallicTexture->Bind(2);
				else
					s_PBRInstance->m_WhiteTexture->Bind(2);

				s_PBRInstance->m_Shader->SetInt("roughnessMap", 3);
				if (TempMesh->second.GetMaterial()->RoughnessTexture != nullptr)
					TempMesh->second.GetMaterial()->RoughnessTexture->Bind(3);
				else
					s_PBRInstance->m_WhiteTexture->Bind(3);
			}
			else {
				s_PBRInstance->m_Shader->Bind();
				s_PBRInstance->m_Shader->SetInt("albedoMap", 0);
				s_PBRInstance->m_Shader->SetInt("normalMap", 1);
				s_PBRInstance->m_Shader->SetInt("metallicMap", 2);
				s_PBRInstance->m_Shader->SetInt("roughnessMap", 3);
				s_PBRInstance->m_WhiteTexture->Bind(0);
				s_PBRInstance->m_WhiteTexture->Bind(1);
				s_PBRInstance->m_WhiteTexture->Bind(2);
				s_PBRInstance->m_WhiteTexture->Bind(3);

			}
			s_PBRInstance->m_Shader->Bind();
			// draw in wireframe
			//glPolygonMode(GL_FRONT_AND_BACK,(int)GL_LINES); // keeps this need to put into our game
			if (TempMesh->second.GetMeshSource()->m_FaceCulling == true)
				Renderer::Enable(ProofRenderTest::CullFace);
			if (TempMesh->second.GetMeshSource()->m_Enabled == true) {
				#if 0

				for (SubMesh& mesh : TempMesh->second.GetMeshSource()->meshes) {
					if (mesh.m_Enabled == false)
						continue;
					if (TempMesh->second.HasMaterial() == false) {
						s_PBRInstance->m_Shader->SetInt("DiffuseShader", 7);
						if (mesh.m_Textures.size() > 0)
							mesh.m_Textures[0]->Bind(7);
						else
							s_PBRInstance->m_WhiteTexture->Bind(7);
					}
					mesh.m_VertexArrayObject->Bind();
					mesh.m_IndexBufferObject->Bind();
					s_WorldDrawType = DrawType::Triangles;
					Renderer::DrawElementIndexed(mesh.m_VertexArrayObject, TempAmountMeshes->second, s_WorldDrawType);
				}
				#endif
			}
			if (TempMesh->second.GetMeshSource()->m_FaceCulling == true)
				Renderer::Disable(ProofRenderTest::CullFace); // rename to render stettings
			sizeOffset += TempAmountMeshes->second;
			s_PBRInstance->m_Shader->UnBind();
		}
		s_RenderFrameBuffer->UnBind();
	}
}