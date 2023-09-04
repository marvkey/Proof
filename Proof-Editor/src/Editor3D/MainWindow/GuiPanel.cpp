#include "GuiPanel.h"
#include "Proof/Utils/PlatformUtils.h"

#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/UIRenderer/UIRenderer.h"
#include "Proof/Core/FrameTime.h"
#include "misc/cpp/imgui_stdlib.h"
#include "SceneHierachyPanel.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof
{
	GuiPanel::GuiPanel(Count<class UIPanel> panel)
		:
		m_Camera(45)
	{
		m_UIPanel = panel;

	}
	void GuiPanel::ImGuiRender(FrameTime deltaTime)
	{
		if (m_ShowWindow == false)
			return;

		std::string guiPanelName = fmt::format("{} Panel", AssetManager::GetAssetInfo(m_UIPanel).GetName());
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		ImGui::Begin(guiPanelName.c_str(), &m_ShowWindow);
		/*
		if (ImGui::IsWindowFocused())
		{
			Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_Camera.SetViewportSize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
			m_Camera.OnUpdate(deltaTime);
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}
		*/
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
		}
		ImGui::SameLine();
		{
			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("WIndow", ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y });
			//const void* Text = UiRenderer::DrawUI(m_UIPanel, m_Camera.GetPosition(), glm::mat4(1.0f), glm::mat4(1.0f),
			//	ImGui::GetWindowWidth(), ImGui::GetWindowHeight()).SourceImage;
			//
			//ImGui::Image((ImTextureID)Text, ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void GuiPanel::DrawButtonNode(const std::string& name, UIButton& button) {
		UI::ScopedID id(name);
		bool selcted = false;
		if (name == m_SelectedName && m_SelectedType == Selected::Button)
			selcted = true;
		ImGuiTreeNodeFlags flags = ((selcted == true) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow

		auto uniqueId = (uint64_t)name.c_str() + (uint64_t)Selected::Button;
		bool opened = ImGui::TreeNodeEx((void*)uniqueId, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedName = name;
			m_SelectedType = Selected::Button;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}
	void GuiPanel::DrawImageButtonNode(const std::string& name, UIButtonImage& button)
	{
		UI::ScopedID id(name);
		bool selcted = false;
		if (name == m_SelectedName && m_SelectedType == Selected::ImageButton)
			selcted = true;
		ImGuiTreeNodeFlags flags = ((selcted == true) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow

		auto uniqueId = (uint64_t)name.c_str() + (uint64_t)Selected::ImageButton;
		bool opened = ImGui::TreeNodeEx((void*)uniqueId, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedName = name;
			m_SelectedType = Selected::ImageButton;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}
	void GuiPanel::DrawTextNode(const std::string& name, UIText& text)
	{
		UI::ScopedID id(name);
		bool selcted = false;
		if (name == m_SelectedName && m_SelectedType == Selected::Text)
			selcted = true;
		ImGuiTreeNodeFlags flags = ((selcted == true) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;
		flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow

		auto uniqueId = (uint64_t)name.c_str() + (uint64_t)Selected::Text;
		bool opened = ImGui::TreeNodeEx((void*)uniqueId, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedName = name;
			m_SelectedType = Selected::Text;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}
	void GuiPanel::DrawButtonComponent(const std::string& name, UIButton& button)
	{
		if (AssetManager::HasAsset(m_UIPanel))
			AssetManager::SaveAsset(m_UIPanel->GetID());
		ImGui::ColorEdit4("TintColor", glm::value_ptr(button.TintColour));
		//SceneHierachyPanel::DrawVector2Control("Position", button.Postion);
		//SceneHierachyPanel::DrawVector2Control("Rotation", button.Rotation, 0.0f);
		//SceneHierachyPanel::DrawVector2Control("Size", button.Size, 1.0f);
		ImGui::Checkbox("Visible", &button.Visible);
		ImGui::InputTextMultiline("Text", &button.Text);
	}
	void GuiPanel::DrawImageButtonComponent(const std::string& name, UIButtonImage& button)
	{
		if (AssetManager::HasAsset(m_UIPanel))
			AssetManager::SaveAsset(m_UIPanel->GetID());
		ImGui::ColorEdit4("TintCOlor", glm::value_ptr(button.TintColour));
		//SceneHierachyPanel::DrawVector2Control("Position", button.Postion);
		//SceneHierachyPanel::DrawVector2Control("Rotation", button.Rotation, 0.0f);
		//SceneHierachyPanel::DrawVector2Control("Size", button.Size, 1.0f);
		ImGui::Checkbox("Visible", &button.Visible);

		if (button.Texture != nullptr)
		{
			bool fdasf = true;
			ImGui::Checkbox("##x", &fdasf);
			ImGui::SameLine();
			UI::Image(Renderer::GetWhiteTexture()->GetImage(), {30,30});
		}
		else
		{
			UI::Image(Renderer::GetWhiteTexture()->GetImage(), { 30,30 });
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
			{
				uint64_t Data = *(const uint64_t*)payload->Data;
				if (AssetManager::HasAsset(Data))
				{
					button.Texture = AssetManager::GetAsset<Texture2D>(Data);
				}
			}
			ImGui::EndDragDropTarget();
		}

	}
	void GuiPanel::DrawTextComponent(const std::string& name, UIText& text)
	{
		if (AssetManager::HasAsset(m_UIPanel))
			AssetManager::SaveAsset(m_UIPanel->GetID());
		//SceneHierachyPanel::DrawVector2Control("Position", text.Postion);
		//SceneHierachyPanel::DrawVector2Control("Rotation", text.Rotation, 0.0f);
		//SceneHierachyPanel::DrawVector2Control("Size", text.Size, 1.0f);
		ImGui::DragFloat("Kernng", &text.Param.Kerning, 0.025);
		ImGui::DragFloat("Line Spacing", &text.Param.LineSpacing, 0.025);
		ImGui::ColorEdit4("Color", glm::value_ptr(text.Param.Color));
		ImGui::Checkbox("Visible", &text.Visible);

		ImGui::InputTextMultiline("Text", &text.Text);

	}
	bool GuiPanel::AddElementMenu()
	{
		if (ImGui::MenuItem("Button"))
		{
			std::string buttonName = fmt::format("Button{}", m_UIPanel->GetButtons().size());
			m_UIPanel->SetButton(UIButton(), buttonName);
			return true;
		}
		else if (ImGui::MenuItem("ImageButton"))
		{
			std::string buttonName = fmt::format("ImageButton{}", m_UIPanel->GetImageButtons().size());
			m_UIPanel->SetButtonImage(UIButtonImage(),buttonName );
			return true;
		}
		else if (ImGui::MenuItem("Text"))
		{
			std::string textName = fmt::format("Text{}", m_UIPanel->GetTexts().size());
			m_UIPanel->SetText(UIText(), textName);
			return true;
		}
		return false;
	}
}
