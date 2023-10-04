#include"InputPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Utils/PlatformUtils.h"
#include "Editor3D/Editor3D.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Core/Profile.h"
#include "Proof/ImGui/UI.h"

#include <unordered_map>
namespace Proof {
	void InputPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen) {
		if (isOpen == false)
			return;

		PF_PROFILE_FUNC();

		ImGui::Begin(dsiplayName, &isOpen);
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			bool opened = ImGui::TreeNodeEx((void*)"Action Mapping", treeNodeFlags, "Action Mapping");
			if (opened)
			{
				Action();
				ImGui::TreePop();
			}
			bool openedMotion = ImGui::TreeNodeEx((void*)"Motion Mapping", treeNodeFlags, "Motion Mapping");

			if (openedMotion)
			{
				MotionInput();
				ImGui::TreePop();
			}
		}
		ImGui::End();

	}
	void InputPanel::Action()
	{

		ImGui::SameLine();
		if (ImGui::Button("+", { 20,20 }))
		{
			std::string action = "NewAction";
			int iterate = 0;
			while (InputManager::HasAction(action))
			{
				iterate++;
				action = "NewAction(" + std::to_string(iterate) + ")";
			}

			PF_EC_INFO("action added");
			InputManager::AddAction(action);
		}
		for (const auto& [actionName, action] : InputManager::GetActionMappings())
		{
			UI::ScopedID(actionName.c_str());
			ImGui::Text(actionName.c_str());
			ImGui::SameLine();
			if (ImGui::Button("New Key"))
				ImGui::OpenPopup("Add Key");

			if (ImGui::BeginPopup("Add Key"))
			{
				EnumReflection::ForEach<KeyBoardKey>([&](KeyBoardKey key) {
					if (key == KeyBoardKey::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::ActionAddKey(actionName, InputType(InputDevice::KeyBoard, (int)key));
					}
				});
				ImGui::Separator();
				EnumReflection::ForEach<MouseButton>([&](MouseButton key) {
					if (key == MouseButton::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::ActionAddKey(actionName, InputType(InputDevice::MouseButton, (int)key));
					}
				});

				ImGui::Separator();
				EnumReflection::ForEach<ControllerButton>([&](ControllerButton key) {
					if (key == ControllerButton::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::ActionAddKey(actionName, InputType(InputDevice::ControllerButton, (int)key));
					}
				});

				ImGui::Separator();
				EnumReflection::ForEach<ControllerAxis>([&](ControllerAxis key) {
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::ActionAddKey(actionName, InputType(InputDevice::ControllerAxis, (int)key));
					}
				});
				ImGui::EndPopup();
			}
			for (auto& [device, inputType] : action.Inputs)
			{
				for (auto& type : inputType)
				{
					switch (device)
					{
						case Proof::InputDevice::KeyBoard:
							{
								ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<KeyBoardKey>((KeyBoardKey)type.Key).c_str());
							}
							break;
						case Proof::InputDevice::MouseButton:
							{
								ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<MouseButton>((MouseButton)type.Key).c_str());
							}
							break;
						case Proof::InputDevice::MouseMovement:
							{
								ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<MouseAxis>((MouseAxis)type.Key).c_str());
							}
							break;
						case Proof::InputDevice::ControllerButton:
							{
								ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<ControllerButton>((ControllerButton)type.Key).c_str());
							}
							break;
						case Proof::InputDevice::ControllerAxis:
							{
								ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<ControllerAxis>((ControllerAxis)type.Key).c_str());
							}
							break;
						default:
							continue;
							break;
					}
				}
				
			}
		}
	}
	void InputPanel::MotionInput() {
		ImGui::SameLine();
		if (ImGui::Button("+", { 20,20 })) {
			std::string motion = "NewMotion";
			int iterate = 0;
			while (InputManager::HasMotion(motion))
			{
				iterate++;
				motion = "NewMotion(" + std::to_string(iterate) + ")";

			}
			PF_EC_INFO("New Motion");
			InputManager::AddMotion(motion);
		}
		for (auto& [motionName, motion] : InputManager::GetMotionMappings())
		{
			UI::ScopedID(motionName.c_str());
			ImGui::Text(motionName.c_str());
			ImGui::SameLine();

			if (ImGui::Button("New Key"))
				ImGui::OpenPopup("Add Key");

			if (ImGui::BeginPopup("Add Key"))
			{
				EnumReflection::ForEach<KeyBoardKey>([&](KeyBoardKey key) {
					if (key == KeyBoardKey::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::MotionAddKey(motionName, MotionInputType(InputDevice::KeyBoard, (int)key));
					}
				});
				ImGui::Separator();
				EnumReflection::ForEach<MouseButton>([&](MouseButton key) {
					if (key == MouseButton::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::MotionAddKey(motionName, MotionInputType(InputDevice::MouseButton, (int)key));
					}
				});

				ImGui::Separator();
				EnumReflection::ForEach<ControllerButton>([&](ControllerButton key) {
					if (key == ControllerButton::None)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::MotionAddKey(motionName, MotionInputType(InputDevice::ControllerButton, (int)key));

					}
				});

				ImGui::Separator();
				EnumReflection::ForEach<ControllerAxis>([&](ControllerAxis key) {
					if (ImGui::MenuItem(EnumReflection::EnumString(key).c_str()))
					{
						InputManager::MotionAddKey(motionName, MotionInputType(InputDevice::ControllerAxis, (int)key));
					}
				});
				ImGui::EndPopup();
			}
			for (auto& [device,motionList] : motion.Inputs)
			{
				for (const MotionInputType& type : motionList)
				{
					switch (device)
					{
						case Proof::InputDevice::KeyBoard:
							{
								//ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<KeyBoardKey>((KeyBoardKey)type.Key).c_str());
								ImGui::SameLine();

								const float* constValue = &type.MotionValue;
								float* unConstValue = const_cast<float*>(constValue);
								ImGui::DragFloat("Motion", unConstValue);
							}
							break;
						case Proof::InputDevice::MouseButton:
							{
								//ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<MouseButton>((MouseButton)type.Key).c_str());
								ImGui::SameLine();

								const float* constValue = &type.MotionValue;
								float* unConstValue = const_cast<float*>(constValue);
								ImGui::DragFloat("Motion", unConstValue);
							}
							break;
						case Proof::InputDevice::MouseMovement:
							{
								//ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<MouseAxis>((MouseAxis)type.Key).c_str());
								ImGui::SameLine();

								const float* constValue = &type.MotionValue;
								float* unConstValue = const_cast<float*>(constValue);
								ImGui::DragFloat("Motion", unConstValue);
							}
							break;
						case Proof::InputDevice::ControllerButton:
							{
								//ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<ControllerButton>((ControllerButton)type.Key).c_str());
								ImGui::SameLine();

								const float* constValue = &type.MotionValue;
								float* unConstValue = const_cast<float*>(constValue);
								ImGui::DragFloat("Motion", unConstValue);
							}
							break;
						case Proof::InputDevice::ControllerAxis:
							{
								//ImGui::SameLine();
								ImGui::Text(EnumReflection::EnumString<ControllerAxis>((ControllerAxis)type.Key).c_str());
								ImGui::SameLine();

								const float* constValue = &type.MotionValue;
								float* unConstValue = const_cast<float*>(constValue);
								ImGui::DragFloat("Motion", unConstValue);
							}
							break;
						default:
							continue;
							break;
					}
				}

			}
		}
	}


}