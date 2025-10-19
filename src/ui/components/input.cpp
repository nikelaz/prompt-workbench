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
    return ImGui::InputText(
        label,
        (char*)str->c_str(),
        str->capacity() + 1,
        ImGuiInputTextFlags_CallbackResize,
        resize_callback,
        (void*)str
    );
}

bool ui::components::input_multiline(
    const char* label,
    std::string* str
)
{
    return ImGui::InputTextMultiline(
        label,
        (char*)str->c_str(),
        str->capacity() + 1,
        ImVec2(200, 200),
        ImGuiInputTextFlags_CallbackResize,
        resize_callback,
        (void*)str
    );
}
