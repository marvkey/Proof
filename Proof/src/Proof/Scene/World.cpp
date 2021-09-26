#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Proof/Renderer/Renderer.h"
#include "Model.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/Component.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include<glad/glad.h>
namespace Proof{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad() {
		if (quadVAO == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,1.0f,0.0f,0.0f,1.0f,
				-1.0f,-1.0f,0.0f,0.0f,0.0f,
				1.0f,1.0f,0.0f,1.0f,1.0f,
				1.0f,-1.0f,0.0f,1.0f,0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1,&quadVAO);
			glGenBuffers(1,&quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
			glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),&quadVertices,GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glBindVertexArray(0);
	}
	World::World()
	{
		backgroundShader = Shader::Create("IBL_Background",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/BackgroundShader.glsl");
		equirectangularToCubemapShader = Shader::Create("IBL_CUBEMAPSHADER",ProofCurrentDirectorySrc+"Proof/Renderer/Asset/Shader/3D/equirectangularToCubemapShader.glsl");
		IrradianceShader = Shader::Create("IBL_IRRADIANCESHader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/IradianceShader.glsl");
		m_WorldIBLTexture= HDRTexture::Create("Assets/Textures/hdr/Arches_E_PineTree_3k.hdr");
		prefilterShader = Shader::Create("IBL_PrefeliterShader",ProofCurrentDirectorySrc +"Proof/Renderer/Asset/Shader/3D/prefilterShader.glsl");
		brdfShader = Shader::Create("IBL_brdfShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/brdfShader.glsl");
		//Assets\Textures\hdr
		float skyboxVertices[] = {
		// back face
				-1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f, // bottom-left
				1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,1.0f, // top-right
				1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,0.0f, // bottom-right         
				1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,1.0f, // top-right
				-1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f, // bottom-left
				-1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,1.0f, // top-left
				// front face
				-1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f, // bottom-left
				1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,0.0f, // bottom-right
				1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f, // top-right
				1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f, // top-right
				-1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f, // top-left
				-1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f, // bottom-left
				// left face
				-1.0f,1.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f, // top-right
				-1.0f,1.0f,-1.0f,-1.0f,0.0f,0.0f,1.0f,1.0f, // top-left
				-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-left
				-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-left
				-1.0f,-1.0f,1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f, // bottom-right
				-1.0f,1.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f, // top-right
				// right face
				1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f, // top-left
				1.0f,-1.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-right
				1.0f,1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,1.0f, // top-right         
				1.0f,-1.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-right
				1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f, // top-left
				1.0f,-1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f, // bottom-left     
			   // bottom face
			   -1.0f,-1.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f, // top-right
			   1.0f,-1.0f,-1.0f,0.0f,-1.0f,0.0f,1.0f,1.0f, // top-left
			   1.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f, // bottom-left
			   1.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f, // bottom-left
			   -1.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,0.0f, // bottom-right
			   -1.0f,-1.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f, // top-right
			   // top face
			   -1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f, // top-left
			   1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f, // bottom-right
			   1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,1.0f,1.0f, // top-right     
			   1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f, // bottom-right
			   -1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f, // top-left
			   -1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.0f,0.0f  // bottom-left        
		};
		m_IBLSkyBoxBuffer = VertexBuffer::Create(&skyboxVertices,sizeof(skyboxVertices));
		m_IBLSkyBoxVertexArray = VertexArray::Create();
		m_IBLSkyBoxVertexArray->AddData(0,3,8 * sizeof(float),(void*)0);
		m_IBLSkyBoxVertexArray->AddData(1,3,8 * sizeof(float),(void*)(3 * sizeof(float)));
		m_IBLSkyBoxVertexArray->AddData(2,2,8 * sizeof(float),(void*)(6 * sizeof(float)));
		backgroundShader->UseShader();
		backgroundShader->SetInt("environmentMap",0);

		glGenFramebuffers(1,&captureFBO);
		glGenRenderbuffers(1,&captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,512,512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,captureRBO);
		
		m_WorldCubeMap = CubeMap::Create();
		
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),1.0f,0.1f,10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(-1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,-1.0f,0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,-1.0f,0.0f))
		};
		
		equirectangularToCubemapShader->UseShader();
		equirectangularToCubemapShader->SetInt("equirectangularMap",0);
		equirectangularToCubemapShader->SetMat4("projection",captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,m_WorldIBLTexture->GetID());
		glViewport(0,0,512,512);
		for (unsigned int i = 0; i < 6; ++i) {
			equirectangularToCubemapShader->SetMat4("view",captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,m_WorldCubeMap->GetID(),0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glDepthFunc(GL_LEQUAL);
			m_IBLSkyBoxVertexArray->Bind();
			glDrawArrays(GL_TRIANGLES,0,36);
			m_IBLSkyBoxVertexArray->UnBind();
			glDepthFunc(GL_LESS);
		}
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		glBindTexture(GL_TEXTURE_CUBE_MAP,m_WorldCubeMap->GetID());
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		m_WorldCubeMapIrradiance = CubeMap::Create(32,32);
		glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,32,32);

		IrradianceShader->UseShader();
		IrradianceShader->SetInt("environmentMap",0);
		IrradianceShader->SetMat4("projection",captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_WorldCubeMap->GetID());

		glViewport(0,0,32,32); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
		for (unsigned int i = 0; i < 6; ++i) {
			IrradianceShader->SetMat4("view",captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,m_WorldCubeMapIrradiance->GetID(),0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDepthFunc(GL_LEQUAL);
			m_IBLSkyBoxVertexArray->Bind();
			glDrawArrays(GL_TRIANGLES,0,36);
			m_IBLSkyBoxVertexArray->UnBind();
			glDepthFunc(GL_LESS);

		}
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		PrefelterMap = CubeMap::Create(128,128,true);
		prefilterShader->UseShader();
		prefilterShader->SetInt("environmentMap",0);
		prefilterShader->SetMat4("projection",captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_WorldCubeMap->GetID());
		glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);

		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
	   // reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5,mip);
			unsigned int mipHeight = 128 * std::pow(0.5,mip);
			glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,mipWidth,mipHeight);
			glViewport(0,0,mipWidth,mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader->SetFloat("roughness",roughness);
			for (unsigned int i = 0; i < 6; ++i) {
				prefilterShader->SetMat4("view",captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,PrefelterMap->GetID(),mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDepthFunc(GL_LEQUAL);
				m_IBLSkyBoxVertexArray->Bind();
				glDrawArrays(GL_TRIANGLES,0,36);
				m_IBLSkyBoxVertexArray->UnBind();
				glDepthFunc(GL_LESS);
			}
		}
		glGenTextures(1,&brdfLUTTexture);

   // pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D,brdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RG16F,512,512,0,GL_RG,GL_FLOAT,0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,512,512);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,brdfLUTTexture,0);

		glViewport(0,0,512,512);
		brdfShader->UseShader();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LEQUAL);

		renderQuad();
		glDepthFunc(GL_LESS);

		glBindFramebuffer(GL_FRAMEBUFFER,0);

		int scrWidth,scrHeight;
		glfwGetFramebufferSize(CurrentWindow::GetWindow(),&scrWidth,&scrHeight);
		glViewport(0,0,scrWidth,scrHeight);
		//glDepthFunc(GL_LESS);

	}
	void World::OnUpdateEditor(FrameTime DeltaTime) {
		
		
		Projection = glm::perspective(glm::radians(45.f),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
		Renderer2D::BeginContext(Projection,EditorCamera.GetCameraView());
		for (SpriteComponent* Comp : Registry.SpriteComponents) {
			Renderer2D::DrawQuad(*Comp);
		}
/*
		Renderer3D::BeginContext(Projection,EditorCamera);
		for (MeshComponent* Comp : Registry.SceneMeshComponents) {
			if (Comp->GetModel()!= nullptr){
				Renderer3D::Draw(*Comp);
			}
		}
		
		for(LightComponent* Comp :Registry.LightComponents){
			Renderer3D::RenderLight(*Comp);
		}
		*/
		Renderer3DPBR::BeginContext(Projection,EditorCamera);
		for (MeshComponent* Comp : Registry.SceneMeshComponents) {
			if (Comp->GetModel() != nullptr) {
				Renderer3DPBR::Draw(*Comp);
			}
		}
		for (LightComponent* Comp : Registry.LightComponents) {
			Renderer3DPBR::Draw(*Comp);
		}
		Renderer3DPBR::GetRenderer()->m_Shader->UseShader();
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap",4);
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap",5);
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT",6);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_WorldCubeMapIrradiance->GetID());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP,PrefelterMap->GetID());
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D,brdfLUTTexture);
		Renderer3DPBR::EndContext();
		Renderer3DPBR::Reset();

		glDepthFunc(GL_LEQUAL);
		backgroundShader->UseShader();
		backgroundShader->SetInt("environmentMap",0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_WorldCubeMap->GetID());
		m_IBLSkyBoxVertexArray->Bind();
		glDrawArrays(GL_TRIANGLES,0,36);
		m_IBLSkyBoxVertexArray->UnBind();
		glDepthFunc(GL_LESS);
		
		EditorCamera.OnUpdate(DeltaTime);
		/*
		for (NativeScriptComponent* Scripts : Registry.NativeScripts) {
			if (Scripts->m_HasScriptAttached == false) {
				continue;
			}
			if (Scripts->Instance == nullptr) {
				Scripts->Instance = Scripts->InstantiateScript();
				Scripts->Instance->OwnerEntity = Scripts->GetOwner();
				Scripts->Instance->OnCreate();
				Scripts->Instance->OnlyOnCreate();
			}
			if (Scripts->Instance->b_CallPerframe == true)
				Scripts->Instance->OnUpdate(DeltaTime);
		}
		*/
	}

	void World::OnUpdateRuntime(FrameTime DeltaTime) {
		/*
		Registry.view<NativeScriptComponent>().each([=](auto _Entity,auto& Nsc) {
			if (!Nsc.Instance) {
				Nsc.Instance = Nsc.InstantiateScript();
				Nsc.Instance->OwnerEntity = Nsc.GetOwner();
				Nsc.Instance->OnCreate();
				Nsc.Instance->OnlyOnCreate();
			}
			Nsc.Instance->OnUpdate(DeltaTime);
		});
		*/

	}

	Entity World::CreateEntity(const std::string& EntName) {
		Entity entity = {Registry.Create(),this};
		entity.AddComponent<TagComponent>()->Name =EntName;
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	Entity World::CreateEntity(const std::string& EntName,uint32_t ID) {
		Entity entity = {Registry.Create(ID),this};
		entity.AddComponent<TagComponent>()->Name = EntName;
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void World::EndRuntime() {
	}

	void World::RenderIBLImage() {
		unsigned int captureFBO,captureRBO;
		glGenFramebuffers(1,&captureFBO);
		glGenRenderbuffers(1,&captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,512,512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,captureRBO);

		unsigned int envCubemap;
		glGenTextures(1,&envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP,envCubemap);
		for (unsigned int i = 0; i < 6; ++i) {
			// note that we store each face with 16 bit floating point values
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB16F,
						 512,512,0,GL_RGB,GL_FLOAT,nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	template<class T>
	void World::OnComponentAdded(Entity _Entity,T* component) {
		PF_CORE_ASSERT(false,"Component Not Identified");
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,NativeScriptComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TransformComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;

	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TagComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;

	}
	template<>
	void World::OnComponentAdded(Entity _Entity,MeshComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,SpriteComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,LightComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}
}