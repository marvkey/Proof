#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/RendererCommand.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <tuple>
#include <variant>
#include "MeshWorkShop.h"
#include "Renderer.h"
namespace Proof{
	uint32_t WorldRenderer::Renderer() {
		if(m_RendererPaused==true)
			return 0;
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_World->GetName().c_str());
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {

			if (m_World->m_CurrentState == WorldState::Play) {
				const auto& cameraGroup = m_World->m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
				if (cameraGroup.size() == 0)
					Renderer3DPBR::BeginContext(m_World->m_EditorCamera, m_ScreenFrameBuffer, RenderData);
				else {
					for (auto entity : cameraGroup) {
						const auto& [transform, camera] = cameraGroup.get<TransformComponent, CameraComponent>(entity);
						camera.CalculateProjection(transform.Location, transform.Rotation);
						Renderer3DPBR::BeginContext(camera.GetProjection(), camera.GetView(), transform.Location, m_ScreenFrameBuffer, RenderData);
						break;
					}
				}
			}
			else {
				Renderer3DPBR::BeginContext(m_World->m_EditorCamera, m_ScreenFrameBuffer, RenderData);
			}
			// MESHES
			{
				const auto& meshGroup = m_World->m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
				for (auto& enity : meshGroup) {
					const auto& [mesh, transform] = meshGroup.get(enity);
					Renderer3DPBR::Draw(mesh, transform.GetWorldTransform());
				}

			}
			// LIGHTS
			{
				const auto& directionGroup = m_World->m_Registry.group<DirectionalLightComponent>(entt::get<TransformComponent>);
				const auto& pointGroup = m_World->m_Registry.group<PointLightComponent>(entt::get<TransformComponent>);
				const auto& spotGroup = m_World->m_Registry.group<SpotLightComponent>(entt::get<TransformComponent>);
				for (auto& entity : directionGroup) {
					const auto& [light, transform] = directionGroup.get(entity);
					Renderer3DPBR::SubmitDirectionalLight(light, transform);
				}

				for (auto& entity : pointGroup) {
					const auto& [light, transform] = pointGroup.get(entity);
					Renderer3DPBR::SubmitPointLight(light, transform);
				}

				for (auto& entity : spotGroup) {
					const auto& [light, transform] = spotGroup.get(entity);
					Renderer3DPBR::SubmitSpotLight(light, transform);
				}
			}
			// PBR
			{
				if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
					Renderer3DPBR::GetRenderer()->m_Shader->Bind();
					Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap", 4);
					Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap", 5);
					Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT", 6);

					m_World->m_WorldCubeMap->Bind(4);
					m_World->PrefelterMap->Bind(5);

					m_World->m_brdflTexture->Bind(6);
				}
			}

