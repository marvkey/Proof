#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Font.h"
#include "Proof/Core/UUID.h"
#include "Proof/Renderer/Vertex.h"
#include "Proof/Utils/VariableSystem/VariableRegistryFieldTypes.h"
#include "Proof/Utils/MultiUse.h"
#include <glm/glm.hpp>
#include <entt/entt.hpp>
namespace Proof
{
	enum class UIElementType
	{
		None = 0,
		Button,
		Image,
		Text,
		ProgressBar,
	};

	enum class UIPositionAnchor
	{
		TopLeft = 0,
		TopMiddle,
		TopRight,
		MiddleLeft,
		MiddleMiddle,
		MiddleRight,
		BottomLeft,
		BottomMiddle,
		BottomRight
	};

	inline glm::vec4 GenerateAnchorMinMax(UIPositionAnchor anchor) {
		switch (anchor) 
		{
			case UIPositionAnchor::TopLeft:
				return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // min (0,0), max (0,0)
			case UIPositionAnchor::TopMiddle:
				return glm::vec4(0.5f, 0.0f, 0.5f, 0.0f); // min (0.5,0), max (0.5,0)
			case UIPositionAnchor::TopRight:
				return glm::vec4(1.0f, 0.0f, 1.0f, 0.0f); // min (1,0), max (1,0)

			case UIPositionAnchor::MiddleLeft:
				return glm::vec4(0.0f, 0.5f, 0.0f, 0.5f); // min (0,0.5), max (0,0.5)
			case UIPositionAnchor::MiddleMiddle:
				return glm::vec4(0.5f, 0.5f, 0.5f, 0.5f); // min (0.5,0.5), max (0.5,0.5)
			case UIPositionAnchor::MiddleRight:
				return glm::vec4(1.0f, 0.5f, 1.0f, 0.5f); // min (1,0.5), max (1,0.5)

			case UIPositionAnchor::BottomLeft:
				return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // min (0,1), max (0,1)
			case UIPositionAnchor::BottomMiddle:
				return glm::vec4(0.5f, 1.0f, 0.5f, 1.0f); // min (0.5,1), max (0.5,1)
			case UIPositionAnchor::BottomRight:
				return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // min (1,1), max (1,1)
			default:
				return glm::vec4(0.5f, 0.5f, 0.5f, 0.5f); // Default to MiddleMiddle
		}
	}
	struct UIAnchor
	{
		UIAnchor()
		{

		}
		glm::vec2 Minimum = glm::vec2(0); // Holds Top left 
		glm::vec2 Maximum = glm::vec2(0); // holds bottom right
		UIAnchor(UIPositionAnchor anchor) 
		{
			glm::vec4 minMax = GenerateAnchorMinMax(anchor);
			Minimum = glm::vec2(minMax.x, minMax.y); // Extract min (x, y)
			Maximum = glm::vec2(minMax.z, minMax.w); // Extract max (z, w)
		}
	};

	using UIElementID = UUID;

	struct UITransform
	{
		UIAnchor Anchor;
		glm::vec2 Alignment = glm::vec2(0); // default align top left of square
		glm::vec2 Position = { 0,0 };
		glm::vec2 Rotation = { 0,0 }; // radians
		glm::vec2 Size = { 1, 1};

		
	};
	struct UICoreComponent
	{
		const std::string& GetName()const
		{
			return m_Name;
		}

		UITransform Transform;
		UIElementType ElementType;
		UIElementID GetElementID() const { return m_ElementID; }


	private:
		std::string m_Name;
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

	struct UIProggresBarComponent
	{
		BindableClampedFloat<0.0f, 1.0f> Proggress = { 1.0f };
		glm::vec4 FillColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		glm::vec4 BackgroundColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
	class UIElement;
	class UIMenu : RefCounted
	{
	public:
		UIMenu()
		{

		}
		UIMenu(const UIMenu& other);
		static Count<UIMenu> Copy(Count<UIMenu> other);

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
		UIElement GetUIElement(const std::string& name);

		bool HasUIElement(const std::string& name ) const
		{
			return m_UIElementsNameMap.contains(name);
		}

		bool HasUIElement(const std::string& name )
		{
			return m_UIElementsNameMap.contains(name);
		}

		void SetName(UIElement element, const std::string& name);
		void SetVariableStorageSet(Count <class VariableSetStorage> storageSet);
		

		void DeleteElement(UIElementID id);
	private:
		std::unordered_map<UIElementID, UIElement> m_UIElementsMap;
		std::unordered_map<std::string, UIElementID> m_UIElementsNameMap;
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
			return m_Menu->m_Registry.has<T...>(m_UIElementHandle);
		}

		UIElementType GetElementType()const
		{
			return GetComponent<UICoreComponent>().ElementType;
		}

		//glm::mat4 GetTransform()const
		//{
		//	return GetComponent<UICoreComponent>().Transform.GetTransform();
		//}

		const std::string& GetName()const 
		{
			return GetComponent<UICoreComponent>().GetName();
		}

		void SetName(std::string& name)
		{
			if (*this)
				m_Menu->SetName(*this, name);
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
		UIComponentGroup<UICoreComponent, UIImageComponent, UIButtonComponent, UITextComponent, UIProggresBarComponent>;

	// an uiElement needs only one of this 
	using UIExclusiveComponentGroup =
		UIComponentGroup<UIImageComponent, UIButtonComponent, UITextComponent, UIProggresBarComponent>;
}
