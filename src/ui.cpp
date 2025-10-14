#include "ui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "helpers.h"

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

bool sidebarButton(const char* label, const bool isActive = false) {
  ImVec2 padding = ImVec2(20, 12);
  float rounding = 5.0f;
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
  ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

  ImVec4 buttonColor = isActive ? helpers::hexColor(0xFFFFFF, 0.05f) : helpers::hexColor(0xFFFFFF, 0.00f);
  ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, helpers::hexColor(0xFFFFFF, 0.05f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, helpers::hexColor(0xFFFFFF, 0.03f));
  bool clicked = ImGui::Button(label, ImVec2(-FLT_MIN, 0));
  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(3);
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
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_right);

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

void mainContent(std::function<void()> children)
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
  PAGE_1,
  PAGE_2,
  PAGE_3
};

void ui::mainFrame()
{
    static Route currentRoute = PAGE_1;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    
    dockSpace();

    sidebar([]() {
        if (sidebarButton("Page 1", currentRoute == PAGE_1))
        {
            currentRoute = PAGE_1;
        }

        if (sidebarButton("Page 2", currentRoute == PAGE_2))
        {
            currentRoute = PAGE_2;
        }

        if (sidebarButton("Page 3", currentRoute == PAGE_3))
        {
            currentRoute = PAGE_3;
        }
    });

    mainContent([&]() {
        switch(currentRoute) {
            case PAGE_1:
                ImGui::Text("Page 1");
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
