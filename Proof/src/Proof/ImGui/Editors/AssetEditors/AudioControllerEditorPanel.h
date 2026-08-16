#pragma once
#include "AssetEditor.h"
namespace Proof
{
    class AudioControllerEditorPanel :public AssetEditor
    {
    public:
        AudioControllerEditorPanel();
        void RestartPreview();
        bool IsSubWindowsHovered() override { return false; }
        bool IsSubWindowsFocused() override { return false; }
        void DrawPlaybackPanel();
        virtual void OnImGuiRender() override;
        void DrawProperties();
        virtual void OnUpdate(FrameTime ts);
        virtual void SetAsset(const Count<class Asset>& asset);
        virtual bool IsSaved() { return !m_NeedsSaving; };
        virtual void Save();
    private:
        Count<class AudioController> m_AudioController;
        Count<class Sound> m_Sound;
        bool m_NeedsSaving = true;
    } ;
}
