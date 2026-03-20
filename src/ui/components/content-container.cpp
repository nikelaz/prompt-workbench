#include "ui.h"
#include "imgui.h"
#include <algorithm>

void ui::components::content_container(std::function<void()> children) {
    const float max_width  = 720.0f;
    const float padding_xy = 32.0f;

    const float avail_x = ImGui::GetContentRegionAvail().x;
    const float avail_y = ImGui::GetContentRegionAvail().y;
    const float width    = std::min(avail_x, max_width);
    const float offset_x = std::max(0.0f, (avail_x - width) * 0.5f);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset_x);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, padding_xy));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
    ImGui::BeginChild("##content_container", ImVec2(width, avail_y), false);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PushItemWidth(-FLT_MIN);
    children();
    ImGui::PopItemWidth();

    ImGui::EndChild();
}
