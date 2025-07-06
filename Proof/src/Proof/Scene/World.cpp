#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include "Proof/Scene/Component.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Scene/Component.h"
#include "entt/entt.hpp"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Core/Application.h"
#include "Proof/Audio/AudioEngine.h"
#include "Proof/Audio/AudioUtils.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Physics/PhysicsMeshCache.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Physics/PhysicsShapes.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedPlayer.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedInputDevices/ElevatedInputDevice.h"
#include "Proof/Input/ElevatedInputSystem/InputAction.h"
#include "Proof/Input/ElevatedInputSystem/InputBindingContext.h"
#include "Proof/Renderer/Font.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/UIRenderer/UIRenderer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Math/BasicCollision.h"
#include "WaterSystem/WaterSystem.h"
#include "WaterSystem/Water.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/Renderer/DebugRenderer.h"
#include "Proof/Scene/WaterSystem/BuoyancyActor.h"
#include "Proof/Renderer/Colors.h"
#include "Proof/Scene/SceneUtils.h"

#include "GameMode/LocalGameMode.h"
#include "Proof/Scripting/ScriptWorld.h"
#include <glm/gtx/euler_angles.hpp>
#include "Proof/Utils/ContainerUtils.h"
#include "TerrainRenderer/TerrainRenderer.h"
#include "GrassRenderer/GrassRenderer.h"
namespace Proof 
{


	struct RuntimeSavedData
	{
		std::unordered_map<std::string, ScopeBuffer> m_datas;
	};

	static inline std::vector<WeakCount<World>> AllActiveWorlds;

	World::World(const std::string& name, UUID ID):
		Name(name)
	{


		AllActiveWorlds.emplace_back(WeakCount<World>(this));

		m_ScriptWorld = Count<ScriptWorld>::Create(this);
		Init();
		m_DebugRenderer = Count<DebugRenderer>::Create();
		//m_Registry.on_destroy<ChildComponent>().connect<&World::OnChildComponentDestroy>(this);
	}
	World::~World()
	{
		m_Registry.clear();

		m_Registry.on_construct<MeshColliderComponent>().disconnect(this);
		m_Registry.on_destroy<MeshColliderComponent>().disconnect(this);

		m_Registry.on_construct<WaterComponent>().disconnect(this);
		Utils::Remove(AllActiveWorlds, WeakCount<World>(this));
	}
	bool World::HasEntity(UUID ID)const {
		if (ID == 0)
			return false;
		return m_EntitiesMap.contains(ID);
	}
	
