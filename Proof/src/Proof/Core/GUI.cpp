#include "Proofprch.h"
#include "GUI.h"
#include <stdio.h>

namespace Proof {
    GUI::GUI(const char* Frame, bool CreateNewFrame) {


        this->FrameName = Frame;
        if (CreateNewFrame == true) {
            ImGui::NewFrame();
        }
    }
    void GUI::Starup(const char* AddNewFrame, bool* RefToVariable, ProofFlags_ Flag) {


        ImGui::Begin(AddNewFrame, RefToVariable, (ImGuiWindowFlags_)Flag);

    }
    std::string GUI::GetFrameName() {
        return this->FrameName;
    }

    void GUI::AddCheckBox(const char* CheckBoxName, bool* Variable) {
        ImGui::Checkbox(CheckBoxName, Variable);
    }

    void GUI::AddSliderfloat(const char* Name, float* Variable, float MinValue, float MaxValue) {

        ImGui::SliderFloat(Name, Variable, MinValue, MaxValue);
    }

    void GUI::AddSameLine() {
        ImGui::SameLine();
    }

    void GUI::End() {

        ImGui::End();

    }

    bool GUI::CreateButton(const char* ButtonName, float width, float height) {

        ImVec2 Size = ImVec2(width, height);
        return ImGui::Button(ButtonName, Size);

    }

    bool GUI::BeginMenuBar() {
        return ImGui::BeginMenuBar();
    }

    void GUI::EndMenuBar() {

        ImGui::EndMenuBar();

    }

    bool GUI::BeginMenu(const char* MenuName) {
        return ImGui::BeginMenu(MenuName);
    }

    bool GUI::AddMenuItem(const char* Item) {
        return ImGui::MenuItem(Item);
    }

    void GUI::EndMenu() {
        ImGui::EndMenu();

    }

    void GUI::Colors() {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.995f, 0.49f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

}
