#include "Proofprch.h"
#include "GuiEditorPanel.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/UIRenderer/UIMenu.h"
#include "Proof/ImGui/SelectionManager.h"

#include "Proof/Renderer/UIRenderer/UIRenderer.h"
#include "Proof/Utils/VariableSystem/Variable.h"
#include "Proof/Core/FrameTime.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/Image.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/UIHandlers.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Core/Application.h"

namespace Proof
{
	GuiEditorPanel::GuiEditorPanel()
		:
		AssetEditor("GuiEditorPanel")
		//,m_Camera(30)

	{
	}
#if 0
	void GuiEditorPanel::OnImGuiRender()
	{
		ImGui::BeginChild("UiData", { ImGui::GetContentRegionAvail().x/3, ImGui::GetContentRegionAvail().y});
		{

			ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Heriechy", { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y/2 });
			{
				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered() == false)
				{
					m_SelectedType = Selected::None;
					m_SelectedName = "";
				}
				if (ImGui::BeginPopupContextWindow(0))
				{ // right click adn open a new entitiy
					AddElementMenu();
					ImGui::EndPopup();
				}
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
				flags |= ImGuiTreeNodeFlags_SpanFullWidth;
				{
					bool imageOpen = ImGui::TreeNodeEx((void*)"buttons", flags, "Buttons");
					if (imageOpen)
					{
						for (auto& element : m_UIPanel->GetButtons())
						{
							DrawButtonNode(element.first, m_UIPanel->ButtonGet(element.first));
						}
						ImGui::TreePop();
					}
				}
				{
					bool imageOpen = ImGui::TreeNodeEx((void*)"imageButtons", flags, "ImageButtons");
					if (imageOpen)
					{
						for (auto& element : m_UIPanel->GetImageButtons())
						{
							DrawImageButtonNode(element.first, m_UIPanel->GetImageButton(element.first));
						}
						ImGui::TreePop();
					}
				}
				{
					bool imageOpen = ImGui::TreeNodeEx((void*)"Texts", flags, "Text");
					if (imageOpen)
					{
						for (auto& element : m_UIPanel->GetTexts())
						{
							DrawTextNode(element.first, m_UIPanel->TextGet(element.first));
						}
						ImGui::TreePop();
					}
				}
				ImGui::EndChild();
			}
			ImGui::PopStyleColor();
			ImGui::BeginChild("Properties", { ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y  });
			{
				if (m_SelectedType == Selected::Button && m_UIPanel->ButtonHas(m_SelectedName)) 
				{
					DrawButtonComponent(m_SelectedName, m_UIPanel->ButtonGet(m_SelectedName));
				}
				if (m_SelectedType == Selected::ImageButton && m_UIPanel->ImageButtonHas(m_SelectedName))
				{
					DrawImageButtonComponent(m_SelectedName, m_UIPanel->GetImageButton(m_SelectedName));
				}
				if (m_SelectedType == Selected::Text && m_UIPanel->TextHas(m_SelectedName))
				{
					DrawTextComponent(m_SelectedName, m_UIPanel->TextGet(m_SelectedName));
				}
				ImGui::EndChild();
			}

			ImGui::EndChild();
		}
		ImGui::SameLine();
		{
			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Window", ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y });

			if (ImGui::IsWindowFocused())
			{
				m_IsViewportFocused = true;
			}
			else
			{
				m_IsViewportFocused = false;
			}
			//const void* Text = UiRenderer::DrawUI(m_UIPanel, m_Camera.GetPosition(), glm::mat4(1.0f), glm::mat4(1.0f),
			//	ImGui::GetWindowWidth(), ImGui::GetWindowHeight()).SourceImage;
			//
			//ImGui::Image((ImTextureID)Text, ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::EndChild();
		}
	}

	void GuiEditorPanel::OnUpdate(FrameTime deltaTime)
	{
		if (m_IsViewportFocused)
		{
			//Application::Get()->GetWindow()->SetWindowInputEvent(true);
			///m_Camera.OnUpdate(deltaTime,GetWindowSize());
			//Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}
	
	}
#endif
	void GuiEditorPanel::OnUpdate(FrameTime deltaTime)
	{
		//if (m_IsViewportFocused)
		{
			//Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_Renderer->BeginContext(glm::mat4(1.0f), glm::mat4(1.0f), { m_Camera.GetPosition().x,m_Camera.GetPosition().y,m_Camera.GetPosition().z }, { false }, true);
			m_Renderer->EndContext();

			//UI::Image()
			UIRenderer::DrawUI(m_UIPanel->Menu, m_Renderer, m_WindowSize.x, m_WindowSize.y);

			//m_Renderer->EndContext();

		//	Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_Camera.OnUpdate(deltaTime);
		//	Application::Get()->GetWindow()->SetWindowInputEvent(false);
			//Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}

	}
	void GuiEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::UIPanel)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to GuiEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_UIPanel = asset.As<UIPanel>();
		m_Renderer = Count<Renderer2D>::Create("GuiPanel2DRenderer");
	}

	void GuiEditorPanel::OnEvent(Event& e)
	{
	}

	void GuiEditorPanel::Save()
	{
		if (!m_UIPanel)
		{
			m_NeedsSaving = false;
			AssetManager::SaveAsset(m_UIPanel->GetID());
		}
	}
	void GuiEditorPanel::OnImGuiRender()
	{
		RenderHierarchyPanel();
	}

	void GuiEditorPanel::AddItemMenu()
	{
		UIElement element = {};
		if (ImGui::MenuItem("Button"))
		{
			element = m_UIPanel->Menu->CreateElement(UIElementType::Button);
		}
		if (ImGui::MenuItem("Text"))
		{
			element = m_UIPanel->Menu->CreateElement(UIElementType::Text);
		}
		if (ImGui::MenuItem("Image"))
		{
			element = m_UIPanel->Menu->CreateElement(UIElementType::Image);
		}
		if (element)
		{
			element.GetComponent<UICoreComponent>().Transform.Size = glm::vec2{ 30 };
			element.GetComponent<UICoreComponent>().Transform.Position = glm::vec2{ 30 };
		}
	}

	void GuiEditorPanel::RenderHierarchyPanel()
	{
		ImGui::Begin("test widnow");
		ImGui::BeginChild("UiData", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y});
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });

			ImGui::BeginChild("Child Hierarchy", { ImGui::GetContentRegionAvail().x,ImGui::GetWindowHeight() / 2 });
			/*
			for (auto canvas : m_UIPanel->Canvases)
			{
				for (auto element : canvas->Elements)
				{
					if (element->Parent == nullptr)
						DrawElementNode(element);
				}
			}
			*/

			for (auto [entityId, entity] : m_UIPanel->Menu->GetUIElementsMap())
			{
				DrawElementNode(entity);
			}
			if (ImGui::BeginPopupContextWindow(0))
			{ // right click adn open a new entitiy
				AddItemMenu();
				ImGui::EndPopup();
			}
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered() == false)
			{
				AssetSelectionManager::DeselectAll(AssetSelectionContext::GUIPanel, m_UIPanel->GetID());
			}
			ImGui::EndChild();
			ImGui::PopStyleColor();

			ImGui::BeginChild("Properties", ImGui::GetContentRegionAvail());

			if (AssetSelectionManager::HasSelections(AssetSelectionContext::GUIPanel, m_UIPanel->GetID()))
			{
				auto entity = m_UIPanel->Menu->GetUIElement(AssetSelectionManager::GetSelections(AssetSelectionContext::GUIPanel, m_UIPanel->GetID()).front());
				DrawElementProperty(entity);
			}

			auto pos = m_Camera.GetPosition();
			if (UI::AttributeDrag("PosCammera", pos))
				m_Camera.SetPosition(pos);
			ImGui::EndChild();

		

		}
		ImGui::EndChild();
	#if 0
		ImGui::Begin("Variables");
		if (ImGui::Button("AddVariable"))
		{
			ImGui::OpenPopup("AddVariable");
		}
		if (ImGui::BeginPopup("AddVariable"))
		{
			EnumReflection::ForEach<VariableTypes>([&](VariableTypes type)
				{
					if (type == VariableTypes::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(type).c_str()))
					{
						m_UIPanel->m_VariableTable->SetVariable(fmt::format("PlayerScore", m_UIPanel->m_VariableTable->GetVariables().size()), Variable(type));
					}
				});
			ImGui::EndPopup();
		}

		auto variables = m_UIPanel->m_VariableTable;
		for (auto& [name, var] : variables->GetVariables())
		{
			Variable& nonConstVar = variables->GetVariable(name);
			switch (var.Type)
			{
				case Proof::VariableTypes::None:
					break;
				case Proof::VariableTypes::Int:
					break;
				case Proof::VariableTypes::Float:
					break;
				case Proof::VariableTypes::Vec2:
					break;
				case Proof::VariableTypes::Vec3:
					break;
				case Proof::VariableTypes::Vec4:
					break;
				case Proof::VariableTypes::String:
				{
					std::string value = var.GetValue<std::string>();
					char buffer[256];
					memset(buffer, 0, 256);
					memcpy(buffer, value.c_str(), value.length());
					if (UI::AttributeInputText(name.c_str(), value, 0))
					{
						nonConstVar.SetValue<std::string>(value);
					}
				}
				break;
				default:
					break;
			}
		}
		ImGui::End();
	#endif
		{
		#if 0
			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			auto size = ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y };
			ImGui::BeginChild("Window", size);
			m_Camera.SetViewportSize(size.x, size.y);
			m_WindowSize = glm::vec2(size.x, size.y);

			UI::Image(m_Renderer->GetTargetFrameBuffer()->GetOutput(0), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
				
			ImGui::EndChild();
		#endif

			if (ImGui::Begin("Window"))
			{
				auto size = ImGui::GetContentRegionAvail();
				m_Camera.SetViewportSize(size.x, size.y);
				m_WindowSize = glm::vec2(size.x, size.y);

				UI::Image(m_Renderer->GetTargetFrameBuffer()->GetOutput(0), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
				ImGui::End();
			}
		}
		ImGui::End();
	}

	void GuiEditorPanel::DrawElementNode(UIElement element)
	{
		UI::ScopedID scopeId(element.GetUUID());

		ImGuiTreeNodeFlags flags;
		flags = ((AssetSelectionManager::IsSelected(AssetSelectionContext::Prefab, m_UIPanel->GetID(), element.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow);

		//if (element->Children.empty()) 
		//{
			flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow
		//}

		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)element.GetUUID(), flags, element.GetName().c_str());
		if (ImGui::IsItemClicked())
		{
			AssetSelectionManager::DeselectAll(AssetSelectionContext::GUIPanel, m_UIPanel->GetID());
			AssetSelectionManager::Select(AssetSelectionContext::GUIPanel, m_UIPanel->GetID(), element.GetUUID());
		}

		if (ImGui::BeginPopup("Element Additions"))
		{
			AddItemMenu();
			ImGui::EndPopup();
		}

		if (opened) 
		{
			//for (auto& children : element->Children)
			//	DrawElementNode(children);
			ImGui::TreePop();
		}


	}
	template<typename T, typename UIFunction>
	void GuiEditorPanel::DrawElementType(UIElement element, UIFunction function)
	{
		if (!element.HasComponent<T>())
			return;

		ImGui::Separator();
		T& elementAsType = element.GetComponent<T>();

		function(elementAsType);
	}
	void GuiEditorPanel::DrawElementProperty(UIElement element)
	{

		static std::string nameCopy;
		UICoreComponent& coreComponent = element.GetComponent<UICoreComponent>();

		{
			UI::BeginPropertyGrid();

			nameCopy = coreComponent.GetName();
			if (UI::AttributeInputText("Name", nameCopy))
			{
				element.SetName(nameCopy);
			}

			ImGui::Separator();
			UI::EndPropertyGrid();

			if(UI::AttributeTreeNode("Anchor", false, 2, 2))
			{
					UI::BeginPropertyGrid();
					UIPositionAnchor positionAnchor;
				 	if (UI::EnumCombo("Anchor type", positionAnchor))
					{
						coreComponent.Transform.Anchor = UIAnchor(positionAnchor);
					}

				UI::AttributeDrag("Min", coreComponent.Transform.Anchor.Minimum);
				UI::AttributeDrag("Max", coreComponent.Transform.Anchor.Maximum);
				UI::AttributeDrag("Alignment", coreComponent.Transform.Alignment);
				UI::EndPropertyGrid();

				UI::EndTreeNode();
			}
		}

		UI::AttributeDrag("Position", coreComponent.Transform.Position, 0.25);
		{
			glm::vec2 rotationdeg = glm::degrees(coreComponent.Transform.Rotation);

			if (UI::AttributeDrag("Rotation", rotationdeg, 0.25))
			{
				glm::vec2 rotationRad = glm::radians(rotationdeg);
				coreComponent.Transform.Rotation = rotationRad;
			}
		}
		UI::AttributeDrag("Size", coreComponent.Transform.Size, 0.25);

		//UI::AttributeBool("Visible", element->Visible);

		DrawElementType<UIButtonComponent>(element, [](UIButtonComponent& button)
			{
				UI::AttributeColor("Color", button.TintColor);
			});

		DrawElementType<UIImageComponent>(element, [](UIImageComponent& image)
			{
				UI::AttributeColor("TintColor", image.TintColor);

				AssetID id = 0;
				if (image.Texture != nullptr)
					id = image.Texture->GetID();

				if(UI::AttributeTextureAssetReference("Texture", id))
				{
					image.Texture = AssetManager::GetAsset<Texture2D>(id);
				}
			});

		DrawElementType<UITextComponent>(element, [&](UITextComponent& text)
			{

				UI::AttributeInputTextMultiline("", text.Text, 0);
			
				UI::BeginPropertyGrid();
				UI::AttributeColor("Color", text.TextConfig.Color );

				UI::AttributeDrag("Kerning", text.TextConfig.Kerning, 0.025);
				UI::AttributeDrag("Line Spacing", text.TextConfig.LineSpacing, 0.025);
				UI::EndPropertyGrid();
			});
		
	}
	void GuiEditorPanel::RenderViewPortPanel()
	{
	}
}
