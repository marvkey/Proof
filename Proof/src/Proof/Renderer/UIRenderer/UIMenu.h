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
#include "Proof/Core/Delegate.h"
namespace Proof
{
	enum class UIElementType
	{
		None = 0,
		Button,
		Image,
		Text,
		ProgressBar,
		VerticalBox,
		HorizontalBox,


	};

	enum class UIPositionAnchor
	{
		None = 0,
		TopLeft,
		TopMiddle,
		TopRight,
		MiddleLeft,
		MiddleMiddle,
		MiddleRight,
		BottomLeft,
		BottomMiddle,
		BottomRight,
	};

	inline glm::vec4 GenerateAnchorMinMax(UIPositionAnchor anchor) {
		switch (anchor)
		{
			case UIPositionAnchor::TopLeft:
				return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // fixed at top left glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // full screen stretch
			case UIPositionAnchor::TopMiddle:
				return glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
			case UIPositionAnchor::TopRight:
				return glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);

			case UIPositionAnchor::MiddleLeft:
				return glm::vec4(0.0f, 0.5f, 0.0f, 0.5f);
			case UIPositionAnchor::MiddleMiddle:
				return glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
			case UIPositionAnchor::MiddleRight:
				return glm::vec4(1.0f, 0.5f, 1.0f, 0.5f);

			case UIPositionAnchor::BottomLeft:
				return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			case UIPositionAnchor::BottomMiddle:
				return glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
			case UIPositionAnchor::BottomRight:
				return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			default:
				return glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
		}
	}

	inline UIPositionAnchor GetAnchorFromMinMax(glm::vec2 min, glm::vec2 max)
	{
		if (min == glm::vec2(0.0f, 0.0f) && max == glm::vec2(0.0f, 0.0f))
			return UIPositionAnchor::TopLeft;
		if (min == glm::vec2(0.5f, 0.0f) && max == glm::vec2(0.5f, 0.0f))
			return UIPositionAnchor::TopMiddle;
		if (min == glm::vec2(1.0f, 0.0f) && max == glm::vec2(1.0f, 0.0f))
			return UIPositionAnchor::TopRight;

		if (min == glm::vec2(0.0f, 0.5f) && max == glm::vec2(0.0f, 0.5f))
			return UIPositionAnchor::MiddleLeft;
		if (min == glm::vec2(0.5f, 0.5f) && max == glm::vec2(0.5f, 0.5f))
			return UIPositionAnchor::MiddleMiddle;
		if (min == glm::vec2(1.0f, 0.5f) && max == glm::vec2(1.0f, 0.5f))
			return UIPositionAnchor::MiddleRight;

		if (min == glm::vec2(0.0f, 1.0f) && max == glm::vec2(0.0f, 1.0f))
			return UIPositionAnchor::BottomLeft;
		if (min == glm::vec2(0.5f, 1.0f) && max == glm::vec2(0.5f, 1.0f))
			return UIPositionAnchor::BottomMiddle;
		if (min == glm::vec2(1.0f, 1.0f) && max == glm::vec2(1.0f, 1.0f))
			return UIPositionAnchor::BottomRight;

		// Default if no match
		return UIPositionAnchor::None;
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

	struct UILayouOffset
	{
		glm::vec2 Position = { 0,0 };
	};


	struct OnScreenDrawTransform
	{
		glm::vec2 Position; // Top-left corner in pixels
		glm::vec2 Size;     // Width and height in pixels
		glm::mat4 FinalTransform;
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

		UILayouOffset LayoutOffset; // only really be used when has a parent with a horizontal or vertical box component

		bool HasChild(UIElementID id)
		{
			return std::find(m_Children.begin(), m_Children.end(), id) != m_Children.end();
		}

		bool HasParent()
		{
			return m_ParentID != 0;
		}

		BindableVariableBool Visible = true;

		const OnScreenDrawTransform& GetScreenDrawTransform(){return m_DrawPosition;}
		const OnScreenDrawTransform& GetScreenDrawTransform()const {return m_DrawPosition;}
	private:
		OnScreenDrawTransform m_DrawPosition;
		std::string m_Name;
		UIElementID m_ElementID;
		std::vector<UIElementID> m_Children;
		UIElementID m_ParentID = 0;

		friend class UIMenu;
		friend class UIElement;
		friend class UIRenderer;
		friend class UIPanelAssetSerilizer;
		friend class ElevatedUIRenderer;
	};

	struct UIImageComponent
	{
		BindableVariableVec4 TintColor = glm::vec4{ 1.0f };
		BindableStaticAssetKey Texture = StaticAssetKey(AssetType::Texture);
	};

	enum class UIButtonEvents
	{
		None = 0,
		Click = 1,
		Hovered = 2,
	};

	struct UIButtonEvent
	{
		UUID ButtonID;
		std::string ButtonName;
		UIButtonEvents Event = UIButtonEvents::None;
	};

	struct UIButtonComponent
	{
		BindableVariableVec4 TintColor =  glm::vec4{ 1.0f };
		BindableStaticAssetKey Texture = StaticAssetKey(AssetType::Texture);

		Delegate<bool(const UIButtonEvent&)> OnEvent;
	};

	struct UITextComponent
	{
		BindableVariableString Text = { "Text" };
		TextParams TextConfig;
		Count<Font> Font = Font::GetDefault();
	};

	struct UIProggresBarComponent
	{
		BindableClampedFloat<0.0f, 1.0f> Proggress = { 1.0f };

		BindableVariableVec4 FillColor = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
		BindableVariableVec4 BackgroundColor = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
		const OnScreenDrawTransform& GetScreenFiledDrawTransform(){return m_FilledOnScreenDraw;}
		const OnScreenDrawTransform& GetScreenFiledDrawTransform()const {return m_FilledOnScreenDraw;}
	private:
		OnScreenDrawTransform m_FilledOnScreenDraw;
		friend class ElevatedUIRenderer;

	};

	struct UIPadding
	{
		ClampedValue<float, 0.0000001f, 10000000.0f> Left;
		ClampedValue<float, 0.0000001f, 10000000.0f> Top;
		ClampedValue<float, 0.0000001f, 10000000.0f> Right;
		ClampedValue<float, 0.0000001f, 10000000.0f> Bottom;

		UIPadding(float padding = 0.0f)
			: Left(padding), Top(padding), Right(padding), Bottom(padding) {
		}

		UIPadding(float vertical, float horizontal)
			: Left(horizontal), Top(vertical), Right(horizontal), Bottom(vertical) {
		}

		UIPadding(float left, float top, float right, float bottom)
			: Left(left), Top(top), Right(right), Bottom(bottom) {
		}
	};
	 // padding goes into the children
	struct UIVerticalBoxComponent
	{
		float Spacing = 10.0f;
		bool DrawBorders = false;      // Optional border toggle
		glm::vec4 BorderColor = { 1, 1, 1, 1 };
		float BorderThickness = 1.0f;
	};

	// padding goes into the children
	struct UIHorizontalBoxComponent
	{
		float Spacing = 10.0f;
		bool DrawBorders = false;      // Optional border toggle
		glm::vec4 BorderColor = { 1, 1, 1, 1 };
		float BorderThickness = 1.0f;
	};
	class UIElement;
	class UIMenu : public RefCounted
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


		glm::mat4 GetWorldTransformRaw(UIElement element);
		UITransform GetWorldTransform(UIElement element);
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
		UIElement TryGetUIElement(UIElementID id);
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

		void ParentElement(UIElement child, UIElement parent);
		void UnparentElement(UIElement element);
	
	private:
		std::unordered_map<UIElementID, UIElement> m_UIElementsMap;
		std::unordered_map<std::string, UIElementID> m_UIElementsNameMap;
		entt::registry m_Registry;
		friend class UIElement;
		friend class UIRenderer;
		friend class ElevatedUIRenderer;
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

		bool IsValid() const
		{

			if (m_UIElementHandle == entt::null)
				return false;

			if (m_Menu == nullptr)
				return false;

			if (!m_Menu->m_Registry.valid(m_UIElementHandle))
				return false;

			return true;
		}

		operator bool() const
		{
			return IsValid();
		}

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

		bool HasChildren() const
		{
			return !GetComponent<UICoreComponent>().m_Children.empty();
		}

		void AddChild(UIElement child)
		{
			m_Menu->ParentElement(child, *this);
		}

		void SetParent(UIElement parent)
		{
			m_Menu->ParentElement(*this, parent);
		}

		void Unparent()
		{
			m_Menu->UnparentElement(*this);
		}

		bool HasParent() const
		{
			return m_Menu->HasUIElement(GetComponent<UICoreComponent>().m_ParentID);
		}

		void RemoveChild(UIElement child)
		{
			m_Menu->UnparentElement(child);
		}

		UIElementID GetParentUUID() const
		{
			return GetComponent<UICoreComponent>().m_ParentID;
		}

		UIElement GetParent() const
		{
			return m_Menu->GetUIElement(GetComponent<UICoreComponent>().m_ParentID);
		}

		

		const std::vector<UIElementID>& Children() const
		{
			return GetComponent<UICoreComponent>().m_Children;
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
		UIComponentGroup<UICoreComponent, UIImageComponent, UIButtonComponent, UITextComponent, UIProggresBarComponent, UIVerticalBoxComponent,UIHorizontalBoxComponent>;

	// an uiElement needs only one of this 
	using UIExclusiveComponentGroup =
		UIComponentGroup<UIImageComponent, UIButtonComponent, UITextComponent, UIProggresBarComponent, UIVerticalBoxComponent, UIHorizontalBoxComponent>;
}
