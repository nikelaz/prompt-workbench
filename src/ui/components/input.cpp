#include "ui.h"
#include "imgui.h"

static int resize_callback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::string* str = (std::string*)data->UserData;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    return 0;
}

bool ui::components::input(
    const char* label,
    std::string* str
)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 11));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    std::string id = "##" + std::string(label);

    ImGui::Text("%s", label);

    bool input = ImGui::InputText(
        id.c_str(),
        (char*)str->c_str(),
        str->capacity() + 1,
        ImGuiInputTextFlags_CallbackResize,
        resize_callback,
        (void*)str
    );

    ImGui::PopStyleVar(2);

    return input;
}

bool ui::components::input_multiline(
    const char* label,
    std::string* str
)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 11));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    std::string id = "##" + std::string(label);
    ImGui::Text("%s", label);

    bool input = ImGui::InputTextMultiline(
        id.c_str(),
        (char*)str->c_str(),
        str->capacity() + 1,
        ImVec2(0, 0),
        ImGuiInputTextFlags_CallbackResize,
        resize_callback,
        (void*)str
    );
    
    ImGui::PopStyleVar(2);

    return input;
}
