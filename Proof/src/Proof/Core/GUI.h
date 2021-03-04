#pragma once

#include "ImGUIOpenGL/imgui.h"
#include "ImGUIOpenGL/imgui_impl_glfw.h"
#include "ImGUIOpenGL/imgui_impl_opengl3.h"
#include "Core.h"
#include <iostream>
#include<stdio.h> 
#include<stdarg.h>				
namespace Proof {
    class Proof_API GUI {
    public:
        static	enum  ProofFlags_{
            ProofFlags_None = 0,
            ProofFlags_NoTitleBar = 1 << 0,   // Disable title-bar
            ProofFlags_NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
            ProofFlags_NoMove = 1 << 2,   // Disable user moving the window
            ProofFlags_NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
            ProofFlags_NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
            ProofFlags_NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it
            ProofFlags_AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
            ProofFlags_NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
            ProofFlags_NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
            ProofFlags_NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
            ProofFlags_MenuBar = 1 << 10,  // Has a menu-bar
            ProofFlags_HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
            ProofFlags_NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
            ProofFlags_NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
            ProofFlags_AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
            ProofFlags_AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
            ProofFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
            ProofFlags_NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
            ProofFlags_NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
            ProofFlags_UnsavedDocument = 1 << 20,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
            ProofFlags_NoNav = ProofFlags_NoNavInputs | ProofFlags_NoNavFocus,
            ProofFlags_NoDecoration = ProofFlags_NoTitleBar | ProofFlags_NoResize | ProofFlags_NoScrollbar | ProofFlags_NoCollapse,
            ProofFlags_NoInputs = ProofFlags_NoMouseInputs | ProofFlags_NoNavInputs | ProofFlags_NoNavFocus,

            // [Internal]
            ProofFlags_NavFlattened = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to cross over parent border to this child (only use on child that have no scrolling!)
            ProofFlags_ChildWindow = 1 << 24,  // Don't use! For internal use by BeginChild()
            ProofFlags_Tooltip = 1 << 25,  // Don't use! For internal use by BeginTooltip()
            ProofFlags_Popup = 1 << 26,  // Don't use! For internal use by BeginPopup()
            ProofFlags_Modal = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
            ProofFlags_ChildMenu = 1 << 28   // Don't use! For internal use by BeginMenu()

            // [Obsolete]
            //ProofFlags_ResizeFromAnySide    = 1 << 17,  // --> Set io.ConfigWindowsResizeFromEdges=true and make sure mouse cursors are supported by backend (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)
        };

        GUI(const char* Frame, bool CreateNewFrame); // create A new frame
        void Starup(const char* AddNewFrame, bool* RefToVariable = nullptr, int Flag = ProofFlags_None);
        void End();

        std::string GetFrameName(); // get the frame name 
        void AddCheckBox(const char* CheckBoxName, bool* Variable);
        void AddSliderfloat(const char* Name, float* Variable, float MinValue, float MaxValue);
        void GUI::AddText( const char* format, ...) {
			va_list arg;
			va_start(arg, format);
            va_end(arg);
            ImGui::Text(format);
          
		}
        

        void AddSameLine();
        bool CreateButton(const char* ButtonName, float width, float height); // use this in an if statement never works on its own
        bool BeginMenuBar();
        void EndMenuBar();

        bool BeginMenu(const char* MenuName);
        void EndMenu();
        bool AddMenuItem(const char* Item);
        void ButtonHoverColor(float R = 0.2f, float G = 0.995f, float B = 0.49f, float A = 1.0f) {
            auto& colors = ImGui::GetStyle().Colors;
            colors[ImGuiCol_Button] = ImVec4{R, G, B, A };
        };

        void ButtonColor(float R = 0.15f, float G = 0.1505f, float B = 0.151f, float A = 1.0f) {
        };
        void ButtonActiveColor(float R = 0.15f, float G = 0.1505f, float B = 0.151f, float A = 1.0f) {
        };
    private:
        std::string FrameName;
        void Colors();
    };
}

