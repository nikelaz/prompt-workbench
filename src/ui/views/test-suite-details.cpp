#include "ui.h"
#include "imgui.h"
#include "fonts.h"
#include "helpers.h"
#include "accent_color.h"
#include <GLFW/glfw3.h>

void ui::views::test_suite_details(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompt::UserPromptViewModel& user_prompt_details_vm,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm,
    vm::run_tests::RunTestsViewModel& run_tests_vm,
    vm::edit_test_suite::EditTestSuiteViewModel& edit_test_suite_vm,
    const std::string& system_prompt
)
{
    if (test_suites_vm.current_test_suite) {
        vm::run_tests::commit_if_done(run_tests_vm, dba_state, user_prompt_details_vm);
    }

    ui::components::page_header(router, fonts, "Test Suite");

    if (!test_suites_vm.current_test_suite)
    {
        ImGui::Text("An unexpected error occured. Could not load the test suite");
        return;
    }

    ImGui::TextDisabled("Title:");
    ImGui::TextWrapped("%s", test_suites_vm.current_test_suite->title.c_str());

    ui::components::spacer(8.0f);

    if (!test_suites_vm.current_test_suite->description.empty()) {
        ImGui::TextDisabled("Description:");
        ImGui::TextWrapped("%s", test_suites_vm.current_test_suite->description.c_str());
        ui::components::spacer(8.0f);
    }

    ui::components::spacer(8.0f);

    ImGui::TextDisabled("Model:");
    ImGui::TextWrapped("%s", test_suites_vm.current_test_suite->model.c_str());

    ui::components::spacer(8.0f);

    if (ui::components::secondary_button("Edit")) {
        vm::edit_test_suite::prepare(edit_test_suite_vm, *test_suites_vm.current_test_suite);
        routing::push(router, routing::EDIT_TEST_SUITE);
    }

    ImGui::SameLine();

    if (ui::components::secondary_button("Delete")) {
        ImGui::OpenPopup("##delete_test_suite");
    }

    ImGui::SameLine();

    bool is_running = run_tests_vm.is_running.load();
    if (is_running) ImGui::BeginDisabled();
    if (ui::components::button("Run Tests")) {
        Settings settings = dba::get_settings(dba_state);
        vm::run_tests::run(
            run_tests_vm,
            *test_suites_vm.current_test_suite,
            user_prompt_details_vm.user_prompts,
            settings,
            system_prompt
        );
        ImGui::OpenPopup("##running_tests");
    }
    if (is_running) ImGui::EndDisabled();

    if (ui::components::confirm_dialog(
        "##delete_test_suite",
        "Delete Test Suite?",
        "Are you sure you want to delete this test suite?\nThis action cannot be undone.",
        "Delete"
    )) {
        dba::delete_test_suite(dba_state, test_suites_vm.current_test_suite->id);
        test_suites_vm.current_test_suite = std::nullopt;
        vm::test_suites::refresh(test_suites_vm, dba_state);
        routing::push(router, routing::TEST_SUITES);
    }

    ui::components::spacer(16.0f);


    if (ui::components::card("user_prompts_nav", "User Prompts", std::nullopt, std::nullopt))
    {
      routing::push(router, routing::USER_PROMPTS_LIST);
    }

    ui::components::spacer(8.0f);

    if (ui::components::card("result_runs_nav", "Result Runs", std::nullopt, std::nullopt))
    {
      routing::push(router, routing::RESULT_RUNS_LIST);
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
