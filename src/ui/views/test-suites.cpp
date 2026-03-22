#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::test_suites(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompt::UserPromptViewModel& user_prompts_vm
)
{
    float heading_height = 24.0f;
    float button_height = ImGui::GetFontSize() + 2.0f * 7.0f; // font + 2 * frame padding y
    float row_height = std::max(heading_height, button_height);
    float row_y = ImGui::GetCursorPosY();

    ImGui::SetCursorPosY(row_y + (row_height - heading_height) * 0.5f);
    ImGui::PushFont(fonts.heading);
    ImGui::Text("Test Suites");
    ImGui::PopFont();

    ImGui::SameLine();

    float button_width = ImGui::CalcTextSize("Create Test Suite").x + 2.0f * 18.0f;
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - button_width);
    ImGui::SetCursorPosY(row_y + (row_height - button_height) * 0.5f);

    if (ui::components::button("Create Test Suite"))
    {
        routing::push(router, routing::CREATE_TEST_SUITE);
    }

    ui::components::spacer(12.0f);

    for (TestSuite test_suite : test_suites_vm.test_suites)
    {
        if (ui::components::card(
            std::to_string(test_suite.id),
            test_suite.title,
            test_suite.description,
            test_suite.model
        ))
        {
            vm::user_prompt::set_test_suite_id(
                dba_state,
                user_prompts_vm,
                test_suite.id
            );
            vm::test_suites::set_current_test_suite(
                test_suites_vm,
                dba_state,
                test_suite.id
            );
            routing::push(router, routing::TEST_SUITES_DETAILS);
        }
    }
}
