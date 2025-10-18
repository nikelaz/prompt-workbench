#include "ui.h"
#include "imgui.h"

void ui::views::test_suite_details(
    routing::Router& router,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompts::UserPromptsViewModel& user_prompts_vm
)
{
    ui::components::top_bar(router);

    if (!test_suites_vm.current_test_suite)
    {
        ImGui::Text("An unexpected error occured. Could not load the test suite");
    }

    ImGui::Text(
        "Title: %s",
        test_suites_vm.current_test_suite->title.c_str()
    );
    ImGui::Text(
        "Description: %s",
        test_suites_vm.current_test_suite->description.c_str()
    );
    ImGui::Text(
        "System Prompt: %s",
        test_suites_vm.current_test_suite->system_prompt.c_str()
    );
    ImGui::Text(
        "Model: %s",
        test_suites_vm.current_test_suite->model.c_str()
    );

    ImGui::Text("User Prompts");

    for (UserPrompt user_prompt : user_prompts_vm.user_prompts)
    {
        if (
            ui::components::card(
                "user_prompt_" +  std::to_string(user_prompt.id),
                user_prompt.prompt,
                std::nullopt,
                std::nullopt
            )
        )
        {
            user_prompts_vm.current_user_prompt = user_prompt;
            routing::push(router, routing::USER_PROMPT_DETAILS);
        }
    }

    ImGui::Text("Result Runs");

    for (ResultRun result_run : user_prompts_vm.result_runs)
    {
      ui::components::card(
            "result_run_" +  std::to_string(result_run.id),
            result_run.date,
            std::nullopt,
            std::nullopt
        );
    }
}
