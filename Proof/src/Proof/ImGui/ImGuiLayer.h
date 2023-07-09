#pragma once
#include "Proof/Core/Layer.h"
/*There is going to be a solution to a promblem when in edit mode ImGui is not going to receive input from the normal input system
When the game is in the mode of gmaplay we are going to make multi window of ImGui false so that in the game Mode we can use the normal input
ImGUI WILL BE THE first to receive input then the gampley if the Input is not needed by imgui then the next layer in the layer stack will check if 
it needs it if that is false then we will move on untill the whole layer is looped.
*/
namespace Proof {

    class Proof_API ImGuiLayer :public Layer{
    public:
        ImGuiLayer();
        ImGuiLayer(const std::string& name);
        virtual ~ImGuiLayer() {};
        virtual void Begin() = 0;
        virtual void End() = 0;
        void SetDarkTheme();
        static Count<ImGuiLayer> Create();
        virtual ImTextureID ToImguiImage(Count<class Image2D> iamge) = 0;
        virtual ImTextureID ToImguiImage(Count<class Texture2D> texture) = 0;

    };
}

