#pragma once

#include "AssetEditor.h"
#include "Proof/Audio/AudioMixer.h"

namespace Proof
{
    class AudioMixerEditorPanel : public AssetEditor
    {
    public:
        AudioMixerEditorPanel();

        virtual void SetAsset(const Count<class Asset>& asset) override;
        virtual void OnUpdate(FrameTime ts) override;
        virtual void OnImGuiRender() override;
        virtual void Save() override;
        virtual  bool IsSaved() {return  !m_NeedsSaving;};
    private:
        void DrawToolbar();
        void DrawGroupHierarchy();
        void DrawGroupNode(UUID groupID);
        void DrawGroupInspector();

        void DrawEffectTable(const Count<class AudioMixerGroup>& group);
        void DrawEffect(const Count<class AudioEffect>& effect, uint32_t index);

        void CreateGroup(UUID parentID);
        void DeleteSelectedGroup();

        void SelectGroup(UUID groupID);

        static float DecibelsToVolume(float decibels);
        static float VolumeToDecibels(float volume);
        virtual bool IsSubWindowsHovered() {return false;};
        virtual bool IsSubWindowsFocused() {return false;};


    private:
        Count<AudioMixer> m_AudioMixer = nullptr;

        UUID m_SelectedGroupID = { 0 };
        float m_DisplayedOutputLevel = 0.0f;
        bool m_NeedsSaving = false;
    };
}