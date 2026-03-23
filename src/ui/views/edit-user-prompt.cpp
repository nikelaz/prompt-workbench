#include "ui.h"
#include "fonts.h"
#include "imgui.h"

void ui::views::edit_user_prompt(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm,
    vm::edit_user_prompt::EditUserPromptViewModel& edit_user_prompt_vm
)
{
    ui::components::page_header(router, fonts, "Edit User Prompt");

    ui::components::input_multiline(
        "Prompt",
        &edit_user_prompt_vm.prompt
    );
    ui::components::input_error(
        edit_user_prompt_vm.prompt_error,
        fonts
    );

    ui::components::spacer(16.0f);

    if (ui::components::button("Save Changes"))
    {
        vm::edit_user_prompt::validate(edit_user_prompt_vm);

        if (!edit_user_prompt_vm.prompt_error.has_error)
        {
            vm::edit_user_prompt::update_user_prompt(
                dba_state,
                edit_user_prompt_vm,
                user_prompt_vm
            );

            routing::back(router);
        }
    }
}
