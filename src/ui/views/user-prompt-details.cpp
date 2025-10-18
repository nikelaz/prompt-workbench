#include "ui.h"
#include "imgui.h"

void ui::views::user_prompt_details(
    routing::Router& router,
    vm::user_prompts::UserPromptsViewModel& user_prompts_vm
)
{
    ui::components::top_bar(router);

    if (!user_prompts_vm.current_user_prompt)
    {
        ImGui::Text("An unexpected error occured. Could not load the user_prompt");
    }

    ImGui::Text(
        "Prompt: %s",
        user_prompts_vm.current_user_prompt->prompt.c_str()
    );
}
