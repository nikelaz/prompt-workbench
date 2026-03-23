#include "ui.h"
#include "fonts.h"
#include "imgui.h"

void ui::views::edit_test_suite(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::edit_test_suite::EditTestSuiteViewModel& edit_test_suite_vm
)
{
    ui::components::page_header(router, fonts, "Edit Test Suite");

    ui::components::input(
        "Title",
        &edit_test_suite_vm.title
    );
    ui::components::input_error(
        edit_test_suite_vm.title_error,
        fonts
    );

    ui::components::spacer(12.0f);

    ui::components::input_multiline(
        "Description",
        &edit_test_suite_vm.description
    );

    ui::components::input_error(
        edit_test_suite_vm.description_error,
        fonts
    );

    ui::components::spacer(12.0f);

    if (!edit_test_suite_vm.models_loaded && !edit_test_suite_vm.models_loading.load()) {
        Settings s = dba::get_settings(dba_state);
        vm::edit_test_suite::fetch_models(edit_test_suite_vm, s);
    }

    if (edit_test_suite_vm.models_loading.load()) {
        ImGui::Text("Loading models...");
    } else if (edit_test_suite_vm.models_loaded && !edit_test_suite_vm.available_models.empty()) {
        std::vector<const char*> model_ptrs;
        for (auto& m : edit_test_suite_vm.available_models)
            model_ptrs.push_back(m.c_str());

        ui::components::select(
            "##model",
            &edit_test_suite_vm.model_selection,
            model_ptrs.data(),
            (int)model_ptrs.size()
        );

        edit_test_suite_vm.model = edit_test_suite_vm.available_models[edit_test_suite_vm.model_selection];
    } else {
        ui::components::input("Model", &edit_test_suite_vm.model);
    }

    ui::components::input_error(
        edit_test_suite_vm.model_error,
        fonts
    );

    ui::components::spacer(16.0f);

    if (ui::components::button("Save Changes"))
    {
        vm::edit_test_suite::validate(edit_test_suite_vm);

        if (
            !edit_test_suite_vm.title_error.has_error
            && !edit_test_suite_vm.description_error.has_error
            && !edit_test_suite_vm.model_error.has_error
        )
        {
            vm::edit_test_suite::update_test_suite(
                dba_state,
                edit_test_suite_vm,
                test_suites_vm
            );

            routing::back(router);
        }
    }
}
