#include "ui.h"
#include "imgui.h"
#include "fonts.h"

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

    if (result_run_details_vm.current_result_run)
    {
        ImGui::TextDisabled("System Prompt:");
        ImGui::TextWrapped("%s", result_run_details_vm.current_result_run->system_prompt.c_str());
        ui::components::spacer(8.0f);
    }

    ImGui::TextDisabled("Answer:");
    ImGui::TextWrapped("%s", result_run_details_vm.current_answer->answer.c_str());
}
