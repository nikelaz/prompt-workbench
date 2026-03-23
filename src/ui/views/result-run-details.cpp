#include "ui.h"
#include "imgui.h"
#include "fonts.h"
#include "helpers.h"

void ui::views::result_run_details(
    routing::Router& router,
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm,
    vm::compare_result_runs::CompareResultRunsViewModel& compare_vm
)
{
    ui::components::page_header(router, fonts, "Result Run");

    if (!result_run_details_vm.current_result_run)
    {
        ImGui::Text("An unexpected error occured. Could not load the result run.");
        return;
    }

    if (result_run_details_vm.editing_title)
    {
        ui::components::input("Title", &result_run_details_vm.title_edit_buffer);
        ui::components::spacer(4.0f);
        if (ui::components::button("Save"))
        {
            dba::update_result_run_title(
                dba_state,
                result_run_details_vm.current_result_run->id,
                result_run_details_vm.title_edit_buffer
            );
            result_run_details_vm.current_result_run->title = result_run_details_vm.title_edit_buffer;
            result_run_details_vm.editing_title = false;
            vm::user_prompt::refresh(user_prompt_vm, dba_state);
        }
        ImGui::SameLine();
        if (ui::components::secondary_button("Cancel"))
        {
            result_run_details_vm.editing_title = false;
        }
    }
    else
    {
        const ResultRun& run = *result_run_details_vm.current_result_run;
        bool has_title = run.title.has_value();
        ImGui::TextDisabled("Title:");
        ImGui::TextWrapped("%s", has_title ? run.title->c_str() : run.date.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7.0f);
        if (ui::components::secondary_button("Edit"))
        {
            result_run_details_vm.title_edit_buffer = run.title.value_or(run.date);
            result_run_details_vm.editing_title = true;
        }
    }

    ui::components::spacer(8.0f);

    auto truncate_preview = [](const std::string& s, int max_lines = 4, bool* truncated = nullptr) -> std::string {
        int newlines = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '\n' && ++newlines == max_lines) {
                if (truncated) *truncated = true;
                return s.substr(0, i) + "\n...";
            }
        }
        if (truncated) *truncated = false;
        return s;
    };

    bool sp_truncated = false;
    std::string sp_preview = truncate_preview(result_run_details_vm.current_result_run->system_prompt, 4, &sp_truncated);
    ImGui::TextDisabled("System Prompt:");
    ImGui::TextWrapped("%s", sp_preview.c_str());
    if (sp_truncated) {
        ui::components::spacer(4.0f);
        if (ui::components::secondary_button("View Full System Prompt"))
            ImGui::OpenPopup("##full_system_prompt");
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, helpers::hex_color(0x121212, 1.00f));
    bool prompt_modal_open = ImGui::BeginPopupModal(
        "##full_system_prompt",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize
    );
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    if (prompt_modal_open)
    {
        ImGui::Text("System Prompt");
        ui::components::spacer(8.0f);
        ImGui::SetNextItemWidth(600.0f);
        ImGui::InputTextMultiline(
            "##system_prompt_full",
            const_cast<char*>(result_run_details_vm.current_result_run->system_prompt.c_str()),
            result_run_details_vm.current_result_run->system_prompt.size() + 1,
            ImVec2(600.0f, 400.0f),
            ImGuiInputTextFlags_ReadOnly
        );
        ui::components::spacer(8.0f);
        if (ui::components::secondary_button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ui::components::spacer(8.0f);

    if (ui::components::secondary_button("Compare"))
    {
        ImGui::OpenPopup("##compare_run_select");
    }

    ImGui::SameLine();

    if (ui::components::secondary_button("Delete"))
    {
        ImGui::OpenPopup("##delete_result_run");
    }

    if (ui::components::confirm_dialog(
        "##delete_result_run",
        "Delete Result Run?",
        "Are you sure you want to delete this result run?\nThis action cannot be undone.",
        "Delete"
    ))
    {
        dba::delete_result_run(dba_state, result_run_details_vm.current_result_run->id);
        result_run_details_vm.current_result_run = std::nullopt;
        vm::user_prompt::refresh(user_prompt_vm, dba_state);
        routing::back(router);
    }

    static int selected_idx = -1;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, helpers::hex_color(0x121212, 1.00f));
    bool modal_open = ImGui::BeginPopupModal(
        "##compare_run_select",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize
    );
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    if (modal_open)
    {
        ImGui::Text("Select a result run to compare");
        ui::components::spacer(8.0f);

        std::vector<ResultRun> other_runs;
        for (const ResultRun& run : user_prompt_vm.result_runs)
        {
            if (run.id != result_run_details_vm.current_result_run->id)
                other_runs.push_back(run);
        }

        for (int i = 0; i < (int)other_runs.size(); i++)
        {
            bool is_selected = (selected_idx == i);
            std::string run_label = other_runs[i].title.value_or(other_runs[i].date);
            if (ImGui::Selectable(run_label.c_str(), is_selected, ImGuiSelectableFlags_DontClosePopups))
                selected_idx = i;
        }

        ui::components::spacer(8.0f);

        if (ui::components::secondary_button("Cancel"))
        {
            selected_idx = -1;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        bool compare_disabled = (selected_idx < 0);
        if (compare_disabled) ImGui::BeginDisabled();
        if (ui::components::button("Compare"))
        {
            vm::compare_result_runs::prepare(
                dba_state,
                compare_vm,
                *result_run_details_vm.current_result_run,
                other_runs[selected_idx],
                user_prompt_vm.user_prompts
            );
            selected_idx = -1;
            ImGui::CloseCurrentPopup();
            routing::push(router, routing::COMPARE_RESULT_RUNS);
        }
        if (compare_disabled) ImGui::EndDisabled();

        ImGui::EndPopup();
    }

    ui::components::spacer(16.0f);

    ImGui::Text("Answers:");

    ui::components::spacer(8.0f);

    float text_width = ImGui::GetContentRegionAvail().x - 32.0f;

    for (Answer answer : result_run_details_vm.answers)
    {
        std::string preview = helpers::truncate_to_lines(answer.answer, text_width, 2);
        if (ui::components::card(
            "answer_" + std::to_string(answer.id),
            preview,
            std::nullopt,
            std::nullopt
        ))
        {
            result_run_details_vm.current_answer = answer;
            result_run_details_vm.current_user_prompt = dba::get_user_prompt(dba_state, answer.user_prompt_id);
            routing::push(router, routing::ANSWER_DETAILS);
        }
    }
}
