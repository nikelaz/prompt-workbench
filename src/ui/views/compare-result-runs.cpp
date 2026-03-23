#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::compare_result_runs(
    routing::Router& router,
    fonts::Fonts& fonts,
    vm::compare_result_runs::CompareResultRunsViewModel& compare_vm
)
{
    ui::components::page_header(router, fonts, "Compare");

    std::string label_a = compare_vm.run_a.title.value_or(compare_vm.run_a.date);
    std::string label_b = compare_vm.run_b.title.value_or(compare_vm.run_b.date);

    ImGui::Text("Run A: %s", label_a.c_str());
    ImGui::SameLine();
    ImGui::Text("   Run B: %s", label_b.c_str());

    ui::components::spacer(12.0f);

    ImGui::Text("System Prompts");
    ui::components::spacer(4.0f);

    bool prompts_differ = (compare_vm.run_a.system_prompt != compare_vm.run_b.system_prompt);

    auto truncate_preview = [](const std::string& s, int max_lines = 4) -> std::string {
        int newlines = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '\n' && ++newlines == max_lines)
                return s.substr(0, i) + "\n...";
        }
        return s;
    };

    if (ImGui::BeginTable("##system_prompts", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn(label_a.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(label_b.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        if (prompts_differ)
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(100, 80, 0, 100));

        ImGui::TableSetColumnIndex(0);
        ImGui::TextWrapped("%s", truncate_preview(compare_vm.run_a.system_prompt).c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::TextWrapped("%s", truncate_preview(compare_vm.run_b.system_prompt).c_str());

        ImGui::EndTable();
    }

    ui::components::spacer(8.0f);

    if (ui::components::button("View Full System Prompt Comparison"))
        routing::push(router, routing::SYSTEM_PROMPT_COMPARISON);

    ui::components::spacer(16.0f);

    if (ImGui::BeginTable(
        "##compare_table",
        4,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY
    ))
    {
        ImGui::TableSetupColumn("User Prompt",   ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(label_a.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(label_b.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Meaning Diff %", ImGuiTableColumnFlags_WidthFixed, 120.0f);
        ImGui::TableHeadersRow();

        for (size_t i = 0; i < compare_vm.user_prompts.size(); i++)
        {
            const UserPrompt& up = compare_vm.user_prompts[i];

            std::string answer_a, answer_b;
            for (const Answer& a : compare_vm.answers_a)
                if (a.user_prompt_id == up.id) { answer_a = a.answer; break; }
            for (const Answer& a : compare_vm.answers_b)
                if (a.user_prompt_id == up.id) { answer_b = a.answer; break; }

            ImGui::TableNextRow();

            bool highlight = !compare_vm.diff_scores.empty()
                && compare_vm.diff_scores[i] >= 0.0f
                && compare_vm.diff_scores[i] > 50.0f;

            if (highlight)
            {
                ImGui::TableSetBgColor(
                    ImGuiTableBgTarget_RowBg0,
                    IM_COL32(100, 80, 0, 100)
                );
            }

            ImGui::TableSetColumnIndex(0);
            ImGui::TextWrapped("%s", up.prompt.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::TextWrapped("%s", answer_a.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::TextWrapped("%s", answer_b.c_str());

            ImGui::TableSetColumnIndex(3);
            if (compare_vm.diff_scores.empty() || compare_vm.diff_scores[i] < 0.0f) {
                ImGui::TextDisabled("--");
            } else {
                ImGui::Text("%.1f%%", compare_vm.diff_scores[i]);
            }
        }

        ImGui::EndTable();
    }
}
