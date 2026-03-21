#include "ui.h"
#include "imgui.h"
#include "fonts.h"
#include <algorithm>

void ui::components::page_header(
    routing::Router& router,
    const fonts::Fonts& fonts,
    const char* title
)
{
    const float heading_height   = 24.0f;
    const float button_padding_y = 7.0f; // must match frame padding y in button.cpp
    const float button_height    = ImGui::GetFontSize() + 2.0f * button_padding_y;
    const float row_height       = std::max(heading_height, button_height);

    ui::components::spacer(12.0f);

    float row_y = ImGui::GetCursorPosY();

    ImGui::SetCursorPosY(row_y + (row_height - button_height) * 0.5f);
    if (ui::components::button("Back"))
    {
        routing::back(router);
    }

    ImGui::SameLine();

    // Subtract button_padding_y to compensate for DC.CurrLineTextBaseOffset,
    // which ImGui silently adds to text draw position after a button on the same line.
    ImGui::SetCursorPosY(row_y + (row_height - heading_height) * 0.5f - button_padding_y);
    ImGui::PushFont(fonts.heading);
    ImGui::Text("%s", title);
    ImGui::PopFont();

    ui::components::spacer(12.0f);
}
