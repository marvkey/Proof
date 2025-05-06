#include "Proofprch.h"
#include "GerstnerWave.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Asset/MeshImpoter.h"
#include "Proof/Math/Random.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Scene/SceneUtils.h"

namespace Proof
{
	
	GerstnerWave::GerstnerWave(Count<class Water> water)
		: Wave(water, WaveType::GerstnerWave)
	{
		BaseInit();
	}

	GerstnerWave::GerstnerWave(Count<class Water> water, Count<GerstnerWave> other)
		: Wave(water, WaveType::GerstnerWave)
	{
		Texture = other->Texture;
		WaterData = other->WaterData;
		GerstnerData = other->GerstnerData;

		BaseInit();


	}

	void GerstnerWave::Update(float deltaTime)
	{
		RecomputeWaves();
		OnUpdatePhysics(deltaTime, GetWorld());
	}

	void GerstnerWave::Render(Count<class WorldRenderer> renderer)
	{
		//	RecomputeWaves();

		m_UBWaterDataSet->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&WaterData, sizeof(WaterData)));
		m_RenderMaterial->Set("u_TextureMap", Renderer::GetWhiteTexture());
		//renderer->SubmitWaterMesh(m_Plane, m_RenderMaterial, transform);

		{
			m_UBGerstnerData.WaveCount = glm::min(GerstnerData.WaveCount, GerstnerData.MaxGerstnerWavesCount);
			m_UBGerstnerData.Color = GerstnerData.Color;
			m_UBGerstnerWaveDataSet->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&m_UBGerstnerData, sizeof(m_UBGerstnerData)));

			m_SBGerstnerWavesSet->SetData(Renderer::GetCurrentFrameInFlight(), Buffer((void*)m_Waves.data(), m_Waves.size() * sizeof(UBGerstnerWave)));

		}
		renderer->SubmitMesh(m_Plane, m_RenderMaterial, GetTransform());
		//renderer->SubmitMesh(AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Cube).As<Mesh>(), AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Cube).As<Mesh>()->GetMaterialTable(), glm::mat4(1.0f));


	}

	void GerstnerWave::Render2D(Count<class Renderer2D> renderer2D)
	{
		if (!VisualizeWaveHeight)
			return;

		std::vector<glm::vec3> positions = Utils::SamplePlanePoints(GerstnerData.PlaneSize);


		for (auto localPos : positions)
		{
			auto worldPos = Utils::LocalToWorld(localPos, GetTransform());

			glm::vec3 pos = localPos;

			pos.y += GetWaveHeightAtPosition(localPos, FrameTime::GetTime());

			pos = Utils::LocalToWorld(pos, GetTransform());

			renderer2D->DrawLine(worldPos, pos);
			renderer2D->FillCircle(pos, glm::vec3(glm::radians(90.0f), 0.0f, 0), 0.8);

		}

	}

	float GerstnerWave::GetWaveHeightAtPosition(glm::vec3 position, float applicationTime)
	{
		// Calculate the inverse of the model matrix
		glm::mat4 inverseModelMatrix = glm::inverse(GetTransform());

		// Convert the world position to a vertex position in model space
		glm::vec3 vertexPosition = glm::vec3(inverseModelMatrix * glm::vec4(position, 1.0f));

		// because the more iterations we do to estimate wave height 
		// the more performance heavy it will be so setting th maxIteration to 10 if they are more than 10
		// waves will help performance saftey
		const uint32_t maxIterations = 10;
		//uint32_t waveCount = glm::min(GerstnerData.WaveCount, GerstnerData.MaxGerstnerWavesCount);
		uint32_t waveCount = glm::min(GerstnerData.WaveCount, maxIterations);

		glm::vec3 currentPosition = GetWaveOffset(vertexPosition, applicationTime);

		for (uint32_t i = 0; i < waveCount; i++)
		{
			glm::vec3 diff = glm::vec3(position.x - currentPosition.x, 0, position.z - currentPosition.z);
			currentPosition = GetWaveOffset(diff, applicationTime);
		}

		return currentPosition.y;
	}

	float GerstnerWave::GetWorldWaveHeightAtPosition(glm::vec3 position, float applicationTime)
	{
		TransformComponent component;
		component.SetTransform(GetTransform());

		return GetWaveHeightAtPosition(position, applicationTime) + component.Location.y;
	}

	void GerstnerWave::RegeneratePlane()
	{
		PF_ENGINE_ERROR("Have to Fix this because if we call regenerate plane we are still keeping the old one in the runtime asset");
		m_Plane = MeshWorkShop::GeneratePlane(GerstnerData.PlaneSize / 5, GerstnerData.PlaneSize);
		//m_Plane = MeshWorkShop::GenerateCube();
		AssetManager::CreateRuntimeAsset(m_Plane.As<Asset>(), "GerstnerWavePlane");

	}

	glm::vec3 GerstnerWave::GetWaveOffset(glm::vec3 position, float applicationTime)
	{
		glm::vec3 result = glm::vec3(0.0f);
		uint32_t waveCount = glm::min(GerstnerData.WaveCount, GerstnerData.MaxGerstnerWavesCount);
		for (uint32_t i = 0; i < waveCount; i++)
		{
			UBGerstnerWave& wave = m_Waves[i];
			result += CalculateWave(wave, position, applicationTime);
		}

		return result;
	}

	glm::vec3 GerstnerWave::CalculateWave(const UBGerstnerWave& wave, glm::vec3 position, float applicationTime)
	{
		float k = 2 * glm::pi<float>() / wave.Wavelength;

		float c = sqrt(wave.Speed / k);

		glm::vec2 d = normalize(wave.WaveDirection);

		float f = k * (glm::dot(d, glm::vec2(position.x, position.z)) - c * applicationTime);

		float a = wave.Steepness / k;

		return glm::vec3(d.x * (a * glm::cos(f)), a * glm::sin(f) * wave.Amplitude, d.y * (a * glm::cos(f)));

	}

	void GerstnerWave::BaseInit()
	{
		m_RenderMaterial = RenderMaterial::Create(RenderMaterialConfiguration{ "GerstnerWave", Renderer::GetShader("GerstnerWave") });
		m_UBWaterDataSet = UniformBufferSet::Create(sizeof(WaterData));
		RegeneratePlane();

		{

			m_Waves.resize(GerstnerData.MaxGerstnerWavesCount);
			m_UBGerstnerWaveDataSet = UniformBufferSet::Create(sizeof(m_UBGerstnerData));
			m_SBGerstnerWavesSet = StorageBufferSet::Create(GerstnerData.MaxGerstnerWavesCount * sizeof(UBGerstnerWave));
		}

		m_RenderMaterial->Set("GerstnerWaterData", m_UBGerstnerWaveDataSet);
		m_RenderMaterial->Set("GerstnerWaterStorageBuffer", m_SBGerstnerWavesSet);

	}


	void GerstnerWave::OnUpdatePhysics(float deltaTime, Count<class World> world)
	{
		PF_PROFILE_FUNC();
		if (!world)
			return;
 		if (!world->IsPlaying())return;
		auto  buoyancyEntities = world->GetAllEntitiesWith<BuoyancyComponent, RigidBodyComponent>();
		for (auto e : buoyancyEntities)
		{
			Entity buoyancyEntity = { e, world.Get() };

			Count<PhysicsActor> physicsActor = world->GetPhysicsWorld()->GetActor(buoyancyEntity);

			if (!physicsActor->IsDynamic())return;

			BuoyancyComponent& buoyancyComponent = buoyancyEntity.GetComponent<BuoyancyComponent>();

			std::vector<size_t> activeFloatersIndex;
			size_t entityIndex = 0;
			for (auto& [entityID, floater] : buoyancyComponent.Floaters)
			{
				if (world->TryGetEntityWithUUID(entityID))
					activeFloatersIndex.emplace_back(entityIndex);
				entityIndex++;
			}
			auto physicsWorld = world->GetPhysicsWorld();

			for (auto& index : activeFloatersIndex)
			{
				auto& [entityID, floater] = buoyancyComponent.Floaters[index];

				Entity floaterEntity = world->GetEntity(entityID);

				auto worldTransformComponent = world->GetWorldSpaceTransformComponent(floaterEntity);
				physicsActor->AddForceAtPosition(physicsWorld->GetGravity() / (float)activeFloatersIndex.size(), worldTransformComponent.Location, ForceMode::Acceleration);

				auto vertexPos = Utils::WorldToLocal(worldTransformComponent.Location, GetTransform());
				float waveHeight = GetWaveHeightAtPosition(vertexPos, FrameTime::GetTime());

				// if below water
				if (vertexPos.y < waveHeight)
				{

					auto gravity = physicsWorld->GetGravity();
					float displacementMulti = glm::clamp((waveHeight - vertexPos.y) / floater.SubmersionDepth, 0.f, 1.f) * floater.BuoyancyStrength;

					//upward buoyant force
					physicsActor->AddForceAtPosition(glm::vec3(0, glm::abs(gravity.y) * displacementMulti, 0), worldTransformComponent.Location, ForceMode::Acceleration);

					// apply water drag force against velocity
					physicsActor->AddForce(displacementMulti * -physicsActor->GetLinearVelocity() * floater.Drag * deltaTime, ForceMode::VelocityChange);

					// apply water angularDrag torque against angular velocity
					physicsActor->AddTorque(displacementMulti * -physicsActor->GetAngularVelocity() * floater.AngularDrag * deltaTime, ForceMode::VelocityChange);


				}
			}




		}

	}

	void GerstnerWave::RecomputeGerstnerWave(UBGerstnerWave& wave)
	{
		const float gravity = 981.0f;
		wave.Dispersion = 2.0 * glm::pi<float>() / wave.Wavelength;
		wave.WaveVector = wave.WaveDirection * wave.Dispersion;
		//wave.Speed = glm::sqrt(wave.Dispersion * gravity);
		//wave.Speed = glm::sqrt(wave.Dispersion * GerstnerData.Speed);
		wave.Speed = GerstnerData.Speed;
		wave.WKA = wave.Amplitude * wave.Dispersion;
		wave.Q = wave.Amplitude * (wave.Steepness / wave.WKA); // in shader we dont multiply amplatiude
		//wave.Q = (wave.Steepness / wave.WKA);
	}

	void GerstnerWave::RecomputeWaves()
	{
		int seed = GerstnerData.RandomSeed == true ? Random::Int<int>() : GerstnerData.Seed;
		RandomStream randomStream(seed);

		uint32_t waveCount = glm::min(GerstnerData.WaveCount, GerstnerData.MaxGerstnerWavesCount);
		for (uint32_t i = 0; i < waveCount; i++)
		{

			float alpha = glm::clamp(1.0f - (float(i) / float(waveCount)) + randomStream.FRandRange(GerstnerData.WaveDistribution * (-1.0f / float(waveCount)), GerstnerData.WaveDistribution * (1.0f / float(waveCount))), 0.0f, 1.0f);

			auto& gerstnerWave = m_Waves[i];

			int randx = randomStream.Next();
			int randy = randomStream.Next();


			gerstnerWave.WaveDirection = glm::vec2(cos((float)randx / 801571.f), sin((float)randy / 10223.f));

			// lerp with wind direction & make sure it's normalized
			gerstnerWave.WaveDirection = glm::normalize(glm::mix(GerstnerData.WaveDirection, (gerstnerWave.WaveDirection * 2.f) - 1.f, GerstnerData.WaveSpread));


			float falloff = glm::mix(1.0f, alpha, GerstnerData.WavelengthFalloff);
			gerstnerWave.Wavelength = glm::mix(GerstnerData.MinMaxWavelength.x, GerstnerData.MinMaxWavelength.y, falloff);

			falloff = glm::mix(1.0f, alpha, GerstnerData.AmplitudeFallOff);
			gerstnerWave.Amplitude = glm::max(glm::mix(GerstnerData.MinMaxAmplitude.x, GerstnerData.MinMaxAmplitude.y, falloff), 0.0001f);

			falloff = glm::mix(1.0f, alpha, GerstnerData.SteepnessFallOff);
			gerstnerWave.Steepness = glm::mix(GerstnerData.MinMaxSteepness.x, GerstnerData.MinMaxSteepness.y, falloff);

			RecomputeGerstnerWave(gerstnerWave);
		}
	}

}

