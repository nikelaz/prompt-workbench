#include "ui.h"
#include "imgui.h"
#include "imgui_internal.h"

void main_content(std::function<void()> children)
{
    ImGui::Begin(
        "MainContent",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus
    );
    children();
    ImGui::End();
}

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

    // Main content goes here
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
        ImGui::DockBuilderSetNodeSize(
            dockspace_id,
            ImGui::GetMainViewport()->Size
        );

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
        if (left_node) left_node->LocalFlags  |= ImGuiDockNodeFlags_NoTabBar;
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
    vm::test_suites::TestSuitesViewModel& test_suites_vm,
    vm::user_prompt_details::UserPromptDetailsViewModel& user_prompt_details_vm,
    vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm
)
{
    static routing::Router router = routing::init(routing::TEST_SUITES); 

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    dock_space();

    sidebar([]() {
        if (
            sidebar_button(
                "Test Suites",
                router.current_route == routing::TEST_SUITES
            )
        )
        {
            routing::push(router, routing::TEST_SUITES);
        }

        if (
            sidebar_button(
                "Page 2",
                router.current_route == routing::PAGE_2
            )
        )
        {
            routing::push(router, routing::PAGE_2);
        }

        if (
            sidebar_button(
                "Page 3",
                router.current_route == routing::PAGE_3
            )
        )
        {
            routing::push(router, routing::PAGE_3);
        }
    });

    main_content([&]() {
        switch(router.current_route) {
            case routing::TEST_SUITES:
                ui::views::test_suites(
                    router,
                    dba_state,
                    test_suites_vm,
                    user_prompt_details_vm
                );
                break;
            case routing::TEST_SUITES_DETAILS:
                ui::views::test_suite_details(
                    router,
                    dba_state,
                    test_suites_vm,
                    user_prompt_details_vm,
                    result_run_details_vm
                ); 
                break;
            case routing::USER_PROMPT_DETAILS:
                ui::views::user_prompt_details(router, user_prompt_details_vm); 
                break;
            case routing::RESULT_RUN_DETAILS:
                ui::views::result_run_details(router, result_run_details_vm); 
                break;
            case routing::PAGE_2:
                ImGui::Text("Page 2");
                break;
            case routing::PAGE_3:
                ImGui::Text("Page 3");
                break;
            default:
                ImGui::Text("404");
        }
    });
}
