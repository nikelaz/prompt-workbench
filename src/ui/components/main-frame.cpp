#include "ui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "fonts.h"
#include "helpers.h"
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <algorithm>
#include <filesystem>
#include <fstream>

void dock_space() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(
        "MainWindow",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoDecoration
    );

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

    ImGui::DockSpace(
        dockspace_id,
        ImVec2(0, 0),
        ImGuiDockNodeFlags_PassthruCentralNode
    );

    static bool is_first_render = true;

    if (is_first_render)
    {
        is_first_render = false;
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        ImGuiID dock_id_left, dock_id_right;
        ImGui::DockBuilderSplitNode(
            dockspace_id,
            ImGuiDir_Left,
            0.20f,
            &dock_id_left,
            &dock_id_right
        );

        ImGuiDockNode* left_node  = ImGui::DockBuilderGetNode(dock_id_left);
        ImGuiDockNode* right_node = ImGui::DockBuilderGetNode(dock_id_right);
        if (left_node)  left_node->LocalFlags  |= ImGuiDockNodeFlags_NoTabBar;
        if (right_node) right_node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

        ImGui::DockBuilderDockWindow("Sidebar", dock_id_left);
        ImGui::DockBuilderDockWindow("MainContent", dock_id_right);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void ui::components::main_frame(
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::prompt_editor::PromptEditorViewModel& prompt_editor_vm,
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm,
    vm::create_test_suite::CreateTestSuiteViewModel& create_test_suite_vm,
    vm::create_user_prompt::CreateUserPromptViewModel& create_user_prompt_vm,
    vm::api_credentials::ApiCredentialsViewModel& api_credentials_vm,
    vm::run_tests::RunTestsViewModel& run_tests_vm,
    vm::edit_test_suite::EditTestSuiteViewModel& edit_test_suite_vm,
    vm::compare_result_runs::CompareResultRunsViewModel& compare_vm,
    vm::edit_user_prompt::EditUserPromptViewModel& edit_user_prompt_vm
)
{
    static routing::Router router = routing::init(routing::PROMPT_EDITOR);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    dock_space();

    sidebar([&]() {
        ImGuiStyle& style = ImGui::GetStyle();
        float avail = ImGui::GetContentRegionAvail().x;
        float half_w = (avail - style.ItemSpacing.x) * 0.5f;

        if (ImGui::Button("New Prompt", ImVec2(half_w, 0))) {
            nfdchar_t* rawPath = nullptr;
            nfdfilteritem_t filter[1] = { {"Markdown", "md"} };
            if (NFD_SaveDialogU8(&rawPath, filter, 1, nullptr, nullptr) == NFD_OKAY && rawPath) {
                std::string path = rawPath;
                NFD_FreePathU8(rawPath);
                std::ofstream(path).close();
                vm::prompt_editor::open_file(prompt_editor_vm, dba_state, path);
                routing::push(router, routing::PROMPT_EDITOR);
                glfwPostEmptyEvent();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Open...", ImVec2(-FLT_MIN, 0))) {
            nfdchar_t* rawPath = nullptr;
            nfdfilteritem_t filters[2] = { {"Markdown", "md"}, {"Text", "txt"} };
            if (NFD_OpenDialogU8(&rawPath, filters, 2, nullptr) == NFD_OKAY && rawPath) {
                std::string path = rawPath;
                NFD_FreePathU8(rawPath);
                vm::prompt_editor::open_file(prompt_editor_vm, dba_state, path);
                routing::push(router, routing::PROMPT_EDITOR);
                glfwPostEmptyEvent();
            }
        }

        ui::components::spacer(8.f);
        ImGui::Separator();
        ui::components::spacer(4.f);

        for (auto& p : prompt_editor_vm.open_prompts) {
            ImGui::PushID((int)p.id);

            bool is_active = prompt_editor_vm.active_prompt_id.has_value() &&
                             prompt_editor_vm.active_prompt_id.value() == p.id;

            std::string filename = std::filesystem::path(p.file_path).filename().string();

            float close_btn_w = ImGui::CalcTextSize("x").x + 2.0f * style.FramePadding.x;
            float label_w = ImGui::GetContentRegionAvail().x - close_btn_w - style.ItemSpacing.x;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
            ImVec4 btn_color = is_active
                ? helpers::hex_color(0xFFFFFF, 0.05f)
                : helpers::hex_color(0xFFFFFF, 0.00f);
            ImGui::PushStyleColor(ImGuiCol_Button, btn_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, helpers::hex_color(0xFFFFFF, 0.05f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, helpers::hex_color(0xFFFFFF, 0.03f));

            if (ImGui::Button(filename.c_str(), ImVec2(label_w, 0))) {
                vm::prompt_editor::set_active_prompt(prompt_editor_vm, dba_state, p.id);
                routing::push(router, routing::PROMPT_EDITOR);
            }

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(3);

            ImGui::SameLine();

            if (ImGui::SmallButton("x")) {
                vm::prompt_editor::close_prompt(prompt_editor_vm, dba_state, p.id);
                routing::push(router, routing::PROMPT_EDITOR);
                glfwPostEmptyEvent();
            }

            ImGui::PopID();
        }

        // Settings pinned to bottom
        float footer_h = ImGui::GetFontSize() + 2.0f * style.FramePadding.y + style.ItemSpacing.y;
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footer_h);

        if (sidebar_button("Settings", router.current_route == routing::API_CREDENTIALS)) {
            routing::push(router, routing::API_CREDENTIALS);
        }
    });

    ImGui::Begin(
        "MainContent",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse
    );

    auto render_main = [&]() {
        if (router.current_route == routing::COMPARE_RESULT_RUNS) {
            ui::views::compare_result_runs(router, fonts, compare_vm);
        } else if (router.current_route == routing::SYSTEM_PROMPT_COMPARISON) {
            ui::views::system_prompt_comparison(router, fonts, compare_vm);
        } else {
            ui::components::content_container([&]() {
                switch (router.current_route) {
                    case routing::API_CREDENTIALS:
                        ui::views::api_credentials(dba_state, fonts, api_credentials_vm);
                        break;
                    default:
                        ui::views::prompt_editor(
                            router, dba_state, fonts, prompt_editor_vm,
                            test_suites_vm, user_prompt_vm, result_run_details_vm,
                            create_test_suite_vm, create_user_prompt_vm, run_tests_vm,
                            edit_test_suite_vm, compare_vm, edit_user_prompt_vm
                        );
                        break;
                }
            });
        }
    };

    if (prompt_editor_vm.chat_open) {
        vm::prompt_editor::commit_chat_if_done(prompt_editor_vm);

        ImGuiStyle& style = ImGui::GetStyle();
        static float chat_w = 300.f;
        const float splitter_w = 4.f;
        float avail_x = ImGui::GetContentRegionAvail().x;
        float avail_h = ImGui::GetContentRegionAvail().y;
        float content_w = avail_x - chat_w - splitter_w - style.ItemSpacing.x * 2;
        content_w = std::max(content_w, 100.f);

        ImGui::BeginChild("##editor_pane", ImVec2(content_w, avail_h), false,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        render_main();
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, helpers::hex_color(0xFFFFFF, 0.08f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, helpers::hex_color(0xFFFFFF, 0.12f));
        ImGui::Button("##chat_splitter", ImVec2(splitter_w, avail_h));
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();

        if (ImGui::IsItemActive()) {
            chat_w -= ImGui::GetIO().MouseDelta.x;
            chat_w = std::clamp(chat_w, 150.f, avail_x - 200.f);
        }
        if (ImGui::IsItemHovered() || ImGui::IsItemActive())
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

        ImGui::SameLine();

        ImGui::BeginChild("##chat_pane", ImVec2(chat_w, avail_h), false);

        float input_h = ImGui::GetFrameHeightWithSpacing();
        float dropdown_h = ImGui::GetFrameHeightWithSpacing();
        float history_h = ImGui::GetContentRegionAvail().y - input_h - dropdown_h;

        ImGui::BeginChild("##chat_history", ImVec2(0, history_h), false);
        for (auto& msg : prompt_editor_vm.chat_history) {
            ImGui::TextDisabled("%s", msg.role == "user" ? "You:" : "Assistant:");
            ImGui::TextWrapped("%s", msg.content.c_str());
            ui::components::spacer(8.f);
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();

        if (!prompt_editor_vm.chat_models_loaded && !prompt_editor_vm.chat_models_loading.load()) {
            Settings s = dba::get_settings(dba_state);
            vm::prompt_editor::fetch_chat_models(prompt_editor_vm, s);
        }

        if (prompt_editor_vm.chat_models_loading.load()) {
            ImGui::Text("Loading models...");
        } else if (prompt_editor_vm.chat_models_loaded && !prompt_editor_vm.chat_available_models.empty()) {
            std::vector<const char*> model_ptrs;
            for (auto& m : prompt_editor_vm.chat_available_models)
                model_ptrs.push_back(m.c_str());

            ImGui::SetNextItemWidth(-1.f);
            ImGui::Combo("##chat_model", &prompt_editor_vm.chat_model_selection, model_ptrs.data(), (int)model_ptrs.size());
            prompt_editor_vm.chat_model = prompt_editor_vm.chat_available_models[prompt_editor_vm.chat_model_selection];
        } else {
            ImGui::SetNextItemWidth(-1.f);
            ImGui::InputText("##chat_model_input", &prompt_editor_vm.chat_model);
        }

        float send_w = ImGui::CalcTextSize("Send").x + 36.f;
        ImGui::SetNextItemWidth(-send_w - style.ItemSpacing.x);
        ImGui::InputText("##chat_input", &prompt_editor_vm.chat_input);
        ImGui::SameLine();

        bool sending = prompt_editor_vm.chat_is_sending.load();
        if (sending) ImGui::BeginDisabled();
        if (ui::components::button("Send") && !prompt_editor_vm.chat_input.empty()) {
            Settings s = dba::get_settings(dba_state);
            vm::prompt_editor::send_chat_message(prompt_editor_vm, s);
        }
        if (sending) ImGui::EndDisabled();

        ImGui::EndChild();
    } else {
        render_main();
    }

    ImGui::End();
}
