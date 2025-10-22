#include "ui.h"
#include "imgui.h"

void ui::views::create_test_suite(
    routing::Router& router,
    dba::DBAState& dba_state,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::create_test_suite::CreateTestSuiteViewModel& create_test_suite_vm
)
{
    ui::components::top_bar(router);

    ui::components::input(
        "Title",
        &create_test_suite_vm.title
    );
    ui::components::input_error(
        create_test_suite_vm.title_error
    );

    ui::components::input_multiline(
        "Description",
        &create_test_suite_vm.description
    );

    ui::components::input_error(
        create_test_suite_vm.description_error
    );

    ui::components::input_multiline(
        "System Prompt",
        &create_test_suite_vm.system_prompt
    );

    ui::components::input_error(
        create_test_suite_vm.system_prompt_error
    );

    ui::components::input(
        "Model",
        &create_test_suite_vm.model
    );

    ui::components::input_error(
        create_test_suite_vm.model_error
    );
    
    if (ui::components::button("Create Test Suite"))
    {
        vm::create_test_suite::validate(create_test_suite_vm);
    
        if (
            !create_test_suite_vm.title_error.has_error 
            && !create_test_suite_vm.description_error.has_error 
            && !create_test_suite_vm.system_prompt_error.has_error 
            && !create_test_suite_vm.model_error.has_error 
        )
        {
            vm::create_test_suite::create_test_suite(
                dba_state,
                create_test_suite_vm,
                test_suites_vm
            );

            routing::push(router, routing::TEST_SUITES);

            create_test_suite_vm.title = "";
            create_test_suite_vm.description = "";
            create_test_suite_vm.system_prompt = "";
            create_test_suite_vm.model = "";
        }
    }
}
