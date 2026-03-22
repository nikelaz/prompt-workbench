#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::user_prompt_details(
    routing::Router& router,
    fonts::Fonts& fonts,
    vm::user_prompt::UserPromptViewModel& user_prompts_vm
)
{
    ui::components::page_header(router, fonts, "User Prompt");

    if (!user_prompts_vm.current_user_prompt)
    {
        ImGui::Text("An unexpected error occured. Could not load the user_prompt");
    }

    ImGui::TextDisabled("Prompt:");
    ImGui::TextWrapped("%s", user_prompts_vm.current_user_prompt->prompt.c_str());
}
