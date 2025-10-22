#include "ui.h"
#include "imgui.h"
#include "helpers.h"

void ui::components::input_error(
  const helpers::FormError& error
) 
{
    if (error.has_error)
    {
        ImGui::TextColored(
            helpers::hex_color(0xE57373, 1.0f),
            "%s", error.message.c_str()
        );
    }
}
