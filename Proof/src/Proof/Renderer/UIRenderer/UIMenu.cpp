#include "Proofprch.h"
#include "UIMenu.h"
namespace Proof
{

    UIElement UIMenu::CreateElement(UIElementType type)
    {
        auto element = CreateElement(type, GenerateID());
        return element;
    }
    UIMenu::UIMenu(const UIMenu& other)
    {
        for (const auto& [id, element] : other.m_UIElementsMap)
            CreateElement(element.GetElementType(), id);
    }
    UIElement UIMenu::CreateElement(UIElementType type,UIElementID id)
    {
        
        UIElement uiElement = { m_Registry.create(), this };
        m_Registry.emplace<UICoreComponent>(uiElement.GetenttID()).m_ElementID = id;
        uiElement.GetComponent<UICoreComponent>().ElementType = type;

        m_UIElementsMap[id] = uiElement;

        switch (type)
        {
            case Proof::UIElementType::None:
                break;
            case Proof::UIElementType::Button:
                m_Registry.emplace<UIButtonComponent>(uiElement);
                uiElement.GetComponent<UICoreComponent>().Name = "Button";
                break;
            case Proof::UIElementType::Image:
                m_Registry.emplace<UIImageComponent>(uiElement);
                uiElement.GetComponent<UICoreComponent>().Name = "Image";
                break;
            case Proof::UIElementType::Text:
                m_Registry.emplace<UITextComponent>(uiElement);
                uiElement.GetComponent<UICoreComponent>().Name = "Text";
                break;
            default:
                break;
        }
        return uiElement;
    }
    UIElement UIMenu::CreateElement(const std::string& name, UIElementID id, UIElementType type)
    {
        auto element = CreateElement(type,id);
        element.GetComponent<UICoreComponent>().Name = name;
        return element;
    }
    UIElement UIMenu::GetUIElement(UIElementID id)
    {
        PF_CORE_ASSERT(HasUIElement(id));
        return m_UIElementsMap.at(id);
    }
    UIElement UIMenu::GetUIElement(UIElementID id) const
    {
        PF_CORE_ASSERT(HasUIElement(id));

        return m_UIElementsMap.at(id);
    }
}