	void World::OnUpdateEditor(FrameTime DeltaTime) 
	{
		PF_PROFILE_FUNC();
		DeleteEntitiesfromQeue();
		{
			PF_PROFILE_FUNC("World::OnUpdateEditor - Particle OnUpdate");

			const auto& particleView = m_Registry.view<ParticleSystemComponent>();
			for (auto entity : particleView)
			{
				auto& particleSystem = particleView.get<ParticleSystemComponent>(entity);
				if (particleSystem.ParticleHandlerTable == nullptr)continue;
				Entity wfadfas{ entity,this };
				for (auto& [index, handler] : particleSystem.ParticleHandlerTable->GetHandlers())
				{
					if (handler != nullptr)
					{
						if(handler->Visible == true)
							handler->Update(DeltaTime,GlmVecToProof( GetWorldSpaceLocation(wfadfas)));
					}
				}
			}
		}

		{

			ForEachEnitityWith<WaterComponent>([&](Entity e)
				{
					glm::mat4 transform = GetWorldSpaceTransform(e);

					e.GetComponent<WaterComponent>().Water->Update(DeltaTime, transform);
				});
		}

		{
			auto view = m_Registry.view<TerrainComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				auto& terrainComponent = e.GetComponent<TerrainComponent>();
				glm::mat4 transform = GetWorldSpaceTransform(e);

				terrainComponent.Terrain->Update(DeltaTime, transform);
			}
		}

		
	}
	
	glm::vec3 GetAnyPerpendicularUnitVector(const glm::vec3& vec)
	{
		if (vec.y != 0.0f || vec.z != 0.0f)
			return glm::vec3(1, 0, 0);
		else
			return glm::vec3(0, 1, 0);
	}

	std::vector<glm::vec3> SamplePlaneKeyPoints(float planeSize) 
	{
		std::vector<glm::vec3> positions;

		float offset = 0.8;
		float halfSize = (planeSize * offset) / 2.f;

		// Calculate step sizes
		float verticalStep = (planeSize * offset) / 4.0f;  // Corrected: Divides the plane into 5 rows (4 intervals)
		float horizontalStep = (planeSize * offset) / 4.0f; // Divides the plane horizontally into 5 columns (4 intervals)

		// Loop through each column (left, middle-left, middle, middle-right, right)
		for (int col = 0; col < 5; ++col) {
			float x = -halfSize + col * horizontalStep;

			// Loop through each row (top, top-middle, middle, bottom-middle, bottom)
			for (int row = 0; row < 5; ++row) {
				float z = halfSize - row * verticalStep;

				positions.push_back(glm::vec3(x, 0.0f, z));
			}
		}

		return positions;
	}
	glm::vec2 WorldToScreenSpace(const glm::vec3& worldPos, const Camera& camera,glm::vec2 screenDimesnion)
	{
		// Step 1: Get the view and projection matrices from the camera

		// Step 2: Transform world position to view space
		glm::vec4 viewSpacePos = camera.GetViewMatrix() * glm::vec4(worldPos, 1.0f);

		// Step 3: Transform view space position to clip space
		glm::vec4 clipSpacePos = camera.GetProjectionMatrix() * viewSpacePos;

		// Step 4: Perform perspective division (clip space to NDC)
		glm::vec3 ndcPos = glm::vec3(clipSpacePos) / clipSpacePos.w; // Divide by w

		// Step 5: Convert NDC to screen space coordinates
		glm::vec2 screenSpacePos;
		screenSpacePos.x = (ndcPos.x * 0.5f + 0.5f) * screenDimesnion.x;
		screenSpacePos.y = (ndcPos.y * 0.5f + 0.5f) * screenDimesnion.y;

		// Optional: Flip Y if needed (for top-left origin)
		screenSpacePos.y = screenDimesnion.y - screenSpacePos.y;

		return screenSpacePos;
	}
	void World::OnRender(Count<class WorldRenderer> worldRenderer, FrameTime timestep, const Camera& camera, const glm::vec3& cameraLocation, float nearPlane, float farPlane, float fov)
	{
		PF_PROFILE_FUNC();

		glm::vec2 screenDimensions = worldRenderer->GetScreenData().FullResolution;
		worldRenderer->SetContext(this);
		worldRenderer->BeginScene({ camera,nearPlane,farPlane,fov }, cameraLocation);

		// lighting
		{
			//directional lights
			{
				auto dirLights = m_Registry.group<DirectionalLightComponent>(entt::get<TransformComponent>);
				SBDirectionalLightsSceneData directionaLightScene;
				directionaLightScene.DirectionalLights.resize(dirLights.size());
				int index = 0;
				for (auto& entityID : dirLights)
				{
					Entity entity(entityID, this);
					const auto& dirLightComponent = dirLights.get<DirectionalLightComponent>(entityID);
					glm::vec3 direction = GetWorldSpaceRotation(entity);
					direction = glm::normalize(direction);
					directionaLightScene.DirectionalLights[index].Color = dirLightComponent.Color;
					directionaLightScene.DirectionalLights[index].Intensity = dirLightComponent.Intensity;
					directionaLightScene.DirectionalLights[index].Direction = -direction;
					directionaLightScene.DirectionalLights[index].ShadowSoftness = dirLightComponent.ShadowSoftness;
					directionaLightScene.DirectionalLights[index].ShadowStrength = dirLightComponent.ShadowStrength;
					directionaLightScene.DirectionalLights[index].bCastShadows = dirLightComponent.CastShadow;
					directionaLightScene.DirectionalLights[index].bCastSoftShadows = dirLightComponent.CastSoftShadow;

					index++;
				}
				if (!dirLights.empty())
					worldRenderer->SubmitDirectionalLight(directionaLightScene);
			}

			// sky Light
			{
				auto skylights = m_Registry.group<SkyLightComponent>(entt::get<TransformComponent>);
				if (!skylights.empty())
				{
					auto entityID = skylights.front();
					Entity entity(entityID, this);
					auto& skyLightComponent = entity.GetComponent<SkyLightComponent>();

					Count<Environment> environment = skyLightComponent.Environment;

					UBSkyLight skyLightInfo;
					skyLightInfo.TintColor = skyLightComponent.ColorTint;
					skyLightInfo.Rotation = skyLightComponent.MapRotation;
					skyLightInfo.Intensity = skyLightComponent.Intensity;
					skyLightInfo.Lod = skyLightComponent.SkyBoxLoad;

					if (environment->IsDynamic())
						skyLightInfo.Lod = 0;

					if (environment->GetEnvironmentState() == EnvironmentState::HosekWilkie)
					{
						auto data = environment->GetHosekWilkieDataSkyData();

						data.SunDirection = normalize(GetWorldSpaceRotation(entity));

						environment->Update(data);
					}
					else if (environment->GetEnvironmentState() == EnvironmentState::PreethamSky)
					{
						auto data = environment->GetPreethamSkyData();

						data.SunDirection = normalize(GetWorldSpaceRotation(entity));

						environment->Update(data);
					}

					if (skyLightComponent.Environment != nullptr)
					{
						worldRenderer->SubmitSkyLight(skyLightInfo, skyLightComponent.Environment);
					}
					Renderer::UpdateAllEnvironment();
				}

			}
			//point light
			{
				SBPointLightSceneData pointLightScene;
				auto pointLights = m_Registry.group<PointLightComponent>(entt::get<TransformComponent>);
				pointLightScene.PointLights.resize(pointLights.size());

				uint32_t pointLightIndex = 0;
				for (auto e : pointLights)
				{
					Entity entity(e, this);
					auto [transformComponent, pointLight] = pointLights.get<TransformComponent, PointLightComponent>(e);
					auto transform = GetWorldSpaceTransformComponent(entity);
					pointLightScene.PointLights[pointLightIndex] =
					{
							transform.Location,
							pointLight.Intensity,
							pointLight.Color,
							pointLight.MinRadius,
							pointLight.Radius,
							pointLight.Falloff,
							pointLight.CastsShadows,
							pointLight.SoftShadows,
							pointLight.ShadowStrength,
							pointLight.ShadowSoftness,
					};
					pointLightIndex++;
				}
				if (!pointLights.empty())
				{
					worldRenderer->SubmitPointLight(pointLightScene);
				}
			}

			//spot lights
			{
				SBSpotLightSceneData spotLightSceneData;
				auto spotLights = m_Registry.group<SpotLightComponent>(entt::get<TransformComponent>);
				spotLightSceneData.SpotLights.resize(spotLights.size());
				uint32_t spotLightIndex = 0;
				for (auto e : spotLights)
				{
					Entity entity(e, this);
					auto [transformComponent, spotLight] = spotLights.get<TransformComponent, SpotLightComponent>(e);
					auto transform = GetWorldSpaceTransformComponent(entity);
					glm::vec3 direction = glm::normalize(glm::rotate(transform.GetRotation(), glm::vec3(1.0f, 0.0f, 0.0f)));

					spotLightSceneData.SpotLights[spotLightIndex] =
					{
						transform.Location,
						spotLight.Intensity,
						direction,
						spotLight.AngleAttenuation,
						spotLight.Color,
						spotLight.Range,
						spotLight.Angle,
						spotLight.Falloff,
						spotLight.CastsShadows,
						spotLight.SoftShadows,
						spotLight.ShadowStrength,
						spotLight.ShadowSoftness,
					};
					spotLightIndex++;
				}
				if (!spotLights.empty())
				{
					worldRenderer->SubmitSpotLight(spotLightSceneData);
				}
			}
		}

		Frustum cameraFrustrum = Frustum::CreateFrustrum(&camera);
		WorldRendererStatistics& rendererStats = worldRenderer->m_Stats;
		// render meshes
		{

			{
				//static Count<Material> transparentMaterial = Count<Material>::Create("Test Transparent", Renderer::GetShader("ProofPBRTransparent_Static"));
				//
 				//{
				//	AssetManager::CreateRuntimeAsset(transparentMaterial);
				//}
				//static Count<MaterialTable> transparentMaterialTable = Count<MaterialTable>::Create();
				//transparentMaterialTable->SetMaterial(0,transparentMaterial);
				auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
				for (auto entity : group)
				{
					auto [transformComponent, staticMeshComponent] = group.get<TransformComponent, MeshComponent>(entity);
					if (!staticMeshComponent.Visible)
						continue;

					auto mesh = staticMeshComponent.GetMesh();
					if (mesh)
					{
						rendererStats.TotalMeshProcessedCpu++;
						Entity e = Entity(entity, this);
						glm::mat4 transform = GetWorldSpaceTransform(e);

						auto aabb = mesh->GetMeshSource()->GetBoundingBox().ScaleAABB(transform);

						if (BasicCollision::AABBIsOnFrustum(aabb, cameraFrustrum))
						{
							rendererStats.TotalMeshSentToGpu++;
							worldRenderer->SubmitMesh(mesh, staticMeshComponent.MaterialTable, transform, staticMeshComponent.CastShadow);
						}
					}
				}


			}
			{

				auto view = m_Registry.view<DynamicMeshComponent>();
				for (auto entity : view)
				{
					Entity e = { entity, this };
					auto& dynamicMeshComponent = e.GetComponent<DynamicMeshComponent>();
					if (!dynamicMeshComponent.Visible)
						continue;

					Count<DynamicMesh> mesh = dynamicMeshComponent.GetMesh();
					if (mesh)
					{
						rendererStats.TotalMeshProcessedCpu++;

						Entity e = Entity(entity, this);
						glm::mat4 transform = GetWorldSpaceTransform(e);
						auto aabb = mesh->GetMeshSource()->GetSubMesh(dynamicMeshComponent.GetSubMeshIndex()).BoundingBox.ScaleAABB(transform);

						if (BasicCollision::AABBIsOnFrustum(aabb, cameraFrustrum))
						{
							rendererStats.TotalMeshSentToGpu++;
							worldRenderer->SubmitDynamicMesh(mesh, dynamicMeshComponent.MaterialTable, dynamicMeshComponent.GetSubMeshIndex(), transform, dynamicMeshComponent.CastShadow);
						}
					}
				}
			}


			{
				auto view = m_Registry.view<TerrainComponent>();
				for (auto entity : view)
				{
					Entity e = { entity, this };
					auto& terrainComponent = e.GetComponent<TerrainComponent>();
					glm::mat4 transform = GetWorldSpaceTransform(e);
					if(terrainComponent.Terrain)
						terrainComponent.Terrain->Render(worldRenderer);
				}
			}
		}

		// render water
		{
			ForEachEnitityWith<WaterComponent>([&] (Entity e)
				{
					e.GetComponent<WaterComponent>().Water->Render(worldRenderer);
				});
			
		}
		RenderPhysicsDebug(worldRenderer, false);

		worldRenderer->EndScene();


		// render 2d
		Count<Renderer2D> renderer2D = worldRenderer->GetRenderer2D();

		RenderPhysicsDebug2D(worldRenderer, camera, cameraLocation,false);
		renderer2D->SetTargetFrameBuffer(worldRenderer->GetExternalCompositePassFrameBuffer());

		renderer2D->BeginContext(camera.GetProjectionMatrix(), camera.GetViewMatrix(), GlmVecToProof(cameraLocation));

		
		Count<Texture2D> prefilter2D;
		auto skylights = m_Registry.group<SkyLightComponent>(entt::get<TransformComponent>);
		if (!skylights.empty())
		{
			auto entityID = skylights.front();
			Entity entity(entityID, this);
			auto& skyLightComponent = entity.GetComponent<SkyLightComponent>();

			Count<Environment> environment = skyLightComponent.Environment;
			prefilter2D = environment->GetPrefilterMap2D();


		}

		{
			auto view = m_Registry.view<TransformComponent, SpriteComponent>();
			for (auto entity : view)
			{
				Entity e = Entity(entity, this);
				auto [transformComponent, spriteRendererComponent] = view.get<TransformComponent, SpriteComponent>(entity);
				spriteRendererComponent.Texture = prefilter2D->GetID();
				/*
				if (spriteRendererComponent.Texture)
				{
					//Count<Texture2D> texture = AssetManager::GetAsset<Texture2D>(spriteRendererComponent.Texture);
					renderer2D->DrawQuad(spriteRendererComponent, transformComponent);
				}
				else
				{
					renderer2D->DrawQuad(spriteRendererComponent, transformComponent);
				}
				*/
				renderer2D->DrawQuad(spriteRendererComponent, transformComponent);

			}
		}
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();

			for (auto entity : view)
			{
				auto [transformComponent, textComponent] = view.get<TransformComponent, TextComponent>(entity);
				Entity e = Entity(entity, this);
				auto font = Font::GetDefault();

				if (!textComponent.Visible)
					continue;
				TextParams params;
				params.Color = textComponent.Colour;
				params.Kerning = textComponent.Kerning;
				params.LineSpacing = textComponent.LineSpacing;
				if (textComponent.UseLocalRotation)
					renderer2D->DrawString(textComponent.Text, font, params, GetWorldSpaceTransformUsingLocalRotation(e));
				else
					renderer2D->DrawString(textComponent.Text, font, params, GetWorldSpaceTransform(e));
			}
			
		}

		// render water
		{

			ForEachEnitityWith<WaterComponent>([&](Entity e)
				{
					e.GetComponent<WaterComponent>().Water->Render2D(renderer2D);
				});
		}
		{
#if 0
			auto view = m_Registry.view<TransformComponent, MeshComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				auto& staticMeshComponent = e.GetComponent<MeshComponent>();
				if (!staticMeshComponent.Visible)
					continue;


				auto mesh = staticMeshComponent.GetMesh();   
				if (mesh)
				{

					renderer2D->DrawAABB(mesh, GetWorldSpaceTransform(e), { 1,0,0,1 });
				}
			}