			Renderer3DPBR::EndContext();
			m_ScreenFrameBuffer->Bind();
			// cube collider
			m_World->ForEachEntitiesWithMultiple<CubeColliderComponent, TransformComponent>([&](Entity entity) {
				auto& cubeCollider = *entity.GetComponent<CubeColliderComponent>();
				auto& transformComp = *entity.GetComponent<TransformComponent>();

				Vector translation = cubeCollider.OffsetLocation + transformComp.GetWorldLocation();
				Vector scale = cubeCollider.OffsetScale.GetPositive() * transformComp.GetWorldScale().GetPositive();
				auto rotation = transformComp.GetWorldRotation();

				glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(rotation.X), glm::radians(rotation.Y), glm::radians(rotation.Z) }));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ translation })
					* rotationTransform
					* glm::scale(glm::mat4(1.0f), glm::vec3{ scale });
				Renderer3DPBR::DrawDebugMesh(MeshWorkShop::GetCubeMesh().get(), transform);
			});

			m_World->ForEachEntitiesWithMultiple< SphereColliderComponent, TransformComponent>([&](Entity entity) {
				auto& sphereCollider = *entity.GetComponent<SphereColliderComponent>();
				auto& transformComp = *entity.GetComponent<TransformComponent>();
				auto rotation = transformComp.GetWorldRotation();

				Vector translation = sphereCollider.OffsetLocation + transformComp.GetWorldLocation();
				Vector scale = Vector{ sphereCollider.Radius * 2.0f } *transformComp.GetWorldScale().GetPositive();

				glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(rotation.X), glm::radians(rotation.Y), glm::radians(rotation.Z) }));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ translation })
					* rotationTransform
					* glm::scale(glm::mat4(1.0f), glm::vec3{ scale });
				Renderer3DPBR::DrawDebugMesh(MeshWorkShop::GetSphere().get(), transform);

			});

			m_World->ForEachEntitiesWithMultiple<CapsuleColliderComponent, TransformComponent>([&](Entity entity) {
				auto& capsuleCollider = *entity.GetComponent<CapsuleColliderComponent>();
				auto& transformComp = *entity.GetComponent<TransformComponent>();
				Vector translation = capsuleCollider.OffsetLocation + transformComp.GetWorldLocation();
				Vector scale = Vector{ capsuleCollider.Radius * 1.0f } *transformComp.GetWorldScale().GetPositive();
				auto rotation = transformComp.GetWorldRotation();
				switch (capsuleCollider.Direction) {
					case CapsuleDirection::X:
						{
							scale.X *= capsuleCollider.Height;
							rotation.X += 90;
							break;
						}
					case CapsuleDirection::Y:
						{

							scale.Y *= capsuleCollider.Height;
							rotation.Y += 0; // its default is already facing the Y direction
							break;
						}
					case CapsuleDirection::Z:
						{
							scale.Z *= capsuleCollider.Height;
							rotation.Z += 90;
							break;
						}
				}
				glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(rotation.X), glm::radians(rotation.Y), glm::radians(rotation.Z) }));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ translation })
					* rotationTransform
					* glm::scale(glm::mat4(1.0f), glm::vec3{ scale });
				Renderer3DPBR::DrawDebugMesh(MeshWorkShop::GetCapsule().get(), transform);
			});
			m_World->ForEachEntitiesWithMultiple<MeshColliderComponent, TransformComponent>([&](Entity entity) {
				auto& meshCollider = *entity.GetComponent<MeshColliderComponent>();
				auto& transformComp = *entity.GetComponent<TransformComponent>();
				Renderer3DPBR::DrawDebugMesh(meshCollider.GetMesh(), transformComp.GetWorldTransform());
			});
			m_ScreenFrameBuffer->UnBind();
			if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
				m_ScreenFrameBuffer->Bind();
				RendererCommand::DepthFunc(DepthType::Equal);
				m_World->backgroundShader->Bind();
				m_World->backgroundShader->SetInt("environmentMap", 0);
				m_World->m_WorldCubeMap->Bind(0);
				m_World->m_IBLSkyBoxVertexArray->Bind();
				RendererCommand::DrawArray(36);
				m_World->m_IBLSkyBoxVertexArray->UnBind();
				RendererCommand::DepthFunc(DepthType::Less);
				m_ScreenFrameBuffer->UnBind();
			}

			return m_ScreenFrameBuffer->GetTexture();
		}
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			return 0;
		}
	}
	uint32_t WorldRenderer::Renderer(CameraComponent& comp, Vector& location) {

	
		Renderer3DPBR::BeginContext(comp.GetProjection(), comp.GetView(), location, m_ScreenFrameBuffer, RenderData);
		// MESHES
		{
			const auto& meshGroup = m_World->m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto& enity : meshGroup) {
				const auto& [mesh, transform] = meshGroup.get(enity);
				Renderer3DPBR::Draw(mesh, transform.GetWorldTransform());
			}

		}
		// LIGHTS
		{
			const auto& directionGroup = m_World->m_Registry.group<DirectionalLightComponent>(entt::get<TransformComponent>);
			const auto& pointGroup = m_World->m_Registry.group<PointLightComponent>(entt::get<TransformComponent>);
			const auto& spotGroup = m_World->m_Registry.group<SpotLightComponent>(entt::get<TransformComponent>);
			for (auto& entity : directionGroup) {
				const auto& [light, transform] = directionGroup.get(entity);
				Renderer3DPBR::SubmitDirectionalLight(light, transform);
			}

			for (auto& entity : pointGroup) {
				const auto& [light, transform] = pointGroup.get(entity);
				Renderer3DPBR::SubmitPointLight(light, transform);
			}

			for (auto& entity : spotGroup) {
				const auto& [light, transform] = spotGroup.get(entity);
				Renderer3DPBR::SubmitSpotLight(light, transform);
			}
		}
		// PBR
		{
			if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
				Renderer3DPBR::GetRenderer()->m_Shader->Bind();
				Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap", 4);
				Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap", 5);
				Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT", 6);

				m_World->m_WorldCubeMap->Bind(4);
				m_World->PrefelterMap->Bind(5);

				m_World->m_brdflTexture->Bind(6);
			}
		}

		Renderer3DPBR::EndContext();
		m_ScreenFrameBuffer->Bind();
		// cube collider
		m_World->ForEachEntitiesWithMultiple<CubeColliderComponent, TransformComponent>([&](Entity entity) {
			auto& cubeCollider = *entity.GetComponent<CubeColliderComponent>();
			auto& transformComp = *entity.GetComponent<TransformComponent>();

			Vector translation = cubeCollider.OffsetLocation + transformComp.GetWorldLocation();
			Vector scale = cubeCollider.OffsetScale.GetPositive() * transformComp.GetWorldScale().GetPositive();
			auto rotation = transformComp.GetWorldRotation();

			glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(rotation.X), glm::radians(rotation.Y), glm::radians(rotation.Z) }));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ translation })
				* rotationTransform
				* glm::scale(glm::mat4(1.0f), glm::vec3{ scale });
			Renderer3DPBR::DrawDebugMesh(MeshWorkShop::GetCubeMesh().get(), transform);
		});

		m_World->ForEachEntitiesWithMultiple< SphereColliderComponent, TransformComponent>([&](Entity entity) {
			auto& sphereCollider = *entity.GetComponent<SphereColliderComponent>();
			auto& transformComp = *entity.GetComponent<TransformComponent>();
			auto rotation = transformComp.GetWorldRotation();

			Vector translation = sphereCollider.OffsetLocation + transformComp.GetWorldLocation();
			Vector scale = Vector{ sphereCollider.Radius * 2.0f } *transformComp.GetWorldScale().GetPositive();

			glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(rotation.X), glm::radians(rotation.Y), glm::radians(rotation.Z) }));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ translation })
				* rotationTransform
				* glm::scale(glm::mat4(1.0f), glm::vec3{ scale });
			Renderer3DPBR::DrawDebugMesh(MeshWorkShop::GetSphere().get(), transform);

		});

		m_World->ForEachEntitiesWithMultiple<CapsuleColliderComponent, TransformComponent>([&](Entity entity) {
			auto& capsuleCollider = *entity.GetComponent<CapsuleColliderComponent>();
			auto& transformComp = *entity.GetComponent<TransformComponent>();
			Vector translation = capsuleCollider.OffsetLocation + transformComp.GetWorldLocation();
			Vector scale = Vector{ capsuleCollider.Radius * 1.0f } *transformComp.GetWorldScale().GetPositive();
			auto rotation = transformComp.GetWorldRotation();
			switch (capsuleCollider.Direction) {
				case CapsuleDirection::X:
					{
						scale.X *= capsuleCollider.Height;
						rotation.X += 90;
						break;
					}
				case CapsuleDirection::Y:
					{

						scale.Y *= capsuleCollider.Height;
						rotation.Y += 0; // its default is already facing the Y direction
						break;
					}
				case CapsuleDirection::Z:
					{
						scale.Z *= capsuleCollider.Height;
						rotation.Z += 90;
						break;
					}
			}
			glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(rotation.X), glm::radians(rotation.Y), glm::radians(rotation.Z) }));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ translation })
				* rotationTransform
				* glm::scale(glm::mat4(1.0f), glm::vec3{ scale });
			Renderer3DPBR::DrawDebugMesh(MeshWorkShop::GetCapsule().get(), transform);
		});
		m_World->ForEachEntitiesWithMultiple<MeshColliderComponent, TransformComponent>([&](Entity entity) {
			auto& meshCollider = *entity.GetComponent<MeshColliderComponent>();
			auto& transformComp = *entity.GetComponent<TransformComponent>();
			Renderer3DPBR::DrawDebugMesh(meshCollider.GetMesh(), transformComp.GetWorldTransform());
		});
		m_ScreenFrameBuffer->UnBind();
		if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
			m_ScreenFrameBuffer->Bind();
			RendererCommand::DepthFunc(DepthType::Equal);
			m_World->backgroundShader->Bind();
			m_World->backgroundShader->SetInt("environmentMap", 0);
			m_World->m_WorldCubeMap->Bind(0);
			m_World->m_IBLSkyBoxVertexArray->Bind();
			RendererCommand::DrawArray(36);
			m_World->m_IBLSkyBoxVertexArray->UnBind();
			RendererCommand::DepthFunc(DepthType::Less);
			m_ScreenFrameBuffer->UnBind();
		}

		return m_ScreenFrameBuffer->GetTexture();
	}
}