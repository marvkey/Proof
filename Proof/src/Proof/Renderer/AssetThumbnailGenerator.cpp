#include "Proofprch.h"
#include "AssetThumbnailGenerator.h"
#include "Proof/Asset/AssetManager.h"
#
#include "Proof/Renderer/WorldRenderer.h"
namespace Proof{
	std::unordered_map<UUID, uint32_t> AssetThumbnailGenerator::AssetThumbnails;
	AssetThumbnailGenerator::AssetThumbnailGenerator() {
		//m_World = Count<World>::Create("Thumbmnail World");
		//m_WorldRenderer = Count<WorldRenderer>::Create(m_World,100,100);
	}
	AssetThumbnailGenerator::~AssetThumbnailGenerator() {
	}
	void AssetThumbnailGenerator::CreateThumbnail(UUID ID) {
		auto info = AssetManager::GetAssetInfo(ID);
		if (info.State == AssetState::Ready)return;

		//auto camera = m_World->CreateEntity("Camera");
		//camera.GetComponent<TransformComponent>().Location.Z -=5;
		//camera.AddComponent<CameraComponent>();
		//
		////m_World->m_EditorCamera.m_Positon.z -= 10;
		//m_World->CreateEntity("Light").AddComponent<DirectionalLightComponent>()->Color = Vector{ 1,1,1 };
		//if (info.Type == AssetType::Mesh) {
		//	m_World->CreateEntity("Mesh").AddComponent<MeshComponent>()->SetMesh(ID);
		//}
		//AssetThumbnails.insert({ ID, m_WorldRenderer->Renderer() });
	}
	void AssetThumbnailGenerator::GenerateThumbnail(UUID ID) {
		AssetThumbnailGenerator generator;
		generator.CreateThumbnail(ID);
	}
}
