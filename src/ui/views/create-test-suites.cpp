#include "ui.h"
#include "fonts.h"
#include "imgui.h"

void ui::views::create_test_suite(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::create_test_suite::CreateTestSuiteViewModel& create_test_suite_vm
)
{
    ui::components::page_header(router, fonts, "Create Test Suite");

    ui::components::input(
        "Title",
        &create_test_suite_vm.title
    );
    ui::components::input_error(
        create_test_suite_vm.title_error,
        fonts
    );

    ui::components::spacer(12.0f);

    ui::components::input_multiline(
        "Description",
        &create_test_suite_vm.description
    );

    ui::components::input_error(
        create_test_suite_vm.description_error,
        fonts
    );

    ui::components::spacer(12.0f);

    ui::components::input_multiline(
        "System Prompt",
        &create_test_suite_vm.system_prompt
    );

    ui::components::input_error(
        create_test_suite_vm.system_prompt_error,
        fonts
    );

    ui::components::spacer(12.0f);

    if (!create_test_suite_vm.models_loaded && !create_test_suite_vm.models_loading.load()) {
        Settings s = dba::get_settings(dba_state);
        vm::create_test_suite::fetch_models(create_test_suite_vm, s);
    }

    if (create_test_suite_vm.models_loading.load()) {
        ImGui::Text("Loading models...");
    } else if (create_test_suite_vm.models_loaded && !create_test_suite_vm.available_models.empty()) {
        std::vector<const char*> model_ptrs;
        for (auto& m : create_test_suite_vm.available_models)
            model_ptrs.push_back(m.c_str());

        ui::components::select(
            "##model",
            &create_test_suite_vm.model_selection,
            model_ptrs.data(),
            (int)model_ptrs.size()
        );

        create_test_suite_vm.model = create_test_suite_vm.available_models[create_test_suite_vm.model_selection];
    } else {
        ui::components::input("Model", &create_test_suite_vm.model);
    }

    ui::components::input_error(
        create_test_suite_vm.model_error,
        fonts
    );

    ui::components::spacer(16.0f);

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
            create_test_suite_vm.models_loaded = false;
            create_test_suite_vm.models_loading.store(false);
            create_test_suite_vm.model_selection = 0;
        }
    }
}
