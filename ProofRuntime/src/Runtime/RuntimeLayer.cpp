#include "RuntimeLayer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/SceneSerializer.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof {
	void RuntimeLayer::OnAttach()
	{
		m_World = Count<World>::Create();
		auto startworld = Application::Get()->GetProject()->GetConfig().StartWorld;

		if (AssetManager::HasAsset(startworld))
		{
			auto Info = AssetManager::GetAssetInfo(startworld);
			SceneSerializer scerelizer(m_World.Get());
			auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
			if (scerelizer.DeSerilizeText(path.string()) == true)
			{
				AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
			}
		}

		m_World->StartRuntime();
	}
	void RuntimeLayer::OnDetach()
	{
		m_World->EndRuntime();
	}
	void RuntimeLayer::OnUpdate(FrameTime DeltaTime)
	{

		m_World->OnUpdateRuntime(DeltaTime);
	}
}
