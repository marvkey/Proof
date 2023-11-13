#include "Proofprch.h"
#include "UI.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include <imgui/imgui_internal.h>
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Scripting/ScriptField.h"
#include <regex>
#include "UiUtilities.h"
#include "Proof/Utils/StringUtils.h"
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
    static int s_UIContextID = 0;
    static uint32_t s_Counter = 0;

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

    void PushID()
    {
        s_UIContextID++;
        ImGui::PushID(s_UIContextID);
        s_Counter = 0;
    }

    void PopID()
    {
        ImGui::PopID();
        s_UIContextID--;
    }

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
    
    void BeginPropertyGrid(uint32_t columns )
    {
        PushID();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
        ImGui::Columns(columns);
    }

    void EndPropertyGrid()
    {
        Draw::Underline();
        ImGui::Columns(1);
        ImGui::PopStyleVar(2); // ItemSpacing, FramePadding
        ShiftCursorY(18.0f);

        PopID();
    }

    bool DrawVec3Control(const std::string& label, glm::vec3& values, const glm::vec3 resetValues, float columnWidth)
    {
        bool bValueChanged = false;
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, columnWidth);
        AttributeLabel(label.c_str());
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
    void AttributeLabel(const std::string& label)
    {
        if (label.empty())
            return;

        ImGui::Text(label.c_str());
    }
    bool AttributeInputText(const std::string& label, std::string& value, const std::string& helpMessage)
    {
        static char buffer[256];
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        AttributeLabel(label.c_str());
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
        AttributeLabel(label.c_str());
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
        AttributeLabel(label.c_str());
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
        AttributeLabel(label.c_str());
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
        bool changeState = false;
        if (AssetManager::HasAsset(ID))
        {
            auto assetInfo = AssetManager::GetAssetInfo(ID);
            UI::AttributeTextBar(label, assetInfo.GetName());
        }
        else
        {
            changeState = true;
            ID = 0;
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
                {
                    ID = assetID;
                    changeState = true;
                }
            }
            ImGui::EndDragDropTarget();
        }

        return changeState;
    }
    bool AttributeAssetTextBar(const std::string& label, Count<class Asset> asset, AssetType type, bool includeRemove )
    {
        bool changeState = false;

        static char  searchCharacters[512];
        if (AssetManager::HasAsset(asset))
        {

            auto assetInfo = AssetManager::GetAssetInfo(asset->GetID());
            UI::AttributeTextBar(label, assetInfo.GetName());
        }
        else
        {
            asset = nullptr;
            changeState = true;
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
                {
                    changeState = false;
                    asset = AssetManager::GetAsset<Asset>(assetID);
                }
            }
            ImGui::EndDragDropTarget();
        }
        


        
        return changeState;
    }
    bool AttributeEntity(const std::string& label, Count<class World> worldContext, UUID& entityID)
    {
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
        AttributeLabel(label.c_str());
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
    template<class ValueType>
    bool AttributeSliderBase(const std::string& label, ValueType& value, ValueType min, ValueType max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format,ImGuiDataType_ type)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        AttributeLabel(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderScalar(s_IDBuffer, type, &value, &min, &max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeSlider(const std::string& label, uint8_t& value, uint8_t min, uint8_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<uint8_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_U8);
    }

    bool AttributeSlider(const std::string& label, uint16_t& value, uint16_t min, uint16_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<uint16_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_U16);
    }

    bool AttributeSlider(const std::string& label, uint32_t& value, uint32_t min, uint32_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<uint32_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_U32);
    }
    bool AttributeSlider(const std::string& label, uint64_t& value, uint64_t min, uint64_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<uint64_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_U64);
    }

    // For signed data types
    bool AttributeSlider(const std::string& label, int8_t& value, int8_t min, int8_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<int8_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_S8);
    }

    bool AttributeSlider(const std::string& label, int16_t& value, int16_t min, int16_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<int16_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_S16);
    }

    bool AttributeSlider(const std::string& label, int32_t& value, int32_t min, int32_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<int32_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_S32);
    }

    bool AttributeSlider(const std::string& label, int64_t& value, int64_t min, int64_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<int64_t>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_S64);
    }

    // For floating-point data types
    bool AttributeSlider(const std::string& label, float& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<float>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_Float);
    }

    bool AttributeSlider(const std::string& label, double& value, double min, double max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderBase<double>(label, value, min, max, helpMessage, flags, format, ImGuiDataType_Double);
    }
    template<class ValueType>
    bool AttributeSliderNBase(const std::string& label, ValueType& value, ValueType min, ValueType max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format,ImGuiDataType_  dataType, uint32_t count)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        AttributeLabel(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::SliderScalarN(s_IDBuffer, dataType, &value,(int)count,&min, &max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeSlider(const std::string& label, glm::vec2& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderNBase<float>(label, value.x, min, max, helpMessage, flags, format, ImGuiDataType_Float, 2);
    }

    bool AttributeSlider(const std::string& label, glm::vec3& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderNBase<float>(label, value.x, min, max, helpMessage, flags, format, ImGuiDataType_Float, 3);
    }

    bool AttributeSlider(const std::string& label, glm::vec4& value, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeSliderNBase<float>(label, value.x, min, max, helpMessage, flags, format, ImGuiDataType_Float, 4);
    }

    template<class ValueType>
    bool AttributeDragBase(const std::string& label, ValueType& value, float speed, ValueType min, ValueType max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format, ImGuiDataType_ dataType)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        AttributeLabel(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        bModified = ImGui::DragScalar(s_IDBuffer, dataType, &value, speed, &min, &max, format, flags);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }

    bool AttributeDrag(const std::string& label, int8_t& value, float speed, int8_t min, int8_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_S8);
    }
    bool AttributeDrag(const std::string& label, int16_t& value, float speed, int16_t min, int16_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_S16);
    }
    bool AttributeDrag(const std::string& label, int32_t& value, float speed, int32_t min, int32_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_S32);
    }
    bool AttributeDrag(const std::string& label, int64_t& value, float speed, int64_t min, int64_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_S64);
    }

    bool AttributeDrag(const std::string& label, uint8_t& value, float speed, uint8_t min, uint8_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            max = Math::GetMaxType<uint8_t>();
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_U8);
    }
    bool AttributeDrag(const std::string& label, uint16_t& value, float speed, uint16_t min, uint16_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            max = Math::GetMaxType<uint16_t>();
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_U16);
    }
    bool AttributeDrag(const std::string& label, uint32_t& value, float speed, uint32_t min, uint32_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            max = Math::GetMaxType<uint32_t>();
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_U32);
    }
    bool AttributeDrag(const std::string& label, uint64_t& value, float speed, uint64_t min, uint64_t max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        if (min == 0 && max == 0)
            max = Math::GetMaxType<uint64_t>();
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_U64);
    }

    bool AttributeDrag(const std::string& label, float& value, float speed, float min, float max, const std::string& helpMessage , ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_Float);
    }
    bool AttributeDrag(const std::string& label, double& value, float speed, double min, double max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragBase(label, value, speed, min, max, helpMessage, flags, format, ImGuiDataType_Double);
    }

    template<class ValueType>
    bool AttributeDragNBase(const std::string& label, ValueType& value, float speed, ValueType min, ValueType max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format, ImGuiDataType_ dataType, uint32_t count)
    {
        if (min == 0 && max == 0)
            flags = flags & ~ImGuiSliderFlags_AlwaysClamp;

        bool bModified = false;
        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        AttributeLabel(label.c_str());
        if (helpMessage.size())
        {
            ImGui::SameLine();
            UI::HelpMarker(helpMessage);
        }
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        bModified = ImGui::DragScalarN(s_IDBuffer, dataType, &value,(int)count, speed, &min, &max, format, flags);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        return bModified;
    }
    bool AttributeDrag(const std::string& label, glm::vec2& value, float speed, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragNBase<float>(label, value.x, speed, min, max, helpMessage, flags, format, ImGuiDataType_Float, 2);
    }

    bool AttributeDrag(const std::string& label, glm::vec3& value, float speed, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragNBase<float>(label, value.x, speed, min, max, helpMessage, flags, format, ImGuiDataType_Float, 3);
    }

    bool AttributeDrag(const std::string& label, glm::vec4& value, float speed, float min, float max, const std::string& helpMessage, ImGuiSliderFlags flags, const char* format)
    {
        return AttributeDragNBase<float>(label, value.x, speed, min, max, helpMessage, flags, format, ImGuiDataType_Float, 4);
    }

    bool AttributeColor(const std::string& label, glm::vec3& value, const std::string& helpMessage)
    {
        bool bModified = false;

        UpdateIDBuffer(label);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        AttributeLabel(label.c_str());
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
        AttributeLabel(label.c_str());
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
        AttributeLabel(label.c_str());
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
        AttributeLabel(label.c_str());
        ImGui::SameLine();


        InputTextCallbackData cb_user_data(value);
        cb_user_data.ChainCallback = nullptr;
        cb_user_data.ChainCallbackUserData = nullptr;

        return ImGui::InputTextMultiline(label.c_str(), (char*)value.c_str(), value.capacity() + 1, ImVec2(0,0),flags, InputTextCallback, &cb_user_data);

    }
    static bool isDisabledSet = false;
    void PushItemDisabled()
    {
        isDisabledSet = true;
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    void PopItemDisabled()
    {
        isDisabledSet = false;
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
        bool localisDisabledSet = isDisabledSet;
        if (localisDisabledSet)
            UI::PopItemDisabled();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(text.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        if (localisDisabledSet)
            UI::PushItemDisabled();

    }
    #define ATTRIBUTE_FIELD_VALUE(storage,fieldName,DataType, delta, min, max, toolTip, useSlider,result ) \
    { \
        DataType value = storage->GetValue<DataType>(); \
        if(useSlider){\
            if (AttributeSlider(fieldName, value, (DataType) min, (DataType) max, toolTip)) \
            { \
                storage->SetValue(value); \
                result = true; \
            } \
        }\
        else if (AttributeDrag(fieldName, value, delta, (DataType) min, (DataType) max, toolTip)) \
        { \
            storage->SetValue(value); \
            result = true; \
        } \
    }

    bool DrawFieldValue(Count<class World> worldContext, const std::string& fieldName, Count<Proof::FieldStorage>& storage)
    {
        if (!storage)
            return false;

        const ScriptField* field = storage->GetFieldInfo();

        float min = 0.0f;
        float max = 0.0f;
        float delta = 0.1f;

        bool useSlider = false;
        const std::string& toolTip = field->ToolTip;

        //field range attribute
        {
            const auto& fieldRangeAttribute = field->FieldRangeAttribute;

            if (fieldRangeAttribute.IsAllSet())
            {
                useSlider = true;
                min = fieldRangeAttribute.MinValue;
                max = fieldRangeAttribute.MaxValue;
            }

            else if (fieldRangeAttribute.IsMinSet())
            {
                useSlider = true;
                min = fieldRangeAttribute.MinValue;
            }
            else if (fieldRangeAttribute.IsMaxSet())
            {
                useSlider = true;
                max = fieldRangeAttribute.MaxValue;
            }

            if (useSlider == true)
                delta = 0.5f;
        }

        
        std::string id = fmt::format("{0}-{1}", fieldName, field->Name);
        ImGui::PushID(id.c_str());

        if (field->HasFlag(FieldFlag::ReadOnly))
            UI::PushItemDisabled();
        bool result = false;

        switch (field->Type)
        {
            case ScriptFieldType::Bool:
                {
                    bool value = storage->GetValue<bool>();
                    if (AttributeBool(fieldName.c_str(), value, toolTip))
                    {
                        storage->SetValue(value);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Int8:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, int8_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::Int16:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, int16_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::Int32:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, int32_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::Int64:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, int64_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::UInt8:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, uint8_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::UInt16:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, uint16_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::UInt32:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, uint32_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::UInt64:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, uint64_t, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::Float:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, float, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::Double:
                {
                    ATTRIBUTE_FIELD_VALUE(storage, fieldName, double, delta, min, max, toolTip, useSlider, result);
                    break;
                }
            case ScriptFieldType::String:
                {
                    std::string value = storage->GetValue<std::string>();
                    char buffer[256];
                    memset(buffer, 0, 256);
                    memcpy(buffer, value.c_str(), value.length());
                    if (AttributeInputText(fieldName.c_str(), value, toolTip))
                    {
                        storage->SetValue<std::string>(buffer);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Vector2:
                {
                    glm::vec2 value = storage->GetValue<glm::vec2>();
                    if (AttributeDrag(fieldName.c_str(), value))
                    {
                        storage->SetValue(value);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Vector3:
                {
                    glm::vec3 value = storage->GetValue<glm::vec3>();
                    if (AttributeDrag(fieldName.c_str(), value))
                    {
                        storage->SetValue(value);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Vector4:
                {
                    glm::vec4 value = storage->GetValue<glm::vec4>();
                    if (AttributeDrag(fieldName.c_str(), value))
                    {
                        storage->SetValue(value);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Prefab:
                {
                    AssetID handle = storage->GetValue<AssetID>();
                    if ((fieldName.c_str(), handle))
                    {
                        storage->SetValue(handle);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Entity:
                {
                    UUID uuid = storage->GetValue<UUID>();
                    //if (AttributeEntity(fieldName.c_str(), uuid, sceneContext))
                    //{
                    //    storage->SetValue(uuid);
                    //    result = true;
                    //}
                    break;
                }
            case ScriptFieldType::Mesh:
                {
                    AssetID handle = storage->GetValue<AssetID>();
                    if (AttributeAssetTextBar(fieldName.c_str(), handle,AssetType::Mesh))
                    {
                        storage->SetValue(handle);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::DynamicMesh:
                {
                    AssetID handle = storage->GetValue<AssetID>();
                    if (AttributeAssetTextBar(fieldName.c_str(), handle, AssetType::DynamicMesh))
                    {
                        storage->SetValue(handle);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Material:
                {
                    AssetID handle = storage->GetValue<AssetID>();
                    if (AttributeAssetTextBar(fieldName.c_str(), handle, AssetType::Material))
                    {
                        storage->SetValue(handle);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::PhysicsMaterial:
                {
                    AssetID handle = storage->GetValue<AssetID>();
                    if (AttributeAssetTextBar(fieldName.c_str(), handle, AssetType::PhysicsMaterial))
                    {
                        storage->SetValue(handle);
                        result = true;
                    }
                    break;
                }
            case ScriptFieldType::Texture2D:
                {
                    AssetID handle = storage->GetValue<AssetID>();
                    if (AttributeAssetTextBar(fieldName.c_str(), handle, AssetType::Texture))
                    {
                        storage->SetValue(handle);
                        result = true;
                    }
                    break;
                }
        }

        ImGui::PopID();
        if (field->HasFlag(FieldFlag::ReadOnly))
            UI::PopItemDisabled();

        return result;
    }

    bool DrawFieldValue(Count < World > worldContext, const std::string& fieldName,Count<EnumFieldStorage>& storage)
    {
        if (!storage)
            return false;

        const ScriptField* field = storage->GetFieldInfo();

        ManageEnumClass* managedEnumClass = ScriptRegistry::GetManagedEnumClassByName(field->RegistryClassName);

        if (managedEnumClass == nullptr)
            return false;

        std::string id = fmt::format("{0}-{1}", fieldName, field->Name);
        UI::PushID();

        bool result = false;

        if (managedEnumClass->EnumFields.empty())
        {
            ImGui::Text("Empty Enum");
            return result;
        }
        {
            std::vector<std::string> options;
            options.reserve(managedEnumClass->EnumFields.size());
            int selectionIndex = -1;
            
            for (uint32_t i =0; i< managedEnumClass->EnumFields.size(); i++)
            {
                const auto& enumMetaData = managedEnumClass->EnumFields[i];
                options.emplace_back(enumMetaData.Name);
                if (*storage->GetValueBuffer().Data == *enumMetaData.GetValueBuffer().Data)
                {
                    selectionIndex = i;
                }
            }

            if (selectionIndex == -1)
            {
                const auto& enumMetaData = managedEnumClass->EnumFields[0];
                storage->SetValueBuffer(enumMetaData.GetValueBuffer());
                selectionIndex = 0;
                result = true;
            }
            auto [changed, outIndex, outSelectionString] = UI::Combo(fieldName, options,managedEnumClass->EnumFields[selectionIndex].Name);

            if (changed)
            {
                const auto& enumMetaData = managedEnumClass->EnumFields[outIndex];
                storage->SetValueBuffer(enumMetaData.GetValueBuffer());
                result = true;
            }
        }
        UI::PopID();
        return result;
    }

   

    bool AttributeTreeNode(const std::string& label, bool openByDefault)
    {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_FramePadding;

        if (openByDefault)
            treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

        bool open = false;
        const float framePaddingX = 3.0f;
        const float framePaddingY = 3.0f; // affects height of the header

        UI::ScopedStyleVar headerRounding(ImGuiStyleVar_FrameRounding, 0.0f);
        UI::ScopedStyleVar headerPaddingAndHeight(ImGuiStyleVar_FramePadding, ImVec2{ framePaddingX, framePaddingY });

        UpdateIDBuffer(label);

        ImGui::PushID(s_IDBuffer);

        open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, Utils::String::ToUpper(label).c_str());
        ImGui::PopID();

        return open;
    }

    bool AttributeTreeNodeIcon(const std::string& label, const Count<Texture2D>& icon, const ImVec2& size, bool openByDefault)
    {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_FramePadding;

        if (openByDefault)
            treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

        bool open = false;
        const float framePaddingX = 3.0f;
        const float framePaddingY = 3.0f; // affects height of the header

        UI::ScopedStyleVar headerRounding(ImGuiStyleVar_FrameRounding, 0.0f);
        UI::ScopedStyleVar headerPaddingAndHeight(ImGuiStyleVar_FramePadding, ImVec2{ framePaddingX, framePaddingY });

        ImGui::PushID(label.c_str());
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, "");

        float lineHeight = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
        ImGui::SameLine();
        UI::ShiftCursorY(size.y / 2.0f - 1.0f);
        UI::Image(icon, size);
        ImGui::SameLine();
        UI::ShiftCursorY(-(size.y / 2.0f) + 1.0f);
        ImGui::TextUnformatted(Utils::String::ToUpper(label).c_str());

        ImGui::PopID();

        return open;
    }

}

