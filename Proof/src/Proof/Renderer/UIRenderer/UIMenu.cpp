#include "Proofprch.h"
#include "UIMenu.h"
#include "Proof/Utils/StringUtils.h"
namespace Proof
{
 

    template<typename... Component>
    static void CopyComponentSingleMenu(entt::registry& dst, entt::registry& src, const std::unordered_map<UIElementID, entt::entity>& enttMap)
    {
        ([&]() {

            auto view = src.view<Component>();
            for (auto srcEntity : view)
            {
                UUID id = src.get<UICoreComponent>(srcEntity).GetElementID();
                entt::entity dstEntity = enttMap.at(id);

                auto& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);

            }
            }(), ...);

    }

    template<typename... Component>
    static void CopyComponent(UIComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UIElementID, entt::entity>& enttMap)
    {
        CopyComponentSingleMenu<Component...>(dst, src, enttMap);
    }

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

    Count<UIMenu> UIMenu::Copy(Count<UIMenu> menuToCopy)
    {
        Count<UIMenu> newMenu = Count<UIMenu>::Create();


        auto& srcSceneRegistry = menuToCopy->m_Registry;
        auto& dstSceneRegistry = newMenu->m_Registry;
        std::unordered_map<UIElementID, entt::entity> enttMap;

        // Create entities in new scene
        // in reverse order
        auto idView = srcSceneRegistry.view<UICoreComponent>();
        auto begin = idView.begin();
        auto end = idView.end();

        while (end != begin) {
            --end;
            entt::entity e = *end;

            UICoreComponent coreComponent = srcSceneRegistry.get<UICoreComponent>(e);
            UIElement newElement = newMenu->CreateElement(coreComponent.GetName(), coreComponent.GetElementID(), coreComponent.ElementType);
            enttMap[coreComponent.GetElementID()] = (entt::entity)newElement;
        }

        // Copy components 
        CopyComponent(UIAllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);
        return newMenu;
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
			case Proof::UIElementType::ProgressBar:
				m_Registry.emplace<UIProggresBarComponent>(uiElement);
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

    UIElement UIMenu::GetUIElement(const std::string& name)
    {
        PF_CORE_ASSERT(HasUIElement(name));

        return GetUIElement(m_UIElementsNameMap.at(name));
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
    void UIMenu::SetVariableStorageSet(Count<class VariableSetStorage> storageSet)
    {
        for (auto& [uiElementId, uiElement] : m_UIElementsMap)
        {
            switch (uiElement.GetElementType())
            {
                case UIElementType::ProgressBar:
                {
                    auto& progressBar = uiElement.GetComponent<UIProggresBarComponent>();
					progressBar.Proggress.ChangeStorageSet(storageSet);
                }
                break;
            default:
                break;
            }
        }
    }
}
