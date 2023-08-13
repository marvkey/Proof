#include "Proofprch.h"
#include "UI.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/ImGuiLayer.h"

namespace Proof::HeaderFileOnly {

    ImTextureID TextureUI::GetTexture(Count<Image> image)
    {
        return Application::Get()->GetImguiLayer()->ToImguiImage(image);
    }
}

