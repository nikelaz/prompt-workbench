#include "ui.h"
#include "fonts.h"

void ui::views::create_user_prompts(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::create_user_prompt::CreateUserPromptViewModel& create_user_prompt_vm,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm
)
{
    ui::components::page_header(router, fonts, "Create User Prompt");

    ui::components::input_multiline(
        "User Prompt",
        &create_user_prompt_vm.prompt
    );

    ui::components::input_error(
        create_user_prompt_vm.prompt_error,
        fonts
    );

    ui::components::spacer(16.0f);

    if (ui::components::button("Create User Prompt"))
    {
        vm::create_user_prompt::validate(create_user_prompt_vm);

        if (!create_user_prompt_vm.prompt_error.has_error)
        {
            vm::create_user_prompt::create_user_prompt(
                dba_state,
                create_user_prompt_vm,
                user_prompt_vm
            );

            vm::user_prompt::refresh(user_prompt_vm, dba_state);

            routing::push(router, routing::USER_PROMPTS_LIST);

            create_user_prompt_vm.prompt = "";
        }
    }
}
