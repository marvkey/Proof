#pragma once
#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Physics/PhysicsTypes.h"

namespace Proof
{
	enum class ECollisionComplexity : uint8_t;
	class MeshColliderEditorPanel : public AssetEditor
	{

	public:
		MeshColliderEditorPanel();

		virtual void OnImGuiRender()override;
		virtual void OnUpdate(FrameTime ts);
		virtual void SetAsset(const Count<class Asset>& asset);

	protected:
		virtual void OnEvent(class Event& e);
		virtual bool IsSaved() { return !m_NeedsSaving; }
		virtual void Save();
	private:
		void RenderSettingsPanel();
		void RenderViewPortPanel();

		void UpdatePreviewEntity();
		bool CookMeshCollider();
		void RenderCookingOutput();

		virtual void SetDefaultLayout();
	private:
		Count<class MeshCollider> m_MeshCollider;
		Count<class World> m_World;
		Count<class DetailsPanel> m_DetailsPanel;
		Special<class EditorWorkspaceManager> m_WorkSpaceManager;

		EditorCamera m_Camera;
		bool m_IsViewportFocused = false;
		bool m_NeedsCooking = false;
		bool m_ShowCookingResults = true;
		bool m_IsCookingResultsOpen = false;
		bool m_NeedsSaving = true;
		ImVec2 m_ViewPoartSize{ 0,0 };
		CookingResult m_LastSimpleCookingResult = CookingResult::None;
		CookingResult m_LastComplexCookingResult = CookingResult::None;
		ECollisionComplexity m_CollisionComplexity;// so it does not change wihtout being cooked with using the value
		std::string m_ViewportPanelName;
		std::string m_DetailsPanelName;
	};
}