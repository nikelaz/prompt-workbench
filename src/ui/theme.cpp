#include "imgui.h"
#include "theme.h"
#include "helpers.h"

void theme::apply_color_scheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = helpers::hex_color(0xFFFFFF, 0.98f); 
    colors[ImGuiCol_TextDisabled]           = helpers::hex_color(0xFFFFFF, 0.60f);
    colors[ImGuiCol_WindowBg]               = helpers::hex_color(0x121212, 1.00f); 
    colors[ImGuiCol_ChildBg]                = helpers::hex_color(0xFFFFFF, 0.05f);
    colors[ImGuiCol_PopupBg]                = helpers::hex_color(0xFFFFFF, 0.05f);
    colors[ImGuiCol_Border]                 = helpers::hex_color(0xFFFFFF, 0.05f);
    colors[ImGuiCol_BorderShadow]           = helpers::hex_color(0x000000, 1.00f); 
    colors[ImGuiCol_FrameBg]                = helpers::hex_color(0xFFFFFF, 0.05f);
    colors[ImGuiCol_FrameBgHovered]         = helpers::hex_color(0xFFFFFF, 0.07f);
    colors[ImGuiCol_FrameBgActive]          = helpers::hex_color(0xFFFFFF, 0.05f);
    colors[ImGuiCol_TitleBg]                = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TitleBgActive]          = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TitleBgCollapsed]       = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_MenuBarBg]              = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarBg]            = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarGrab]          = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarGrabHovered]   = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_ScrollbarGrabActive]    = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_CheckMark]              = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SliderGrab]             = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SliderGrabActive]       = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_Button]                 = helpers::hex_color(0x303F9F, 1.00f); 
    colors[ImGuiCol_ButtonHovered]          = helpers::hex_color(0x3F51B5, 1.00f); 
    colors[ImGuiCol_ButtonActive]           = helpers::hex_color(0x303F9F, 1.00f);
    colors[ImGuiCol_Header]                 = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_HeaderHovered]          = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_HeaderActive]           = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_Separator]              = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SeparatorHovered]       = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_SeparatorActive]        = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ResizeGrip]             = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ResizeGripHovered]      = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_ResizeGripActive]       = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_InputTextCursor]        = colors[ImGuiCol_Text];
    colors[ImGuiCol_Tab]                    = helpers::hex_color(0xFFFFFF, 0.05f);
    colors[ImGuiCol_TabHovered]             = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TabSelected]            = helpers::hex_color(0xFFFFFF, 0.09f); 
    colors[ImGuiCol_TabSelectedOverline]    = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TabDimmed]              = helpers::hex_color(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TabDimmedSelected]      = helpers::hex_color(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TabDimmedSelectedOverline] = helpers::hex_color(0xFFFFFF, 0.09f);

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
