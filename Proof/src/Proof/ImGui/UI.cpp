#include "Proofprch.h"
#include "UI.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include <imgui/imgui_internal.h>
#include "Proof/Asset/AssetManager.h"
#include <regex>
//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/e7c349b70bd95af3ab673556cdb56cb2cc40b48e/Engine/X2/ImGui/ImGuiUtilities.h#L285
// have 
/*
#include <iostream>
#include <vector>
#include <regex>

int main() {
    // Sample word you want to find similar words for
    std::string givenWord = "apple";

    // List of words to search through
    std::vector<std::string> wordList = {"apples", "appliance", "banana", "apex", "application"};

    // Define a regular expression pattern to match words with a common prefix
    std::regex pattern(givenWord + ".*", std::regex_constants::icase); // Case-insensitive match

    // Find words that match the pattern
    std::vector<std::string> similarWords;
    for (const std::string& word : wordList) {
        if (std::regex_match(word, pattern)) {
            similarWords.push_back(word);
        }
    }

    // Print similar words
    std::cout << "Similar words:" << std::endl;
    for (const std::string& word : similarWords) {
        std::cout << word << std::endl;
    }

    return 0;
}

*/
/*
* #include <imgui.h>
#include <vector>

std::vector<std::string> items = {"Apple", "Banana", "Cherry", "Date", "Grape", "Lemon", "Mango", "Orange"};
char searchInput[256] = ""; // Buffer to store search input

// In your rendering loop
ImGui::InputText("Search", searchInput, sizeof(searchInput));

// Create the combo box
if (ImGui::BeginCombo("Items", selected_item.c_str()))
{
    for (const std::string& item : items)
    {
        // Apply the search filter
        if (strstr(item.c_str(), searchInput))
        {
            bool is_selected = (item == selected_item);
            if (ImGui::Selectable(item.c_str(), is_selected))
            {
                selected_item = item;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
    }
    ImGui::EndCombo();
}

*/
namespace Proof::HeaderFileOnly {

    ImTextureID TextureUI::GetTexture(Count<Image> image)
    {
        return Application::Get()->GetImguiLayer()->ToImguiImage(image);
    }


}

namespace Proof::UI
{
    static constexpr int s_IDBufferSize = 32;
    static uint64_t s_ID = 0;
    static char s_IDBuffer[s_IDBufferSize];

