#include "ui.h"
#include "imgui.h"
#include "helpers.h"
#include "fonts.h"
#include "errors.h"

void ui::components::input_error(
    const errors::DisplayError& error,
    const fonts::Fonts& fonts
) 
{
    if (error.has_error)
    {
        ImGui::PushFont(fonts.regular_small);
        ImGui::TextColored(
            helpers::hex_color(0xE57373, 1.0f),
            "%s", error.message.c_str()
        );
        ImGui::PopFont();
    }
}
