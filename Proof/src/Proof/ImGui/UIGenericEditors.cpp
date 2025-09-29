#include "Proofprch.h"
#include "UIGenericEditors.h"
#include "UiUtilities.h"
#include "UI.h"
#include <ImCurveEdit.h>
#include <ImGradient.h>

namespace Proof::UI
{

    class AnimationCurveEditor : public ImCurveEdit::Delegate 
    {
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

    class ColorGradientDelegate : public ImGradient::Delegate
    {
    public:
        ColorGradientDelegate(Proof::ColorGradient* gradient = nullptr)
            : m_Gradient(gradient) 
        {

        }
        size_t GetPointCount() override
        {
            return m_Gradient ? m_Gradient->GetKeys().size() : 0;
        }

        ImVec4* GetPoints() override
        {
            m_Points.clear();
            // Provide a contiguous array of ImVec4 with time stored in .w
            if (m_Gradient)
            {
                for (auto& k : m_Gradient->GetKeys())
                    m_Points.push_back(ImVec4(k.Color.r, k.Color.g, k.Color.b, k.Time));
            }
            return m_Points.data();
        }

        int EditPoint(int pointIndex, ImVec4 value) override
        {
            if (!m_Gradient || pointIndex < 0 ||
                pointIndex >= (int)m_Gradient->GetKeys().size())
                return -1;

            auto key = m_Gradient->GetKey(pointIndex);
            key.Color = glm::vec4(value.x, value.y, value.z, key.Color.a); // preserve alpha
            key.Time = glm::clamp(value.w, 0.0f, 1.0f);
			m_Gradient->Updatekey(pointIndex, key);

            return pointIndex;
        }

        ImVec4 GetPoint(float t) override
        {
            if (!m_Gradient) return ImVec4(1, 1, 1, 1);
            glm::vec4 c = m_Gradient->Evaluate(t);
            return ImVec4(c.r, c.g, c.b, t);
        }

        void AddPoint(ImVec4 value) override
        {
            if (!m_Gradient) return;
            m_Gradient->AddColorKey(glm::vec4(value.x, value.y, value.z, 1.0f), value.w);
        }

        virtual float GetPointAlpha(float t) { return m_Gradient->Evaluate(t).w; };


    private:
        Proof::ColorGradient* m_Gradient;
        std::vector<ImVec4> m_Points;

    };

    ColorGradientDelegate del;
    bool ColorGradientEditor(const std::string& name, ColorGradient& gradient, ImVec2 size)
    {

        UI::AttributeLabel(name);
        static ColorGradient* activeGradient = nullptr;

        activeGradient = &gradient;

        del = ColorGradientDelegate(&gradient);

        int sel = 0;
        bool dirty = false;
        int colorIndex;

		ImGradient::GradientMode mode = gradient.Mode == ColorGradientMode::Blend ? ImGradient::GradientMode::Blend : ImGradient::GradientMode::Fixed;
        ImGui::SameLine();
        dirty |= ImGradient::Edit(name.c_str(), del, size, colorIndex, mode);

		gradient.Mode = mode == ImGradient::GradientMode::Blend ? ColorGradientMode::Blend : ColorGradientMode::Fixed;

        if (colorIndex != -1)
        {
			auto key = gradient.GetKey(colorIndex); // ensure valid
            dirty |= ImGui::ColorEdit4(fmt::format("##Color{}",name).c_str(), &key.Color.x);


            ImGui::DragFloat(fmt::format("##Time{}", name).c_str(), &key.Time, 0.01, 0, 1.0f);

			gradient.Updatekey(colorIndex, key);
        }

        return dirty;
    }
}
