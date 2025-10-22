#pragma once

#include <cstdint>
#include "imgui.h"
#include <string> 

namespace helpers {
  struct FormError {
      bool has_error = false;
      std::string message = "";
  };

  ImVec4 hex_color(uint32_t hex, float alpha);
}
