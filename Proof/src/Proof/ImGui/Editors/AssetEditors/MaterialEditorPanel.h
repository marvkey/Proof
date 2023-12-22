#pragma once
#include "AssetEditor.h"
namespace Proof
{
	class MaterialEditorPanel:public AssetEditor 
	{
	public:
		MaterialEditorPanel();

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(FrameTime ts);
		virtual void SetAsset(const Count<class Asset>& asset);
		virtual bool IsSaved() { return !m_NeedsSaving; };
		virtual void Save();
	private:
		bool m_NeedsSaving = true;
		Count<class Material> m_Material;
	};
	class PhysicsMaterialEditorPanel :public AssetEditor 
	{
	public:
		PhysicsMaterialEditorPanel();
		virtual void OnImGuiRender() override;
		virtual bool IsSaved() { return !m_NeedsSaving; };
		virtual void Save();
		virtual void SetAsset(const Count<class Asset>& asset);
	private:
		Count<class PhysicsMaterial> m_Material;
		bool m_NeedsSaving = true;
	};

	
}
