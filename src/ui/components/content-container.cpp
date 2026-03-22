#include "ui.h"
#include "imgui.h"
#include <algorithm>

void ui::components::spacer(float height) {
    ImGui::Dummy(ImVec2(0.0f, height));
}

void ui::components::content_container(std::function<void()> children) {
    const float max_width  = 720.0f;
    const float padding_xy = 32.0f;

    const float avail_x = ImGui::GetContentRegionAvail().x;
    const float avail_y = ImGui::GetContentRegionAvail().y;
    const float width    = std::min(avail_x, max_width);
    const float offset_x = std::max(0.0f, (avail_x - width) * 0.5f);

    // Outer full-width child: scrollbar appears at the right window edge
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
    ImGui::BeginChild("##scroll_outer", ImVec2(avail_x, avail_y), ImGuiChildFlags_None);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset_x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding_xy);

    // Inner constrained child: auto-height, no own scrollbar
    // GetContentRegionAvail().x inside children() == width (== max_width when window is wide)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
    ImGui::BeginChild(
        "##content_container",
        ImVec2(width, 0),
        ImGuiChildFlags_AutoResizeY,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
    );
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::PushItemWidth(-FLT_MIN);
    children();
    ImGui::PopItemWidth();

    ImGui::EndChild(); // content_container

    ImGui::Dummy(ImVec2(0, padding_xy)); // bottom padding in scroll region

    ImGui::EndChild(); // scroll_outer
}
