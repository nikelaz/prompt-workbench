#include "ui.h"
#include "imgui.h"
#include "fonts.h"
#include "helpers.h"
#include <GLFW/glfw3.h>

void ui::views::answer_details(
    routing::Router& router,
    fonts::Fonts& fonts,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm
)
{
    ui::components::page_header(router, fonts, "Answer");

    if (!result_run_details_vm.current_answer)
    {
        ImGui::Text("An unexpected error occurred. Could not load the answer.");
        return;
    }

    auto truncate_preview = [](const std::string& s, int max_lines = 4, bool* truncated = nullptr) -> std::string {
        int newlines = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '\n' && ++newlines == max_lines) {
                if (truncated) *truncated = true;
                return s.substr(0, i) + "\n...";
            }
        }
        if (truncated) *truncated = false;
        return s;
    };

    if (result_run_details_vm.current_result_run)
    {
        bool sp_truncated = false;
        std::string sp_preview = truncate_preview(result_run_details_vm.current_result_run->system_prompt, 4, &sp_truncated);
        ImGui::TextDisabled("System Prompt:");
        ImGui::TextWrapped("%s", sp_preview.c_str());
        if (sp_truncated) {
            ui::components::spacer(4.0f);
            if (ui::components::secondary_button("View Full System Prompt")) {
                ImGui::OpenPopup("##full_system_prompt");
                glfwPostEmptyEvent();
            }
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_PopupBg, helpers::hex_color(0x121212, 1.00f));
        bool prompt_modal_open = ImGui::BeginPopupModal(
            "##full_system_prompt",
            nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize
        );
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();

        if (prompt_modal_open)
        {
            ImGui::Text("System Prompt");
            ui::components::spacer(8.0f);
            const std::string& sp = result_run_details_vm.current_result_run->system_prompt;
            ImGui::InputTextMultiline(
                "##system_prompt_full",
                const_cast<char*>(sp.c_str()),
                sp.size() + 1,
                ImVec2(600.0f, 400.0f),
                ImGuiInputTextFlags_ReadOnly
            );
            ui::components::spacer(8.0f);
            if (ui::components::secondary_button("Close")) {
                ImGui::CloseCurrentPopup();
                glfwPostEmptyEvent();
            }
            ImGui::EndPopup();
        }

        ui::components::spacer(8.0f);
    }

    if (result_run_details_vm.current_user_prompt)
    {
        bool up_truncated = false;
        std::string up_preview = truncate_preview(result_run_details_vm.current_user_prompt->prompt, 4, &up_truncated);
        ImGui::TextDisabled("User Prompt:");
        ImGui::TextWrapped("%s", up_preview.c_str());
        if (up_truncated) {
            ui::components::spacer(4.0f);
            if (ui::components::secondary_button("View Full User Prompt")) {
                ImGui::OpenPopup("##full_user_prompt");
                glfwPostEmptyEvent();
            }
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_PopupBg, helpers::hex_color(0x121212, 1.00f));
        bool user_prompt_modal_open = ImGui::BeginPopupModal(
            "##full_user_prompt",
            nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize
        );
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();

        if (user_prompt_modal_open)
        {
            ImGui::Text("User Prompt");
            ui::components::spacer(8.0f);
            const std::string& up = result_run_details_vm.current_user_prompt->prompt;
            ImGui::InputTextMultiline(
                "##user_prompt_full",
                const_cast<char*>(up.c_str()),
                up.size() + 1,
                ImVec2(600.0f, 400.0f),
                ImGuiInputTextFlags_ReadOnly
            );
            ui::components::spacer(8.0f);
            if (ui::components::secondary_button("Close")) {
                ImGui::CloseCurrentPopup();
                glfwPostEmptyEvent();
            }
            ImGui::EndPopup();
        }

        ui::components::spacer(8.0f);
    }

    ImGui::TextDisabled("Answer:");
    ImGui::TextWrapped("%s", result_run_details_vm.current_answer->answer.c_str());
}
