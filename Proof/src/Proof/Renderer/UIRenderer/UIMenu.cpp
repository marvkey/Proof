#include "Proofprch.h"
#include "UIMenu.h"
#include "Proof/Utils/StringUtils.h"
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
    UIElement UIMenu::CreateElement(UIElementType type, UIElementID id)
    {

        return CreateElement(EnumReflection::EnumString(type), id, type);
    }
    UIElement UIMenu::CreateElement(const std::string& name, UIElementID id, UIElementType type)
    {
        if (id == 0)
        {

            PF_EC_ERROR("Cannot create ui with and id 0");
            return {};
        }

        if (HasUIElement(id))
        {
            PF_EC_ERROR("Cannot create ui with and id {} already being used", id);
            return GetUIElement(id);
        }
        UIElement uiElement = { m_Registry.create(), this };
        m_Registry.emplace<UICoreComponent>(uiElement.GetenttID()).m_ElementID = id;
        uiElement.GetComponent<UICoreComponent>().ElementType = type;

        m_UIElementsMap[id] = uiElement;

        std::string actualName = Utils::String::GenerateUniqueName(name, m_UIElementsNameMap);
        uiElement.GetComponent<UICoreComponent>().m_Name = actualName;
        m_UIElementsNameMap[actualName] = id;


        switch (type)
        {
            case Proof::UIElementType::None:
                break;
            case Proof::UIElementType::Button:
                m_Registry.emplace<UIButtonComponent>(uiElement);
                break;
            case Proof::UIElementType::Image:
                m_Registry.emplace<UIImageComponent>(uiElement);
                break;
            case Proof::UIElementType::Text:
                m_Registry.emplace<UITextComponent>(uiElement);
                break;
            default:
                break;
        }
        return uiElement;
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
    void UIMenu::SetName(UIElement element, const std::string& newName)
    {
        std::string oldName = element.GetComponent<UICoreComponent>().m_Name;

        if (m_UIElementsNameMap.contains(newName))
            return;

        m_UIElementsNameMap.erase(oldName);

        m_UIElementsNameMap[newName] = element.GetUUID();
        element.GetComponent<UICoreComponent>().m_Name = newName;

    }
}
