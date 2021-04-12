#pragma once
#include "Proof/Core/Layer.h"
namespace Proof {

    class Proof_API ImGuiLayer :public Proof::Layer{
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;
        virtual void OnAttach()override;
        virtual void OnDetach()override;
        virtual void OnImGuiDraw() override;
        void Begin();
        void End();
        void SetDarkTheme();
    private:
        ImGuiIO& io = ImGui::GetIO();
    };
}