    static void UpdateIDBuffer(const std::string& label)
    {
        s_IDBuffer[0] = '#';
        s_IDBuffer[1] = '#';
        memset(s_IDBuffer + 2, 0, s_IDBufferSize - 2);

        for (int i = 0; i < (s_IDBufferSize - 2) && i < label.size(); ++i)
        {
            s_IDBuffer[i + 2] = label[i];
        }
    }
    struct InputTextCallbackData
    {
        std::string& Str;
        ImGuiInputTextCallback  ChainCallback;
        void* ChainCallbackUserData;
    };
     

    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        InputTextCallbackData& user_data = (InputTextCallbackData&)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string& str = user_data.Str;
            str.resize(data->BufTextLen);
            data->Buf = (char*)str.c_str();
        }
        else if (user_data.ChainCallback)
        {
            // Forward to user callback, if any
            data->UserData = user_data.ChainCallbackUserData;
            return user_data.ChainCallback(data);
        }
        return 0;
    }

    void BeginPropertyGrid(const std::string& gridName)
    {
        ImGui::PushID(gridName.c_str());
        ImGui::Columns(2);
    }

    void EndPropertyGrid()
    {
        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool DrawVec3Control(const std::string& label, glm::vec3& values, const glm::vec3 resetValues, float columnWidth)
    {
        bool bValueChanged = false;
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });

        float lineHeight = (GImGui->Font->FontSize * boldFont->Scale) + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

        //X
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f,0.0f,0.0f,1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValues.x;
            bValueChanged = true;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        bValueChanged |= ImGui::DragFloat("##X", &values.x, 0.1f);
        ImGui::PopItemWidth();
        UI::Tooltip(std::to_string(values.x));
        ImGui::SameLine();

        //Y
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,.5f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,.5f,0.0f,1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValues.y;
            bValueChanged = true;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        bValueChanged |= ImGui::DragFloat("##Y", &values.y, 0.1f);
        ImGui::PopItemWidth();
        UI::Tooltip(std::to_string(values.y));
        ImGui::SameLine();

        //Z
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,0.5f,0.0f,1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,0.0f,1.0f,1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValues.z;
            bValueChanged = true;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        bValueChanged |= ImGui::DragFloat("##Z", &values.z, 0.1f);
        ImGui::PopItemWidth();
        UI::Tooltip(std::to_string(values.z));

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return bValueChanged;
    }

    bool AttributeInputText(const std::string& label, std::string& value, const std::string& helpMessage)
    {
        static char buffer[256];
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        strcpy_s(buffer, 256, value.c_str());

        if (ImGui::InputText(s_IDBuffer, buffer, 256))
        {
            value = buffer;
            bModified = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return bModified;
    }

    bool AttributeBool(const std::string& label, bool& value, const std::string& helpMessage)
    {
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::Checkbox(s_IDBuffer, &value);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return bModified;
    }

    bool AttributeLabels(const std::string& label, const std::vector<std::string>& customLabels, bool* values, const std::string& helpMessage)
    {
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        size_t count = customLabels.size();
        for (size_t i = 0; i < count; ++i)
        {
            UpdateIDBuffer(label + customLabels[i]);
            ImGui::Text(customLabels[i].c_str());
            ImGui::SameLine();
            bModified |= ImGui::Checkbox(s_IDBuffer, &values[i]);
            if (i != (count - 1))
                ImGui::SameLine();
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return bModified;
    }
    static inline ImRect RectExpanded(const ImRect& rect, float x, float y)
    {
        ImRect result = rect;
        result.Min.x -= x;
        result.Min.y -= y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }
    static inline ImRect GetItemRect()
    {
        return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    }
    static void DrawItemActivityOutline(float rounding = 0.0f, bool drawWhenInactive = false, ImColor colourWhenActive = (236, 158, 36, 255))
    {
        auto* drawList = ImGui::GetWindowDrawList();
        const ImRect rect = RectExpanded(GetItemRect(), 1.0f, 1.0f);
        if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
        {
            drawList->AddRect(rect.Min, rect.Max,
                ImColor(60, 60, 60), rounding, 0, 1.5f);
        }
        if (ImGui::IsItemActive())
        {
            drawList->AddRect(rect.Min, rect.Max,
                colourWhenActive, rounding, 0, 1.0f);
        }
        else if (!ImGui::IsItemHovered() && drawWhenInactive)
        {
            drawList->AddRect(rect.Min, rect.Max,
                ImColor(50, 50, 50), rounding, 0, 1.0f);
        }
    };
    static bool IsItemDisabled()
    {
        return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
    }
    bool AttributeTextBar(const std::string& label, const std::string& text)
    {
        ImGui::Text(label.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        //UI::PushItemDisabled();
       // ImGui::BeginDisabled(true);
        ImGui::InputText(fmt::format("##{0}", label).c_str(), (char*)text.c_str(), text.size(), ImGuiInputTextFlags_ReadOnly);
        //ImGui::EndDisabled();
        //UI::PopItemDisabled();
        
        if (IsItemDisabled())
           DrawItemActivityOutline(2.0f, true);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return false;
    }

    bool AttributeAssetTextBar(const std::string& label, AssetID& ID, AssetType type, bool includeRemove )
    {
        if (AssetManager::HasAsset(ID))
        {
            auto assetInfo = AssetManager::GetAssetInfo(ID);
            UI::AttributeTextBar(label, assetInfo.GetName());
        }
        else
        {
            UI::AttributeTextBar(label, fmt::format("Null ({})",EnumReflection::EnumString(type)));
        }

        if (includeRemove)
        {

            if (ImGui::BeginPopupContextItem("RemoveAssetItem"))
            {
                ImGui::EndPopup();
            }


            if (ImGui::BeginPopup("RemoveAssetItem"))
            {
                if (ImGui::MenuItem("Remove"))
                {
                    ID = 0;
                }

                ImGui::EndPopup();
            }
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(type).c_str()))
            {
                ImVec2 targetMin = ImGui::GetItemRectMin();
                ImVec2 targetMax = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddRect(targetMin, targetMax, IM_COL32(255, 255, 255, 255)); // Green border
                UUID assetID = *(UUID*)payload->Data;
                if (AssetManager::HasAsset(assetID) && AssetManager::GetAssetInfo(assetID).Type == type)
                    ID = assetID;
            }
            ImGui::EndDragDropTarget();
        }

        return false;
    }
    bool AttributeAssetTextBar(const std::string& label, Count<class Asset> asset, AssetType type, bool includeRemove )
    {
        if (AssetManager::HasAsset(asset))
        {

            auto assetInfo = AssetManager::GetAssetInfo(asset->GetID());
            UI::AttributeTextBar(label, assetInfo.GetName());
        }
        else
        {
            UI::AttributeTextBar(label, fmt::format("Null ({})", EnumReflection::EnumString(type)));
        }
        if (includeRemove)
        {
            if (ImGui::BeginPopupContextItem("RemoveAssetItem"))
            {
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("RemoveAssetItem"))
            {
                if (ImGui::MenuItem("Remove"))
                {
                    asset = nullptr;
                }

                ImGui::EndPopup();
            }
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(type).c_str()))
            {
                ImVec2 targetMin = ImGui::GetItemRectMin();
                ImVec2 targetMax = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddRect(targetMin, targetMax, IM_COL32(0, 255, 0, 255)); // Green border
                UUID assetID = *(UUID*)payload->Data;
                if (AssetManager::HasAsset(assetID) && AssetManager::GetAssetInfo(assetID).Type == type)
                    asset = AssetManager::GetAsset<Asset>(assetID);
            }
            ImGui::EndDragDropTarget();
        }
      
        
        return false;
    }
    bool AttributeText(const std::string& text)
    {
        ImGui::Text(text.c_str());
        return false;
    }
    std::tuple<bool, int, std::string> Combo(const std::string& label, const std::vector<std::string>& options, const std::string& currentSelected, const std::vector<std::string>& tooltips, const std::string& helpMessage)
    {
        std::string returnValue = currentSelected;
        int currentSelection = 0;
        size_t tooltipsSize = tooltips.size();
        bool bModified = false;
        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (ImGui::BeginCombo(s_IDBuffer, currentSelected.c_str()))
        {
            for (int i = 0; i < options.size(); ++i)
            {
                bool isSelected = (currentSelected == options[i]);

                if(isSelected)
                    currentSelection = i;

                if (ImGui::Selectable(options[i].c_str(), isSelected))
                {
                    bModified = true;
                    currentSelection = i;
                    returnValue = options[i];
                }

                if (i < tooltipsSize)
                    if (!tooltips[i].empty())
                        Tooltip(tooltips[i]);

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (currentSelection < tooltipsSize)
            if (!tooltips[currentSelection].empty())
                Tooltip(tooltips[currentSelection]);

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return { bModified,currentSelection,returnValue };
    }

    bool AttributeSlider(const std::string& label, int& value, int min, int max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderInt(s_IDBuffer, &value, min, max,format,flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;

    }

    bool AttributeSlider(const std::string& label, float& value, float min, float max, const std::string& helpMessage,ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;
         
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderFloat(s_IDBuffer, &value, min, max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeSlider(const std::string& label, glm::vec2& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderFloat2(s_IDBuffer, &value.x, min, max,format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;

    }

    bool AttributeSlider(const std::string& label, glm::vec3& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderFloat3(s_IDBuffer, &value.x, min, max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeSlider(const std::string& label, glm::vec4& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderFloat4(s_IDBuffer, &value.x, min, max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified; 
    }

    bool AttributeDrag(const std::string& label, uint32_t& value, float speed, uint32_t min, uint32_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;


        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        bModified = ImGui::DragScalar(s_IDBuffer, ImGuiDataType_U32, &value, speed, &min, &max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeDrag(const std::string& label, float& value, float speed, float min, float max, const std::string& helpMessage , ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::DragFloat(s_IDBuffer, &value, speed,min,max,format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeDrag(const std::string& label, glm::vec2& value, float speed, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;
        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::DragFloat2(s_IDBuffer, &value.x, speed, min, max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeDrag(const std::string& label, glm::vec3& value, float speed, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::DragFloat3(s_IDBuffer, &value.x, speed, min, max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeDrag(const std::string& label, glm::vec4& value, float speed, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::DragFloat4(s_IDBuffer, &value.x, speed, min, max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeColor(const std::string& label, glm::vec3& value, const std::string& helpMessage)
    {
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::ColorEdit3(s_IDBuffer, &value.x);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeColor(const std::string& label, glm::vec4& value, const std::string& helpMessage)
    {
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::ColorEdit4(s_IDBuffer, &value.x);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeButton(const std::string& label, const std::string& buttonText, const ImVec2& size)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bool result = ImGui::Button(buttonText.c_str(), size);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return result;
    }

    bool AttributeInputTextMultiline(const std::string& label, std::string& value, ImGuiInputTextFlags flags)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;


        bool modfified = false;
        std::string id = fmt::format("##{}", label);
        ImGui::Text(label.c_str());
        ImGui::SameLine();


        InputTextCallbackData cb_user_data(value);
        cb_user_data.ChainCallback = nullptr;
        cb_user_data.ChainCallbackUserData = nullptr;

        return ImGui::InputTextMultiline(label.c_str(), (char*)value.c_str(), value.capacity() + 1, ImVec2(0,0),flags, InputTextCallback, &cb_user_data);

    }

    void PushItemDisabled()
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    void PopItemDisabled()
    {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }

    void Tooltip(const std::string& tooltip, float treshHold)
    {
        if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > treshHold)
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(tooltip.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void HelpMarker(const std::string& text)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(text.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

}

