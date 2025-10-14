#include <cstdint>
#include "imgui.h"
#include "theme.h"

constexpr ImVec4 hexColor(uint32_t hex, float alpha = 1.0f) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8)  & 0xFF) / 255.0f;
    float b = ((hex >> 0)  & 0xFF) / 255.0f;
    return ImVec4(r, g, b, alpha);
}

void theme::applyColorScheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = hexColor(0xFFFFFF, 0.98f); 
    colors[ImGuiCol_TextDisabled]           = hexColor(0xFFFFFF, 0.60f);
    colors[ImGuiCol_WindowBg]               = hexColor(0x121212, 1.00f); 
    colors[ImGuiCol_ChildBg]                = hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_PopupBg]                = hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_Border]                 = hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_BorderShadow]           = hexColor(0x000000, 1.00f); 
    colors[ImGuiCol_FrameBg]                = hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_FrameBgHovered]         = hexColor(0xFFFFFF, 0.07f);
    colors[ImGuiCol_FrameBgActive]          = hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_TitleBg]                = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TitleBgActive]          = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TitleBgCollapsed]       = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_MenuBarBg]              = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarBg]            = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarGrab]          = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ScrollbarGrabHovered]   = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_ScrollbarGrabActive]    = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_CheckMark]              = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SliderGrab]             = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SliderGrabActive]       = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_Button]                 = hexColor(0x303F9F, 1.00f); 
    colors[ImGuiCol_ButtonHovered]          = hexColor(0x3F51B5, 1.00f); 
    colors[ImGuiCol_ButtonActive]           = hexColor(0x303F9F, 1.00f);
    colors[ImGuiCol_Header]                 = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_HeaderHovered]          = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_HeaderActive]           = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_Separator]              = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_SeparatorHovered]       = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_SeparatorActive]        = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ResizeGrip]             = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_ResizeGripHovered]      = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_ResizeGripActive]       = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_InputTextCursor]        = colors[ImGuiCol_Text];
    colors[ImGuiCol_Tab]                    = hexColor(0xFFFFFF, 0.05f);
    colors[ImGuiCol_TabHovered]             = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TabSelected]            = hexColor(0xFFFFFF, 0.09f); 
    colors[ImGuiCol_TabSelectedOverline]    = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TabDimmed]              = hexColor(0xFFFFFF, 0.05f); 
    colors[ImGuiCol_TabDimmedSelected]      = hexColor(0xFFFFFF, 0.07f); 
    colors[ImGuiCol_TabDimmedSelectedOverline] = hexColor(0xFFFFFF, 0.09f);

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
