#include "ui.h"
#include "imgui.h"
#include "helpers.h"

bool ui::components::button(const std::string& label)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 7));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    bool clicked = ImGui::Button(label.c_str());

    ImGui::PopStyleVar(2);

    return clicked;
}

bool ui::components::secondary_button(const std::string& label)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 7));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleColor(ImGuiCol_Button,        helpers::hex_color(0xFFFFFF, 0.10f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, helpers::hex_color(0xFFFFFF, 0.16f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  helpers::hex_color(0xFFFFFF, 0.10f));

    bool clicked = ImGui::Button(label.c_str());

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    return clicked;
}
