#pragma once
#include "Proof/Core/Layer.h"
#include "UITypes.h"
#include <stack>
/*There is going to be a solution to a promblem when in edit mode ImGui is not going to receive input from the normal input system
When the game is in the mode of gmaplay we are going to make multi window of ImGui false so that in the game Mode we can use the normal input
ImGUI WILL BE THE first to receive input then the gampley if the Input is not needed by imgui then the next layer in the layer stack will check if 
it needs it if that is false then we will move on untill the whole layer is looped.
*/
namespace Proof 
{
   
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ImGuiLayer(const std::string& name);
        virtual ~ImGuiLayer() {};
        virtual void Begin() = 0;
        virtual void End();
        void SetDarkTheme();
        static Count<class ImGuiLayer> Create();
        virtual ImTextureID ToImguiImage(Count<class Image> iamge) = 0;
        virtual ImTextureID ToImguiImage(Count<class Texture2D> texture) = 0;

        void PushModified(bool& value)
        {
            m_ModifiedTypes.push(&value);
        }
        void PopModified();
        void UpdateCurrentModifiedType(bool value);


		void ShowRawMessageBox(const std::string& title, UI::UIMessageBoxData& data)
		{
			m_MessageBoxes[title] = data;
			data.Title = fmt::format("{0}##MessageBox{1}", title, m_MessageBoxes.size() + 1);
		}
    private:
        std::stack<bool*> m_ModifiedTypes;
        std::unordered_map<std::string, UI::UIMessageBoxData> m_MessageBoxes;
    };
}

