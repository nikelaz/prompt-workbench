#include "ui.h"
#include "imgui.h"
#include "helpers.h"
#include <GLFW/glfw3.h>
#include <string>

bool ui::components::select(
    const char* id,
    int* current_item,
    const char* const* items,
    int items_count
) {
    bool changed = false;

    const float height = 42.0f;
    const float padding_x = 18.0f;
    const float width = ImGui::CalcItemWidth();

    ImGui::PushStyleColor(ImGuiCol_Button,        helpers::hex_color(0xFFFFFF, 0.05f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, helpers::hex_color(0xFFFFFF, 0.07f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  helpers::hex_color(0xFFFFFF, 0.05f));

    ImVec2 start = ImGui::GetCursorScreenPos();

    ImGui::InvisibleButton(id, ImVec2(width, height));
    bool hovered = ImGui::IsItemHovered();
    bool clicked = ImGui::IsItemClicked();
    bool active  = ImGui::IsItemActive();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 bg_color = ImGui::GetColorU32(ImGuiCol_Button);
    if (hovered) bg_color = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    if (active)  bg_color = ImGui::GetColorU32(ImGuiCol_ButtonActive);

    draw_list->AddRectFilled(start, ImVec2(start.x + width, start.y + height), bg_color, 5.0f);

    float font_size = ImGui::GetFontSize();
    ImGui::SetCursorScreenPos(ImVec2(start.x + padding_x, start.y + (height - font_size) * 0.5f));
    ImGui::Text("%s", items[*current_item]);

    // Draw chevron
    float cx = start.x + width - 20.0f;
    float cy = start.y + height * 0.5f;
    ImU32 chevron_color = IM_COL32(255, 255, 255, 180);
    draw_list->AddLine(ImVec2(cx - 5, cy - 3), ImVec2(cx, cy + 3), chevron_color, 1.5f);
    draw_list->AddLine(ImVec2(cx, cy + 3), ImVec2(cx + 5, cy - 3), chevron_color, 1.5f);

    ImGui::SetCursorScreenPos(ImVec2(start.x, start.y + height));
    ImGui::PopStyleColor(3);

    // Popup
    std::string popup_id = std::string(id) + "_popup";
    if (clicked) ImGui::OpenPopup(popup_id.c_str());

    ImGui::SetNextWindowPos(ImVec2(start.x, start.y + height + 2));
    ImGui::SetNextWindowSize(ImVec2(width, 0));

    ImGui::PushStyleColor(ImGuiCol_PopupBg,       helpers::hex_color(0x121212, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,  helpers::hex_color(0xFFFFFF, 0.07f));
    ImGui::PushStyleColor(ImGuiCol_Header,         helpers::hex_color(0xFFFFFF, 0.05f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,       ImVec2(18, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,        ImVec2(18, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));

    if (ImGui::BeginPopup(popup_id.c_str(), ImGuiWindowFlags_NoMove)) {
        for (int i = 0; i < items_count; i++) {
            if (ImGui::Selectable(items[i], *current_item == i, 0, ImVec2(0, ImGui::GetFrameHeight()))) {
                *current_item = i;
                changed = true;
                glfwPostEmptyEvent();
            }
        }
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(3);

    return changed;
}
