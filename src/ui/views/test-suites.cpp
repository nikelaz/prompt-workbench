#include "ui.h"

void ui::views::test_suites(
    routing::Router& router,
    dba::DBAState& dba_state,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompts::UserPromptsViewModel& user_prompts_vm
)
{
    for (TestSuite test_suite : test_suites_vm.test_suites)
    {
        if (ui::components::card(
            std::to_string(test_suite.id),
            test_suite.title,
            test_suite.description,
            test_suite.model
        ))
        {
            vm::user_prompts::set_test_suite_id(
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
