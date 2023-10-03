#include "Proofprch.h"
#include "UI.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include <regex>

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

}

