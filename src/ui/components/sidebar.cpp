#include "ui.h"
#include "imgui.h"
#include "helpers.h"

void ui::components::sidebar(std::function<void()> children)
{
    ImGui::Begin(
        "Sidebar",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize
    );

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos  = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();

    ImU32 bg_color = ImColor(1.0f, 1.0f, 1.0f, 0.05f);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color);

    children();

    ImGui::End();
}

bool ui::components::sidebar_button(
    const std::string& label,
    const bool is_active = false
)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    ImVec4 button_color =
        is_active
        ? helpers::hex_color(0xFFFFFF, 0.05f)
        : helpers::hex_color(0xFFFFFF, 0.00f);

    ImGui::PushStyleColor(ImGuiCol_Button, button_color);
    ImGui::PushStyleColor(
        ImGuiCol_ButtonHovered,
        helpers::hex_color(0xFFFFFF, 0.05f)
    );
    ImGui::PushStyleColor(
        ImGuiCol_ButtonActive,
        helpers::hex_color(0xFFFFFF, 0.03f)
    );

    bool clicked = ImGui::Button(
        label.c_str(),
        ImVec2(-FLT_MIN, 0)
    );

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);

    return clicked;
}
