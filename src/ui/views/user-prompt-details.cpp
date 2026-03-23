#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::user_prompt_details(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::user_prompt::UserPromptViewModel& user_prompts_vm,
    vm::edit_user_prompt::EditUserPromptViewModel& edit_user_prompt_vm
)
{
    ui::components::page_header(router, fonts, "User Prompt");

    if (!user_prompts_vm.current_user_prompt)
    {
        ImGui::Text("An unexpected error occured. Could not load the user_prompt");
        return;
    }

    ImGui::TextDisabled("Prompt:");
    ImGui::TextWrapped("%s", user_prompts_vm.current_user_prompt->prompt.c_str());

    ui::components::spacer(8.0f);

    if (ui::components::secondary_button("Edit")) {
        vm::edit_user_prompt::prepare(edit_user_prompt_vm, *user_prompts_vm.current_user_prompt);
        routing::push(router, routing::EDIT_USER_PROMPT);
    }

    ImGui::SameLine();

    if (ui::components::secondary_button("Delete")) {
        ImGui::OpenPopup("##delete_user_prompt");
    }

    if (ui::components::confirm_dialog(
        "##delete_user_prompt",
        "Delete User Prompt?",
        "Are you sure you want to delete this user prompt?\nThis action cannot be undone.",
        "Delete"
    )) {
        dba::delete_user_prompt(dba_state, user_prompts_vm.current_user_prompt->id);
        user_prompts_vm.current_user_prompt = std::nullopt;
        vm::user_prompt::refresh(user_prompts_vm, dba_state);
        routing::push(router, routing::USER_PROMPTS_LIST);
    }
}
