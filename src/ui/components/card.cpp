#include "ui.h"
#include "imgui.h"
#include "helpers.h"

bool ui::components::card(
    const std::string& id,
    const std::string& headline,
    const std::optional<std::string>& subtext,
    const std::optional<std::string>& right_label
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    ImGui::PushStyleColor(
        ImGuiCol_Button,
        helpers::hex_color(0xFFFFFF, 0.05f)
    );
    ImGui::PushStyleColor(
        ImGuiCol_ButtonHovered,
        helpers::hex_color(0xFFFFFF, 0.07f)
    );
    ImGui::PushStyleColor(
        ImGuiCol_ButtonActive,
        helpers::hex_color(0xFFFFFF, 0.05f)
    );

    ImVec2 start = ImGui::GetCursorScreenPos();
    ImVec2 card_size = ImVec2(ImGui::GetContentRegionAvail().x, 60);

    ImGui::InvisibleButton(id.c_str(), card_size); 
    bool clicked = ImGui::IsItemClicked();
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 bg_color = ImGui::GetColorU32(ImGuiCol_Button);
    if (hovered) bg_color = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    if (active) bg_color = ImGui::GetColorU32(ImGuiCol_ButtonActive);

    draw_list->AddRectFilled(
        start,
        ImVec2(start.x + card_size.x, start.y + card_size.y),
        bg_color,
        5.0f
    );

    ImGui::SetCursorScreenPos(ImVec2(start.x + 16, start.y + 12));
    ImGui::Text("%s", headline.c_str());

    if (subtext)
    {
        ImGui::SetCursorScreenPos(ImVec2(start.x + 16, start.y + 32));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 180, 180, 255));
        ImGui::Text("%s", subtext.value().c_str());
        ImGui::PopStyleColor();
    }

    if (right_label)
    {
        const char* right_label_value = right_label.value().c_str();
        ImVec2 right_text_size = ImGui::CalcTextSize(right_label_value);
        ImGui::SetCursorScreenPos(
            ImVec2(start.x + card_size.x - right_text_size.x - 16, start.y + 22)
        );
        ImGui::Text("%s", right_label_value);
    }

    ImGui::SetCursorScreenPos(ImVec2(start.x, start.y + card_size.y));
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);

    ImGui::Dummy(ImVec2(6, 6));

    return clicked;
}
