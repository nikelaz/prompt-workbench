#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::system_prompt_comparison(
    routing::Router& router,
    fonts::Fonts& fonts,
    vm::compare_result_runs::CompareResultRunsViewModel& compare_vm
)
{
    ui::components::page_header(router, fonts, "System Prompt Comparison");

    std::string label_a = compare_vm.run_a.title.value_or(compare_vm.run_a.date);
    std::string label_b = compare_vm.run_b.title.value_or(compare_vm.run_b.date);

    ImGui::Text("Run A: %s", label_a.c_str());
    ImGui::SameLine();
    ImGui::Text("   Run B: %s", label_b.c_str());

    ui::components::spacer(12.0f);

    bool prompts_differ = (compare_vm.run_a.system_prompt != compare_vm.run_b.system_prompt);

    if (ImGui::BeginTable(
        "##system_prompts_full",
        2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY
    ))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(label_a.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(label_b.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        if (prompts_differ)
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(100, 80, 0, 100));

        ImGui::TableSetColumnIndex(0);
        ImGui::TextWrapped("%s", compare_vm.run_a.system_prompt.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::TextWrapped("%s", compare_vm.run_b.system_prompt.c_str());

        ImGui::EndTable();
    }
}
