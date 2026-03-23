#include "ui.h"
#include "imgui.h"
#include "fonts.h"
#include "helpers.h"
#include "accent_color.h"

void ui::views::result_runs_list(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm,
    vm::run_tests::RunTestsViewModel& run_tests_vm,
    const std::string& system_prompt
)
{
    if (test_suites_vm.current_test_suite) {
        vm::run_tests::commit_if_done(run_tests_vm, dba_state, user_prompt_vm);
    }

    const float heading_height   = 24.0f;
    const float button_padding_y = 7.0f;
    const float button_height    = ImGui::GetFontSize() + 2.0f * button_padding_y;
    const float row_height       = std::max(heading_height, button_height);
    const float row_y            = ImGui::GetCursorPosY();

    ImGui::SetCursorPosY(row_y + (row_height - button_height) * 0.5f);
    if (ui::components::secondary_button("Back"))
        routing::back(router);

    ImGui::SameLine();

    ImGui::SetCursorPosY(row_y + (row_height - heading_height) * 0.5f - button_padding_y);
    ImGui::PushFont(fonts.heading);
    ImGui::Text("Result Runs");
    ImGui::PopFont();

    ImGui::SameLine();

    bool is_running = run_tests_vm.is_running.load();
    const char* run_label = "Run Tests";
    float run_button_width = ImGui::CalcTextSize(run_label).x + 2.0f * 18.0f;
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - run_button_width);
    ImGui::SetCursorPosY(row_y + (row_height - button_height) * 0.5f);
    if (is_running) ImGui::BeginDisabled();
    if (ui::components::button(run_label)) {
        Settings settings = dba::get_settings(dba_state);
        vm::run_tests::run(
            run_tests_vm,
            *test_suites_vm.current_test_suite,
            user_prompt_vm.user_prompts,
            settings,
            system_prompt
        );
        ImGui::OpenPopup("##running_tests");
    }
    if (is_running) ImGui::EndDisabled();

    ui::components::spacer(12.0f);

    for (ResultRun result_run : user_prompt_vm.result_runs)
    {
        if (ui::components::card(
            "result_run_" + std::to_string(result_run.id),
            result_run.title.value_or(result_run.date),
            std::nullopt,
            std::nullopt
        ))
        {
            vm::result_run_details::set_current_result_run(dba_state, result_run_details_vm, result_run);
            routing::push(router, routing::RESULT_RUN_DETAILS);
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, helpers::hex_color(0x121212, 1.00f));
    bool modal_open = ImGui::BeginPopupModal(
        "##running_tests",
        nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar
    );
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    if (modal_open)
    {
        int completed = run_tests_vm.completed_count.load();
        int total = run_tests_vm.total_count;
        float fraction = total > 0 ? static_cast<float>(completed) / static_cast<float>(total) : 0.0f;

        ImGui::Text("Running Tests");
        ImGui::Dummy(ImVec2(0, 16.0f));
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, platform::get_accent_color());
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 999.0f);
        ImGui::ProgressBar(fraction, ImVec2(300.0f, 12.0f), "");
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0, 4.0f));
        ImGui::Text("%d / %d", completed, total);
        ImGui::Dummy(ImVec2(0, 16.0f));

        if (ui::components::secondary_button("Stop")) {
            run_tests_vm.abort_requested.store(true);
        }

        if (!run_tests_vm.is_running.load()) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
