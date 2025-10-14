#include "ui.h"
#include "imgui.h"
#include "imgui_internal.h"

bool ui::button(const char* label) 
{
  ImVec2 padding = ImVec2(18, 7);
  float rounding = 5.0f;
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
  bool clicked = ImGui::Button(label);
  ImGui::PopStyleVar(2);
  return clicked;
}

void dockSpace() {
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

    static bool isFirstRender = true;

    if (isFirstRender)
    {
        isFirstRender = false;
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        ImGuiID dock_id_left, dock_id_right;
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, &dock_id_left, &dock_id_right);

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

void mainContent(std::function<void()> children)
{
    ImGui::Begin("MainContent", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoBringToFrontOnFocus
                 );

    children();

    ImGui::End();
}

void ui::mainFrame()
{
    static int currentRoute = 0;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    
    dockSpace();

    sidebar([]() {
        if (ui::button("Page 1"))
        {
            currentRoute = 0;
        }

        if (ui::button("Page 2"))
        {
            currentRoute = 1;
        }

        if (ui::button("Page 3"))
        {
            currentRoute = 2;
        }
    });

    mainContent([&]() {
        switch(currentRoute) {
            case 0:
                ImGui::Text("Page 1");
                break;
            case 1:
                ImGui::Text("Page 2");
                break;
            case 2:
                ImGui::Text("Page 3");
                break;
            default:
                ImGui::Text("404");
        }
    });
}
