#include "Proofprch.h"
#include "UIGenericEditors.h"
#include "UiUtilities.h"
#include "UI.h"
#include <ImCurveEdit.h>

namespace Proof::UI
{

    class AnimationCurveEditor : public ImCurveEdit::Delegate {
    public:
        InterpolationCurve* curve;

        AnimationCurveEditor(InterpolationCurve* c) : curve(c) {}

        size_t GetCurveCount() override { return 1; }
        ImCurveEdit::CurveType GetCurveType(size_t) const override { return ImCurveEdit::CurveSmooth; }
        ImVec2& GetMin() override { return *(ImVec2*)&curve->Min; }
        ImVec2& GetMax() override { return *(ImVec2*)&curve->Max; }
        size_t GetPointCount(size_t) override { return curve->Points.size(); }
        uint32_t GetCurveColor(size_t) override { return 0xFF00FFFF; }
        ImVec2* GetPoints(size_t) override { return reinterpret_cast<ImVec2*>(curve->Points.data()); }

        int EditPoint(size_t, int index, ImVec2 value) override {
            curve->EditPoint(index, *(glm::vec2*)&value);
            return index;
        }

        void AddPoint(size_t, ImVec2 value) override {
            curve->AddPoint(*(glm::vec2*)&value);
        }
    };


    bool InterpolationCurveEditor(const std::string& name, InterpolationCurve& curve, ImVec2 size)
    {
        static bool openPopup = false;
        static InterpolationCurve* activeCurve = nullptr;

        if (UI::AttributeButton(name.c_str()))
        {
            ImGui::OpenPopup(name.c_str());
            openPopup = true;
            activeCurve = &curve;
        }

        bool modified = false;

        //if (ImGui::IsPopupOpen(name.c_str()))
        //    ImGui::SetNextWindowSize(size, ImGuiCond_Once); // <-- Set popup size before opening

        bool wasOpen = false;
        if (ImGui::BeginPopupModal(name.c_str(), &openPopup))
        {
            AnimationCurveEditor delegate(activeCurve);

            if (ImCurveEdit::Edit(delegate, ImGui::GetContentRegionAvail(), ImGui::GetID("CurveEditor")))
                modified = true;

            wasOpen = true;
           

            ImGui::EndPopup();
        }
        if (!openPopup && wasOpen)
        {
            ImGui::CloseCurrentPopup();
            openPopup = false;
            activeCurve = nullptr;
        }
        HandleModified(modified);
        return modified;
    }
}
