#include "ui.h"
#include "imgui.h"

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

