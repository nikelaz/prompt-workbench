#include "ui.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "fonts.h"
#include <GLFW/glfw3.h>
#include <chrono>

void ui::views::prompt_editor(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::prompt_editor::PromptEditorViewModel& vm,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm,
    vm::create_test_suite::CreateTestSuiteViewModel& create_test_suite_vm,
    vm::create_user_prompt::CreateUserPromptViewModel& create_user_prompt_vm,
    vm::run_tests::RunTestsViewModel& run_tests_vm,
    vm::edit_test_suite::EditTestSuiteViewModel& edit_test_suite_vm,
    vm::compare_result_runs::CompareResultRunsViewModel& compare_vm,
    vm::edit_user_prompt::EditUserPromptViewModel& edit_user_prompt_vm
)
{
    if (!vm.active_prompt_id) {
        ImGui::TextDisabled("Open or create a prompt to get started.");
        return;
    }

    // Chat toggle button (top-right corner)
    const char* chat_label = vm.chat_open ? "Close Chat" : "Chat";
    float chat_btn_w = ImGui::CalcTextSize(chat_label).x + 24.f;
    ImGui::SameLine(ImGui::GetContentRegionMax().x - chat_btn_w);
    if (ui::components::secondary_button(chat_label)) {
        vm.chat_open = !vm.chat_open;
        glfwPostEmptyEvent();
    }

    // Sync active_tab from router state so code-driven navigation reflects in tabs
    bool is_ts_route = (router.current_route != routing::PROMPT_EDITOR);
    static routing::Route s_prev_route = routing::PROMPT_EDITOR;
    ImGuiTabItemFlags editor_flag = 0;
    ImGuiTabItemFlags ts_flag = 0;
    if (s_prev_route != router.current_route) {
        if (is_ts_route) ts_flag = ImGuiTabItemFlags_SetSelected;
        else             editor_flag = ImGuiTabItemFlags_SetSelected;
        s_prev_route = router.current_route;
    }

    if (ImGui::BeginTabBar("##prompt_tabs")) {

        if (ImGui::BeginTabItem("Editor", nullptr, editor_flag)) {
            if (router.current_route != routing::PROMPT_EDITOR) {
                routing::push(router, routing::PROMPT_EDITOR);
            }

            ImVec2 avail = ImGui::GetContentRegionAvail();
            if (ImGui::InputTextMultiline("##editor", &vm.active_file_content, avail)) {
                vm.content_dirty = true;
                vm.last_edit_time = std::chrono::steady_clock::now();
            }
            vm::prompt_editor::save_if_dirty(vm);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Test Suites", nullptr, ts_flag)) {
            // Refresh test suites when prompt scope changes
            if (test_suites_vm.current_prompt_id != vm.active_prompt_id) {
                test_suites_vm.current_prompt_id = vm.active_prompt_id;
                vm::test_suites::refresh_for_prompt(test_suites_vm, dba_state, *vm.active_prompt_id);
            }

            // Navigate to TEST_SUITES if we just entered this tab
            if (router.current_route == routing::PROMPT_EDITOR) {
                routing::push(router, routing::TEST_SUITES);
            }

            switch (router.current_route) {
                case routing::TEST_SUITES:
                    ui::views::test_suites(router, dba_state, fonts, test_suites_vm, user_prompt_vm);
                    break;
                case routing::TEST_SUITES_DETAILS:
                    ui::views::test_suite_details(
                        router, dba_state, fonts, test_suites_vm, user_prompt_vm,
                        result_run_details_vm, run_tests_vm, edit_test_suite_vm,
                        vm.active_file_content
                    );
                    break;
                case routing::CREATE_TEST_SUITE:
                    ui::views::create_test_suite(router, dba_state, fonts, test_suites_vm, create_test_suite_vm);
                    break;
                case routing::EDIT_TEST_SUITE:
                    ui::views::edit_test_suite(router, dba_state, fonts, test_suites_vm, edit_test_suite_vm);
                    break;
                case routing::USER_PROMPTS_LIST:
                    ui::views::user_prompts_list(router, fonts, user_prompt_vm);
                    break;
                case routing::USER_PROMPT_DETAILS:
                    ui::views::user_prompt_details(router, dba_state, fonts, user_prompt_vm, edit_user_prompt_vm);
                    break;
                case routing::CREATE_USER_PROMPTS:
                    ui::views::create_user_prompts(router, dba_state, fonts, create_user_prompt_vm, user_prompt_vm);
                    break;
                case routing::EDIT_USER_PROMPT:
                    ui::views::edit_user_prompt(router, dba_state, fonts, user_prompt_vm, edit_user_prompt_vm);
                    break;
                case routing::RESULT_RUNS_LIST:
                    ui::views::result_runs_list(
                        router, dba_state, fonts, test_suites_vm, user_prompt_vm,
                        result_run_details_vm, run_tests_vm, vm.active_file_content
                    );
                    break;
                case routing::RESULT_RUN_DETAILS:
                    ui::views::result_run_details(router, dba_state, fonts, result_run_details_vm, user_prompt_vm, compare_vm);
                    break;
                case routing::ANSWER_DETAILS:
                    ui::views::answer_details(router, fonts, result_run_details_vm);
                    break;
                default:
                    ui::views::test_suites(router, dba_state, fonts, test_suites_vm, user_prompt_vm);
                    break;
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}
