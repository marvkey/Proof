#include "Proofprch.h"
#include "UIRenderer.h"
#include "UIPanel.h"
#include "UIMenu.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/RenderPass.h"

#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof {

    static Count<Renderer2D> s_Renderer2D = nullptr;
    static Count<RenderPass> s_RenderPass = nullptr;
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

 
    void UIRenderer::DrawUI(Count<class UIPanel> panel, Count<class Renderer2D> renderer, const glm::mat4& projectionMatrix, const glm::mat4 viewProjection, uint32_t screenWidth, uint32_t screenHeight)
    {

      //  glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f);
        glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f);

        renderer->BeginContext(orthoMatrix, glm::mat4(1.0f), Vector(0.0f), { true });

        auto menu = panel->Menu;


        for (auto& [uiElementId,uiElement] : menu->m_UIElementsMap)
        {
            //if (element->Parent != nullptr)
                    //continue;
            DrawElement(panel, renderer, projectionMatrix, viewProjection, screenWidth, screenHeight, uiElement);

        }
          
        renderer->EndContext();
    }

    struct UIRenderFinalData 
    {
        glm::vec2 Position; // Top-left corner in pixels
        glm::vec2 Size;     // Width and height in pixels
    };

    UIRenderFinalData  CalculateRenderData(const UITransform& renderData, glm::vec2 screenSize, glm::vec2 elementSize)
    {
        /*
        const auto& anchor = renderData.Anchor;
        // Calculate the top-left and bottom-right positions from the anchor
        glm::vec2 topLeft = glm::vec2(
            anchor.Minimum.x * screenSize.x, // Scale anchor min.x by screen width
            anchor.Minimum.y * screenSize.y  // Scale anchor min.y by screen height
        );

        glm::vec2 bottomRight = glm::vec2(
            anchor.Maximum.x * screenSize.x, // Scale anchor max.x by screen width
            anchor.Maximum.y * screenSize.y  // Scale anchor max.y by screen height
        );

        // Calculate the center of the anchor rectangle
        glm::vec2 anchorCenter = (topLeft + bottomRight) * 0.5f;

        // Calculate the size of the element (using the anchor bounds for scaling)
        glm::vec2 scaledSize = bottomRight - topLeft;

        // Apply alignment offset
        glm::vec2 alignmentOffset = glm::vec2(
            renderData.Alignment.x * elementSize.x, // Scale alignment.x by element width
            renderData.Alignment.y * elementSize.y  // Scale alignment.y by element height
        );

        // Calculate the final position (anchorCenter adjusted by alignment offset)
        glm::vec2 position = anchorCenter - alignmentOffset;
        glm::vec2 size = bottomRight - topLeft;
        */

        // Step 1: Calculate the anchor rectangle in screen space
        glm::vec2 anchorTopLeft = screenSize * renderData.Anchor.Minimum;
        glm::vec2 anchorBottomRight = screenSize * renderData.Anchor.Maximum;
        glm::vec2 anchorSize = anchorBottomRight - anchorTopLeft;

        // Step 2: Apply alignment within the anchor rectangle
        glm::vec2 alignedPosition = anchorTopLeft + anchorSize * renderData.Alignment;

        // Step 3: Add offset position
        glm::vec2 finalPosition = alignedPosition + renderData.Position;

     
        return UIRenderFinalData{ finalPosition, elementSize };
    }

    void UIRenderer::DrawElement(Count<class UIPanel> panel, Count<class Renderer2D> renderer, const glm::mat4& projectionMatrix, const glm::mat4 viewProjection, uint32_t screenWidth, uint32_t screenHeight, class UIElement element)
    {

        glm::vec2 elementSize = element.GetComponent<UICoreComponent>().Transform.Size;     // Element size in pixels

        // Calculate the transform based on the anchor
        UIRenderFinalData renderData = CalculateRenderData(element.GetComponent<UICoreComponent>().Transform, glm::vec2{screenWidth,screenHeight}, elementSize);

        glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(element.GetComponent<UICoreComponent>().Transform.Rotation, 1.0)));

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Create a transformation matrix for this elemecv nt
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(renderData.Position, 0.0f)) 
            *  glm::scale(glm::mat4(1.0f), glm::vec3(renderData.Size, 1.0f));

  

        // 6. Combine both matrices
        glm::mat4 finalTransform = transform;

        switch (element.GetElementType())
        {
            case UIElementType::Button:
            {
                auto button = element.GetComponent< UIButtonComponent>();
                renderer->DrawQuad(finalTransform, button.TintColor,button.Texture == nullptr ? Renderer::GetWhiteTexture() : button.Texture);
                break;
            }
            case UIElementType::Image:
            {
                auto image = element.GetComponent< UIImageComponent>();
                renderer->DrawQuad(finalTransform, image.TintColor,image.Texture == nullptr ? Renderer::GetWhiteTexture() : image.Texture);
                break;
            }
            case UIElementType::Text:
            {
                auto text = element.GetComponent<UITextComponent>();
                renderer->DrawString(text.Text, text.Font, text.TextConfig, finalTransform);
                break;
            }
        }
    }
}
