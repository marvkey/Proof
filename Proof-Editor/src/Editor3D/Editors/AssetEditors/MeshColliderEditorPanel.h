#pragma once
#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Physics/PhysicsTypes.h"

namespace Proof
{
	class MeshColliderEditorPanel :public AssetEditor
	{

	public:
		MeshColliderEditorPanel();

		virtual void OnImGuiRender()override;
		virtual void OnUpdate(FrameTime ts);
		virtual void SetAsset(const Count<class Asset>& asset);
	private:
		void RenderSettingsPanel();
		void RenderViewPortPanel();

		void UpdatePreviewEntity();
		bool CookMeshCollider();
		void RenderCookingOutput();

	private:
		float m_SaveCountDown = 100;
		Count<class MeshCollider> m_MeshCollider;
		Count<class WorldRenderer> m_WorldRenderer;
		Count<class World> m_World;
		EditorCamera m_Camera{ 200,200 };
		bool m_IsViewportFocused = false;
		bool m_NeedsCooking = false;
		bool m_ShowCookingResults = true;
		bool m_IsCookingResultsOpen = false;
		ImVec2 m_ViewPoartSize{ 0,0 };
		CookingResult m_LastSimpleCookingResult = CookingResult::None;
		CookingResult m_LastComplexCookingResult = CookingResult::None;

	};
}