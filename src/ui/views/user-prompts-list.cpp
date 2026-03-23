#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::user_prompts_list(
    routing::Router& router,
    fonts::Fonts& fonts,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm
)
{
    const float heading_height   = 24.0f;
    const float button_padding_y = 7.0f;
    const float button_height    = ImGui::GetFontSize() + 2.0f * button_padding_y;
    const float row_height       = std::max(heading_height, button_height);
    const float row_y            = ImGui::GetCursorPosY();

    ImGui::SetCursorPosY(row_y + (row_height - button_height) * 0.5f);
    if (ui::components::secondary_button("Back"))
        routing::back(router);

    ImGui::SameLine();

    ImGui::SetCursorPosY(row_y + (row_height - heading_height) * 0.5f - button_padding_y);
    ImGui::PushFont(fonts.heading);
    ImGui::Text("User Prompts");
    ImGui::PopFont();

    ImGui::SameLine();

    const char* create_label = "Create User Prompt";
    float create_button_width = ImGui::CalcTextSize(create_label).x + 2.0f * 18.0f;
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - create_button_width);
    ImGui::SetCursorPosY(row_y + (row_height - button_height) * 0.5f);
    if (ui::components::button(create_label))
    {
        routing::push(router, routing::CREATE_USER_PROMPTS);
    }

    ui::components::spacer(12.0f);

    for (UserPrompt user_prompt : user_prompt_vm.user_prompts)
    {
        if (ui::components::card(
            "user_prompt_" + std::to_string(user_prompt.id),
            user_prompt.prompt,
            std::nullopt,
            std::nullopt
        ))
        {
            user_prompt_vm.current_user_prompt = user_prompt;
            routing::push(router, routing::USER_PROMPT_DETAILS);
        }
    }
}
