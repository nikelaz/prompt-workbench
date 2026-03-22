#pragma once

#include <string>
#include <vector>

namespace embedding {
    bool init(const std::string& model_path);
    void deinit();
    bool is_available();
    std::vector<float> compute(const std::string& text);
}
