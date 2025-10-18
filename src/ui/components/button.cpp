#include "ui.h"
#include "imgui.h"

bool ui::components::button(const std::string& label) 
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 7));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    bool clicked = ImGui::Button(label.c_str());

    ImGui::PopStyleVar(2);

    return clicked;
}
