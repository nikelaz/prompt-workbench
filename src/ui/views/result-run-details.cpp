#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::result_run_details(
    routing::Router& router,
    fonts::Fonts& fonts,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm
)
{
    ui::components::page_header(router, fonts, "Result Run");

    if (!result_run_details_vm.current_result_run)
    {
        ImGui::Text("An unexpected error occured. Could not load the result run.");
    }

    ImGui::Text(
        "Date: %s",
        result_run_details_vm.current_result_run->date.c_str()
    );

    ui::components::spacer(16.0f);

    ImGui::Text("Answers:");

    ui::components::spacer(8.0f);

    for (Answer answer : result_run_details_vm.answers)
    {
        ui::components::card(
            "answer_" + std::to_string(answer.id),
            answer.answer,
            std::nullopt,
            std::nullopt
        );
    }
}
