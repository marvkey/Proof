#include "Proofprch.h"
#include "UIRenderer.h"
#include "UIPanel.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/CommandBuffer.h"
namespace Proof {

    static Count<Renderer2D> s_Renderer2D = nullptr;
    static Count<RenderPass> s_RenderPass = nullptr;
    Count<ScreenFrameBuffer> s_ScreenFrameBuffer;
    Count<RenderCommandBuffer> s_CommandBuffer;
    #if 0
    Image UiRenderer::DrawUI(Count<class UIPanel> panel, glm::vec3 cameraLocaion, glm::mat4 projectionMatrix, glm::mat4 viewProjection, uint32_t width, uint32_t height)
    {
        if (s_Renderer2D ==nullptr)
        {
            s_ScreenFrameBuffer = ScreenFrameBuffer::Create(width, height);
            RenderPassConfig renderPassConfig("Ui RenderPass", s_ScreenFrameBuffer->GetFrameBuffer()->GetConfig());
            s_RenderPass = RenderPass::Create(renderPassConfig);
            s_CommandBuffer = RenderCommandBuffer::Create();
            s_Renderer2D = Count<Renderer2D>::Create(s_RenderPass,true);
        }
        Renderer::BeginCommandBuffer(s_CommandBuffer);
        Renderer::BeginRenderPass(s_CommandBuffer, s_RenderPass, s_ScreenFrameBuffer);  

        s_Renderer2D->BeginContext(projectionMatrix, viewProjection, GlmVecToProof(cameraLocaion), s_ScreenFrameBuffer, s_CommandBuffer);
        for (auto& [Id, button] : panel->GetImageButtons())
        {
            if (button.Visible == false)
                continue;
            glm::vec2 copy = { button.Postion.x / 4,button.Postion.y / 4 };
            s_Renderer2D->DrawQuad({ copy,0 }, { button.Rotation,0 }, { button.Size.x / 4,button.Size.y / 4,1 }, button.TintColour, button.Texture);
        }
        // gonna do this for render order
        for (auto& [Id,button] : panel->GetButtons())
        {

            if (button.Visible == false)
                continue;
            glm::vec2 copy = { button.Postion.x / 4,button.Postion.y/4 };
            s_Renderer2D->DrawQuad({ copy,0 }, { button.Rotation,0 }, { button.Size.x/4,button.Size.y / 4,1 }, button.TintColour,nullptr);
        }

      
        TextParams textParam;
        glm::mat4 textTransform;
        for(auto& [Id, text] : panel->GetTexts()){
            if (text.Visible == false)
                continue;
            textParam.Color = text.Param.Color;
            textParam.Kerning = text.Param.Kerning;
            textParam.LineSpacing = text.Param.LineSpacing;
            glm::vec2 copy = { text.Postion.x / 4,text.Postion.y / 4 };

            textTransform = glm::translate(glm::mat4(1.0f), { copy,0 }) *
                glm::rotate(glm::mat4(1.0f), glm::radians(text.Rotation.x), { 1,0,0 })
                * glm::rotate(glm::mat4(1.0f), glm::radians(text.Rotation.y), { 0,1,0 })
                * glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,0,1 })
                * glm::scale(glm::mat4(1.0f), { text.Size.x/8,text.Size.y / 8,1/8 });
            s_Renderer2D->DrawString(text.Text, text.Font, textParam, textTransform);
        }

        s_Renderer2D->EndContext();

        Renderer::EndRenderPass(s_RenderPass);
        Renderer::EndCommandBuffer(s_CommandBuffer);
        Renderer::SubmitCommandBuffer(s_CommandBuffer);

        return s_ScreenFrameBuffer->GetImage();
    }
    #endif
}
