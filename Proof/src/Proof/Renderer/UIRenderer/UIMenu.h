#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Font.h"
#include "Proof/Core/UUID.h"
#include "Proof/Renderer/Vertex.h"
#include "Proof/Utils/VariableSystem/VariableFieldTypes.h"

#include <glm/glm.hpp>
#include <entt/entt.hpp>
namespace Proof
{
	enum class UIElementType
	{
		None = 0,
		Button,
		Image,
		Text
	};

	using UIElementID = UUID;

	struct UITransform
	{
		glm::vec2 Position = { 0,0 };
		glm::vec2 Rotation = { 0,0 }; // radians
		glm::vec2 Size = { 1, 1};

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), glm::vec3{ Position,0.0f })
				* glm::toMat4(glm::quat(glm::vec3(Rotation, 0.0f)))
				* glm::scale(glm::mat4(1.0f), { Size,1.0 });
		}
	};
	struct UICoreComponent
	{
		std::string Name;
		UITransform Transform;
		UIElementType ElementType;
		UIElementID GetElementID() const { return m_ElementID; }
	private:
		UIElementID m_ElementID;
		friend class UIMenu;
		friend class UIElement;
		friend class UIRenderer;
	};

	struct UIImageComponent
	{
		glm::vec4 TintColor{ 1.0f };
		Count<Texture2D> Texture = nullptr;
	};
	struct UIButtonComponent
	{
		glm::vec4 TintColor{ 1.0f };
		Count<Texture2D> Texture = nullptr;
	};
	struct UITextComponent
	{
		std::string Text = "Text";
		TextParams TextConfig;
		Count<Font> Font = Font::GetDefault();
	};
	class UIElement;
	class UIMenu : RefCounted
	{
	public:
		UIMenu()
		{

		}
		UIMenu(const UIMenu& other);
		UIElement CreateElement(UIElementType type, UIElementID id);
		UIElement CreateElement(UIElementType type);
		UIElement CreateElement(const std::string& name, UIElementID id, UIElementType type);

		UIElementID GenerateID()
		{
		a:
			UIElementID id = UIElementID();
			if (HasUIElement(id))
				goto a;

			return id;
		}

		bool HasUIElement(UIElementID id) const
		{
			return m_UIElementsMap.contains(id);
		}
		const std::unordered_map<UIElementID, UIElement>& GetUIElementsMap()const
		{
			return m_UIElementsMap;
		};
		UIElement GetUIElement(UIElementID id);
		UIElement GetUIElement(UIElementID id)const;
	private:
		std::unordered_map<UIElementID, UIElement> m_UIElementsMap;
		entt::registry m_Registry;
		friend class UIElement;
		friend class UIRenderer;
	};

	class UIElement
	{
	public:
		UIElement(entt::entity handle, class UIMenu* panel)
			: m_UIElementHandle(handle), m_Menu(panel)
		{

		}	
		UIElement()
		{

		}
		template<class... Components>
		auto& GetComponent()
		{
			return m_Menu->m_Registry.get<Components...>(m_UIElementHandle);
		}

		template<class... Components>
		auto& GetComponent()const
		{
			return m_Menu->m_Registry.get<Components...>(m_UIElementHandle);
		}

		template<class... T>
		bool HasComponent()const 
		{
			return m_Menu->m_Registry.all_of<T...>(m_UIElementHandle);
		}

		UIElementType GetElementType()const
		{
			return GetComponent<UICoreComponent>().ElementType;
		}

		glm::mat4 GetTransform()const
		{
			return GetComponent<UICoreComponent>().Transform.GetTransform();
		}

		const std::string& GetName()const 
		{
			return GetComponent<UICoreComponent>().Name;
		}
		entt::entity GetenttID() { return m_UIElementHandle; }
		operator entt::entity() const {
			return m_UIElementHandle;
		}

		operator bool() const { return m_UIElementHandle != entt::null && m_Menu != nullptr && m_Menu->m_Registry.valid(m_UIElementHandle); }

		bool operator==(const UIElement& other) const {
			return m_UIElementHandle == other.m_UIElementHandle && m_Menu == other.m_Menu;
		}

		bool operator!=(const UIElement& other) const {
			return !(*this == other);
		}

		UIElementID GetUUID() const
		{
			if (!*this)
				return 0;
			return GetComponent<UICoreComponent>().GetElementID();
		}

	private:
		UIMenu* m_Menu = nullptr;
		entt::entity m_UIElementHandle { entt::null };
	};

	

	template<class ... Component>
	struct UIComponentGroup {

	};
	// a ui component needs one of these
	using UIAllComponents =
		UIComponentGroup<UICoreComponent, UIImageComponent, UIButtonComponent, UITextComponent>;

	// an uiElement needs only one of this 
	using UIExclusiveComponentGroup =
		UIComponentGroup<UIImageComponent, UIButtonComponent, UITextComponent>;
}
