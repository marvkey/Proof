#include "Proofprch.h"
#include "UIMenu.h"
#include "Proof/Utils/StringUtils.h"
#include <glm/gtx/matrix_decompose.hpp>

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
            case Proof::UIElementType::VerticalBox:
                m_Registry.emplace<UIVerticalBoxComponent>(uiElement);
                break;
            case Proof::UIElementType::HorizontalBox:
                m_Registry.emplace<UIHorizontalBoxComponent>(uiElement);
                break;
            default:
                break;
        }
        return uiElement;
    }
    glm::mat4 UIMenu::GetWorldTransformRaw(UIElement element)
    {
        glm::mat4 transform = glm::mat4(1.0f);

        if (element.HasParent())
            transform = GetWorldTransformRaw(element.GetParent());

        const auto& local = element.GetComponent<UICoreComponent>().Transform;
        glm::mat4 localTransform =
            glm::translate(glm::mat4(1.0f), glm::vec3(local.Position, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), local.Rotation.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), local.Rotation.y, glm::vec3(0, 0, 1)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(local.Size, 1.0f));

        return transform * localTransform;
    }
    UITransform UIMenu::GetWorldTransform(UIElement element)
    {

        glm::mat4 worldMatrix = GetWorldTransformRaw(element);

        glm::vec3 scale, translation, skew;
        glm::quat rotationQuat;
        glm::vec4 perspective;

        if (!glm::decompose(worldMatrix, scale, rotationQuat, translation, skew, perspective))
            return UITransform(); // fallback if decomposition fails

        glm::vec3 euler = glm::eulerAngles(rotationQuat); // get XYZ rotation angles in radians

        // Walk up the hierarchy to get root-most Alignment and Anchor
        UIElement current = element;
        UIElement root = current;
        while (current.HasParent())
        {
            current = current.GetParent();
            root = current;
        }
        const auto& rootTransform = root.GetComponent<UICoreComponent>().Transform;

        UITransform result;
        result.Position = glm::vec2(translation.x, translation.y);
        result.Size = glm::vec2(scale.x, scale.y);
        result.Rotation = glm::vec2(euler.x, euler.y); // preserve full XY rotation for completeness
        result.Alignment = rootTransform.Alignment;
        result.Anchor = rootTransform.Anchor;
        return result;
    }
    UIElement UIMenu::GetUIElement(UIElementID id)
    {
        PF_CORE_ASSERT(HasUIElement(id));
        return m_UIElementsMap.at(id);
    }
    UIElement UIMenu::TryGetUIElement(UIElementID id)
    {
        if (HasUIElement(id))
            return m_UIElementsMap.at(id);
        return UIElement();
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
    void UIMenu::DeleteElement(UIElementID id)
    {
        if (!HasUIElement(id))
            return;

        auto element = GetUIElement(id);
        if (element.HasParent())
            element.GetParent().RemoveChild(element);

        for (auto childID : element.Children())
        {
            DeleteElement(childID);
        }
		m_UIElementsNameMap.erase(element.GetComponent<UICoreComponent>().m_Name);
		m_UIElementsMap.erase(id);
        m_Registry.destroy(element.GetenttID());
    }

    void UIMenu::ParentElement(UIElement child, UIElement parent)
    {
        if (!child || !parent || child == parent)
            return;

        auto& childComp = child.GetComponent<UICoreComponent>();
        auto& parentComp = parent.GetComponent<UICoreComponent>();

        if (childComp.m_ParentID == parentComp.m_ElementID)
            return;

        if (childComp.HasParent())
        {
            UIElement previousParent = GetUIElement(childComp.m_ParentID);
            UnparentElement(child);
        }

        childComp.m_ParentID = parentComp.m_ElementID;
        parentComp.m_Children.emplace_back(childComp.m_ElementID);
    }

    void UIMenu::UnparentElement(UIElement element)
    {
        if (!element) return;
        auto& comp = element.GetComponent<UICoreComponent>();
        UIElement parent = GetUIElement(comp.m_ParentID);
        if (!parent) return;


        auto& parentComp = parent.GetComponent<UICoreComponent>();
        parentComp.m_Children.erase(std::remove(parentComp.m_Children.begin(), parentComp.m_Children.end(), element.GetUUID()), parentComp.m_Children.end());
        comp.m_ParentID = 0;
    }
    
}
