#include "imgui.h"
#include "theme.h"
#include "helpers.h"

void theme::applyColorScheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = helpers::hexColor(0xFFFFFF, 0.98f); 
    colors[ImGuiCol_TextDisabled]           = helpers::hexColor(0xFFFFFF, 0.60f);
    colors[ImGuiCol_WindowBg]               = helpers::hexColor(0x121212, 1.00f); 
    colors[ImGuiCol_ChildBg]                = helpers::hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_PopupBg]                = helpers::hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_Border]                 = helpers::hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_BorderShadow]           = helpers::hexColor(0x000000, 1.00f); 
    colors[ImGuiCol_FrameBg]                = helpers::hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_FrameBgHovered]         = helpers::hexColor(0xFFFFFF, 0.07f);
    colors[ImGuiCol_FrameBgActive]          = helpers::hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_TitleBg]                = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TitleBgActive]          = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TitleBgCollapsed]       = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_MenuBarBg]              = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarBg]            = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarGrab]          = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarGrabHovered]   = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_ScrollbarGrabActive]    = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_CheckMark]              = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SliderGrab]             = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SliderGrabActive]       = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_Button]                 = helpers::hexColor(0x303F9F, 1.00f); 
    colors[ImGuiCol_ButtonHovered]          = helpers::hexColor(0x3F51B5, 1.00f); 
    colors[ImGuiCol_ButtonActive]           = helpers::hexColor(0x303F9F, 1.00f);
    colors[ImGuiCol_Header]                 = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_HeaderHovered]          = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_HeaderActive]           = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_Separator]              = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SeparatorHovered]       = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_SeparatorActive]        = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ResizeGrip]             = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ResizeGripHovered]      = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_ResizeGripActive]       = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_InputTextCursor]        = colors[ImGuiCol_Text];
    colors[ImGuiCol_Tab]                    = helpers::hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_TabHovered]             = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TabSelected]            = helpers::hexColor(0xFFFFFF, 0.09f); 
    colors[ImGuiCol_TabSelectedOverline]    = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TabDimmed]              = helpers::hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TabDimmedSelected]      = helpers::hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TabDimmedSelectedOverline] = helpers::hexColor(0xFFFFFF, 0.09f);

    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink]               = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_TreeLines]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_UnsavedMarker]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_NavCursor]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