#endif
		}

		renderer2D->EndContext();

		
		// Debug Renderer
		{
			renderer2D->BeginContext(camera.GetProjectionMatrix(), camera.GetViewMatrix(), GlmVecToProof(cameraLocation));

			auto& renderQueue = m_DebugRenderer->GetRenderQueue();
			for (auto&& func : renderQueue)
				func(renderer2D);

			m_DebugRenderer->ClearRenderQueue();

			renderer2D->EndContext();

		}



	}
	void World::RenderPhysicsDebug(Count<WorldRenderer> renderer, bool runtime)
	{
		if (renderer->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders == WorldRendererDebugOptions::PhysicsColliderView::None)
			return;
		bool selectedOnly = renderer->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders == WorldRendererDebugOptions::PhysicsColliderView::Selected;

#if 0
		{
			auto view = m_Registry.view<CapsuleColliderComponent>();
			Count<Mesh> capsuleDebugMesh = PhysicsMeshCache::GetCapsuleColliderMesh();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				auto  worldtransformComponent = GetWorldSpaceTransformComponent(e);
				const auto& collider = e.GetComponent<CapsuleColliderComponent>();
				auto capsuleData = GetCapsuleData(collider.Direction, worldtransformComponent);
				glm::mat4 colliderTransform = glm::mat4(1.0f); // Initialize with identity matrix
				colliderTransform = glm::translate(colliderTransform, collider.Center + worldtransformComponent.Location);

				if (collider.Direction == CapsuleDirection::X)
				{
					colliderTransform = glm::rotate(colliderTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate 90 degrees around X-axis
				}
				else if (collider.Direction == CapsuleDirection::Z)
				{
					colliderTransform = glm::rotate(colliderTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate 90 degrees around Z-axis
				}
				// Apply translation
				colliderTransform = colliderTransform * glm::mat4(worldtransformComponent.GetRotation());
				// Adjust scaling based on capsule direction
				float scale_x = (collider.Direction == CapsuleDirection::X) ? (collider.Height) * capsuleData.scaleDirection : (collider.Radius * 2.0f) * capsuleData.radiusScale;
				float scale_y = (collider.Direction == CapsuleDirection::Y) ? (collider.Height) * capsuleData.scaleDirection : (collider.Radius * 2.0f) * capsuleData.radiusScale;
				float scale_z = (collider.Direction == CapsuleDirection::Z) ? (collider.Height) * capsuleData.scaleDirection : (collider.Radius * 2.0f) * capsuleData.radiusScale;
					
				colliderTransform = glm::scale(colliderTransform, glm::vec3(scale_x, scale_y, scale_z));
				//glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0), ProofToglmVec(collider.OffsetLocation)) * glm::scale(glm::mat4(1.0f), glm::vec3(collider.Radius * 2.0f, collider.Height, collider.Radius * 2.0f));
				renderer->SubmitPhysicsDebugMesh(capsuleDebugMesh, colliderTransform);
			}
		}
#endif
		{

			auto view = m_Registry.view<MeshColliderComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				if (selectedOnly)
				{
					if (!SelectionManager::IsEntityOrAncestorSelected(SelectionContext::Scene, e))
						continue;
				}
				auto& collider = e.GetComponent<MeshColliderComponent>();
				Count<MeshCollider> colliderAsset = nullptr;

				if (collider.ColliderKey.IsValid())
					colliderAsset = AssetManager::GetAsset<MeshCollider>(collider.ColliderKey.GetAssetID());

				if (colliderAsset)
				{
					glm::mat4 transform = GetWorldSpaceTransform(e);

					Count<Mesh> complexDebugMesh = PhysicsMeshCache::GetDebugMesh(colliderAsset);
					if (complexDebugMesh && colliderAsset->CollisionComplexity != ECollisionComplexity::UseSimpleAsComplex)
						renderer->SubmitPhysicsDebugMesh(complexDebugMesh, transform);

					Count<DynamicMesh> simpleDebugMesh = PhysicsMeshCache::GetDebugDynamicMesh(colliderAsset);
					if (simpleDebugMesh && colliderAsset->CollisionComplexity != ECollisionComplexity::UseComplexAsSimple)
						renderer->SubmitPhysicsDynamicDebugMesh(simpleDebugMesh, collider.SubMeshIndex, transform);
				}
			}
		}
	}
	void World::RenderPhysicsDebug2D(Count<WorldRenderer> renderer, const Camera& camera, const glm::vec3& cameraLocation, bool runtime)
	{
		if (renderer->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders == WorldRendererDebugOptions::PhysicsColliderView::None)
			return;


		bool selectedOnly = renderer->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders == WorldRendererDebugOptions::PhysicsColliderView::Selected;

		Count<Renderer2D> renderer2D = renderer->GetRenderer2D();
		Renderer2DContextSettings settings;
		if (renderer->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders == WorldRendererDebugOptions::PhysicsColliderView::OnTop)
			settings.RenderOnTop = true;

		renderer2D->BeginContext(camera.GetProjectionMatrix(), camera.GetViewMatrix(), GlmVecToProof(cameraLocation), settings);

		renderer2D->SetTargetFrameBuffer(renderer->GetExternalCompositePassFrameBuffer());
		//box colliders

		
		{

			auto view = m_Registry.view<BoxColliderComponent>();

			for (auto entity : view)
			{
				
				Entity e = { entity, this };
				if (selectedOnly)
				{
					if (!SelectionManager::IsEntityOrAncestorSelected(SelectionContext::Scene, e))
						continue;
				}
				const auto& collider = e.GetComponent<BoxColliderComponent>();
				TransformComponent worldTransformComp = GetWorldSpaceTransformComponent(e);
				renderer2D->DrawDebugCube(collider.Center + worldTransformComp.Location, worldTransformComp.GetRotationEuler(),
					(collider.Size/2.f) * worldTransformComp.Scale
					, renderer->DebugOptions.PhysicsDebugOptions.PhysicsColliderColor);
			}

		}
		// sphere colliders
		{
			auto view = m_Registry.view<SphereColliderComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				if (selectedOnly)
				{
					if (!SelectionManager::IsEntityOrAncestorSelected(SelectionContext::Scene, e))
						continue;
				}
				const auto& collider = e.GetComponent<SphereColliderComponent>();
				TransformComponent worldTransformComp = GetWorldSpaceTransformComponent(e);

				auto location = worldTransformComp.Location;
				glm::vec3 center = collider.Center;
				float radius = collider.Radius * glm::max(worldTransformComp.Scale.x, glm::max(worldTransformComp.Scale.y, worldTransformComp.Scale.z));
				glm::vec3 rotation = worldTransformComp.GetRotationEuler();
				glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);  // Adjust color as needed
				renderer2D->DrawDebugSphere(location + center, rotation, radius, renderer->DebugOptions.PhysicsDebugOptions.PhysicsColliderColor);
			}
		}
		//capsule COlliders
		{
			auto view = m_Registry.view<CapsuleColliderComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				if (selectedOnly)
				{
					if (!SelectionManager::IsEntityOrAncestorSelected(SelectionContext::Scene, e))
						continue;
				}
				const auto& collider = e.GetComponent<CapsuleColliderComponent>();
				TransformComponent worldTransformComp = GetWorldSpaceTransformComponent(e);

				auto location = worldTransformComp.Location;
				glm::vec3 center = collider.Center;
				glm::quat rotation = worldTransformComp.GetRotation();

				if (collider.Direction == CapsuleDirection::X)
					rotation = rotation * glm::vec3(glm::radians(90.f), 0, 0);
				else if (collider.Direction == CapsuleDirection::Z)
					rotation = rotation * glm::vec3(0.f, 0, glm::radians(90.f));

				auto capsuleData = GetCapsuleData(collider.Direction, worldTransformComp);
				float radius = capsuleData.radiusScale * collider.Radius;
				float height = capsuleData.scaleDirection * collider.Height * 0.5f;
				renderer2D->DrawCapsule(location + center, glm::eulerAngles(rotation), height, radius, renderer->DebugOptions.PhysicsDebugOptions.PhysicsColliderColor);

			}
		}

		
		{
			auto view = m_Registry.view<CharacterControllerComponent>();
			for (auto entity : view)
			{

				Entity e = { entity, this };
				if (selectedOnly)
				{
					if (!SelectionManager::IsEntityOrAncestorSelected(SelectionContext::Scene, e))
						continue;
				}
				const auto& collider = e.GetComponent<CharacterControllerComponent>();
				TransformComponent worldTransformComp = GetWorldSpaceTransformComponent(e);

				if (collider.ColliderType == CharacterControllerType::Box)
				{
					//character controller always faces up
					glm::vec3 localUp = glm::normalize(Math::GetUpVector());
					glm::vec3 currentUp = glm::normalize(worldTransformComp.GetUpVector());
					glm::quat rotation = glm::rotation(currentUp, localUp);

					// skin width
					renderer2D->DrawDebugCube(
						collider.Center + worldTransformComp.Location,
						glm::eulerAngles(rotation * worldTransformComp.GetRotation()),  // Apply the new rotation
						collider.Size * worldTransformComp.Scale + (collider.SkinOffset),
						glm::vec4(1,0,0,1)
					);  

					renderer2D->DrawDebugCube(
						collider.Center + worldTransformComp.Location,
						glm::eulerAngles( rotation * worldTransformComp.GetRotation()),  // Apply the new rotation
						collider.Size * worldTransformComp.Scale,
						renderer->DebugOptions.PhysicsDebugOptions.PhysicsColliderColor
					);
				}
				else if (collider.ColliderType == CharacterControllerType::Capsule)
				{

					glm::vec3 localUp = glm::normalize(Math::GetUpVector());
					glm::vec3 currentUp = glm::normalize(worldTransformComp.GetUpVector());
					glm::quat rotation = glm::rotation(currentUp, localUp);

					auto capsuleData = GetCapsuleData(CapsuleDirection::Y, worldTransformComp);

					float radius = capsuleData.radiusScale * collider.Radius;
					float height = capsuleData.scaleDirection * collider.Height/2;

					// skin width
					renderer2D->DrawCapsule(collider.Center + worldTransformComp.Location, 
						glm::eulerAngles(rotation* worldTransformComp.GetRotation()), 
						height+ collider.SkinOffset, radius + collider.SkinOffset, glm::vec4(1, 0, 0, 1));

					renderer2D->DrawCapsule(collider.Center + worldTransformComp.Location, glm::eulerAngles(rotation * worldTransformComp.GetRotation()), height, radius, 
						renderer->DebugOptions.PhysicsDebugOptions.PhysicsColliderColor);
				}
			}
		}

		// water
		{
			auto  buoyancyEntities = m_Registry.view<BuoyancyComponent, RigidBodyComponent>();

			for (auto e : buoyancyEntities)
			{
				Entity buoyancyEntity = { e, this };
				if (selectedOnly)
				{
					if (!SelectionManager::IsEntityOrAncestorSelected(SelectionContext::Scene, buoyancyEntity))
						continue;
				}
				BuoyancyComponent& buoyancyComponent = buoyancyEntity.GetComponent<BuoyancyComponent>();
				for (auto& [entityID, floater] : buoyancyComponent.Floaters)
				{
					auto floaterEntity = TryGetEntityWithUUID(entityID);
					if (!floaterEntity)continue;

					TransformComponent worldTransformComp = GetWorldSpaceTransformComponent(floaterEntity);

					auto location = worldTransformComp.Location;

					float baseRadius = 0.25f;
					float radius = baseRadius * glm::max(worldTransformComp.Scale.x, glm::max(worldTransformComp.Scale.y, worldTransformComp.Scale.z));

					glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);  // Adjust color as needed
					glm::vec3 rotation = worldTransformComp.GetRotationEuler();

					renderer2D->DrawDebugSphere(location, rotation, radius, color);
				}

				if (m_PhysicsWorld != nullptr)
				{
					auto bouyancyActor = m_PhysicsWorld->GetBuoyancyActor(buoyancyEntity);

					if (bouyancyActor)
					{
						glm::mat4 transform= GetWorldSpaceTransform(buoyancyEntity);
						auto physicsActor = m_PhysicsWorld->GetActor(buoyancyEntity);

						glm::mat4 physicsTransfrom = physicsActor->GetTransform();

						{
							//Transform trans;
							//trans.SetTransform(transform);
							//
							//PF_EC_INFO("boyancyEntity Tranfrom {}", trans.ToString());
							//trans.SetTransform(physicsTransfrom);
							//PF_EC_INFO("boyancyEntity Physics actor transform Tranfrom {}", trans.ToString());

						}
						for (const auto& [id, voxelPos] : bouyancyActor->GetVoxels())
						{

							//renderer2D->DrawDebugCube(Utils::LocalToWorld(voxelPos, physicsActor->GetTransform()), glm::vec3(0), bouyancyActor->GetVoxelSize() *0.8f, Colors::Magenta);
							renderer2D->DrawDebugCube(Utils::LocalToWorld(voxelPos, transform), glm::vec3(0), bouyancyActor->GetVoxelSize()*0.3f, Colors::Magenta);
						}
					}
				}

			}
		}

		
		renderer2D->EndContext();
	}
	void World::Init()
	{
		m_Registry.on_construct<MeshColliderComponent>().connect<&World::OnMeshColliderComponentConstruct>(this);
		m_Registry.on_destroy<MeshColliderComponent>().connect<&World::OnMeshColliderComponentDestroy>(this);

		m_Registry.on_construct<WaterComponent>().connect<&World::OnWaterComponentCreate>(this);
	}

	void World::DeleteEntitiesfromQeue()
	{

		for (auto it = m_EnttiesDeletAfterTime.begin(); it != m_EnttiesDeletAfterTime.end(); ) 
		{
			it->second -= FrameTime::GetWorldDeltaTime();
			if (it->second <= 0.0f)
			{
				m_EntityDeleteQueue.insert(it->first);
				
				it = m_EnttiesDeletAfterTime.erase(it);
			}
			else
			{
				++it;
			}
		}

		
		// job to remove entites does not care if has child or not
		for (auto& ID : m_EntityDeleteQueue)
		{
			// incase we only deleting the head enitty
			Entity entity = GetEntity(ID);
			entity.EachChild([&](Entity childEntity) {
				childEntity.Unparent();
			});
			if (entity.HasParent())
			{
				if (TryGetEntityWithUUID(entity.GetParentUUID()))
				{
					GetEntity((entity.GetParentUUID())).RemoveChild(entity);
				}
			}
			m_Registry.destroy((entt::entity)entity);
			m_EntitiesMap.erase(ID);
		}

		m_EntityDeleteQueue.clear();
	}


	void World::OnRigidBodyComponentCreate(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };
		if (!m_RigidBodyOnConstruct)
		{
			m_RigidBodyWaitingList.push_back(e);
			return;
		}
		
		m_PhysicsWorld->CreateActor(e);
	}


	void World::OnRigidBodyComponentDelete(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };

		m_PhysicsWorld->RemoveActor(e);
	}

	void World::OnScriptAdded(entt::registry& component, entt::entity entityID)
	{
		if (m_ScriptWorld == nullptr)
			return;

		Entity e = { entityID, this };
		m_ScriptWorld->InstantiateScriptEntity(e);
	}

	void World::OnScriptDelete(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };
		m_ScriptWorld->DestroyEntityScript(e);
	}
	void World::OnWaterComponentCreate(entt::registry& registry, entt::entity entityID)
	{
		Entity e = { entityID, this };

		e.GetComponent<WaterComponent>().Water->SetWorld(this);
	}
	void World::BuildDynamicMeshEntityHierarchy(Entity parent, Count<DynamicMesh> mesh, const MeshNode& node, bool generateColliders)
	{
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		const auto& nodes = meshSource->GetNodes();

		// Skip empty root node
		if (node.IsRoot() && node.Submeshes.empty())
		{
			for (uint32_t child : node.Children)
				BuildDynamicMeshEntityHierarchy(parent, mesh, nodes[child], generateColliders);

			return;
		}

		Entity nodeEntity = CreateChildEntity(parent, node.Name);

		nodeEntity.GetTransformComponent().SetTransform(node.LocalTransform);

		if (node.Submeshes.size() == 1)
		{
			// Node == Mesh in this case
			uint32_t submeshIndex = node.Submeshes[0];

			nodeEntity.AddComponent<DynamicMeshComponent>();
			DynamicMeshComponent& component = nodeEntity.GetComponent<DynamicMeshComponent>();
			component.SetMesh(mesh->GetID());
			component.SetSubMeshIndex(submeshIndex);


			if (generateColliders)
			{
				auto& colliderComponent = nodeEntity.AddComponent<MeshColliderComponent>();
				Count<MeshCollider> colliderAsset = PhysicsEngine::GetOrCreateColliderAsset(nodeEntity, colliderComponent);
				colliderComponent.ColliderKey = colliderAsset->GetID();
				colliderComponent.SubMeshIndex = submeshIndex;
				colliderComponent.UseSharedShape = colliderAsset->AlwaysShareShape;
				nodeEntity.AddComponent<RigidBodyComponent>().RigidBodyType = RigidBodyType::Dynamic;

			}
		}
		else if (node.Submeshes.size() > 1)
		{
			// Create one entity per child mesh, parented under node
			for (uint32_t i = 0; i < node.Submeshes.size(); i++)
			{
				uint32_t submeshIndex = node.Submeshes[i];
				Entity childEntity = CreateChildEntity(nodeEntity, meshSource->GetSubMesh(submeshIndex).Name);

				childEntity.AddComponent<DynamicMeshComponent>();
				DynamicMeshComponent& component = childEntity.GetComponent<DynamicMeshComponent>();
				component.SetMesh(mesh->GetID());
				component.SetSubMeshIndex(submeshIndex);


				if (generateColliders)
				{
					auto& colliderComponent = childEntity.AddComponent<MeshColliderComponent>();
					Count<MeshCollider> colliderAsset = PhysicsEngine::GetOrCreateColliderAsset(childEntity, colliderComponent);
					colliderComponent.ColliderKey = colliderAsset->GetID();
					colliderComponent.SubMeshIndex = submeshIndex;
					colliderComponent.UseSharedShape = colliderAsset->AlwaysShareShape;
					childEntity.AddComponent<RigidBodyComponent>().RigidBodyType = RigidBodyType::Dynamic;
				}
			}
		}

		// Recursively process children
		for (uint32_t child : node.Children)
			BuildDynamicMeshEntityHierarchy(nodeEntity, mesh, nodes[child], generateColliders);
	}
	
	void World::OnUpdateRuntime(FrameTime DeltaTime) 
	{
		PF_PROFILE_FUNC();
		
		m_RuntimeConfig.PlayerInputCount = m_Registry.view<PlayerInputComponent>().size();
		if (m_CurrentState == WorldState::Pause)
			return;
		{
			PF_PROFILE_FUNC("World::OnUpdate - Particle OnUpdate");

			const auto& particleView = m_Registry.view<ParticleSystemComponent>();
			for (auto entity : particleView)
			{
				auto& particleSystem = particleView.get<ParticleSystemComponent>(entity);
				if (particleSystem.ParticleHandlerTable == nullptr)continue;
				Entity wfadfas{ entity,this };
				for (auto& [index, handler] : particleSystem.ParticleHandlerTable->GetHandlers())
				{
					if (handler != nullptr)
					{
						if(handler->Visible == true)
							handler->Update(DeltaTime, GlmVecToProof( GetWorldSpaceLocation(wfadfas)));
					}
				}
			}
		}

		{
			// let this go first so that it can also initilize scripts
			// things like physics update physcsi component so if scripts not initilized may crash
			PF_PROFILE_FUNC("World::OnUpdate - C# OnUpdate");
			m_ScriptWorld->OnUpdate(DeltaTime);
		}
		
		{
			PF_PROFILE_FUNC("World::OnUpdate PlayerInput")
			auto view = m_Registry.view<PlayerInputComponent>();

			for (auto e : view)
			{

				Entity entity = { e,this };
				auto& playerInputComponent = entity.GetComponent<PlayerInputComponent>();

				auto player = playerInputComponent.Player;

				if (player == nullptr)
					continue;
				player->OnUpdate(DeltaTime);
			}
		}
#if 0
		{
			PF_PROFILE_FUNC("World::OnUpdate - Audio");
			{
				PF_PROFILE_FUNC("World::OnUpdate - AudioListener");
				//m_Camera

				Entity listener;
				auto view = m_Registry.view<AudioListenerComponent>();
				for (auto e : view)
				{
					Entity check = { e,this };
					auto& listenerComponent = check.GetComponent<AudioListenerComponent>();

					if (listenerComponent.Active)
					{
						listener = check;

						auto transform = Utils::TransformToAudioTransform(GetWorldSpaceTransformComponent(listener));
						
						AudioEngine::UpdateListenerPosition(transform);
						AudioEngine::UpdateListenerConeAttenuation(listenerComponent.ConeInnerAngleInRadians, listenerComponent.ConeOuterAngleInRadians, listenerComponent.ConeOuterGain);
						auto physicsActor = m_PhysicsWorld->GetActor(listener);
						if (physicsActor)
						{

							if(physicsActor->IsDynamic())
								AudioEngine::UpdateAudioListenerVelocity( physicsActor->GetLinearVelocity());
						}
						else
						{
							AudioEngine::UpdateAudioListenerVelocity(glm::vec3{ 0 });
						}
						break;

					}
				}
			#if 0
				// we are going to remove this soon just for now testing
				if (!listener)
				{
					TransformComponent comp;
					comp.Location = m_CameraPositon;
					auto transform = Utils::TransformToAudioTransform(comp);
					AudioEngine::UpdateListenerPosition(transform);

					AudioListenerComponent defaultSettings;
					AudioEngine::UpdateListenerConeAttenuation(defaultSettings.ConeInnerAngleInRadians, defaultSettings.ConeOuterAngleInRadians, defaultSettings.ConeOuterGain);
				}
			#endif
			}


			{
				PF_PROFILE_FUNC("World::OnUpdate - Audio");
				auto view = m_Registry.view<AudioComponent>();

				for (auto e : view)
				{

					Entity audioEntity = { e,this };
					auto& audioComponent = audioEntity.GetComponent<AudioComponent>();
					auto transform = Utils::TransformToAudioTransform(GetWorldSpaceTransformComponent(audioEntity));

					AudioEngine::UpdateAudio(audioEntity.GetUUID(), Utils::AudioComponentToSoundConfig(audioComponent));
					AudioEngine::UpdateAudioTransform(audioEntity.GetUUID(), transform);
					auto physicsActor = m_PhysicsWorld->GetActor(audioEntity);

					if (physicsActor)
					{
						if(physicsActor->IsDynamic())
							AudioEngine::UpdateAudioVelocity(audioEntity.GetUUID(), physicsActor->GetLinearVelocity());
					}
					else
					{
						AudioEngine::UpdateAudioVelocity(audioEntity.GetUUID(), glm::vec3{ 0 });
					}
				}
			}
		}
#endif
		{

			ForEachEnitityWith<WaterComponent>([&](Entity e)
				{
					glm::mat4 transform = GetWorldSpaceTransform(e);

					e.GetComponent<WaterComponent>().Water->Update(DeltaTime, transform);
				});
		}

		{
			auto view = m_Registry.view<TerrainComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				auto& terrainComponent = e.GetComponent<TerrainComponent>();
				glm::mat4 transform = GetWorldSpaceTransform(e);

				terrainComponent.Terrain->Update(DeltaTime,transform);
			}
		}
		m_PhysicsWorld->Simulate(DeltaTime);

		{
			PF_PROFILE_FUNC("World::OnUpdate - C# OnPostUpdate");
			m_ScriptWorld->OnPostUpdate(DeltaTime);
		}
		DeleteEntitiesfromQeue();
	}

	Entity World::TryGetEntityWithUUID(UUID id)const {
		if (const auto iter = m_EntitiesMap.find(id); iter != m_EntitiesMap.end())
			return iter->second;
		return Entity{};
	}
	Entity World::GetEntity(UUID id)
	{
		PF_CORE_ASSERT(m_EntitiesMap.find(id) != m_EntitiesMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
		return m_EntitiesMap.at(id);
	}

	void World::OnSimulatePhysics(FrameTime DeltaTime) {
		DeleteEntitiesfromQeue();
	}

	bool World::HasWorldCamera() {
		const auto& cameraGroup = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
		return cameraGroup.size() > 0;
	}

	Entity World::GetWorldCameraEntity() 
	{
		if (HasEntity(m_RuntimeConfig.WorldCameraEntity))
		{
			Entity camera = GetEntity(m_RuntimeConfig.WorldCameraEntity).GetCamera();
			if (camera)return camera;
		}
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if(camera.ActiveForRendering)
				return Entity{ entity, this };
		}
		return {};
	}

	void World::OnRenderEditor(Count<class WorldRenderer> renderer, FrameTime time, const EditorCamera& camera)
	{
		
		OnRender(renderer, time, camera,camera.GetPosition(), camera.GetNearPlane(), camera.GetFarPlane(),camera.GetFOV());
	}

	void World::OnRenderRuntime(Count<class WorldRenderer> renderer, FrameTime time)
	{
		m_GameMode->RenderRuntime(renderer, time);
	}

	Entity World::CreateEntity(const std::string& EntName) {
		return CreateEntity(EntName, UUID());
	}

	Entity World::CreateEntity(const std::string& EntName, UUID ID) {
		/* we have to do some custmization of entt because when we pass an ID the entities create a vecot of the size of ID*/

		Entity entity = { m_Registry.create(),this };

		entity.AddComponent<IDComponent>(ID);
		entity.AddComponent<TagComponent>().Tag = EntName;
		entity.AddComponent<HierarchyComponent>();
		entity.AddComponent<TransformComponent>() = {};

		m_EntitiesMap[ID] = entity;
		return entity;
	}

	template<typename... Componnents>
	static void CopyComponentIfExistEntitySingle(Entity dst, Entity src, bool isdstPrefab = false, bool isSrcPrefab = false)
	{
		([&]()
		{
			if ((typeid(Componnents) == typeid(IDComponent) || typeid(Componnents) == typeid(HierarchyComponent)) )
			{
				return;
			}
			if (typeid(Componnents) == typeid(InternalPlayerInputComponent))
				return;

			if (src.HasComponent<Componnents>())
				dst.AddorReplaceComponent<Componnents>(src.GetComponent<Componnents>());
			else
				return;

			
			if(typeid(Componnents) == typeid(ScriptComponent))
				dst.GetCurrentWorld()->GetScriptWorld()->DuplicateScriptInstance(src, dst);

		}(), ...);
	}
	template<typename... Component>
	static void CopyComponentIfExistsEntity(ComponentGroup<Component...>, Entity dst, Entity src, bool isdstPrefab = false, bool isSrcPrefab = false)
	{
		//if (isdstPrefab == true && isSrcPrefab == true)
		//{
		//	PF_CORE_ASSERT(false);
		//}
		CopyComponentIfExistEntitySingle<Component...>(dst, src, isdstPrefab, isSrcPrefab);
	}

	bool World::OnElevatedKeyEvent(const ElevatedInputKeyParams& keyParams)
	{
		bool returnValue = false;
		// add players to them
		if (m_CurrentState == WorldState::Edit)
			return false;

		PF_PROFILE_FUNC();
		{
			auto view = m_Registry.group<InternalPlayerInputComponent,PlayerInputComponent>();


			for (auto e : view)
			{

				Entity entity = { e,this };
				auto& internalPlayerInputComponent = entity.GetComponent<InternalPlayerInputComponent>();
				auto& playerInputComponent = entity.GetComponent<PlayerInputComponent>();

				auto player = playerInputComponent.Player;

				if (player == nullptr)
					continue;

				if (internalPlayerInputComponent.GetPlayer() != keyParams.InputDevice->GetPlayer())
					continue;
				returnValue |= player->InputKey(keyParams);
			}
		}

		return returnValue;
	}

	void World::OnWorldTransition(AssetID id)
	{
		if (!AssetManager::HasAssetAndAssetType(id, AssetType::World))
			return;

		if (m_OnWorldTransitionCallback)
			m_OnWorldTransitionCallback(id);
		else
			PF_ENGINE_WARN("Cannot Transition World - no callback set");

	}

	std::vector<WeakCount<World>>& const World::GetAllActiveWorlds()
	{
		return AllActiveWorlds;
	}

	void World::PrefabCopyEntity(Count<class Prefab> prefab, Entity srcEntity, Entity parentEntity,bool includeChildren)
	{
		// first id is the src, second is dstEntity
		std::unordered_map<UUID, UUID> entitySwapIDs;
		PrefabCopyEntityReal(prefab, srcEntity, parentEntity, entitySwapIDs, includeChildren);
	}
	//src entity is used as childenitty 
	void World::PrefabCopyEntityReal(Count<class Prefab> prefab, Entity srcEntity, Entity parentEntity, std::unordered_map<UUID, UUID>& entitySwapID, bool includeChildren)
	{
		// first function call for base has to be null
		// handle it

		// id of each swap id of entity
		entitySwapID[srcEntity.GetUUID()] = { parentEntity.GetUUID() };
		CopyComponentIfExistsEntity(AllComponents{}, parentEntity, srcEntity, true);

		if (includeChildren)
		{
			srcEntity.EachChild([&](Entity childEntity)
				{
					Entity newEntity;
					newEntity = prefab->m_World->CreateEntity();
					newEntity.SetParent(parentEntity);
					//Entity newChild = prefab->m_World->CreateEntity(childEntity, true);
					PrefabCopyEntityReal(prefab, childEntity, newEntity, entitySwapID,true);
				});
		}

		m_ScriptWorld->PostDuplicateScriptInstance(srcEntity, parentEntity, entitySwapID);
	}

	Entity World::CreateEntity(Entity entity, bool includeChildren) 
	{
		PauseRigidBodyOnConstruct();
		// first id is the src, second is dstEntity
		std::unordered_map<UUID, UUID> entitySwapIDs;
		Entity newEntity = CreateEntityFromOtherReal(entity,entitySwapIDs, includeChildren);

		UnPauseRigidBodyOnConstruct();

		return entity;
	}
	Entity World::CreateEntityFromOtherReal(Entity entity, std::unordered_map<UUID, UUID>& entitySwapID,bool includeChildren)
	{
		Entity newEntity = CreateEntity(entity.GetName());
		if (entity.HasParent())
		{
			newEntity.SetParent(entity.GetParent());
		}
		CopyComponentIfExistsEntity(AllComponents{}, newEntity, entity);

		// first id is the src, second is dstEntity
		entitySwapID[entity.GetUUID()] = newEntity.GetUUID();

		if (includeChildren == true)
		{
			entity.EachChild([&](Entity childEntity) 
				{

					Entity newChild = CreateEntity(childEntity, true);
					TransformComponent transform = newChild.GetComponent<TransformComponent>();
					newEntity.AddChild(newChild);
					newChild.GetComponent<TransformComponent>() = transform;
					/*
					if (m_PhysicsWorld)
					{
						if (m_PhysicsWorld->HasActor(newChild))
						{
							auto physicsActor = m_PhysicsWorld->GetActor(newChild);
							physicsActor->SetTransform(GetWorldSpaceTransform(newChild));
						}
					}
					*/
					entitySwapID[childEntity.GetUUID()] = newChild.GetUUID();
				});
		}

		m_ScriptWorld->PostDuplicateScriptInstance(entity, newEntity, entitySwapID);

		return newEntity;
	}

	void World::UnPauseRigidBodyOnConstruct()
	{
		m_RigidBodyOnConstruct = true;

		for(auto& e : m_RigidBodyWaitingList)
			m_PhysicsWorld->CreateActor(e);

		m_RigidBodyWaitingList.clear();
	}

	Entity World::CreateEntity(Count<class DynamicMesh> mesh, bool generateCollider)
	{
		PF_CORE_ASSERT(mesh->GetID());
		auto info = AssetManager::GetAssetInfo(mesh);
		Entity root = CreateEntity(info.GetName());
		BuildDynamicMeshEntityHierarchy(root, mesh, mesh->GetMeshSource()->GetRootNode(), generateCollider);
		return root;
	}
	Entity World::CreateChildEntity(Entity parent, const std::string& name)
	{
		PF_PROFILE_FUNC();
		Entity entity = CreateEntity(name);

		if (parent)
		{
			entity.GetComponent<TransformComponent>().Scale = glm::vec3{ 1.0f };
			entity.SetParent(parent);
		}
		return entity;
	}
	#if 0
	template<typename... Component>
	static void CopyComponentSinglPrefab(Entity dstEntity, entt::registry& dstMap,const entt::registry& src, UUID srcID,const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			if (typeid(Component) == typeid(IDComponent))
			{
				return;
			}

			entt::entity destinationID = (entt::entity)dstEntity;
			if (!src.all_of<Component>(srcID))
				return;

			const auto& srcComponent = src.get<Component>(srcID);
			dstMap.emplace_or_replace<Component>(destinationID, srcComponent);
			
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentPrefab(ComponentGroup<Component...>, Entity dst, entt::registry& dstMap,const entt::registry& src,UUID srcID, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponentSinglPrefab<Component...>(dst, dstMap, src, srcID, enttMap);
	}
	#endif

	static Entity CreateEntityPrefabStatic(Count<Prefab> prefab, World* world, Entity prefabSource, bool includeChildren = true)
	{
		Entity newEntity = world->CreateEntity();
		CopyComponentIfExistsEntity(AllComponents{}, newEntity, prefabSource, false, true);
		newEntity.AddComponent<PrefabComponent>();
		newEntity.GetComponent<PrefabComponent>().PrefabKey.SetAssetID(prefab->GetID());
		newEntity.GetComponent<PrefabComponent>().PrefabEntityID = prefabSource.GetUUID();

		if (includeChildren)
		{

			prefabSource.EachChild([&](Entity child) 
				{
				Entity newChild = CreateEntityPrefabStatic(prefab, world, child, true);

				TransformComponent transform = newChild.GetComponent<TransformComponent>();
				newChild.SetParent(newEntity);
				// setting because with children set parent changes to local so can messup
				newChild.GetComponent<TransformComponent>() = transform;


				/*
				if (newChild.GetCurrentWorld()->GetPhysicsWorld())
				{
					if (newChild.GetCurrentWorld()->GetPhysicsWorld()->HasActor(newChild))
					{
						auto physicsActor = newChild.GetCurrentWorld()->GetPhysicsWorld()->GetActor(newChild);
						physicsActor->SetTransform(newChild.GetCurrentWorld()->GetWorldSpaceTransform(newChild));
					}
				}
				*/
			});
		}
		return newEntity;
	}
	Entity World::CreateEntity(const std::string& name, Count<Prefab> prefab, TransformComponent transfom, UUID id)
	{
		PF_PROFILE_FUNC();
		/**
		 * when an entity with sub children has rigid body this fucntions crahses 
		 * it could be a problem with the emplace or replace in the copy compoentnt single
		 * 
		 */
		PauseRigidBodyOnConstruct();

		Entity prefabBaseEntity = prefab->GetBaseEntity();
		if (!prefabBaseEntity)return {};
		Entity newEntity = CreateEntityPrefabStatic(prefab, this, prefabBaseEntity, true);
		newEntity.SetName(name);
		newEntity.GetComponent<TransformComponent>() = transfom;

		UnPauseRigidBodyOnConstruct();
		return newEntity;
	}
	template<typename... Component>
	static void CopyComponentSingleWorld(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			if (typeid(Component) == typeid(IDComponent))
			{
				return;
			}
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				UUID id = src.get<IDComponent>(srcEntity).GetID();
				entt::entity dstEntity = enttMap.at(id);

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);

				//dst.GetCurrentWorld()->GetScriptWorld()->DuplicateScriptInstance(src, dst);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponentSingleWorld<Component...>(dst, src, enttMap);
	}

	//static void CopyComponent
	Count<World> World::Copy(Count<World> worldToCopy) {
		Count<World> newWorld = Count<World>::Create();
#if 1

		newWorld->Name = worldToCopy->Name;
		//newWorld->m_ID = worldToCopy->GetID();
	
		auto& srcSceneRegistry = worldToCopy->m_Registry;
		auto& dstSceneRegistry = newWorld->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		// in reverse order
		auto idView = srcSceneRegistry.view<IDComponent>();
		auto begin = idView.begin();
		auto end = idView.end();

		while (end != begin) {
			--end;
			entt::entity e = *end;

			EntityID uuid = srcSceneRegistry.get<IDComponent>(e).GetID();
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newWorld->CreateEntity(name, uuid);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent )
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		newWorld->m_ScriptWorld = ScriptWorld::CopyScriptWorld(worldToCopy->GetScriptWorld(), newWorld);
#endif
		return newWorld;
	}

	void World::StartRuntime(RuntimeConfiguration runtimeConfig)
	{
		m_RuntimeConfig = runtimeConfig;
		
		m_CurrentState = WorldState::Play;
		m_GameMode = Count<LocalGameMode>::Create(this);


	
		m_GameMode->Start();

		m_PhysicsWorld = Count<PhysicsWorld>::Create(this);
		m_PhysicsWorld->StartWorld();
		m_Registry.on_construct<RigidBodyComponent>().connect<&World::OnRigidBodyComponentCreate>(this);
		m_Registry.on_destroy<RigidBodyComponent>().connect < &World::OnRigidBodyComponentDelete>(this);


		m_Registry.on_construct<ScriptComponent>().connect<&World::OnScriptAdded>(this);
		m_Registry.on_destroy<ScriptComponent>().connect < &World::OnScriptDelete>(this);


		Count<World> instance = this;
		//AudioEngine::BeginContext(instance);
		m_ScriptWorld->BeginRuntime();

		ForEachEnitityWith<TerrainComponent>([&](Entity e)
		{
				e.GetComponent<TerrainComponent>().Terrain->SetWorld(this);

		});
		///
		///
		//PhysicsWorldConfig config;
		//config.PvdClient = true;
		//config.Gravity = { 0,-9.8f,0 };// for multiplayer scene

	}
	void World::EndRuntime() {


		m_ScriptWorld->EndRuntime();

		m_Registry.on_construct<RigidBodyComponent>().disconnect(this);
		m_Registry.on_destroy<RigidBodyComponent>().disconnect(this);

		m_Registry.on_construct<ScriptComponent>().disconnect(this);
		m_Registry.on_destroy<ScriptComponent>().disconnect(this);
		
		//AudioEngine::EndContext();
		m_PhysicsWorld->EndWorld();
		m_PhysicsWorld = nullptr;
		m_GameMode->End();

		m_GameMode = nullptr;
		//m_Registry.clear(); // some components hold a shred refrence to the world sowe need to get rid of them
	}

	void World::DeleteEntity(class Entity ent, bool deleteChildren , float time)
	{
		UUID enttUUID = ent.GetUUID();
		if (!HasEntity(enttUUID))
			return;
		if (time <= 0.0f)
		{
			m_EntityDeleteQueue.insert(ent.GetUUID());
			if (deleteChildren)
			{
				ent.EachChild([&](Entity childEntity) 
					{
						DeleteEntity(childEntity, true);
					});
			}
		}
		else
		{
			m_EnttiesDeletAfterTime[ent.GetUUID()] = time;

			if (deleteChildren)
			{
				ent.EachChild([&](Entity childEntity) {
					DeleteEntity(childEntity, true, time);
					});
			}
		}
	}


	void World::ConvertToWorldSpaceTransform(Entity entity)
	{
		PF_PROFILE_FUNC();

		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());

		if (!parent)
			return;

		glm::mat4 transform = GetWorldSpaceTransform(entity);
		auto& entityTransform = entity.GetComponent<TransformComponent>();
		entityTransform.SetTransform(transform);
	}

	Entity World::TryGetEntityByTag(const std::string& tag) {
		Entity returnEntity;
		auto entities = GetAllEntitiesWith<TagComponent>();
		for (auto e : entities)
		{
			if (entities.get<TagComponent>(e).Tag == tag)
				return Entity(e, const_cast<World*>(this));
		}

		return Entity{};
	}
