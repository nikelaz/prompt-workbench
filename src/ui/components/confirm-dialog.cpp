#include "ui.h"
#include "imgui.h"
#include "helpers.h"

bool ui::components::confirm_dialog(
    const char* id,
    const char* title,
    const char* message,
    const char* confirm_label,
    const char* cancel_label
) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, helpers::hex_color(0x121212, 1.00f));
    bool modal_open = ImGui::BeginPopupModal(id, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    if (!modal_open) return false;

    bool confirmed = false;

    ImGui::Text("%s", title);
    ImGui::Dummy(ImVec2(0, 16.0f));
    ImGui::Text("%s", message);
    ImGui::Dummy(ImVec2(0, 16.0f));

    if (ui::components::secondary_button(cancel_label)) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ui::components::button(confirm_label)) {
        confirmed = true;
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
    return confirmed;
}
