#include "ui.h"
#include "imgui.h"

void ui::views::result_run_details(
    routing::Router& router,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm
)
{
    ui::components::top_bar(router);

    if (!result_run_details_vm.current_result_run)
    {
        ImGui::Text("An unexpected error occured. Could not load the result run.");
    }

    ImGui::Text(
        "Date: %s",
        result_run_details_vm.current_result_run->date.c_str()
    );

    ImGui::Text("Answers:");

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