#define  CaluclateTransformationWithStep 0
	glm::vec3 World::GetWorldSpaceLocation(Entity entity) const 
	{
	#if CaluclateTransformationWithStep
		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.Location + GetWorldSpaceLocation(entity.GetParent());
		return transformComp.Location;
	#else
		return GetWorldSpaceTransformComponent(entity).Location;

	#endif
	}

	glm::vec3 World::GetWorldSpaceRotation(Entity entity) const 
	{
	#if CaluclateTransformationWithStep

		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.GetRotationEuler() + GetWorldSpaceRotation(entity.GetParent());
		return transformComp.GetRotationEuler();
	#else
		return GetWorldSpaceTransformComponent(entity).GetRotationEuler();
	#endif
	}

	glm::vec3 World::GetWorldSpaceScale(Entity entity) const 
	{
	#if CaluclateTransformationWithStep

		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.Scale + GetWorldSpaceScale(entity.GetParent());
		return transformComp.Scale;
	#else
		return GetWorldSpaceTransformComponent(entity).Scale;
	#endif
	}

	TransformComponent World::GetWorldSpaceTransformComponent(Entity entity) const
	{
	#if CaluclateTransformationWithStep

		TransformComponent component;
		component.Scale = GetWorldSpaceScale(entity);
		component.Location = GetWorldSpaceScale(entity);
		component.SetRotationEuler(GetWorldSpaceRotation(entity));
		
		return component;
	#else
		glm::mat4 transform = GetWorldSpaceTransform(entity);
		TransformComponent transformComponent;
		transformComponent.SetTransform(transform);
		return transformComponent;
	#endif
	}
	
	glm::mat4 World::GetWorldSpaceTransform(Entity entity) const {
	#if CaluclateTransformationWithStep
		auto rotation = GetWorldSpaceRotation(entity);
		return glm::translate(glm::mat4(1.0f), { GetWorldSpaceLocation(entity) }) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { GetWorldSpaceScale(entity)});

	#else
		glm::mat4 transform(1.0f);
		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
		if (parent)
			transform = GetWorldSpaceTransform(parent);
		
		return transform * entity.Transform();
	#endif
	}

	glm::mat4 World::GetWorldSpaceTransformUsingLocalRotation(Entity entity) const
	{
	#if CaluclateTransformationWithStep

		auto rotation = entity.GetComponent<TransformComponent>().GetRotation();
		return glm::translate(glm::mat4(1.0f), { GetWorldSpaceLocation(entity) }) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { GetWorldSpaceScale(entity) });
	#else
		auto rotation = entity.GetComponent<TransformComponent>().GetRotation();
		return glm::translate(glm::mat4(1.0f), { GetWorldSpaceLocation(entity) }) *
			glm::toMat4(rotation)
			* glm::scale(glm::mat4(1.0f), { GetWorldSpaceScale(entity) });

	#endif
	}

	TransformComponent World::GetWorldSpaceTransformComponentUsingLocalRotation(Entity entity) const
	{

		glm::mat4 transform = GetWorldSpaceTransformUsingLocalRotation(entity);
		TransformComponent transformComponent;
		transformComponent.SetTransform(transform);
		return transformComponent;
	}

	void World::ConvertToLocalSpace(Entity entity)
	{
		PF_PROFILE_FUNC();

		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());

		if (!parent)
			return;

		TransformComponent& transform = entity.GetTransformComponent();
		glm::mat4 parentTransform = GetWorldSpaceTransform(parent);
		glm::mat4 localTransform = glm::inverse(parentTransform) * transform.GetTransform(); // keep this it works, the other way was taking
		transform.SetTransform(localTransform);
	}

	void World::ParentEntity(Entity entity, Entity parent)
	{
		PF_PROFILE_FUNC();
		if (entity.GetParent() == parent)
			return;

		if (!parent)
			return;
		if (parent.IsDescendantOf(entity))
		{
			UnparentEntity(parent);

			Entity newParent = TryGetEntityWithUUID(entity.GetParentUUID());
			if (newParent)
			{
				UnparentEntity(entity);
				ParentEntity(parent, newParent);
			}
		}
		else
		{
			Entity previousParent = TryGetEntityWithUUID(entity.GetParentUUID());

			if (previousParent)
				UnparentEntity(entity);
		}
		entity.GetComponent<HierarchyComponent>().ParentHandle = parent.GetUUID();
		parent.Children().push_back(entity.GetUUID());

		ConvertToLocalSpace(entity);
	}	

	void World::UnparentEntity(Entity entity, bool convertToWorldSpace)
	{
		PF_PROFILE_FUNC();
		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
		if (!parent)
			return;
		auto& parentChildren = parent.Children();
		parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), entity.GetUUID()), parentChildren.end());

		if (convertToWorldSpace)
			ConvertToWorldSpaceTransform(entity);

		entity.GetComponent<HierarchyComponent>().ParentHandle = 0;
	}

	void World::OnMeshColliderComponentConstruct(entt::registry& registry, entt::entity entity)
	{
		PF_PROFILE_FUNC();

		Entity e = { entity, this };
		auto& component = e.GetComponent<MeshColliderComponent>();
		PhysicsEngine::GetOrCreateColliderAsset(e, component);

		if (component.ColliderKey.IsValid())
		{
			Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderKey.GetAssetID());
			if (colliderAsset && AssetManager::HasAsset(colliderAsset->ColliderMesh) && !PhysicsMeshCache::Exists(colliderAsset))
				PhysicsMeshCooker::CookMesh(colliderAsset);
		}
	}

	void World::OnMeshColliderComponentDestroy(entt::registry& registry, entt::entity entity)
	{
	}
}