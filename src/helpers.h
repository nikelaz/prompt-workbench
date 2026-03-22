#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "imgui.h"

namespace helpers {
  ImVec4 hex_color(uint32_t hex, float alpha);
  std::string truncate_to_lines(const std::string& text, float wrap_width, int max_lines);
  float cosine_similarity(const std::vector<float>& a, const std::vector<float>& b);
}
