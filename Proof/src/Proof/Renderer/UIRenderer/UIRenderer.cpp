#include "Proofprch.h"
#include "UIRenderer.h"
#include "UIPanel.h"
#include "UIMenu.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Asset/AssetManager.h"

#include <glm/gtx/matrix_decompose.hpp>
namespace Proof {

    static Count<Renderer2D> s_Renderer2D = nullptr;
    static Count<RenderPass> s_RenderPass = nullptr;
    Count<RenderCommandBuffer> s_CommandBuffer;
    void UIRenderer::DrawUI(Count<class UIMenu> menu, Count<class Renderer2D> renderer, uint32_t screenWidth, uint32_t screenHeight)
    {

        glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f);



        renderer->BeginContext(orthoMatrix, glm::mat4(1.0f), Vector(0.0f), { true });



        for (auto& [uiElementId,uiElement] : menu->m_UIElementsMap)
        {
            //if (element->Parent != nullptr)
                    //continue;
            DrawElement(menu, renderer, screenWidth, screenHeight, uiElement);

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
        // Step 1: Calculate the anchor rectangle in screen space
         // Flip Y because Vulkan screen space has origin at top-left
        glm::vec2 anchorTopLeft = screenSize * glm::vec2(renderData.Anchor.Minimum.x, 1.0f - renderData.Anchor.Minimum.y);
        glm::vec2 anchorBottomRight = screenSize * glm::vec2(renderData.Anchor.Maximum.x, 1.0f - renderData.Anchor.Maximum.y);
        glm::vec2 anchorSize = anchorBottomRight - anchorTopLeft;

        // Step 2: Apply alignment within the anchor rectangle
        glm::vec2 alignedPosition = anchorTopLeft + anchorSize * renderData.Alignment;

        // Step 3: Add offset position
        glm::vec2 finalPosition = alignedPosition + renderData.Position;

     
        return UIRenderFinalData{ finalPosition, elementSize };
    }
    namespace Utils
    {
        static Count<Texture2D> GetTexture(BindableStaticAssetKey key)
        {
            auto assetKey = key.GetValue();
            if (assetKey.GetExpectedType() != AssetType::Texture)
                return Renderer::GetWhiteTexture();

            if (!assetKey.IsValid())
                return Renderer::GetWhiteTexture();

            return AssetManager::GetAsset<Texture2D>(assetKey.GetAssetID());
        }
    }
   
    void UIRenderer::DrawElement(Count<class UIMenu> menu, Count<class Renderer2D> renderer, uint32_t screenWidth, uint32_t screenHeight, class UIElement element)
    {

        auto worldTransformComp = menu->GetWorldTransform(element);
        glm::vec2 elementSize = worldTransformComp.Size;     // Element size in pixels

        // Calculate the transform based on the anchor
        UIRenderFinalData renderData = CalculateRenderData(worldTransformComp, glm::vec2{screenWidth,screenHeight}, elementSize);

        glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(worldTransformComp.Rotation, 1.0)));

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
                auto& button = element.GetComponent< UIButtonComponent>();


                renderer->DrawQuad(finalTransform, button.TintColor,Utils::GetTexture(button.Texture));
                break;
            }
            case UIElementType::Image:
            {
                auto& image = element.GetComponent< UIImageComponent>();
                renderer->DrawQuad(finalTransform, image.TintColor, Utils::GetTexture(image.Texture));
                break;
            }
            case UIElementType::Text:
            {
                auto text = element.GetComponent<UITextComponent>();
                renderer->DrawString(text.Text.GetValue(), text.Font, text.TextConfig, finalTransform);
                break;
            }
            case UIElementType::VerticalBox:
            {
                auto& layout = element.GetComponent<UIVerticalBoxComponent>();
                glm::vec2 childStartPos = element.GetComponent<UICoreComponent>().Transform.Position;
                float yOffset = 0.0f;
                for (const auto& childID : element.Children())
                {
                    UIElement child = menu->GetUIElement(childID);
                    if (!child) continue;

                    auto& childTransform = child.GetComponent<UICoreComponent>().Transform;
                    childTransform.Position = childStartPos + glm::vec2(0.0f, yOffset);
                    yOffset += childTransform.Size.y + layout.Spacing;

                    DrawElement(menu, renderer, screenWidth, screenHeight, child);
                }
            }

            case UIElementType::HorizontalBox:
            {
                // Compute foreground (filled) width
                glm::mat4 filledTransform = finalTransform;
                glm::vec3 scale, translation, skew; // skew not really impportant
                glm::vec4 perspective;
                glm::quat rotation;
                bool couldDecompose = true;
                // Decompose original transform to modify width
                if (!glm::decompose(finalTransform, scale, rotation, translation, skew, perspective))
                {
                    couldDecompose = false;
                }
                auto& layout = element.GetComponent<UIHorizontalBoxComponent>();
                //glm::vec2 childStartPos = glm::vec2(translation);
                glm::vec2 childStartPos = element.GetComponent<UICoreComponent>().Transform.Position;
                float xOffset = 0.0f;
                for (const auto& childID : element.Children())
                {
                    UIElement child = menu->GetUIElement(childID);
                    if (!child) continue;

                    auto& childTransform = child.GetComponent<UICoreComponent>().Transform;
                    childTransform.Position = childStartPos + glm::vec2(xOffset, 0.0f);
                    xOffset += childTransform.Size.y + layout.Spacing;

                    DrawElement(menu, renderer, screenWidth, screenHeight, child);
                }
                break;
            }
            case UIElementType::ProgressBar:
            {
                auto& progressBar = element.GetComponent<UIProggresBarComponent>();

                // Draw background
                renderer->DrawQuad(finalTransform, progressBar.BackgroundColor,Renderer::GetWhiteTexture());

                // Compute foreground (filled) width
                glm::mat4 filledTransform = finalTransform;
                glm::vec3 scale, translation, skew; // skew not really impportant
                glm::vec4 perspective;
                glm::quat rotation;

                // Decompose original transform to modify width
                if (glm::decompose(finalTransform, scale, rotation, translation, skew, perspective))
                {
                    float originalWidth = scale.x;

                    scale.x *= progressBar.Proggress.GetValue().GetValue(); // Scale width based on progress (0 to 1)
					glm::vec3 offset = glm::vec3(0.0f); // Initialize offset to zero

					// since engien scalees from center, teh translation needs to be adjusted
                    {
                        // Compute offset so bar grows to the right instead of center

                        // Since scaling happens from the center, we shift the quad by half the scaled-down width 
                        // to keep the left edge fixed while the bar fills rightward
                        float offsetX = (originalWidth - scale.x) * 0.5f;
                        offset = glm::vec3(offsetX, 0.0f, 0.0f);

                        // Rotate the offset by the object's rotation to keep world-aligned
                        offset = rotation * offset;
                    }
                    

                    filledTransform = glm::translate(glm::mat4(1.0f), translation - offset) *
                        glm::toMat4(rotation) *
                        glm::scale(glm::mat4(1.0f), scale);
                }

                // Draw filled part
                renderer->DrawQuad(filledTransform, progressBar.FillColor,Renderer::GetWhiteTexture());
                break;
            }
        }
    }
}
