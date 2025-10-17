#include <functional>
#include "ui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "helpers.h"
#include "view-models.h"

bool ui::button(const std::string& label) 
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 7));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    bool clicked = ImGui::Button(label.c_str());

    ImGui::PopStyleVar(2);

    return clicked;
}

bool sidebar_button(
    const std::string& label,
    const bool is_active = false
)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    ImVec4 button_color =
        is_active
        ? helpers::hex_color(0xFFFFFF, 0.05f)
        : helpers::hex_color(0xFFFFFF, 0.00f);

    ImGui::PushStyleColor(ImGuiCol_Button, button_color);
    ImGui::PushStyleColor(
        ImGuiCol_ButtonHovered,
        helpers::hex_color(0xFFFFFF, 0.05f)
    );
    ImGui::PushStyleColor(
        ImGuiCol_ButtonActive,
        helpers::hex_color(0xFFFFFF, 0.03f)
    );

    bool clicked = ImGui::Button(
        label.c_str(),
        ImVec2(-FLT_MIN, 0)
    );

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);

    return clicked;
}

bool card(
    const std::string& id,
    const std::string& headline,
    const std::optional<std::string>& subtext,
    const std::optional<std::string>& right_label
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    ImGui::PushStyleColor(
        ImGuiCol_Button,
        helpers::hex_color(0xFFFFFF, 0.05f)
    );
    ImGui::PushStyleColor(
        ImGuiCol_ButtonHovered,
        helpers::hex_color(0xFFFFFF, 0.07f)
    );
    ImGui::PushStyleColor(
        ImGuiCol_ButtonActive,
        helpers::hex_color(0xFFFFFF, 0.05f)
    );

    ImVec2 start = ImGui::GetCursorScreenPos();
    ImVec2 card_size = ImVec2(ImGui::GetContentRegionAvail().x, 60);

    ImGui::InvisibleButton(id.c_str(), card_size); 
    bool clicked = ImGui::IsItemClicked();
    bool hovered = ImGui::IsItemHovered();
    bool active = ImGui::IsItemActive();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 bg_color = ImGui::GetColorU32(ImGuiCol_Button);
    if (hovered) bg_color = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    if (active) bg_color = ImGui::GetColorU32(ImGuiCol_ButtonActive);

    draw_list->AddRectFilled(
        start,
        ImVec2(start.x + card_size.x, start.y + card_size.y),
        bg_color,
        5.0f
    );

    ImGui::SetCursorScreenPos(ImVec2(start.x + 16, start.y + 12));
    ImGui::Text("%s", headline.c_str());

    if (subtext)
    {
        ImGui::SetCursorScreenPos(ImVec2(start.x + 16, start.y + 32));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 180, 180, 255));
        ImGui::Text("%s", subtext.value().c_str());
        ImGui::PopStyleColor();
    }

    if (right_label)
    {
        const char* right_label_value = right_label.value().c_str();
        ImVec2 right_text_size = ImGui::CalcTextSize(right_label_value);
        ImGui::SetCursorScreenPos(
            ImVec2(start.x + card_size.x - right_text_size.x - 16, start.y + 22)
        );
        ImGui::Text("%s", right_label_value);
    }

    ImGui::SetCursorScreenPos(ImVec2(start.x, start.y + card_size.y));
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);

    ImGui::Dummy(ImVec2(6, 6));

    return clicked;
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

void sidebar(std::function<void()> children)
{
    ImGui::Begin(
        "Sidebar",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize
    );
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos  = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    
    ImU32 bg_color = ImColor(1.0f, 1.0f, 1.0f, 0.05f);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color);

    children();

    ImGui::End();
}

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

enum Route {
  PAGE_TEST_SUITES,
  PAGE_2,
  PAGE_3
};

void test_suites_page(vm::test_suites::TestSuitesViewModel& test_suites_vm) {
    for (TestSuite test_suite : test_suites_vm.test_suites)
    {
        card(
            std::to_string(test_suite.id),
            test_suite.title,
            test_suite.description,
            test_suite.model
        );
    }
}

void ui::main_frame(vm::test_suites::TestSuitesViewModel& test_suites_vm)
{
    static Route current_route = PAGE_TEST_SUITES;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    
    dock_space();

    sidebar([]() {
        if (sidebar_button("Test Suites", current_route == PAGE_TEST_SUITES))
        {
            current_route = PAGE_TEST_SUITES;
        }

        if (sidebar_button("Page 2", current_route == PAGE_2))
        {
            current_route = PAGE_2;
        }

        if (sidebar_button("Page 3", current_route == PAGE_3))
        {
            current_route = PAGE_3;
        }
    });

    main_content([&]() {
        switch(current_route) {
            case PAGE_TEST_SUITES:
                test_suites_page(test_suites_vm);
                break;
            case PAGE_2:
                ImGui::Text("Page 2");
                break;
            case PAGE_3:
                ImGui::Text("Page 3");
                break;
            default:
                ImGui::Text("404");
        }
    });
}
